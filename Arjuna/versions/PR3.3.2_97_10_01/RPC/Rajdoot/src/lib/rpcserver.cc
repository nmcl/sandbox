/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: rpcserver.cc,v 1.32.4.1.2.1 1996/10/10 12:26:24 ngdp Exp $
 */

/* VERSION with duplicateserver logging initiate requests in clist */

/*
 * RPC protocol. Server process module.
 * This file contains the primitives `Init_Server()', `GetWork()', and
 * `SendResult()' for use from application software for constucting server
 * processes. Note that each invocation of the primitive `GetWork()' must be
 * preceeded by an invocation of the macro `ONDEADLINE()' (see `rpcuser.h').
 */


#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef FCNTL_H_
#  include <System/fcntl.h>
#endif

#ifndef MALLOC_H_
#  include <System/malloc.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef SYS_FILE_H_
#  include <System/sys/file.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SYS_STAT_H_
#  include <System/sys/stat.h>
#endif

#ifndef SYS_IPC_H_
#  include <System/sys/ipc.h>
#endif

#ifndef SYS_SEM_H_
#  include <System/sys/sem.h>
#endif

#ifndef SYS_SHM_H_
#  include <System/sys/shm.h>
#endif

#ifndef FSTREAM_H_
#  include <System/fstream.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#include <RPC/Rajdoot/rpcuser.h>
#include "rpc.h"

#ifndef F_ULOCK
#define F_ULOCK 0
#endif

#ifndef F_LOCK
#define F_LOCK  1
#endif

extern void checkEnvironment();

extern int managerShm;
extern int managerSem;
extern Int32 crashcount;
extern char* clistfd;
extern int sem_id;
extern int errno;

static struct MID myunicastid, commitid;
static struct MID myaddress;
static struct MID grouplist[S_ACTGRPSIZE];

SERVERSWORK  latest_work;

static struct MID mygroups[S_GROUPSIZE];
static int no_groups = 0, no_initgroups = 1;

static Boolean p_orphan = FALSE;   /* optimization on looking up clist to 
				    * reset poflag every time valid call 
				    * is received from client 
				    */
static void (*Dump)() = 0;
static Boolean interrupted = FALSE;

char* serverName = (char*) 0;

/* Information shared between getwork and the filter procedure */

struct GROUP_INFO *s_filt_replygrp; /* set by getwork and filter */
int s_successes_expected;           /* Set by getwork */
int s_compare_crit;                 /* Set by getwork */
struct RPC s_correct_rpc;           /* Set by filter */
int s_total_received;               /* Set by filter */
struct HEAD_RPCLIST s_rpcstruct;

int default_server_filter (struct mc_sg_filter *new_msg, Boolean timeout)
{
  /* Attempts to receive messages until either the call's deadline expires
   * or all servers in the expected group reply - note that if no of
   * successes expected is met even before the deadline, it continues to
   * receive - since otherwise, there will be outstanding messages held in
   * the system which must be received and discarded, the next time the
   * user attempts to use any of the primitives
   */

  static int still_expected = 1;

#ifdef DEBUG
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "default filter: received source address: ";
  printgid(new_msg->source);
  debug_stream << flush;

  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "opcode: " << * (Int32 *) new_msg->mc_msg->mc_msg[1].iov_base;
  debug_stream << " buffer1: " << new_msg->mc_msg->mc_msg[2].iov_base << " buffer2: "
	       << new_msg->mc_msg->mc_msg[3].iov_base << "\n" << flush;
#endif
  
  if (dec_filtgrp(s_filt_replygrp, new_msg->source) == SUCCEEDED)
  {
    s_total_received++;
    if (s_total_received == 1)
      still_expected = s_successes_expected;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "default filter: s_successes_expected " << s_successes_expected
	<< ", still_expected " << still_expected << "\n" << flush;
#endif
    
    if (s_compare_crit != compare_NONE)
    {
      /* Build list of rpcs received for subsequent voting, and
	 if necessary, initialise head of list of rpcs received */
      if (s_total_received == 1)
	s_rpcstruct.head_rpc = NULL;

      insert_newmsg(new_msg, &s_rpcstruct); 
    }

    if (--still_expected <= 0)
    {
      /* Required number of servers have replied, vote on server results
	 and complete call if successful; otherwise, continue to receive
	 until timeout expires or all servers respond */

      if (s_compare_crit == compare_NONE)
      {
	set_rpcinfo(&s_correct_rpc,
		    * (Int32 *) new_msg->mc_msg->mc_msg[1].iov_base,
		    new_msg->mc_msg->mc_msg[2].iov_len,
		    (char*) new_msg->mc_msg->mc_msg[2].iov_base, 
		    new_msg->mc_msg->mc_msg[3].iov_len,
		    (char*) new_msg->mc_msg->mc_msg[3].iov_base);
	return(EPRINTSIZE); /* Successful termination, stop receiving */
      }
      else
      {
	if (vote_on_results(&s_rpcstruct, s_successes_expected,
			    s_compare_crit, &s_correct_rpc) != SUCCEEDED)
	{
	  if (isempty_filtgrp(s_filt_replygrp))
	  {
	    /* All servers in group have replied, but voting did not succeed */
	    remove_rpc_list(&s_rpcstruct, TRUE); 
	    return(ETIMEOUT); /* Failure termination, stop receiving */
	  }
	  else
	  {
	    /* Client still needs to receive further calls to succeed */
	    if (timeout)
	    {
	      remove_rpc_list(&s_rpcstruct, TRUE);
	      return(ETIMEOUT); /* Failure termination, stop receiving */
	    }
	    else
	      return(CARRYON); 
	  }
	}
	else
	{
	  return(EPRINTSIZE);
	}
      } /* end else */
    } /* end if */

  } /* end if */


  /* Client still needs to receive further calls to succeed */
  if (timeout)
  {
    if (s_compare_crit != compare_NONE)
	remove_rpc_list(&s_rpcstruct, TRUE); 

    return(ETIMEOUT); /* Failure termination, stop receiving */
  }
  else
    return(CARRYON); /* Carry on receiving */

} /* end default_server_filter() */

// do any tidy up before exiting (may unlink empty debugging files)

void tidyUp ()
{
    pid_t mypid;
    char stdoutfn[OBJECT_NAME_SIZE], stderrfn[OBJECT_NAME_SIZE];
    char tempOfn[OBJECT_NAME_SIZE], tempEfn[OBJECT_NAME_SIZE];
    char *fileName = (char*) 0;
    struct stat buf;

    if (serverName == (char*) 0)
    {
	error_stream << WARNING << "Could not do tidyup as no server name present!" << endl;
	return;
    }

    fileName = ::new char[::strlen(serverName)+3];
    ::memset(fileName, '\0', ::strlen(serverName)+3);
    ::strcpy(fileName, serverName);
    ::strcpy(fileName+::strlen(serverName), "%s");

    mypid = getpid();
    sprintf(tempOfn, "_out_%d", mypid);
    sprintf(tempEfn, "_err_%d", mypid);
    sprintf(stdoutfn, fileName, tempOfn);
    sprintf(stderrfn, fileName, tempEfn);
    
#ifndef GCC_ARRAY_DELETE_BUG
    ::delete [] serverName;
    ::delete [] fileName;
#else
    ::delete serverName;
    ::delete fileName;
#endif

    if (stat(stdoutfn, &buf) == 0)
    {
	if (buf.st_size == 0)
	    ::unlink(stdoutfn);
    }

    if (stat(stderrfn, &buf) == 0)
    {
	if (buf.st_size == 0)
	    ::unlink(stderrfn);
    }
}

void ResetPOFlag (Boolean value)
{
  /* Catch ORPHAN_TIMER signal generated by the expiration of the idle timeout.
   * Declare the server as a potential orphan in the clist and return to
   * GetWork.  If the server receives a call from the client in GetWork,
   * the p_orphan flag is reset.
   */

  struct C_LIST_EL* pclist = NULL; 
  Boolean           found;
  Int32              mypid;
  int               srvrindx;
  int               retry = FILESRETRY, rstat = FAILED;

 /* Open Clist */
  do
  {
      rstat = clistopen(pclist);
  } while ((rstat != SUCCEEDED) && (retry-- > 0));
  if (rstat != SUCCEEDED)
  {  /* Could not read clist after several attempts */
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "Error::rpcs: open failed. Errno " << errno << "\n" << flush;
#endif
      /* clist open failed, so simply "close" clist without writing anything back. */
      
      int openErr = _rpcerr;  // in case we encounter an error on close
      
      (void) clistclose();

      /* Diagnostics and exception handling if clist ops fail for terminate */

      if (openErr == OPENFAILED)
      {
	  error_stream << FATAL << "clist fatal error." << endl;
	  ArjunaFatal();
      }
      
      return;
  }
  
  /* Find server entry */
  mypid = getpid();
  srvrindx = -1; found = FALSE;
  do
  {
    srvrindx++;
    found = ((pclist->s_desc[srvrindx] . process_id == mypid) ? TRUE : FALSE);
  }  while ((srvrindx < MAXSERVERS) && (!found));

  if (found)
  { /* Should be an assertion */
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "server id found in clist\n" << flush;
#endif

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
        debug_stream << "Setting flag to " << ((value == FALSE) ? "FALSE" : "TRUE") << endl;
#endif
	::p_orphan = pclist->s_desc[srvrindx].poflag = value;
  }

  /* Close clist */
  retry = FILESRETRY;  	rstat = FAILED;
  do
  {
      rstat = clistclose();
  } while ((rstat != SUCCEEDED) && (retry-- > 0));
  if (rstat != SUCCEEDED)
  {  /* Could not write clist after several attempts */
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "Error::rpcs: close failed. Errno " << errno << "\n" << flush;
#endif
    /* Diagnostics and exception handling if clist ops fail for terminate */
      return;
  }

  // if server could not find itself in the clist then we should abort!
  
  if (!found)
  {
      error_stream << FATAL << "rpcserver: ResetPOFlag could not find server with pid " << getpid() << " in clist. Aborting." << endl;
      ArjunaFatal();
  }
} /* end ResetPOFlag() */

/* This needs to be re-worked */

#ifdef HAVE_INTSIG
int handle_interrupt (int dummy DOTDOTDOT)
#else
void handle_interrupt (int dummy DOTDOTDOT)
#endif
{
  struct GROUPS     client;
  struct RPC        work;
  struct DEADLINE          deadline;
  
  dummy = dummy;
  
  /* cannot acknowledge server, since source of signal is unknown */

  /* Result params from GetWork ignored.  GetWork called to allow server
   * to receive high priority group management operation request
   */

  interrupted = TRUE;

#ifdef DEBUG
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "\nrpcserver:  server was interrupted - about to GetWork\n\n" << flush;
#endif
  if (getwork(&client, &work, &deadline, NULL) == SUCCEEDED)
  {
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "\nrpcserver: returned from GetWork\n\n" << flush;
#endif
    signal(INTERRUPT_SERVER, handle_interrupt);
  }
  else
  {
    /* when server must terminate, send back message to user - necessary?? */
  }

}/* end handle_interrupt() */

#if 0

/* No longer needed put keep for now */

static void ondeadline (int dummy)
{
/*
 * Catch ABORT signal generated by the expiration of the deadline.
 * Control is returned back to the invocation of `GetWork()'.
 */

    dummy = dummy;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "\nOndeadline: ABORT signal caught\n" << flush;
#endif

    interrupted = FALSE; /* Reset condition used for gm primitives */

    if (::Dump != 0)
	(*::Dump)();
    longjmp(_rpcenv, 0);
} /* end ondeadline */
#endif

int sendresult (struct GROUPS *destn, struct RPC *rslt, Boolean isRetry)
{
/* Transmit result `rslt' to client at address `to'.
 * Return values:
 * SUCCEEDED: result successfully transmitted;
 * FAILED: an error occured;  
 * rpcerr set to:-
 * errno: system error (see `errno.h');
 * EUNKNOWN: system error but `errno' was not set.
 */

    struct RPC_SN         seqno;
    struct MID            dest_group;
    struct mc_msghdr      res_buf;
    int                   indx, sstatus;
			

    if ((destn->groups_type != SRVR_type) ||
	(destn->grp_srvr.srvr.sr_idtype != UID_type))
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "SendResult: groups_type " << destn->groups_type << " << flush; sr_idtype "
		     << destn->grp_srvr.srvr.sr_idtype << "\n" << flush;
#endif 
      return(INVALID_GRP);
    }

    setalarm(0);  /* Cancel deadline timer for call's execution */

    dest_group = destn->grp_srvr.srvr.sr_id;      /* MID type */
    
    /* Find client's latest sequence number */
    indx = 0;
    do
    {
      if (match_clients(&(latest_work[indx].client_address),
			&dest_group) == SUCCEEDED)
      {
#ifdef DEBUG
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "SENDRESULT: found client at indx " << indx << "\n" << flush;
#endif
        break;
      }
      indx++;
    } while (indx < MAXCLIENTS);
    
    if (indx >= MAXCLIENTS)
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "SendResult: client was not in list\n" << flush;
#endif
      return(WRONG_CLIENT);
    }
    else
    {
	if (!isRetry)
	{
            /* Save client's latest result, for retransmission on client's retry */
	    /* Delete old buffers */
	    if (latest_work[indx].result.rpc_buff1)
#ifndef GCC_ARRAY_DELETE_BUG
		::delete [] latest_work[indx].result.rpc_buff1;
#else
	        ::delete latest_work[indx].result.rpc_buff1;
#endif
	    if (latest_work[indx].result.rpc_buff2)
#ifndef GCC_ARRAY_DELETE_BUG
		::delete [] latest_work[indx].result.rpc_buff2;
#else
	        ::delete latest_work[indx].result.rpc_buff2;
#endif

	    latest_work[indx] . result . rpc_code = rslt -> rpc_code; /* network order */

	    /* Make copy of buffers !! */
	    latest_work[indx].result.rpc_buff1 = ::new char[rslt->rpc_length1];
	    memcpy(latest_work[indx].result.rpc_buff1, rslt -> rpc_buff1, (int) rslt -> rpc_length1);
	    latest_work[indx] . result . rpc_length1 = rslt -> rpc_length1;
	    latest_work[indx] . result . rpc_buff2 = ::new char[rslt -> rpc_length2];
	    memcpy(latest_work[indx] . result . rpc_buff2, rslt -> rpc_buff2, (int) rslt -> rpc_length2);
	    latest_work[indx] . result . rpc_length2 = rslt -> rpc_length2;
	}
      
      /* Byte swap sequence number */
      seqno . sec = htonl(latest_work[indx] . seq_no . sec);
      seqno . msec = htonl(latest_work[indx] . seq_no . msec);
      
      /* Build result for transmission */
      res_buf . mc_msg[0] . iov_base = (char *) &seqno; 
      res_buf . mc_msg[0] . iov_len = sizeof(seqno);
      res_buf . mc_msg[1] . iov_base = (char *) & (rslt->rpc_code);
      res_buf . mc_msg[1] . iov_len = sizeof(rslt->rpc_code);
      res_buf . mc_msg[2] . iov_base = (char *) rslt->rpc_buff1;
      res_buf . mc_msg[2] . iov_len = (int) rslt->rpc_length1;
      res_buf . mc_msg[3] . iov_base = (char *) rslt->rpc_buff2;
      res_buf . mc_msg[3] . iov_len = (int) rslt->rpc_length2;
      res_buf . msg_len = 4;

      sstatus = multicast_long_send(&dest_group, &res_buf);

      if ((sstatus != MC_OK) && (sstatus != ELOSTMESSAGE))
      {
	  _rpcerr = (errno == 0) ? EUNKNOWN : errno;
#ifdef DEBUG
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "SendResult: failed to send to client\n" << flush;
#endif
	  return(FAILED);
      }
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "SendResult: sent to client ok\n" << flush;
#endif
	return(SUCCEEDED);
    } /* end else */

} /* End SendResult */

int log_server (char *clistPtr)
{
  /* Obtain ports for communication and log them in the clist */

  struct C_LIST_EL* clist = (struct C_LIST_EL*) clistPtr;
  Boolean found;
  Int32 mypid;
  int server_indx, rcode, retry;

  if (clist == NULL)
      return FAILED;
  
  checkEnvironment();
  
  /* No need to open clist, lock already held */

  mypid = getpid();

  server_indx = 0; found = FALSE;
  do
  {
      found = ((clist->s_desc[server_indx].process_id == mypid) ? TRUE : FALSE);
      if (!found)
	  server_indx++;
  } while ((server_indx < MAXSERVERS) && (!found));

  if (found)
  {
      /*
       * These addresses are wrongly names. The unicastid is actually the one we
       * would use if we were using multicasting - the mcrdaemon sends us data
       * via it. myaddress is actually the unicast address used by the outside
       * world.
       */
      
      multicast_new_mid(&myunicastid);
      multicast_join(&myunicastid);
      multicast_get_mid(&myaddress);
      clist->s_desc[server_indx].myunicastid = myunicastid;
      clist->s_desc[server_indx].sender = myaddress;
  }

  sem_id = semget(managerSem, 1, 0600);

  /* close clist */
  retry = FILESRETRY;
  do
  {
    rcode = clistclose();
  } while ((rcode != SUCCEEDED) && (retry-- > 0));


#ifdef DEBUG
  if (!found)
  {
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "log_server: Warning - did not find server in clist:" << flush;
      debug_stream << "Condition should not occur\n" << flush;
  }
#endif  
  
  return(rcode);
  
} /* end log_server() */
	
void join_grp (struct MID *myid)
{
    if (RAJDOOT_USE_BROADCAST)
    {
	/* Keep track of groups joined */

	if (no_groups >= S_GROUPSIZE)
	    error_stream << WARNING << "rpcserver: join_grp - maximum group size exceeded." << endl;
	else
	{
	    for (int i = 0; i < S_GROUPSIZE; i++)
	    {
		if (mygroups[i].hi_part == 0)
		{
		    no_groups++;
		    mygroups[i].type = myid->type;
		    mygroups[i].hi_part = myid->hi_part;
		    mygroups[i].lo_part = myid->lo_part;
		    multicast_join(myid);
		    return;
		}
	    }
	}

	error_stream << WARNING << "rpcserver: join_grp - could not find free slot for group." << endl;
    }
}/* end join_grp() */

void leave_grp (struct MID *mymid)
{
    if (RAJDOOT_USE_BROADCAST)
    {
	for (int i = 0; i < S_GROUPSIZE; i++)
	{
	    if (identical_group(mymid, &mygroups[i]))
	    {
		multicast_leave(mymid);
		mygroups[i].hi_part = 0;  // reset for future use.
		no_groups--;
		return;
	    }
	}

	error_stream << WARNING << "rpcserver: leave_grp - could not find group to leave." << endl;
    }
} /* end leave_grp() */

void LeaveAllGroups ()
{
    if (RAJDOOT_USE_BROADCAST)
    {    
	for (int i = 0; i < S_GROUPSIZE; i++)
	{
	    if (!isnull_id(&mygroups[i]))
	    {
		mygroups[i].type = htonl(JOINED);
		multicast_leave(&(mygroups[i++]));  /* Leave all multicast groups */
		mygroups[i].hi_part = 0;
	    }
	}

	myunicastid.type = htonl(JOINED);
  
	multicast_leave(&myunicastid);              /* Leave unicast group */
	if (!isnull_id(&commitid))
	{
	    commitid.type = htonl(JOINED);
	    multicast_leave(&commitid);
	}

	no_groups = 0;
    }
} /* end LeaveAllGroups() */
  
int check_mid (struct MID givenmid)
{
    Boolean found = FALSE;
  
#ifdef DEBUG
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "rpcs: NO_GROUPS: " << no_groups << "\n" << flush;
#endif
    for (int i = 0; (i < S_GROUPSIZE) && (!found); i++)
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "rpcs: mygroups[" << i << "]: <" << ntohl(mygroups[i].type) << ","
		     << ntohl(mygroups[i].hi_part) << "," << ntohl(mygroups[i].lo_part) << ">" << flush;
	debug_stream << " and givenmid: <" << ntohl(givenmid.type) << "," << ntohl(givenmid.hi_part) << ","
		     << ntohl(givenmid.lo_part) << " >\n" << flush;
#endif
            
	if ((givenmid . hi_part == mygroups[i] . hi_part) &&
	    (givenmid . lo_part == mygroups[i] . lo_part))
	    found = TRUE;
    }
    
    return ((found) ? SUCCEEDED : FAILED);
} /* end check_mid() */

int cleanupclist ()
{
  struct C_LIST_EL* pclist = NULL;
  Boolean           found;
  Int32              mypid;
  int               srvrindx, i, k;
  int               retry = FILESRETRY, rstat = FAILED;

 /* Open Clist */
  do
  {
      rstat = clistopen(pclist);
  } while ((rstat != SUCCEEDED) && (retry-- > 0));
  if (rstat != SUCCEEDED)
  {  /* Could not read clist after several attempts */
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "Error::rpcs: open failed. Errno " << errno << "\n" << flush;
#endif
      /* clist open failed, so simply "close" clist without writing anything back. */

      int openErr = _rpcerr;  // in case we encounter an error on close
      
      (void) clistclose();

      /* Diagnostics and exception handling if clist ops fail for terminate */

      if (openErr == OPENFAILED)
      {
	  error_stream << FATAL << "clist fatal error." << endl;
	  ArjunaFatal();
      }
      
      return(FAILED);
  }

  /* Find server entry */
  mypid = getpid();
  srvrindx = -1; found = FALSE;
  do
  {
    srvrindx++;
    found = ((pclist->s_desc[srvrindx] . process_id == mypid) ? TRUE : FALSE);
  }  while ((srvrindx < MAXSERVERS) && (!found));
 
  /* Clean up entries for server index found */
  reset_server_info(&(pclist->s_desc[srvrindx]));
  
  /* Find node entries for server */
  for (i = 0; i < NSTATION; i++)
  {
    if (pclist->client[i] . node != 0)
    { /* A node has been logged in */
      for (k = 0; k < MAXSERVERS; k++)
      {
	if (pclist->client[i] . s_indx[k] == srvrindx)
	  /* Server entry found, reset it */
	  pclist->client[i] . s_indx[k] = -1;
       /* Later: check if no more servers logged for that node, remove node entry */
      }
    }
  }
  
  /* Close clist */
  retry = FILESRETRY;  	rstat = FAILED;
  do
  {
      rstat = clistclose();
  } while ((rstat != SUCCEEDED) && (retry-- > 0));
  if (rstat != SUCCEEDED)
  {  /* Could not write clist after several attempts */
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "Error::rpcs: close failed. Errno " << errno << "\n" << flush;
#endif
    /* Diagnostics and exception handling if clist ops fail for terminate */
  }
  
  return(SUCCEEDED);
  
} /* end cleanupclist() */

#ifdef HAVE_INTSIG
int kill_server (int dummy DOTDOTDOT)
#else
void kill_server (int dummy DOTDOTDOT)
#endif
{
	cleanupclist();
	tidyUp();
	exit(0);
}

int removelink (struct MID *givenmid)
{
  /* This routine is invoked when the server receives a terminate request
   * from the client.  It checks for the appropriate client link to remove,
   * deletes it, if one is found, otherwise, it sends an error message to
   * the client
   */

  struct C_LIST_EL* pclist = NULL;
  Boolean           found;
  Int32              mypid;
  int               i, j;
  int               retry = FILESRETRY, rstat = FAILED, rcode;

  rcode = FAILED; /* Initialise return code for removelink */
  /* Open Clist */
  do
  {
      rstat = clistopen(pclist);
  } while ((rstat != SUCCEEDED) && (retry-- > 0));
  if (rstat != SUCCEEDED)
  {  /* Could not read clist after several attempts */

     int openErr = _rpcerr;  // in case we encounter an error on close
     
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "Error::rpcs: open failed. Errno " << errno << "\n" << flush;
#endif

     if (openErr == OPENFAILED)
	 error_stream << FATAL << "rpcserver::removelink - clist fatal error." << endl;
      
     return(FAILED);
    /* Diagnostics and exception handling if clist ops fail for terminate */
  }
  
  /* Find server entry */
  mypid = getpid();
  i = -1; found = FALSE;
#ifdef DEBUG
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "removelink(): looking for server processid: " << mypid << "\n" << flush;
#endif  
  do
  {
    i++;
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "removelink(): pid in clist: " << pclist->s_desc[i].process_id << "\n" << flush;
#endif  
    found = ((pclist->s_desc[i] . process_id == mypid) ?TRUE : FALSE);
  }  while ((i < MAXSERVERS) && (!found));

  if (!found)
  {
      Int32 currentCrashCount = crashcount;
      
      crashcount = -1;
      crashcount = getccount();
      
      if (currentCrashCount == crashcount)
	  rcode = FAILED;
      else
      {
#ifdef DEBUG
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "removelink(): manager has crashed leaving servers active - must terminate" << endl;
#endif	  
	  crashcount = currentCrashCount;
	  rcode = MNGRBOOT;
      }
  }
  else
  {
    j = -1;
    found = FALSE;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "removelink(): looking for groupid: " << flush;
    printgid(*givenmid);
#endif  

    do
    {
	j++;
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "removelink(): groupid in clist " << flush;
	printgid(pclist->s_desc[i].linkspath[j].groupid);
#endif  
	found = (identical_group(&(pclist->s_desc[i].linkspath[j].groupid), givenmid));
	
    } while ((j < MAXCLIENTS) && (!found));

    /*
     * If we cannot find the client in the clist then we assume that this is a retry for
     * a previoud (successfully completed) terminate request. Therefore we simply return
     * succeeded. If this is a spurious request from a client then this should still not
     * cause any problems.
     */
    
    if (!found)
    {
#ifdef DEBUG
	error_stream << WARNING
		     << "rpcserver: removelink - could not find client to removelink for. Assume terminate retry."
		     << endl;
#endif	
	rcode = SUCCEEDED;
    }
    else
    {
      /* Remove client link corresponding to group id to be terminated */
      pclist->s_desc[i] . no_links--;

      if (pclist->s_desc[i] . no_links == 0)
	rcode = CLEANUP;
      else
      {
	/* Clear rpcpath associated with given link */
	reset_path(&(pclist->s_desc[i].linkspath[j]));

	rcode = SUCCEEDED;
      }
    }
  }
  /* Close clist */
  retry = FILESRETRY;  	rstat = FAILED;
  do
  {
      rstat = clistclose();
  } while ((rstat != SUCCEEDED) && (retry-- > 0));
  if (rstat != SUCCEEDED)
  {  /* Could not write clist after several attempts */
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "Error::rpcs: close failed. Errno " << errno << "\n" << flush;
#endif
    /* Diagnostics and exception handling if clist ops fail for terminate */
      rcode = FAILED;
  }
  
  return(rcode);
} /* end removelink() */

void ack_client (struct MID *client, struct RPC_SN *seqno, Int32 code)
{
  struct mc_msghdr    req_buf;
  Int32                ack_invcode, rcode;
  int                 sretry;
  
#ifdef DEBUG
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "rpcs: about to ack_client\n" << flush;
#endif

  /* Build up acknowledgement for client */
  ack_invcode = htonl(code);
  
  /* Prepare buffers for sending acknowledgement to client */
  req_buf . mc_msg[0] . iov_base = (char *) seqno;
  req_buf . mc_msg[0] . iov_len = sizeof(struct RPC_SN);
  req_buf . mc_msg[1] . iov_base = (char *) &ack_invcode;
  req_buf . mc_msg[1] . iov_len = sizeof(ack_invcode);
  req_buf . msg_len = 2;
  
  /* send acknowledgement to client */
  sretry = SERVERRETRY;
  do
  {
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "rpcs: ackclient(): about to send ack\n" << flush;
#endif
    rcode = multicast_long_send(client, &req_buf);
  } while ((rcode != MC_OK)&&(--sretry > 0));
  
  /* Not necessary to wait for retry terminate message from client, 
   * If server is dead for retry, terminate will eventually timeout 
   * else server will leave a group it is not part of, will not decrement any
   * count and send another ack to client, so terminate operation is idempotent
   */
   
} /* end ack_client() */

void reset_buffer (MID clientID)
{
    for (int i = 0; i < MAXCLIENTS; i++)
    {
	if (match_clients(&(latest_work[i].client_address),
			  &clientID) == SUCCEEDED)
	{
	    // could also free up buffers
	    latest_work[i].client_address.lo_part = UNSET_type;
	    return;
	}
    }

#ifdef DEBUG    
    error_stream << WARNING << "rpcserver - could not find client buffer slot to free." << endl;
#endif
}

void init_clients ()
{
  for (int i = 0; i < MAXCLIENTS; i++)
  {
      latest_work[i].client_address.lo_part =  UNSET_type;
      latest_work[i].client_address.hi_part =  UNSET_type;
      latest_work[i].client_address.type =  UNSET_type;      
  }
} /* end init_client() */
   
int init_server (int argc, char **argv)
{
  struct MID           multi_addr;
  struct RPC_SN        n_lastrpcsn;
  struct MID           h_client_addr, n_client_addr;
  struct PATHS         *ptr, path[NSTATION];
  Int32                 client_port;
  Int32                 error;
  int                  f_clistfd, k, rcode = SUCCEEDED;
  register int         i;
  int sm_id;

  checkEnvironment();
  
  errno = _rpcerr = 0;
  interrupted = FALSE;
  ptr = path;

  if (argc < 10)
  {
      perror("Error: not enough arguments passed to init_server.");
      cerr << "Make sure this server is not started by hand, but by the manager." << endl;
      exit(0);
  }

  serverName = ::new char[::strlen(argv[0])+1];
  ::strcpy(serverName, argv[0]);
  
  /* Unpack call and Byte Swap */
  sscanf(argv[1], "%d", & n_client_addr . type);  /* Get client address */
  sscanf(argv[2], "%d", & n_client_addr . hi_part);
  sscanf(argv[3], "%d",  & client_port);
  n_client_addr . lo_part = client_port; 
  h_client_addr . type = ntohl(n_client_addr . type); 
  h_client_addr . hi_part = ntohl(n_client_addr . hi_part); 
  h_client_addr . lo_part = ntohl(n_client_addr . lo_part); 

  sscanf(argv[4], "%d", & n_lastrpcsn.sec);       /* Get sequence number */
  sscanf(argv[5], "%d", & n_lastrpcsn.msec);
  h_lastrpcsn . sec = ntohl(n_lastrpcsn . sec); 
  h_lastrpcsn . msec = ntohl(n_lastrpcsn . msec); 
  
  sscanf(argv[6], "%d", & multi_addr . type); /* Get multicast identifier */
  sscanf(argv[7], "%d", & multi_addr . hi_part);
  sscanf(argv[8], "%d", & multi_addr . lo_part);
  sscanf(argv[9], "%d", & f_clistfd);

  sm_id = shmget(managerShm, sizeof(struct C_LIST_EL), 0666);
  clistfd = (char*)shmat(sm_id, 0, 0);

  if (log_server(clistfd) != SUCCEEDED)
  { 
      error = SYSTEMERROR;
      _rpcerr = errno;
      senderror(&n_client_addr, error);
      return(DIE);
  }

  init_clients();
  for (i = 0; i < S_ACTGRPSIZE; i++)
    getnullmid(&(grouplist[i]));

  if (rcode == SUCCEEDED)
  {
    /* Initialise server groups */
    no_groups = 0;
    no_initgroups = 1;
    for (k = 0; k < S_GROUPSIZE; k++)
	mygroups[k].hi_part = 0;

    join_grp(&multi_addr);
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "Init_Server: joined multiaddr: " << flush;
    printgid(multi_addr);
#endif

    sendmyport(&n_client_addr, myunicastid, myaddress);

    if (!RAJDOOT_USE_BROADCAST)
	myunicastid = myaddress;
    
    /* Can be done before joining group, when using RPC primitive */
    signal(INTERRUPT_SERVER, handle_interrupt);
    signal(KILL_SERVER, kill_server);

    return(SUCCEEDED);
  }
  else
  {
    _rpcerr = LOGERROR;
    senderror(&n_client_addr, GW_ERROR);
    return(DIE);
  }
} /* end Init_Server() */

int check_client (struct RPC_SN *seqno, struct MID *source_address)
{
  /* Check whether user rpc received is a retry - if so, retransmit old
   * result; If not, save client's entry (sequence number and address) for
   * subsequent checks for retransmission
   */
  
  struct GROUPS dest;
  Boolean found;
  int     indx;

  /* Byte swap sequence number */
  seqno -> sec = ntohl(seqno -> sec);  
  seqno -> msec = ntohl(seqno -> msec);  

#ifdef DEBUG
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "check_clients: seqno received was <" << seqno->sec << ","
	       << seqno->msec << ">\n" << flush;
#endif
  
  /* Has client made previous calls? */
  indx = 0; found = FALSE;
  do
  {
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "match_clients: trying to match " << flush;
      printgid(latest_work[indx].client_address);
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "with " << flush;
      printgid(*source_address);
#endif
    if (match_clients(&(latest_work[indx].client_address),
		      source_address) == SUCCEEDED)
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "found\n" << flush;
#endif      
      found = TRUE;
    }
    else
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "not found\n" << flush;
#endif      
      indx++;
    }
  }   while ((indx < MAXCLIENTS) && (!found));
  
  if (found)
  {
    if (identical_sn(seqno, &(latest_work[indx].seq_no)))
    { /* This is a retry;, send previous result to client */
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "rpcs: this was a retry\n" << flush;
#endif
      
      if (latest_work[indx].result.rpc_code == ZEROCODE)  
      {
	/* Call must have timed out, since it was received, but no record
	 * of a reply exists - and since server has returned to receive
	 * requests, it must not have completed the call, due to the
	 * occurrence of a timeout
	 */
#ifdef DEBUG
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "rpcs: Call has not yet been fully executed\n" << flush;
#endif
        return(FAILED);
      }
      else
      {
#ifdef DEBUG
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "rpcs: retransmitting result\n" << flush;
#endif
	dest.groups_type = SRVR_type;
	dest.grp_srvr.srvr.sr_idtype = UID_type;
	dest.grp_srvr.srvr.sr_id = latest_work[indx].client_address;
      	
        sendresult(&dest,&(latest_work[indx].result), TRUE);
        /* SendResult checks latest_work for seqno to send */
      
        return(DONE);
      }
    }
    else
    { /* This is not a retry */
      /* Set new client details */
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "check_clients: this call is not a retry\n" << flush;
#endif      
      latest_work[indx].seq_no = *seqno;
      latest_work[indx].result.rpc_code = ZEROCODE;
      
      return(SUCCEEDED);
    }
  }
  else
  { /* client not found */
    /* Insert client in list */

#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "match_client: client not found - inserting client\n" << flush;
#endif      
    
    indx = 0; found = FALSE;
    do
    {
      if (latest_work[indx].client_address.lo_part == UNSET_type)
        found = TRUE;
      else
	indx++;
    } while ((indx < MAXCLIENTS) && (!found));
    
    if (!found) /* Must insert entry further up the list */
    {
	error_stream << WARNING << "rpcserver: could not find slot to insert client in." << endl;
	return FAILED;
    }
      
    /* Insert client entry */
    latest_work[indx].client_address = *source_address;
    latest_work[indx].seq_no = *seqno;
    latest_work[indx].result.rpc_code = ZEROCODE;
    
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "check_clients: seqno set to <" << latest_work[indx].seq_no.sec
		   << "," << latest_work[indx].seq_no.msec << ">\n" << flush;
#endif      

    return(SUCCEEDED);

  } /* end else */
} /* end check_client() */


int getwork (struct GROUPS *client_address, struct RPC *srvr_work, struct DEADLINE *deadlines, struct FILTER_INFO *filter)
{
  struct mc_msghdr    work, req_buf;
  struct mc_sg_filter call;
  struct MID          givenmid;
  struct RPC_SN       seqno;
  Boolean             invalidOpcode = FALSE;
  int                 (*temp_filt)(struct mc_sg_filter *, Boolean);
  int                 t_servers;  
  Boolean             supplied_filter_info, supplied_filter;  
  unsigned            tmp_deadline, timeout;
  Int32                n_received, opn_code;
  Int32                rs_code, mypid, buff_size;
  int                 rcode;
  char                *start_buff;
  
  if ((supplied_filter_info = ((filter != NULL)?TRUE : FALSE)))
    supplied_filter = ((filter -> filt_name != NULL) ? TRUE : FALSE);
  mypid = getpid();

  /* Set up buffers for receiving RPC */
  work . mc_msg[0] . iov_len  = sizeof(struct RPC_SN);
  work.mc_msg[0].iov_base = 0;
  work . mc_msg[1] . iov_len  = sizeof(Int32);
  work.mc_msg[1].iov_base = 0;
  work . mc_msg[2] . iov_len  = sizeof(struct MID);
  work.mc_msg[2].iov_base = 0;
  work . mc_msg[3] . iov_len  = sizeof(srvr_work -> rpc_code);
  work.mc_msg[3].iov_base = 0;
  work . mc_msg[4] . iov_len  = 0;
  work.mc_msg[4].iov_base = 0;
  work . mc_msg[5] . iov_len  = 0;
  work.mc_msg[5].iov_base = 0;
  work . mc_msg[6] . iov_len  = 0;
  work.mc_msg[6].iov_base = 0;
  work . msg_len = 7;
  call . next_fs = NULL;
  call . mc_msg = &work;

  for(;;)
  {
    /* Set global variables to be shared with filter */
    if (supplied_filter_info)
    {
      s_successes_expected = filter->no_successes;
      s_compare_crit = (((filter->compare != compare_OPCODE) &&
		       (filter->compare != compare_BUFFERS) &&
		       (filter->compare != compare_BOTH))
		      ? compare_NONE : filter->compare);
      s_total_received = 0;
      s_filt_replygrp = &(filter->expected_group->grp_srvr.group);
      init_filtgrp(s_filt_replygrp, &t_servers);
    }

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "rpcserver: waiting to receive in getwork()\n" << flush;
#endif

    if (!supplied_filter_info)
      temp_filt = NULL;
    else
      if (supplied_filter)
	temp_filt = filter->filt_name;
      else
	temp_filt = default_server_filter;
    
    /* Enable ORPHAN_TIMER signal from idle timeout */
    
    do
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "receiving on <" << ntohl(myunicastid.type) << "," << ntohl(myunicastid.hi_part) << ","
	    << ntohl(myunicastid.lo_part) << ">\n" << flush;
#endif

      n_received = multicast_long_receive(&myunicastid, &call,
					  temp_filt, IDLE_TO);

      if (n_received < 0)
      {
	  if (n_received == ETIMEOUT) // only do this if a timeout has occurred with no message received.
	      ResetPOFlag(TRUE);
	  else
	  {
	      if (work.mc_msg[0].iov_base) // free allocated message buffers if any.
	      {
		  free(work.mc_msg[0].iov_base);
		  for (int ll = 0; ll < 5; ll++)
		      work.mc_msg[ll].iov_base = 0;
	      }
	      if (work.mc_msg[5].iov_base)
	      {
#ifndef GCC_ARRAY_DELETE_BUG
		  delete [] work.mc_msg[5].iov_base;
#else
		  delete work.mc_msg[5].iov_base;
#endif
		  work.mc_msg[5].iov_base = 0;
	      }
	      if (work.mc_msg[6].iov_base)
	      {
#ifndef GCC_ARRAY_DELETE_BUG
		  delete [] work.mc_msg[6].iov_base;
#else
		  delete work.mc_msg[6].iov_base;
#endif
		  work.mc_msg[6].iov_base = 0;
	      }
	  }
      } 

#ifdef DEBUG
	if (n_received < 1)
	{
	    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	    debug_stream << "rpcserver: receive failed: " << errno << "\n" << flush;
	}
#endif
    } while (n_received < 1);

    if (::p_orphan)
      ResetPOFlag(FALSE); /* Reset potential orphan flag in clist */
    
    /* Cast buffers received */
    seqno = * (struct RPC_SN *)      work.mc_msg[0].iov_base;
    opn_code = * (Int32 *)            work.mc_msg[1].iov_base;
    givenmid = * (struct MID *)      work.mc_msg[2].iov_base;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "rpcserver: received opcode: " << ntohl(opn_code) << "\n" << flush;
#endif

    opn_code = ntohl(opn_code);
    switch(opn_code)
    {
    case INITIALISE:
    case INT_INITIALISE:

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "received opcode " << htonl(opn_code) << " (INIT)" << INITIALISE
		     << " (INIT_)" << INT_INITIALISE << "\n" << flush;
	debug_stream << "rpcs: about to join group <" << ntohl(givenmid.type) << "," << ntohl(givenmid.hi_part)
		     << "," << ntohl(givenmid.lo_part) << ">\n" << flush;
	debug_stream << " and reply " << flush;
	printgid(call.source);
#endif

      join_grp(&givenmid);
      no_initgroups++;

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "Server " << getpid() << ": number of links " << no_initgroups << endl;
#endif
	
      /* Acknowledge server */
      rs_code = htonl(INIT_DONE);  /* Acknowledgement code */
 
      /* Build request buffer for transmission */
      /* Sequence number sent is the same as was revceived for request */
      req_buf . mc_msg[0] . iov_base = (char *) &seqno;
      req_buf . mc_msg[0] . iov_len = sizeof(struct RPC_SN);
      req_buf . mc_msg[1] . iov_base = (char *) &rs_code;
      req_buf . mc_msg[1] . iov_len = sizeof(rs_code);
      req_buf . msg_len = 2;

      /* Receiver will retry if ack not received */
      multicast_long_send(&(call.source), &req_buf);

      break;
      
    case JOINGRP:
    case INT_JOINGRP:
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "received opcode " << htonl(opn_code) << " (JOIN)" << JOINGRP
		     << " (INIT_)" << INT_JOINGRP << "\n" << flush;
#endif

      /* do not join_grp - which keeps links for initiate */
      join_grp(&givenmid);

      ack_client(&(call.source), &seqno, SUCCEEDED);
      
      break;
      
    case LEAVEGRP:
    case INT_LEAVEGRP:
      
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "received opcode " << htonl(opn_code) << " (LEAVE)" << LEAVEGRP
		     << " (INIT_)" << INT_LEAVEGRP << "\n" << flush;
	debug_stream << "rpcs: received leave group message\n" << flush;
#endif

      if (check_mid(givenmid) == FAILED)
      {
#ifdef DEBUG
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "rpcs: must ack client with error code\n" << flush;
#endif
        ack_client(&(call . source), &seqno, SRVR_ERROR);
      }
      else
      {
	  givenmid.type = htonl(JOINED);
	  
	  /* do not leave_grp which removes links */
	  leave_grp(&givenmid);
	  no_initgroups--;

#ifdef DEBUG
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "rpcs: must ack client with done code\n" << flush;
#endif
	  ack_client(&(call . source), &seqno, TERM_OK);
      }
      break;
	
    case TERMONESRVR:
    case INT_TERMONESRVR:

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "received opcode " << htonl(opn_code) << " (TERM)" << TERMONESRVR
		     << " (INIT_)" << INT_TERMONESRVR << "\n" << flush;
	debug_stream << "rpcs: received terminate one message\n" << flush;
#endif

      if ((rcode = removelink(&givenmid)) == FAILED)
	  rcode = removelink(&givenmid);  /* Retry */
      switch (rcode)
      {
      case FAILED:

#ifdef DEBUG
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "rpcs: removelink failed\n" << flush;
#endif	
	ack_client(&(call . source), &seqno, SRVR_ERROR);
	break;

      case SUCCEEDED:
	
#ifdef DEBUG
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "rpcs: removelink ok\n" << flush;
#endif	
	ack_client(&(call . source), &seqno, TERM_OK);
	if (check_mid(givenmid) != FAILED) 
	{
	  givenmid.type = htonl(JOINED);
	  leave_grp(&givenmid);
	  no_initgroups--;	  
	}

        reset_buffer(call.source);
	  
	break;

      case MNGRBOOT:

#ifdef DEBUG
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "rpcserver: about to terminate server (MNGRBOOT)\n" << flush;
#endif
	  ack_client(&(call . source), &seqno, TERM_OK);
	  // nothing to cleanup

	  if (work.mc_msg[0].iov_base) // free allocated message buffers if any.
	  {
	      free(work.mc_msg[0].iov_base);
	      work.mc_msg[0].iov_base = 0;
	  }
	  if (work.mc_msg[5].iov_base)
	  {
#ifndef GCC_ARRAY_DELETE_BUG
	      delete [] work.mc_msg[5].iov_base;
#else
	      delete work.mc_msg[5].iov_base;
#endif	  
	      work.mc_msg[5].iov_base = 0;
	  }
	  if (work.mc_msg[6].iov_base)
	  {
#ifndef GCC_ARRAY_DELETE_BUG
	      delete [] work.mc_msg[6].iov_base;
#else
	      delete work.mc_msg[6].iov_base;
#endif	  
	      work.mc_msg[6].iov_base = 0;
	  }
	  tidyUp();
	  exit(0);
	  break;

      case CLEANUP:

#ifdef DEBUG
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "rpcserver: about to terminate server (CLEANUP)\n" << flush;
#endif

	ack_client(&(call . source), &seqno, TERM_OK);
	LeaveAllGroups();
	cleanupclist();
	if (work.mc_msg[0].iov_base) // free allocated message buffers if any.
	{
	  free(work.mc_msg[0].iov_base);
	  work.mc_msg[0].iov_base = 0;
	}
	if (work.mc_msg[5].iov_base)
	{
#ifndef GCC_ARRAY_DELETE_BUG
	  delete [] work.mc_msg[5].iov_base;
#else
	  delete work.mc_msg[5].iov_base;
#endif	  
	  work.mc_msg[5].iov_base = 0;
	}
	if (work.mc_msg[6].iov_base)
	{
#ifndef GCC_ARRAY_DELETE_BUG
	  delete [] work.mc_msg[6].iov_base;
#else
	  delete work.mc_msg[6].iov_base;
#endif	  
	  work.mc_msg[6].iov_base = 0;
	}
	  tidyUp();
	exit(0);
	break;
      } /* end switch */
      break;
    
    case TERMALLSRVRS:
    case INT_TERMALLSRVRS:
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "rpcs: received terminate all message (TERMALL)\n" << flush;
#endif
      ack_client(&(call . source), &seqno, TERM_OK);
      LeaveAllGroups();
      cleanupclist();
      if (work.mc_msg[0].iov_base) // free allocated message buffers if any.
      {
	free(work.mc_msg[0].iov_base);
	work.mc_msg[0].iov_base = 0;
      }
      if (work.mc_msg[5].iov_base)
      {
#ifndef GCC_ARRAY_DELETE_BUG
	delete [] work.mc_msg[5].iov_base;
#else
	delete work.mc_msg[5].iov_base;
#endif	
	work.mc_msg[5].iov_base = 0;
      }
      if (work.mc_msg[6].iov_base)
      {
#ifndef GCC_ARRAY_DELETE_BUG
	delete [] work.mc_msg[6].iov_base;
#else
	delete work.mc_msg[6].iov_base;
#endif	
	work.mc_msg[6].iov_base = 0;
      }
	tidyUp();
      exit(0);
      break;
      
    case USER_RPC:

      start_buff = (char*) work.mc_msg[4].iov_base;
      buff_size =  work.mc_msg[4].iov_len;
      set_rpcinfo(srvr_work,
		  * (Int32 *) work.mc_msg[3].iov_base,
		  work.mc_msg[5].iov_len,
		  (char*) work.mc_msg[5].iov_base, 
		  work.mc_msg[6].iov_len,
		  (char*) work.mc_msg[6].iov_base);

      /* Check for retries and insert client entry */
      if (check_client(&seqno, &(call . source)) == SUCCEEDED)
      {
        /* Set call's deadline */
        timeout = (int) ntohl(seqno . t_out);
        seqno . retry = ntohl(seqno . retry);
        if (timeout <= 0)
          tmp_deadline = 1000;  /* deadline in centiseconds */
        else
          tmp_deadline = timeout;
        deadlines->timeout = tmp_deadline;
	
	/* Unpack MID and join/leave relevant groups */
	get_groups(grouplist, start_buff, buff_size);

#ifdef USEONDEADLINE      
	if (deadlines->reset_server)
	{
	  signal(ABORT, ondeadline);
	  setalarm(tmp_deadline);
	}
#endif

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "rpcs: deadline set to " << deadlines->timeout << " or "
	    << tmp_deadline << "\n" << flush;
#endif
        /* Set param: client_address - address type for client: MID */
	client_address->groups_type = SRVR_type;
	client_address->grp_srvr.srvr.sr_idtype = UID_type;
	client_address->grp_srvr.srvr.sr_id = call.source;

	interrupted = FALSE;

	free(work.mc_msg[0].iov_base);
	work.mc_msg[0].iov_base = 0;

        return(SUCCEEDED);
      }
      else
	opn_code = FAILED;
      
      break;
      
    default:
	invalidOpcode = TRUE;
	error_stream << WARNING << "rpcserver: received invalid opcode " << opn_code << " in getwork" << endl;
	break; /* Continue to receive in GetWork */

    } /* end switch */

    if (!invalidOpcode)
    {
	if (((interrupted) && (opn_code >= INITIALISE)) ||
	    ((!interrupted) && (opn_code == USER_RPC)))
	{
#ifdef DEBUG
	    if (interrupted)
	    {
		debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
		debug_stream << "interrupted: " << interrupted << " and opn_code:" << opn_code << "\n" << flush;
	    }
#endif
	    interrupted = FALSE;

	    free(work.mc_msg[0].iov_base);
	    work.mc_msg[0].iov_base = 0;

	    return(SUCCEEDED);
	}
    }
    
    /* Otherwise return to receive in GetWork */

    if (work.mc_msg[0].iov_base) // free allocated message buffers if any.
    {
      free(work.mc_msg[0].iov_base);
      work.mc_msg[0].iov_base = 0;
    }
    if (work.mc_msg[5].iov_base)
    {
#ifndef GCC_ARRAY_DELETE_BUG
      delete [] work.mc_msg[5].iov_base;
#else
      delete work.mc_msg[5].iov_base;
#endif      
      work.mc_msg[5].iov_base = 0;
    }
    if (work.mc_msg[6].iov_base)
    {
#ifndef GCC_ARRAY_DELETE_BUG
      delete [] work.mc_msg[6].iov_base;
#else
      delete work.mc_msg[6].iov_base;
#endif      
      work.mc_msg[6].iov_base = 0;
    }
  } /* end for */

} /* end GetWork() */  
