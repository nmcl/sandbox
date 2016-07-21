/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: acrdemon.cc,v 1.7 1993/03/18 15:42:44 n048z Exp $
 *
 */

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef ASSERT_H_
#include <System/assert.h>
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

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif

#include "defs.h"
#include "phone.h"
#include "connection.h"
#include "strlist.h"
#include "strlistiter.h"

extern str_list * dir(char *, boolean);

#ifdef hpux
#undef SIG_IGN
#define SIG_IGN ((void (*) (int))1)
#endif

main()
{
#ifdef DEBUG
    _arjuna_debug->set_all(FULL_DEBUGGING, FAC_ALL, VIS_ALL);
#endif

	extern boolean caller_ok(connection *);
	extern void handle_query(connection *);

	(void)signal(SIGPIPE, SIG_IGN);

	phone ph(DEMON_PORT, 4);

	for (;;)
	{
			/* wait for business
			 */
		connection * pcon = ph.answer();
#ifdef DEBUG
		debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
		debug_stream << "call from " << pcon->rname() << "\n" << flush;
#endif

		if (pcon != (connection *)null)
		{
				/* If the caller knows the magic
				 * words then accept a query.
				 */
			if (caller_ok(pcon))
				handle_query(pcon);

			delete pcon;
		}
	}
}



/***
 * Exchange id messages with the caller to make sure we're talking
 * to the right program.
 * This is intended as a type check, not a validation.
 *
 * Inputs:
 *	pc	- connection to caller
 *
 * Returns true if the caller seems to be ok.
 */

boolean caller_ok(connection * pc)
{
	int i;
	extern boolean strequ(char *, char *);
	char * demid = DEMID;
	char * manid = MANID;
	int n = strlen(demid) + 1;
	int k = strlen(manid) + 1;

	char * m = pc->cread(&i);
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
	debug_stream << "Id from caller: " << flush;
#endif

	if (i == k && strequ(m, manid))
	{
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
	    debug_stream << "accepted\n" << flush;
#endif
		return (pc->cwrite(demid, n) == n);
	}
	else
	{
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
	    debug_stream << "not accepted\n" << flush;
#endif
		return (false);
	}
}



/***
 * Handle a user query.
 *	This means:
 *		(i) read a list of uids from the caller
 *		(ii) figure out which are committed and which aborted actions
 *		(iii) construct a vector of the responses
 *		(iv) send the response
 * Inputs:
 *	pcon	- connection to calling client
 */


void handle_query(connection * pcon)
{
	char path[PATHBFRSZ];
	char * puid;
	char * aadir = ATOMICACTIONDIR_Q;
	int aadirsz = strlen(aadir);

	str_list * paauids = pcon->cread();	/* get query list */

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
	debug_stream << "user query received (" << paauids->count() << ")\n" << flush;
#endif

	str_list_iterator uid_scan(paauids);	/* we'll iterate over it */

	/* The str_list we expect to be sent is a list of
	 * stringified AtomicAction uids, each of which the
	 * caller wants a commit/abort decision on.
	 * We'll look each one up in the AtomicAction directory,
	 * and send back a character vector with one letter per
	 * uid ('c' for commit, 'a' for abort).
	 *
	 * (The existence of a file in the AA directory corresponding
	 * to an AA-uid implies a commit.)
	 */

	int n = paauids->count() + 1;
	char * ansvec = new char[n];
	char * pansvec = ansvec;
#ifndef hpux
	assert(ansvec != (char *)null);
#endif

	while ((puid = uid_scan.str_next())!= (char *)null)
	{
			/* The buffer size ought to be big enough
			 * so that this never fails.
			 */
#ifndef hpux
		assert(aadirsz + strlen(puid) + 2 <= PATHBFRSZ);
#endif

			/* (not most efficient, but trivial) */
		strcpy(path, aadir);
		strcat(path, "/");
		strcat(path, puid);

#ifdef __GNUG__
		if (access(path, 0) == 0)
		    *pansvec++ =  'c';
		else
		    *pansvec++ =  'a';
#else
		/* g++ 2.2.2 has problems with this... */
		*pansvec++ = (access(path, 0) == 0 ? 'c' : 'a');
#endif

#ifdef DEBUG
		debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
		debug_stream << "\tquery(" << puid << ") -> " pansvec[-1] << "\n" << flush;
#endif
	}

	*pansvec++ = '\0';
#ifndef hpux
	assert(n == pansvec - ansvec);
#endif

	if (pcon->cwrite(ansvec, n) != n)
		fprintf(stderr,
		      "Warning(acrdemon): size mismatch when sending answer\n");

	delete paauids;
	delete ansvec;
}
