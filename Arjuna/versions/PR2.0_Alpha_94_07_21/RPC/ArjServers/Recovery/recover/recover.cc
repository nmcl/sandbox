/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: recover.cc,v 1.8 1993/03/18 15:43:41 n048z Exp $
 *
 */

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#ifndef OSFCN_H_
#  include <System/osfcn.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef ASSERT_H_
#  include <System/assert.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif

#include "defs.h"
#include "phone.h"
#include "connection.h"
#include "strlist.h"
#include "strlistiter.h"
#include "tree.h"

#if defined(DEBUG) && !defined(DEBUG_H_)
#include <Common/Debug.h>
#endif

enum cadecision {abortaa, commitaa};

#ifdef hpux
#undef SIG_IGN
#define SIG_IGN ((void (*) (int))1)
#endif

/***
 * Recover all atomic actions on a single coordinator node.
 *
 * Inputs:
 *	node	- name of coordinator node for all atomic actions
 *		  to be recovered by us
 *	aas	- str_list of the uid's of atomic actions to be
 *		  recovered
 *	aa_to_obj	- tree mapping atomic action uids to lists of
 *			  (full) objectstate filepaths
 *	aa_to_saa	- tree mapping atomic action uids to lists of
 *			  (full) paths of server atomic action record
 *			  files belonging to each atomic action
 * Returns integer status code: 0 => recovery complete ok; other values
 * indicate unspecified failure.
 */

int recover(char * node, str_list * aas, tree * aa_to_obj, tree * aa_to_saa)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
    debug_stream << "Recover(" << node << ")\n" << flush;
#endif

	extern void complete_action(cadecision, char *, tree *, tree *);
	extern boolean server_ok(connection *);

	phone ph;
	connection * pcon = ph.call(node, DEMON_PORT);

		/* If our call to the recovery demon at the
		 * node failed then there's nothing more we
		 * can do.
		 */
	if (pcon == (connection *)null)
		return (1);

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
    debug_stream << "connection to " << node << " demon established\n" << flush;
#endif

	char * reply;
	int length = aas->count() + 1;
	int i;


	if (server_ok(pcon)				   /* right server */
	    && pcon->cwrite(aas)			   /* send query list */
	    && (reply = pcon->cread(&i)) != (char *)null   /* get reply list */
	    && i == length			        /* check reply length */
	   )
	{
	    delete pcon;

	    char * nextuid;
	    str_list_iterator paa(aas);
	    
/*
	    nextuid will step through the action uids
	    while reply scans through the vector of
	    corresponding replies.  Each reply consists
	    of a single character (nominally 'c' for commit,
	    'a' for abort- but we'll treat anything other
	    than 'c' as abort.
*/

	    while ((nextuid = paa.str_next()) != (char *)null)
	    {
		if (*reply++ == 'c')
		    complete_action(commitaa,
				    nextuid, aa_to_obj, aa_to_saa);
		else
		    complete_action(abortaa,
				    nextuid, aa_to_obj, aa_to_saa);
	    }
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
	    debug_stream << "recover(" << node << ") ok\n" << flush;
#endif
	    return (0);
	}
        else
	{
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
	    debug_stream << "recover(" << node << ") failure\n" << flush;
#endif
		delete pcon;
		return (1);
	}
}




/***
 * Exchange id messages with the server to make sure we're talking
 * to the right program.
 * This is intended as a type check.  It is not a validation protocol.
 *
 * Inputs:
 *	pc	- connection to server
 *
 * Returns true if the server seems to be ok.
 */

boolean server_ok(connection * pc)
{
        extern boolean strequ(char *, char *);
        char * demid = DEMID;
        char * manid = MANID;
	int k = strlen(demid) + 1;
	int i = strlen(manid) + 1;
	int n = i;

	if (n == pc->cwrite(manid, i))
	{		/* write our id */
		char * response = pc->cread(&i);	/* read the server's */
		if (i == k && strequ(response, demid)) { /* & check it */
#ifdef DEBUG
		    debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
		    debug_stream << "server id accepted\n" << flush;
#endif
			return (true);
		}
	}
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
	debug_stream << "server id not accepted\n" << flush;
#endif
	return (false);
}




/***
 * Commit or abort an action.
 *
 *	All objects involved in the action will be regenerated
 *	from either the shadow file (commit) or hidden file (abort).
 *
 *	After completion of either job the server atomic action
 *	records for the action will be removed.
 *
 * Inputs:
 *	dec	- abort/commit
 *	puid	- pointer to the string representing the action uid
 *	aa_to_obj	- tree mapping atomic action uids to lists of
 *			  (full) objectstate filepaths
 *	aa_to_saa	- tree mapping atomic action uids to lists of
 *			  (full) paths of server atomic action record
 *			  files belonging to each atomic action
 */

void complete_action(cadecision dec,
		   char * puid, tree * aa_to_obj, tree * aa_to_saa)
{
	extern char * shadow(char *, char, char);

	char * obj;
	char prefix, suffix;

	if (dec == commitaa)
	{
		prefix = '#';
		suffix = '#';
	}
	else
	{
		prefix = '!';
		suffix = '!';
	}

		/* Step through the objects renaming shadow/hidden versions
		 * to real versions.  (Leave the hidden/shadow files around
		 * for safety- it means we can back out.  There'll
		 * never be more than one per object.)
		 */
	str_list_iterator obj_scan(aa_to_obj->find_entry(puid));

	while ((obj = obj_scan.str_next()) != (char *)null)
	{
		char * objshad = shadow(obj, prefix, suffix);
#ifdef DEBUG
		debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
		debug_stream << "rename(" << objshad << ", " << obj << ")\n" << flush;
#endif
		(void)rename(objshad, obj);
		delete objshad;
	}


		/* By now we've done the actual commit/abort.
		 * Clean up by deleting the server atomic
		 * action records for the now recovered action.
		 */
	str_list_iterator saa_scan(aa_to_saa->find_entry(puid));

	while ((obj = saa_scan.str_next()) != (char *)null)
	{
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
	    debug_stream << "unlink(" << obj << ")\n" << flush;
#endif
		(void)unlink(obj);
	}
}


/***
 * Recovery manager.
 *	This program periodically scans the Arjuna objectstore
 *	directory ServerAtomicAction for atomic actions that
 *	have been abandonned in the 'prepare' phase.  This can
 *	have resulted from this machine crashing in the middle
 *	of a distributed commit, or the coordinator of the commit
 *	crashing and leaving the server process on this machine
 *	orphaned.
 *
 *	There is sufficient information recorded in the
 *	ServerAtomicAction records for the abandonned action to
 *	eventually be committed or aborted.
 */

main()
{
#ifdef DEBUG
    _arjuna_debug->set_all(FULL_DEBUGGING, FAC_ALL, VIS_ALL);
#endif

	extern void scan_stale_saas(tree *, tree *, tree *);
	extern void visit_print(char *, int, str_list *, void **);
	extern void visit_fork(char *, int, str_list *, void **);

		/* Dropped virtual circuits can generate this
		 * signal.  We don't want to be interrupted by
		 * it.
		 */
	(void)signal(SIGPIPE, SIG_IGN);

	tree aa_to_objpath("");
	tree aa_to_saa("");
	tree node_to_aa("");

	/* Gather information about abandonned
	 * atomic actions.
	 */

	scan_stale_saas(&aa_to_objpath, &aa_to_saa, &node_to_aa);

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
	debug_stream << "\nAA -> object list\n" << flush;
	aa_to_objpath.traverse(&visit_print, (void **)null);
	debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
	debug_stream << "\nAA -> SAA list\n" << flush;
	aa_to_saa.traverse(&visit_print, (void **)null);
	debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
	debug_stream << "\nnode -> AA list\n" << flush;
	node_to_aa.traverse(&visit_print, (void **)null);
#endif

	for (;;)
	{
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
	    debug_stream << "main() : fork loop\n" << flush;
#endif
		void * xargs[3];
		int n = 0;

		xargs[0] = (void *)&aa_to_objpath;
		xargs[1] = (void *)&aa_to_saa;
			/* Pass a pointer to n down the traversal so a count
			 * can be kept of the number of children forked.
			 */
		xargs[2] = (void *)&n;


			/* Having catalogued which abandonned actions
			 * go with which coordinator nodes, fork
			 * one recovery process for each node.
			 */
		node_to_aa.traverse(&visit_fork, xargs);


			/* Wait for our children to finish.
			 */
		while (n-- > 0)
		    (void) wait(0);

			/* Run again after a few minutes.
			 */
		sleep(30);
	}
}



#ifdef DEBUG

/***
 * Node visit function for traversing & printing a tree.
 */

void visit_print(char * key, int nodecount, str_list * conts, void ** args)
{
	extern boolean strequ(char *, char *);

	if (strequ(key, ""))
		printf("dummy node (%d)\n", nodecount);
	else
	{
		printf("%s (%d): ", key, nodecount);
		if (conts == (str_list *)null)
			printf("null str_list");
		else
			conts->print();
		printf("\n");
	}
}

#endif

/***
 * Node visit function for traversing a tree & generating a recover
 * process for each key.  (to be used on the node->aauids tree; 1 process
 * for each coordinator node)
 */

void visit_fork(char * key, int nodecount, str_list * aauids, void ** args)
{
	extern boolean strequ(char *, char *);
	extern int recover(char *, str_list *, tree *, tree *);

	tree * aa_to_obj = (tree *)args[0];
	tree * aa_to_saa = (tree *)args[1];
	int * pn = (int *)args[2];

	if (!strequ(key, ""))
	{		/* omit dummy node */
		if (fork() == 0)
		{
				/* Child */
			exit(recover(key, aauids, aa_to_obj, aa_to_saa));
		}
		else
		{
				/* parent */
			*pn += 1;		/* keep count of children */
		}
	}
}



/***
 * This procedure scans all (regular) files it finds in the directory
 * $(ObjectStore)/StateManager/AtomicAction/ServerAtomicAction.
 * Each of these files represents a potentially abandoned atomic action
 * that was left in an indeterminate state ('prepare' done.) 
 * For each of these files it first tries to find a living server process
 * by sending a signal to it.  The server is identified by its pid which
 * is recored in the file.  If the process still exists the file is ignored.
 * (There is a small probability that the counter has wrapped and the
 * same pid has been assigned to another process.)
 * All the information neccessary for recovery is added to the trees
 * as we discover it.
 *
 * The input trees are used to hold the following mappings.
 *
 *	aa_obj	: maps atomic action uids to lists of objectstate file
 *		  paths
 *	aa_saa	: maps atomic action uids to lists of server atomic action
 *		  file paths
 *	node_aa	: maps coordinator node names to lists of atomic action
 *		  uids
 */

void scan_stale_saas(tree * aa_obj, tree * aa_saa, tree * node_aa)
{
    extern str_list * dir(char *, boolean);
    extern str_list * file_strings(char *);

    char * saadir = SERVERATOMICACTIONDIR_Q;
    char * saapath;


    /* Get a list of full pathnames of all serveratomicaction
     * files.
     */

    str_list * psl = dir(saadir, true);
    str_list_iterator isl(psl);

    /* Then for each of these decide whether it has to
     * be recovered, and collect the necessary information
     * on those that do.
     */

    while ((saapath = isl.str_next()) != (char *)null)
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
	debug_stream << "Recover: scanning " << saapath << "\n" << flush;
#endif
	int n;
	str_list * saacont = file_strings(saapath);

	if (saacont != (str_list *)null
	    && (n = saacont->count()) >= 4)
	{
	    printf("Here\n");
	    char * aauid = saacont->get_str(0);
	    char * node = saacont->get_str(1);
	    char * pidstr = saacont->get_str(2);
	    int pid;

	    sscanf(pidstr, "%d", &pid);
	    if (kill(pid, 0) == -1)
	    {
		/* kill failed => the server is
		 * not alive; recover the action
		 */

		/* remember the paths of all objects
		 * updated by this action
		 */
		for (int i = 3; i < n; i++)
		{
		    char * objp = saacont->get_str(i);
		    (void)aa_obj->add_entry(aauid, objp);
		}

		/* remember which files we'll have
		 * to delete when recovery of this
		 * action is complete
		 */
		(void)aa_saa->add_entry(aauid, saapath);

		/* save list of all actions
		 * associated with each node
		 */
		(void)node_aa->add_entry(node, aauid);

	    } /* else
		The process is running, so leave it alone.
		It might still complete the atomic action
		itself.
		*/
	}
	delete saacont;
    }

    delete psl;
}
