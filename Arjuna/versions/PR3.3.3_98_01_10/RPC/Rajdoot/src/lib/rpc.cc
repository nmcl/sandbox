/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: rpc.cc,v 1.37.6.1 1996/10/10 12:26:21 ngdp Exp $
 */

/*
 *  Client Primitives INITIATE, JOIN_GROUP, NEW_GROUP, LEAVE_GROUP,
 *  TERMINATE and MULTI_CALL
 */

#ifndef IOSTREAM_H_
#   include <System/iostream.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#   include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#   include <Common/Error.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef STDLIB_H_
#   include <System/stdlib.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef FCNTL_H_
#  include <System/fcntl.h>
#endif

#ifndef MALLOC_H_
#  include <System/malloc.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SYS_FILE_H_
#  include <System/sys/file.h>
#endif

#ifndef SYS_TIMEB_H_
#  include <System/sys/timeb.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef RAJDOOT_RPCUSER_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif

#ifndef RPC_H_
#  include "rpc.h"
#endif

extern Int32 getmyhost(), getccount();
extern void checkEnvironment();

extern Int32 managerAddr;

int getpath (pid_t, struct PATHS *);
int group_opn (struct GROUPS *, struct GROUPS *,
	       struct GROUP_INFO *, Int32, struct OBJECT_DESC *);
int prodserver (Int32, struct MID, struct MID, struct MID, struct OBJECT_DESC *);

static const char RCSid[] = "$Id: rpc.cc,v 1.37.6.1 1996/10/10 12:26:21 ngdp Exp $";

extern char* clistfd;

Int32 crashcount = -1;
Boolean useActionID = FALSE;
struct MID _actionID;

static struct MID client; /* client groupid is not a param to primitives */

/* Information shared between multi_call and the filter procedure */

struct GROUP_INFO *filt_replygrp; /* set by multi_call and filter */
struct RPC_SN call_seqno;         /* Set by multi_call */
int successes_expected;           /* Set by multi_call */
int compare_crit;                 /* Set by multi_call */
struct RPC correct_rpc;           /* Set by filter */
int total_received;               /* Set by filter */
struct HEAD_RPCLIST rpcstruct;

int default_filter (struct mc_sg_filter *new_msg, Boolean timeout)
{
  /* Attempts to receive messages until either the call's deadline expires
   * or all servers in the expected group reply - note that if no of
   * successes expected is met even before the deadline, it continues to
   * receive - since otherwise, there will be outstanding messages held in
   * the system which must be received and discarded, the next time the
   * user attempts to use any of the primitives
   */
  
  struct RPC_SN result_seqno;
  static int still_expected = 1;

  result_seqno = * (struct RPC_SN *)  new_msg->mc_msg->mc_msg[0].iov_base;

#ifdef DEBUG
  debug_stream.lock();
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "default filter: received seqno <" << result_seqno.sec << ","
	       << result_seqno.msec << ">" << flush;

  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << " expecting <" << call_seqno.sec << "," << call_seqno.msec << ">\n" << flush;

  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "default filter: and source address :";
  printgid(new_msg->source);
  debug_stream << flush;

  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "opcode: " << * (Int32 *) new_msg->mc_msg->mc_msg[1].iov_base;
  debug_stream << flush;

  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << " buffer1: " << new_msg->mc_msg->mc_msg[2].iov_base << " buffer2: "
	       << new_msg->mc_msg->mc_msg[3].iov_base << "\n" << flush;
  debug_stream.unlock();
#endif
  
  if ((identical_sn(&call_seqno, &result_seqno)) &&
      (dec_filtgrp(filt_replygrp, new_msg->source) == SUCCEEDED))
  {
    total_received++;
    if (total_received == 1)
      still_expected = successes_expected;

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "default filter: successes_expected :" << successes_expected <<
	", still expected " << still_expected << "\n" << flush;
    debug_stream.unlock();
#endif
    
    if (compare_crit != compare_NONE)
    {
      /* Build list of rpcs received for subsequent voting, and
	 if necessary, initialise head of list of rpcs received */
      if (total_received == 1)
	rpcstruct.head_rpc = NULL;

      insert_newmsg(new_msg, &rpcstruct); 
    }

    if (--still_expected <= 0)
    {
      /* Required number of servers have replied, vote on server results
	 and complete call if successful; otherwise, continue to receive
	 until timeout expires or all servers respond */

      if (compare_crit == compare_NONE)
      {
	set_rpcinfo(&correct_rpc,
		    * (Int32 *) new_msg->mc_msg->mc_msg[1].iov_base,
		    new_msg->mc_msg->mc_msg[2].iov_len,
		    (char*) new_msg->mc_msg->mc_msg[2].iov_base, 
		    new_msg->mc_msg->mc_msg[3].iov_len,
		    (char*) new_msg->mc_msg->mc_msg[3].iov_base);
	return(EPRINTSIZE); /* Successful termination, stop receiving */
      }
      else
      {
	if (vote_on_results(&rpcstruct, successes_expected,
			    compare_crit, &correct_rpc) != SUCCEEDED)
	{
	  if (isempty_filtgrp(filt_replygrp))
	  {
	    /* All servers in group have replied, but voting did not succeed */
	    remove_rpc_list(&rpcstruct, TRUE); 
	    return(ETIMEOUT); /* Failure termination, stop receiving */
	  }
	  else
	  {
	    /* Client still needs to receive further calls to succeed */
	    if (timeout)
	    {
	      remove_rpc_list(&rpcstruct, TRUE);
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
    if (compare_crit != compare_NONE)
	remove_rpc_list(&rpcstruct, TRUE); 

    return(ETIMEOUT); /* Failure termination, stop receiving */
  }
  else
    return(CARRYON); /* Carry on receiving */

} /* end default_filter() */

void setActionID (struct MID actID)
{
    _actionID.type = actID.type;
    _actionID.hi_part = actID.hi_part;
    _actionID.lo_part = actID.lo_part;
    useActionID = TRUE;
}

void getActionID (struct MID& actID)
{
    actID.type = _actionID.type;
    actID.hi_part = _actionID.hi_part;
    actID.lo_part = _actionID.lo_part;
    useActionID = FALSE;
}
    
void reset_init_args (struct OBJECT_DESC *objects, struct GROUPS *group, int *successes, int group_number)
{
  int indx;
  
  *successes = 0;
  if (group_number == 1)
  { /* unicast */
    objects[0].grps_id.groups_type = UNSET_type;
    group->groups_type = UNSET_type;
  }
  else
  {
    /* Reset each server id */
    for (indx = 0; indx < group_number; indx++)
      objects[indx].grps_id.groups_type = UNSET_type;

    /* Reset group id */
    group->groups_type = UNSET_type;
    rmall_supergrps(&(group->grp_srvr.group));
  }

} /* end reset_init_args */

int initiate (struct OBJECT_DESC *objects, struct GROUPS *newgroup, int *successes)
{
  static Boolean initially = TRUE;
  struct mc_msghdr req_buf, res_buf;
  struct mc_sg_filter ack;
  struct MID multithem, actid, srvr_reply;
  struct MID unithem, sender;
  struct MID unimanager;
  struct RPC_SN request_sn, rq_sn, rs_sn;
  struct UNIQUEID uid;
  struct PATHS path[NSTATION];
  Boolean inval, undo, wrong_call = TRUE, machineAlive;
  Int32   request_code, rq_code, rs_code;
  Int32   pathlength, serverport, packed_size;
  pid_t  pid = getpid();
  int    i, group_number, opn_code = NOARG;
  int    indx = 0, rcode, xrstat, xretry, rretry; 
  int    no_failures, max_failures, expectedSize = 0;
  char   *buff, *nextbuff, *packed_path;
  Uint32 timeTaken = 0;

  checkEnvironment();
  
  Int32 manager_port = managerAddr;

  rs_sn.sec = rs_sn.msec = 0;

  /* Check for invalid arguments 
     Assumption: as long as successes criterion is met, invalid arguments are
     not reported.  Default for 'successes' = all, which is used if an invalid
     argument is given */

  while (objects[indx].destn_host != 0)
    indx++;
  if (indx == 0)  /* empty object list */
  {
      error_stream << WARNING << " Rajdoot:initiate : Attempt to initiate null object list\n";
      return(EMPTY_OBJECT_LIST);
  }

  /* Get RPC PATH */

  if ((pathlength = getpath(pid, &path[0])) < 0)
      return(OPENFAILED);
  if (pathlength >= NSTATION)
    return(PATH_TOO_LONG);

  if ((path[pathlength].node = getmyhost()) == 0L)
  {
      error_stream << WARNING << "Rajdoot:initiate : could not get hostname" << endl;
      return FAILED;
  }

  if ((path[pathlength].c_count = getccount()) < 0L)
  {
      error_stream << WARNING << "Rajdoot:initiate : could not get read c_count" << endl;
      return FAILED;
  }

  packed_size = sizeof(struct PATHS) * (pathlength + 1);
  packed_path = ::new char [packed_size];

  
  /* Initialise buffers for packing rpc-path */
  
  buff = nextbuff = packed_path;
  
#ifdef DEBUG
  debug_stream.lock();
  
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "\nRPC:INITIATE() PATH IS: " << flush;
  debug_stream.unlock();
  
#endif

  /* Pack byte swapped rpc-path */

  i = 0;
  while (i < pathlength+1)
  {
#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "<" << path[i].node << "," << path[i].c_count << "> " << flush;
      debug_stream.unlock();
      
#endif
    nextbuff = pack_Int32(htonl(path[i].node), buff);
    buff = nextbuff;
    nextbuff = pack_Int32(htonl(path[i].c_count), buff);
    buff = nextbuff;
    i++;
  }
  
  group_number = indx;
  
  /* Checking for null object name */
  inval = FALSE;
  indx = 0;
  while ((!inval) &&  (objects[indx].destn_host != 0))
  {
    inval =  (objects[indx].object_name == NULL) ? TRUE : FALSE;
    indx++;
  }

  // must get client port if this is the first time OR this is a forked process

  if (initially)
  {
    multicast_get_mid(&client); /* Client's port for communication */
    initially = FALSE;
  }
      
  rcode = FAILED;  /* Initialise return code for initiate */

  rq_code = GET_WORKER;     /* Request Operation Code */
  request_code = htonl(rq_code); 

  if (useActionID)
      getActionID(actid);
  else
      getnullmid(&actid);
  
  multicast_new_mid(&multithem);  /* generate a group identifier for initiated group */
  
#ifdef DEBUG
  debug_stream.lock();
  
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "rpc: initiate multiaddr: ";
  printgid(multithem);
  debug_stream << flush;
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << " and actid ";
  printgid(actid);
  debug_stream << flush;
  debug_stream.unlock();
  
#endif

  /* Build GETWORKER Request */
  req_buf.mc_msg[0].iov_base = (char *) &request_sn;
  req_buf.mc_msg[0].iov_len = sizeof(struct RPC_SN);
  req_buf.mc_msg[1].iov_base = (char *) &request_code;
  req_buf.mc_msg[1].iov_len = sizeof(request_code);
  req_buf.mc_msg[2].iov_base = (char *) &multithem;
  req_buf.mc_msg[2].iov_len = sizeof(struct MID);
  req_buf.mc_msg[3].iov_base = (char *) &actid;
  req_buf.mc_msg[3].iov_len = sizeof(struct MID);
  req_buf.mc_msg[4].iov_base = (char *) &uid;
  req_buf.mc_msg[4].iov_len = sizeof(struct UNIQUEID);
  req_buf.mc_msg[5].iov_base = (char *) &opn_code;
  req_buf.mc_msg[5].iov_len = sizeof(opn_code);  
  req_buf.mc_msg[6].iov_base = (char *) &serverport;
  req_buf.mc_msg[6].iov_len = sizeof(serverport);
  req_buf.mc_msg[7].iov_len = OBJECT_NAME_SIZE;
  req_buf.mc_msg[8].iov_base = (char *) packed_path;
  req_buf.mc_msg[8].iov_len = (int) packed_size;

  req_buf.msg_len = 9;

  /* Prepare Result Buffer */
  res_buf.mc_msg[0].iov_len = sizeof(struct RPC_SN);
  res_buf.mc_msg[0].iov_base = 0;
  rs_code = ZEROCODE;
  res_buf.mc_msg[1].iov_len = sizeof(rs_code);
  res_buf.mc_msg[1].iov_base = 0;
  res_buf.mc_msg[2].iov_len = sizeof(struct MID);
  res_buf.mc_msg[2].iov_base = 0;
  res_buf.mc_msg[3].iov_len = sizeof(struct MID);
  res_buf.mc_msg[3].iov_base = 0;
  res_buf.msg_len = 4;
  ack.next_fs = NULL;
  ack.mc_msg = &res_buf;
  expectedSize = sizeof(struct RPC_SN) + sizeof(rs_code) + 2*sizeof(struct MID);

  unithem.type = htonl(UID_type);
  unithem.lo_part = htonl(manager_port);

#ifdef DEBUG
  debug_stream.lock();
  
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "Initiate: must initiate " << group_number << " objects\n" << flush;
  debug_stream.unlock();
  
#endif

  if (group_number == 1)
  {    /* UNICAST */
#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "initiate: must unicast\n" << flush;
      debug_stream.unlock();
      
#endif
    indx = 0;
    unithem.hi_part = objects[indx].destn_host;
    objects[indx].grps_id.grp_srvr.srvr.sr_id.lo_part = htonl(-1);
    objects[indx].grps_id.grp_srvr.srvr.sr_id.hi_part = htonl(-1);	    
    objects[indx].grps_id.grp_srvr.srvr.sr_id.type = htonl(UNSET_type);
    unimanager = unithem;

    /* Set unique id for object */
    
    uid = objects[indx].u_id;
    
#ifdef DEBUG
    debug_stream.lock();
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "rpc: trying to send to manager at ";
    printgid(unithem);
    debug_stream << flush;
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "action is ";
    printgid(actid);
    debug_stream << flush;
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "uid <" << uid.host << "," << uid.sec << "," << uid.msec << ">\n" << flush;
    debug_stream.unlock();
#endif

    /* Get a Sequence Number */
    getsn(&rq_sn);    
    request_sn.sec = htonl(rq_sn.sec);    /* Byte Swap */
    request_sn.msec = htonl(rq_sn.msec);

    /* Objects on different hosts may have unique names */
    req_buf.mc_msg[7].iov_base = (char *) objects[indx].object_name;

    rretry = GWRETRY;
    xrstat = FAILED;
    machineAlive = TRUE;
      
    do
    {
      xretry = GWRETRY;
      xrstat = FAILED;

      wrong_call = TRUE;
      inval = FALSE;

      do
      {
        xrstat = multicast_long_send(&unithem, &req_buf);
      } while ((xrstat != MC_OK) && (--xretry > 0));
      if (xrstat == MC_OK)
      {
         xrstat = FAILED;
#ifdef DEBUG
	 debug_stream.lock();
	 
	 debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	 debug_stream << "rpc: must UNICAST - send ok waiting to receive on ";
	 printgid(client);
         debug_stream << flush;
	 debug_stream.unlock();
	 
#endif
	 do
	 {
	     xrstat = multicast_long_receive(&client, & ack, 0, GWTIMEOUT); 
	 } while ((xrstat == ENOMESSAGE) || ((xrstat > 0) && (xrstat != expectedSize)));
      }
#ifdef DEBUG
      else
      {
	  debug_stream.lock();
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "rpc: send failed: errno " << errno << "\n" << flush;
	  debug_stream.unlock();
      }
#endif

      if ((xrstat > 0) && (xrstat != expectedSize))
	  xrstat = ENOMESSAGE;

      if (xrstat > 0)
      {
	  /* Cast result buffer */
	  rs_sn = * (struct RPC_SN *)   res_buf.mc_msg[0].iov_base;
	  rs_code = * (Int32 *)          res_buf.mc_msg[1].iov_base;

	  wrong_call = (((!identical_sn(&rs_sn, &request_sn)) ||
			 (ntohl(rs_code) != GW_OK)) ? TRUE : FALSE);

	  if (ntohl(rs_code) != GW_OK)
	  {
	      /*
	       * The first two error codes should make use stop immediately
	       * as they are unlikely to change between retries. Any other
	       * error should cause a retry as it may be transient.
	       */
	      
	      switch (ntohl(rs_code))
	      {
	      case NOSERVER:
		  {
		      inval = TRUE;
		      error_stream << WARNING << "initiate error - no server code present." << endl;
		  }
		  break;
	      case SERVERACCESS:
		  {
		      inval = TRUE;
		      error_stream << WARNING << "initiate error - server access permission problem." << endl;
		  }
		  break;
	      default:
		  {
		      inval = FALSE;
		      error_stream << WARNING << "initiate error received from server side: opcode " << ntohl(rs_code) << endl;
		  }
		  break;
	      }
	  }

	  sender.lo_part = sender.hi_part = htonl(-1);
	  
	  if (!wrong_call)
	  {
	      srvr_reply = * (struct MID *) res_buf.mc_msg[2].iov_base;
	      sender = * (struct MID *)     res_buf.mc_msg[3].iov_base;
	  }
      }

      if (((wrong_call) || (xrstat < 0)) && (rretry > 1) && (!inval))
      {
	  if (res_buf.mc_msg[0].iov_base)
	  {
	      free(res_buf.mc_msg[0].iov_base);
	      res_buf.mc_msg[0].iov_base = 0;
	  }
	  if (res_buf.mc_msg[2].iov_base)
	  {
#ifndef GCC_ARRAY_DELETE_BUG
	      delete [] res_buf.mc_msg[2].iov_base;
#else
	      delete res_buf.mc_msg[2].iov_base;
#endif
	      res_buf.mc_msg[2].iov_base = 0;
	  }
	  if (res_buf.mc_msg[3].iov_base)
	  {
#ifndef GCC_ARRAY_DELETE_BUG
	      delete [] res_buf.mc_msg[3].iov_base;
#else
	      delete res_buf.mc_msg[3].iov_base;
#endif
	      res_buf.mc_msg[3].iov_base = 0;
	  }

	  if (xrstat < 0)
	  {
	      machineAlive = ping(objects[indx].destn_host, timeTaken);
	      if (!machineAlive)
		  objects[indx].grps_id.groups_type = SRVR_dead;
	  }
      }

      /*
       * Because we know there is only one server here we can stop as soon
       * as a machine failure is detected.
       */
      
    } while ((wrong_call) && (--rretry > 0) && (machineAlive) && (!inval));

    if ((!wrong_call) && (!inval) && (machineAlive))
    {
#ifdef DEBUG
	debug_stream.lock();
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "rpc: initiate: received reply from server\n" << flush;
	debug_stream.unlock();
#endif
	
      /* Set output Parameters */
      /* Server Address */
      objects[indx].grps_id.groups_type = SRVR_type;
      objects[indx].grps_id.grp_srvr.srvr.sr_idtype = MID_type;
      objects[indx].grps_id.grp_srvr.srvr.sr_id  = sender;

      set_grp_info(newgroup, multithem);
      insert_servers(&(newgroup->grp_srvr.group), multithem, srvr_reply, sender);

#ifdef DEBUG
      traverseall_supergrps(&(newgroup->grp_srvr.group));
#endif

      *successes = 1;
      rcode = SUCCEEDED;
  
#ifdef DEBUG
      debug_stream.lock();
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "rpc: received from server: ";
      printgid(sender);
      debug_stream << flush;
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "rpc: server port set to " << ntohl(srvr_reply.lo_part) << "\n" << flush;
      debug_stream.unlock();
#endif            
    }       
    else
    {
#ifdef DEBUG
	debug_stream.lock();
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "rpc: initiate not successful\n" << flush;
	debug_stream.unlock();
#endif

      if ((xrstat > 0) && (identical_sn(&rs_sn, &request_sn)))
      {
	  if ((wrong_call) || (inval))
	  {
	      _rpcerr = EUNKNOWN;
	       rcode = RPC_UNABLE;
	  }
	  else
	      _rpcerr = (int) ntohl(srvr_reply.type);
      }
      else
      {
	  _rpcerr = NO_RESPONSE;  // allow terminate, just in case message got through
	  inval = FALSE;
	  rcode = RPC_UNABLE;
      }

      if (!inval)  // if invalid then server has not started anyway, so no terminate needed
      {
	  if (machineAlive)  // if machine is dead, then do not attempt terminate
	  {
	      struct host_address hostAddr;

	      hostAddr.log_host = objects[indx].destn_host;
	      hostAddr.real_host = (char*) 0;
	      
	      getrealhost(&hostAddr);
	      
	      error_stream << WARNING
			   << "Initiate::no response from server on available machine "
			   << hostAddr.real_host << endl;

	      if (hostAddr.real_host)
#ifndef GCC_DELETE_BUG
		  ::delete [] hostAddr.real_host;
#else
	          ::delete hostAddr.real_host;
#endif	      
	      rcode = RPC_UNABLE;  // differentiate between failed and working machine

	      /* Undo initiate */
	      terminate(newgroup, FALSE, objects);
	  }
	  else
		rcode = RPC_NOTDONE;
      }
      else
	rcode = RPC_UNABLE;
      
      reset_init_args(objects, newgroup, successes, group_number);
    }
  } 
  else  /* MULTICAST */
  {
#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "rpc: must multicast\n" << flush;
      debug_stream.unlock();
#endif
      
    wrong_call = TRUE;
    inval = FALSE;

    if (*successes < 1)
        *successes = 1;  /* default successes: 1 */
    else
      if (*successes > group_number)
	*successes = group_number;
    max_failures = group_number - (*successes);
    no_failures = 0;

    set_grp_info(newgroup, multithem);

    indx = 0;  undo = FALSE;
    while (objects[indx].destn_host != 0)
    {
	/* Will be reset if server responds ok */

	objects[indx].grps_id.grp_srvr.srvr.sr_id.lo_part = htonl(-1);
	objects[indx].grps_id.grp_srvr.srvr.sr_id.hi_part = htonl(-1);	    
	objects[indx].grps_id.grp_srvr.srvr.sr_id.type = htonl(UNSET_type);
	objects[indx].grps_id.groups_type = SRVR_dead;
        indx++;
    }
    indx = 0;
    while ((objects[indx].destn_host != 0) && (!undo))
    {
#ifdef DEBUG
	debug_stream.lock();
	
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "object[" << indx << "] host " << objects[indx].destn_host << ", name "
		     << objects[indx].object_name << " uid <" << objects[indx].u_id.host
		     << "," << objects[indx].u_id.sec << "," << objects[indx].u_id.msec << ">\n" << flush;
	debug_stream.unlock();
#endif
	
      /* Initiate message is unicast to all servers */
      unithem.hi_part = objects[indx].destn_host;

      /* Get a Sequence Number */
      getsn(&rq_sn);    
      request_sn.sec = htonl(rq_sn.sec);    /* Byte Swap */
      request_sn.msec = htonl(rq_sn.msec);

      /* Objects on different hosts may have unique names */
      req_buf.mc_msg[7].iov_base = (char *) objects[indx].object_name;

      /* Set action id and unique id for each object */
    
      uid = objects[indx].u_id;
       
      rretry = GWRETRY;
      xrstat = FAILED;
      machineAlive = TRUE;
	
      do
      {
        /* Initialise result's sequence number */
        rs_sn.sec = rs_sn.msec = 0;
        xretry = GWRETRY;
        xrstat = FAILED;
        do
        {
          xrstat = multicast_long_send(&unithem, &req_buf);
        } while ((xrstat != MC_OK) && (--xretry > 0));
        if (xrstat == MC_OK)
        {
           xrstat = FAILED;
#ifdef DEBUG
	   debug_stream.lock();
	   
	   debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	   debug_stream << "rpc: must MULTICAST unicast send ok waiting to receive\n" << flush;
	   debug_stream.unlock();
	   
#endif
	   do
	   {
             xrstat = multicast_long_receive(&client, & ack, 0,GWTIMEOUT);
	   } while ((xrstat == ENOMESSAGE) || ((xrstat > 0) && (xrstat != expectedSize)));
        }
        else
        {
#ifdef DEBUG
	    debug_stream.lock();
	    
	    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	    debug_stream << "rpc: send failed: rstat " << xrstat << " errno " << errno << "\n" << flush;
	    debug_stream.unlock();
#endif	
	  wrong_call = TRUE;
          break;
        }

	if ((xrstat > 0) && (xrstat != expectedSize))
	    xrstat = ENOMESSAGE;

	if (xrstat > 0)
	{
	  /* Cast result buffer */
	  rs_sn = * (struct RPC_SN *)   res_buf.mc_msg[0].iov_base;
	  rs_code = * (Int32 *)          res_buf.mc_msg[1].iov_base;
      
	  wrong_call = (((!identical_sn(&rs_sn, &request_sn)) ||
			 (ntohl(rs_code) != GW_OK) || (xrstat < 1)) ? TRUE : FALSE);

	  if (ntohl(rs_code) == NOSERVER)
	  {
	      error_stream << WARNING << "initiate error - no server code present." << endl;
	      inval = TRUE;
	  }
	  else
	  {
	      if (ntohl(rs_code) == SERVERACCESS)
	      {
		  error_stream << WARNING << "initiate error - server access permission problem." << endl;
		  inval = TRUE;
	      }
	  }

	  sender.lo_part = sender.hi_part = htonl(-1);
	  
	  if (!wrong_call)
	  {
	      srvr_reply = * (struct MID *) res_buf.mc_msg[2].iov_base;
	      sender = * (struct MID *)     res_buf.mc_msg[3].iov_base;
	  }
       }

	if ((wrong_call) && (rretry > 1) && (!inval))
	{
	  if (res_buf.mc_msg[0].iov_base)
	  {
	    free(res_buf.mc_msg[0].iov_base);
	    res_buf.mc_msg[0].iov_base = 0;
	  }
	  if (res_buf.mc_msg[2].iov_base)
	  {
#ifndef GCC_ARRAY_DELETE_BUG
	    delete [] res_buf.mc_msg[2].iov_base;
#else
	    delete res_buf.mc_msg[2].iov_base;
#endif
	    res_buf.mc_msg[2].iov_base = 0;
	  }
	  if (res_buf.mc_msg[3].iov_base)
	  {
#ifndef GCC_ARRAY_DELETE_BUG
	    delete [] res_buf.mc_msg[3].iov_base;
#else
	    delete res_buf.mc_msg[3].iov_base;
#endif	    
	    res_buf.mc_msg[3].iov_base = 0;
	  }

	    machineAlive = ping(objects[indx].destn_host, timeTaken);
	    if (machineAlive == FALSE)
		objects[indx].grps_id.groups_type = SRVR_dead;
	}

	/*
	 * Deal with machine failure immediately, i.e., drop out of loop
	 * at this point.
	 */
	
      } while ((--rretry > 0) && (wrong_call) && (machineAlive) && (!inval));
	
      if ((wrong_call) || (inval))
      {
        no_failures++;
#ifdef DEBUG
	debug_stream.lock();
	
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "rpc: server creation failed for host " << objects[indx].destn_host
		     << " - failures so far " << no_failures << "\n" << flush;
	debug_stream.unlock();
#endif

	/*
	 * We must differentiate between a failed and working machine for the error
	 * conditions. If we have a failure from a server even though the machine
	 * is alive then we will fail completely as this could cause consistency
	 * problems, especially for shared servers.
	 */

	if ((machineAlive) || (inval))
	{
	    rcode = RPC_UNABLE;
	    struct host_address hostAddr;

	    hostAddr.log_host = objects[indx].destn_host;
	    hostAddr.real_host = (char*) 0;
	      
	    getrealhost(&hostAddr);
	      
	    error_stream << WARNING
			 << "Initiate::no response from server on available machine "
			 << hostAddr.real_host << endl;

	    if (hostAddr.real_host)
#ifndef GCC_DELETE_BUG	  
		::delete [] hostAddr.real_host;
#else
	        ::delete hostAddr.real_host;
#endif
	    no_failures = max_failures+1;  // force termination and exit from initiate
	    undo = TRUE;
	}
      }        
      else
      {
	  objects[indx].grps_id.groups_type = SRVR_type;
	  objects[indx].grps_id.grp_srvr.srvr.sr_idtype = MID_type;
	  objects[indx].grps_id.grp_srvr.srvr.sr_id  = sender;
	      
	  insert_servers(&(newgroup->grp_srvr.group), multithem,
			 srvr_reply, sender);

	  if (no_failures > max_failures)
	      undo = TRUE; /* undo all initialised servers, stop initialising */

	  indx++;
      }
    } /* end while */

    if (undo)
    {
      /* _rpcerr cannot have a meaningful value for multicast initiates -
       * since servers could have different reasons for failing - failure
       * return code is the only meaningful message that can be conveyed
       */
      
      /* undo work done by initiate via call to terminate */
      terminate(newgroup, FALSE, objects);
      /* Reset server and group id */
      reset_init_args(objects, newgroup, successes, group_number);
    }
    else
    {
#ifdef DEBUG
	debug_stream.lock();
	
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "RPC: initiate(): set mid <" << ntohl(multithem.type) << ","
		     << ntohl(multithem.hi_part) << "," << ntohl(multithem.lo_part) << ">\n" << flush;
	debug_stream.unlock();
	
#endif

      *successes = group_number - no_failures;
      rcode = SUCCEEDED;
    }      

  } /* end else */    

  if (packed_path)
#ifndef GCC_ARRAY_DELETE_BUG
      delete [] packed_path;
#else
      delete packed_path;
#endif

  if (res_buf.mc_msg[0].iov_base)
      free(res_buf.mc_msg[0].iov_base); // was malloc-ed rather than new-ed

  if (res_buf.mc_msg[2].iov_base)
#ifndef GCC_ARRAY_DELETE_BUG
      delete [] res_buf.mc_msg[2].iov_base;
#else
      delete res_buf.mc_msg[2].iov_base;
#endif  

  if (res_buf.mc_msg[3].iov_base)
#ifndef GCC_ARRAY_DELETE_BUG
      delete [] res_buf.mc_msg[3].iov_base;
#else
      delete res_buf.mc_msg[3].iov_base;
#endif

  if (inval)
    return(INVALID_ARG);
  else
    return(rcode);

} /* end initiate() */

int terminate (struct GROUPS *grp, Boolean all, struct OBJECT_DESC *objects)
{
  struct GROUP_INFO replygrp;
  Int32             rq_code;

  if ((!grp) || (!objects))
    return TERMNOSRVRS;
  
  if ((grp->groups_type == GRP_type) && (grp->grp_srvr.group.head_gd == NULL))
    return(TERMNOSRVRS);

  /* Set operation code for server */
  if (all) 
    rq_code = TERMALLSRVRS;
  else
    rq_code = TERMONESRVR;

  group_opn(grp, grp, &replygrp, rq_code, objects);
  
  /* For terminate: group containing MID is same as group on which group opn
   * is to be performed.  Ignore return code, since terminate succeeds even
   * if nodes do not respond due to a crash.
   */

  /* Reset output param 'grp' */
  if (grp->groups_type == GRP_type)
     rmall_supergrps(&(grp->grp_srvr.group));

  grp->groups_type = UNSET_type;
  
  return(SUCCEEDED);
    
} /* end terminate() */

int join_group (struct GROUPS  *grp, struct GROUPS  *sub_grps)
{
  struct GROUPS *current_subgrp;
  struct GROUP_INFO replygrp;
  struct MID current_mid;
  struct MID current_src;
  Boolean failed = FALSE;
  int rcode;

  /* Parameter range checks */
  if ((grp->groups_type != GRP_type) ||
      (grp->grp_srvr.group.head_gd == NULL) || 
      (sub_grps->groups_type == UNSET_type) ||
      ((sub_grps->groups_type == GRP_type) &&
       (sub_grps->grp_srvr.group.head_gd == NULL)))
    return(INVALID_GRP);

  current_subgrp = sub_grps;
  while ((current_subgrp->groups_type != UNSET_type) && (!failed))
  {
    failed = (((rcode = group_opn(grp, current_subgrp,
				 &replygrp, JOINGRP, 0)) != SUCCEEDED) ? TRUE : FALSE);
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "rpc: returned from group_opn for Join with code " << rcode
	<< " (OK," << SUCCEEDED << ")\n" << flush;
    debug_stream.unlock();
    
#endif
    current_subgrp++;
  }

  if (!failed)
  {
    current_subgrp = sub_grps;
    while (current_subgrp->groups_type != UNSET_type)
    {
      if (current_subgrp->groups_type == SRVR_type)
      { /* insert server */
	if (current_subgrp->grp_srvr.srvr.sr_idtype == MID_type)
	{
	  current_mid = current_subgrp->grp_srvr.srvr.sr_id;
	  getnullmid(&current_src);
	}
	else
	{
	  current_src = current_subgrp->grp_srvr.srvr.sr_id;
	  getnullmid(&current_mid);
	}
	insert_servers(&(grp->grp_srvr.group),
		       grp->grp_srvr.group.head_gd->mymid,
		       current_mid, current_src);
      }
      else
      { 
	if (current_subgrp->groups_type == GRP_type)
	{ /* insert group */
	  current_mid = current_subgrp->grp_srvr.group.head_gd->mymid;
	  insert_groups(&(grp->grp_srvr.group),
			grp->grp_srvr.group.head_gd->mymid,
			current_mid);
	}
      }
      current_subgrp++;
#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "JOIN: going through all groups\n" << flush;
      traverseall_supergrps(&(grp->grp_srvr.group));
      debug_stream.unlock();
      
#endif      
    }
    return(SUCCEEDED);
  }
  else
    return(FAILED);

} /* end join_group() */
    
int new_group (struct GROUPS *new_grp, struct GROUPS *sub_grps)
{
  struct GROUPS *current_subgrp;
  struct GROUP_INFO replygrp;
  struct SERVERIDS *current_server;
  struct MID current_mid, newgroupid;
  struct MID current_src;
  Boolean failed = FALSE;
  int rcode;

  /* Parameter range checks */
  if ((sub_grps->groups_type == UNSET_type) ||
      ((sub_grps->groups_type == GRP_type) &&
       (sub_grps->grp_srvr.group.head_gd == NULL)))
    return(INVALID_GRP);

  /* Set group type */
  multicast_new_mid(&newgroupid);
  set_grp_info(new_grp, newgroupid);

  current_subgrp = sub_grps;
  while ((current_subgrp->groups_type != UNSET_type) && (!failed))
  {
    failed = ((rcode = (group_opn(new_grp, current_subgrp,
				 &replygrp, JOINGRP, 0)) != SUCCEEDED) ? TRUE : FALSE);
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "rpc: returned from group_opn for NEW_GROUP with code " << rcode
	<< " (OK," << SUCCEEDED << ")\n" << flush;
    debug_stream.unlock();
    
#endif    
    current_subgrp++;
  }

  if (!failed)
  {
    current_subgrp = sub_grps;
    while (current_subgrp->groups_type != UNSET_type)
    {
      if (current_subgrp->groups_type == GRP_type)
      { /* insert group */
	current_mid = current_subgrp->grp_srvr.group.head_gd->mymid;
	insert_groups(&(new_grp->grp_srvr.group),
		      new_grp->grp_srvr.group.head_gd->mymid,
		      current_mid);

	/* insert all servers in current list */
	current_server =
	  current_subgrp->grp_srvr.group.head_gd->sids.head_sids;
	while (current_server != NULL)
	{
	  insert_servers(&(new_grp->grp_srvr.group),
			 new_grp->grp_srvr.group.head_gd->mymid,
			 current_server->s_id, current_server->sender);
	  current_server = current_server->next_serverid;
	}
      }
      else
	if (current_subgrp->groups_type == SRVR_type)
	{
	  /* insert server */
	  if (current_subgrp->grp_srvr.srvr.sr_idtype == MID_type)
	  {
	    current_mid = current_subgrp->grp_srvr.srvr.sr_id;
	    getnullmid(&current_src);
	  }
	  else
	  {
	    current_src = current_subgrp->grp_srvr.srvr.sr_id;
	    getnullmid(&current_mid);
	  }
	  insert_servers(&(new_grp->grp_srvr.group),
			 new_grp->grp_srvr.group.head_gd->mymid,
			 current_mid, current_src);
	}

      current_subgrp++;

    } /* end while */
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "NEWGROUP: \n" << flush;
    traverseall_supergrps(&(new_grp->grp_srvr.group));
    debug_stream.unlock();
    
#endif    
    return(SUCCEEDED);
  }
  else
  { /* new_group failed - do not change group information */
    new_grp->groups_type = UNSET_type;
    return(FAILED);
  }
} /* end new_group() */

int leave_group (struct GROUPS *grp, struct GROUPS *sub_grps)
{
  /* 'sub-grps' give list of groups that are to leave 'grp' */

  struct GROUPS *current_subgrp;
  struct GROUP_INFO replygrp;
  int rcode;

  /* Parameter range checks */
  if ((grp->groups_type != GRP_type) ||
      (grp->grp_srvr.group.head_gd == NULL) || 
      (sub_grps->groups_type == UNSET_type) ||
      ((sub_grps->groups_type == GRP_type) &&
       (sub_grps->grp_srvr.group.head_gd == NULL)))
    return(INVALID_GRP);

  /* Make leave calls to servers */
  current_subgrp = sub_grps;
  while (current_subgrp->groups_type != UNSET_type)
  {
    rcode = group_opn(grp, current_subgrp, &replygrp, LEAVEGRP, 0);
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "rpc: returned from group_opn for leave with code " << rcode
	<< " (OK," << SUCCEEDED << ")\n" << flush;
    debug_stream.unlock();
    
#endif    
    /* Operation should succeed for leave operation */
    current_subgrp++;
  }

  /* Change group information */
  current_subgrp = sub_grps;
  while (current_subgrp->groups_type != UNSET_type)
  {
    if (current_subgrp->groups_type == SRVR_type)
      remove_server(&(grp->grp_srvr.group),
		    grp->grp_srvr.group.head_gd,
		    current_subgrp->grp_srvr.srvr.sr_id,
		    FALSE, TRUE);
    else
      remove_group(&(grp->grp_srvr.group),
		   grp->grp_srvr.group.head_gd,
		   current_subgrp->grp_srvr.group.head_gd->mymid,
		   FALSE, TRUE);
    
    current_subgrp++;
  }

  if (empty_grp((grp->grp_srvr.group.head_gd)))
    grp->groups_type = UNSET_type;

  /* Leave_group always succeeds */
  return(SUCCEEDED);

} /* end leave_group() */

int group_opn (struct GROUPS *grp, struct GROUPS *sub_grp, struct GROUP_INFO *replygrp,
	       Int32 opcode, struct OBJECT_DESC *objects)
{
  struct MID           serverid;
  struct MID           sender, tmp_midsrvr;
  struct MID           group_mid, transmit_group;
  struct RPC_SN        rq_sn, rq_invseqno, rs_invseqno;
  struct mc_msghdr     req_buf, res_buf;
  struct mc_sg_filter  termcall;
  Boolean              isserver, done, ignore;
  Int32                rq_code, opn_code;
  int                  xstat, acks_expected;
  int                  xretry = CL_TERM_RETRY, rretry;
  Int32                rs_code;
  int                  temp_rcode, no_servers = (grp->grp_srvr.group).head_gd->sids.no_srs;
  int                  expectedSize = 0;

  switch (sub_grp->groups_type)
  {
  case SRVR_type:
    isserver = TRUE;
    tmp_midsrvr = sub_grp->grp_srvr.srvr.sr_id;
    group_mid.hi_part = 0;   /* To enable server to ignore group field */
    break;
    
  case GRP_type:
    isserver = FALSE;
    group_mid      =     grp->grp_srvr.group.head_gd->mymid;
    transmit_group = sub_grp->grp_srvr.group.head_gd->mymid;
    break;

  default:  /* including UNSET_type */
    return(INVALID_ARG);
  } /* end switch */
  
  /* Set operation code for server */
  rq_code = htonl(opcode);  /* Operation code, byte swapped */
  
  getsn(&rq_sn);  /* Obtain a sequence number */
  rq_invseqno.sec = htonl(rq_sn.sec); 
  rq_invseqno.msec = htonl(rq_sn.msec);

  /* Set up call message */
  req_buf.mc_msg[0].iov_base = (char *) &rq_invseqno;
  req_buf.mc_msg[0].iov_len = sizeof(struct RPC_SN);
  req_buf.mc_msg[1].iov_base = (char *) &rq_code;
  req_buf.mc_msg[1].iov_len = sizeof(rq_code);
  req_buf.mc_msg[2].iov_base = (char *) &group_mid;
  req_buf.mc_msg[2].iov_len = sizeof(struct MID);
  req_buf.mc_msg[3].iov_len = 0;
  req_buf.mc_msg[3].iov_base = 0;
  req_buf.mc_msg[4].iov_len = 0;
  req_buf.mc_msg[4].iov_base = 0;		 
  req_buf.mc_msg[5].iov_len = 0;
  req_buf.mc_msg[5].iov_base = 0;
  req_buf.mc_msg[6].iov_len = 0;
  req_buf.mc_msg[6].iov_base = 0;
  req_buf.msg_len = 7; 

  res_buf.mc_msg[0].iov_len = sizeof(struct RPC_SN);
  res_buf.mc_msg[0].iov_base = 0;
  rs_code = ZEROCODE;
  res_buf.mc_msg[1].iov_len = sizeof(rs_code);
  res_buf.mc_msg[1].iov_base = 0;
  res_buf.msg_len = 2;
  termcall.next_fs = NULL;
  termcall.mc_msg = &res_buf;

  expectedSize = res_buf.mc_msg[0].iov_len + res_buf.mc_msg[1].iov_len;
  
  if (!isserver)
  {
      if ((no_servers == 1) && (objects[0].grps_id.groups_type == SRVR_dead))
	  return SUCCEEDED;
      else
      {
	  replygrp = &(sub_grp->grp_srvr.group);
	  init_filtgrp(replygrp, &acks_expected);
      }
    
#ifdef DEBUG
      debug_stream.lock();
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "group_opn: sending opcode " << ntohl(rq_code) << "\n";
      debug_stream << "group_opn: no of acks expected from servers: " << acks_expected << "\n";
      debug_stream << "rpc: group_opn: trying to send to server <" << ntohl(transmit_group.type)
		   << "," << ntohl(transmit_group.hi_part) << "," << ntohl(transmit_group.lo_part) << ">\n" << flush;
      debug_stream.unlock();
#endif
  }
  else
  {
      /*
       * Have we already detected the failure of this machine?
       * If so, we can return immediately.
       */
      
      if (objects[0].grps_id.groups_type == SRVR_dead)
	  return SUCCEEDED;
      else
	  acks_expected = 1;  /* for 1 server */
  }
  
  do
  {
      if (RAJDOOT_USE_BROADCAST)
      {
	  if (!isserver)
	      xstat = multicast_long_send(&transmit_group, &req_buf);
	  else
	      xstat = multicast_long_send(&tmp_midsrvr, &req_buf);
      }
      else
      {
	  /*
	   * If initiate failed in such a way that we never got a reply
	   * then we don't really have a valid address to try to use.
	   * So, we rely upon multicast_send_message detecting this for
	   * each server and ignoring the send.
	   */
	  
	  xstat = multicast_send_message(objects, &req_buf);
      }

#ifdef DEBUG
      if (xstat != SUCCEEDED)
      {
	  debug_stream.lock();
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "rpc: group_opn: send failed: " << errno << "\n" << flush;
	  debug_stream.unlock();
      }
#endif

      if (xstat == SUCCEEDED)
      {
#ifdef DEBUG
	  debug_stream.lock();
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "rpc: group_opn: sent ok to servers\n" << flush;
	  debug_stream.unlock();
#endif

	  rretry = acks_expected * 2 + 1;
	  
	  /* Since initiate or multi_call may not have required replies from all
	     servers, those that are on the pipeline and have not been picked up
	     may be received in group_opn: This may be remedied by discarding the
	     unwanted messages and listening for further (correct) ones.
	     */

	  while ((acks_expected > 0) && (rretry-- > 0))
	  {
	      if (res_buf.mc_msg[0].iov_base)
	      {
#ifndef GCC_ARRAY_DELETE_BUG
		  delete [] res_buf.mc_msg[0].iov_base;
#else
		  delete res_buf.mc_msg[0].iov_base;
#endif	    
		  res_buf.mc_msg[0].iov_base = 0;
	      }
	      if (res_buf.mc_msg[1].iov_base)
	      {
#ifndef GCC_ARRAY_DELETE_BUG
		  delete [] res_buf.mc_msg[1].iov_base;
#else
		  delete res_buf.mc_msg[1].iov_base;
#endif	    
		  res_buf.mc_msg[1].iov_base = 0;
	      }

	      ignore = FALSE;
	      
	      do
	      {
		  int sizeReceived = multicast_long_receive(&client, &termcall, 0, CL_TERM_TO);
		  
		  if (sizeReceived < 0)
		  {
#ifdef DEBUG
		      debug_stream.lock();
		      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
		      debug_stream << "rpc: group_opn: failed to recv: " << errno << "\n" << flush;
		      debug_stream.unlock();
#endif
		  }
		  else
		  {
		      if (sizeReceived != expectedSize)  // not a reply for us, so ignore
			  ignore = TRUE;
		      else
		      {
			  /* Cast result buffer */
			  rs_invseqno = * (struct RPC_SN *) res_buf.mc_msg[0].iov_base;
			  rs_code = * (Int32 *)             res_buf.mc_msg[1].iov_base; 
	
			  rs_code = ntohl(rs_code);
#ifdef DEBUG
			  debug_stream.lock();
			  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
			  debug_stream << "rpc: group_opn: ack received\n";
			  debug_stream << "rpc: opcode was " << rs_code << " TERM_OK(" << TERM_OK << ")";
			  debug_stream << " seqno sent:<" << rq_invseqno.sec << "," << rq_invseqno.msec << "> received:<"
				       << rs_invseqno.sec << "," << rs_invseqno.msec << ">\n" << flush;
			  debug_stream.unlock();
#endif
			  /*
			   * Do not expect acks from server that responds with SRVR_ERROR -
			   * whether part of a unicast or multicast group, since it has no
			   * record of group that it is asked to group_opn
			   */
		  
			  if (identical_sn(&rq_invseqno, &rs_invseqno))
			  {
			      if (isserver)
				  acks_expected--;
			      else
			      {
				  if (dec_filtgrp(replygrp, termcall.source) == SUCCEEDED)
				  {
				      acks_expected--;   /* Ack received from one server */
#ifdef DEBUG
				      debug_stream.lock();
				      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
				      debug_stream << "rpc:group_opn group: received correct ack\n" << flush;
				      debug_stream.unlock();
#endif
				  }
#ifdef DEBUG	      
				  else
				  {
				      debug_stream.lock();
				      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
				      debug_stream << "rpc: group_opn group:dec_filtgrp failed\n" << flush;
				      debug_stream.unlock();
				  }
#endif	      
			      }
			  } /* end if */
			  else
			      ignore = TRUE;  // probably from previous terminate to another server
			  
		      } /* end else */
		  }
		  
	      } while (ignore);

	      /*
	       * We should probably redo this for groups since we will ping whenever
	       * we get here, even if a message is waiting.
	       */
	      
	      if (!RAJDOOT_USE_BROADCAST)
	      {
		  /*
		   * Always ping after a failure, so we can give a "definitive" answer
		   */

		  if (acks_expected > 0)
		  {
		      int _indx = 0;

		      while (objects[_indx].destn_host)
		      {
			  Uint32 timeTaken = 0;

			  if ((objects[_indx].grps_id.groups_type == SRVR_dead) || 
			      (!ping(objects[_indx].destn_host, timeTaken)))
			      acks_expected--;
			  _indx++;
		      }
		  }
	      }
	  }
      } /* end while */
      
  } while ((--xretry > 0) && (acks_expected > 0) && (xstat != SUCCEEDED));
  
#ifdef DEBUG
  debug_stream.lock();
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "rpc: group_opn: after all recvs expecting acks from " << acks_expected
	       << " servers\n" << flush;
  debug_stream.unlock();
#endif

  /* Server id that was requested to perform group_opn did not exist.  This
   * condition can arise if the id is invalid or if it has been terminated
   * already  - This can be tested for later if necessary */
  
  if (((!isserver) && (isempty_filtgrp(replygrp))) ||
      ((isserver) && (acks_expected <= 0)))
  {
      if (termcall.mc_msg->mc_msg[0].iov_base)
      {
#ifndef GCC_ARRAY_DELETE_BUG
	  delete [] termcall.mc_msg->mc_msg[0].iov_base;
#else
	  delete termcall.mc_msg->mc_msg[0].iov_base;
#endif	  
	  termcall.mc_msg->mc_msg[0].iov_base = 0;
      }
      if (termcall.mc_msg->mc_msg[1].iov_base)
      {
#ifndef GCC_ARRAY_DELETE_BUG
	  delete [] termcall.mc_msg->mc_msg[1].iov_base;
#else
	  delete termcall.mc_msg->mc_msg[1].iov_base;
#endif	  
	  termcall.mc_msg->mc_msg[1].iov_base = 0;
      }

      return(SUCCEEDED);
  }
  else
  {
    /* Retry calls for each remaining server */
#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "rpc: group_opn: retrying calls to some servers\n" << flush;
      debug_stream.unlock();
#endif
    
    /* Set interrupt operation code */
    opn_code = opcode + GMCODEOFFSET;

    done = FALSE;
    no_servers = get_noservers(replygrp);
    while (no_servers > 0)
    {
      get_filtserver(replygrp, &serverid, &sender);
      temp_rcode = prodserver(opn_code, serverid, sender, group_mid, objects);
#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "group_opn: terminated prod server with code:" << temp_rcode << "\n";
      debug_stream << "group_opn: waiting for " << no_servers << " more replies\n" << flush;
      debug_stream.unlock();
      
#endif
      /* For below opcodes primitives terminate successfully even if server
       * does not respond due to a crash 
       */
      if ((temp_rcode == SUCCEEDED) ||
	  (opn_code == INT_TERMONESRVR) ||
	  (opn_code == INT_TERMALLSRVRS) ||
	  (opn_code == INT_LEAVEGRP))
	if (!isserver)
	  dec_filtgrp(replygrp, sender);

      no_servers--;
    } 
  } /* end else */

  if (((!isserver) && (isempty_filtgrp(replygrp))) ||
      ((isserver) && (acks_expected <= 0)))
  {
      if (termcall.mc_msg->mc_msg[0].iov_base)
      {
#ifndef GCC_ARRAY_DELETE_BUG
	  delete [] termcall.mc_msg->mc_msg[0].iov_base;
#else
	  delete termcall.mc_msg->mc_msg[0].iov_base;
#endif	  
	  termcall.mc_msg->mc_msg[0].iov_base = 0;
      }
      if (termcall.mc_msg->mc_msg[1].iov_base)
      {
#ifndef GCC_ARRAY_DELETE_BUG
	  delete [] termcall.mc_msg->mc_msg[1].iov_base;
#else
	  delete termcall.mc_msg->mc_msg[1].iov_base;
#endif	  
	  termcall.mc_msg->mc_msg[1].iov_base = 0;
      }
      return(SUCCEEDED);
  }
  else
  {
      if (termcall.mc_msg->mc_msg[0].iov_base)
      {
#ifndef GCC_ARRAY_DELETE_BUG
	  delete [] termcall.mc_msg->mc_msg[0].iov_base;
#else
	  delete termcall.mc_msg->mc_msg[0].iov_base;
#endif	  
	  termcall.mc_msg->mc_msg[0].iov_base = 0;
      }
      if (termcall.mc_msg->mc_msg[1].iov_base)
      {
#ifndef GCC_ARRAY_DELETE_BUG
	  delete [] termcall.mc_msg->mc_msg[1].iov_base;
#else
          delete termcall.mc_msg->mc_msg[1].iov_base;
#endif
	  termcall.mc_msg->mc_msg[1].iov_base = 0;
      }
      return(FAILED);
  }
} /* end group_opn() */

int removegroup (struct GROUPS *grp)
{
  /* Initialise group type and deallocate storage occupied by 'grp' */

  if (grp->groups_type == GRP_type)
  {
    grp->groups_type = UNSET_type;
    rmall_supergrps(&(grp->grp_srvr.group));
    return(SUCCEEDED);
  }
  else
    return(INVALID_GRP);

} /* end removegroup() */

/* struct GROUPS  *destn;       may be unicast or multicast - in param */
/* struct RPC     *service;     Service Request - in parameter */
/* struct RPC     *result;      Result of call - out parameter */
/* Uint32  timeout;      Timeout for call in centisecs - in param */
/* long           retry;        No. of retries - in parameter */
/* struct FILTER_INFO *filter;  filter procedure, no_successes, comparison info */
/* struct OBJECT_DESC *objects; */

int multi_call (struct GROUPS  *destn, struct RPC *service, struct RPC *result, Uint32 timeout,
		Int32 retry, struct FILTER_INFO *filter, struct OBJECT_DESC *objects)
{
  struct mc_msghdr result_buf;
  struct RPC_SN result_seqno;
  struct MID red_gmid;
  struct mc_msghdr req_buf;
  struct mc_sg_filter ack;
  struct RPC_SN call_sn;
  Boolean supplied_filter = FALSE, failed;
  Uint32 t_out = 0, timeTaken = 0;
  Int32 indicate_opcode, packed_size, opn_code = 0;
  Int32 result_opcode;
  int rcode, mc_recv_rcode = 1, df_recv_rcode = 1, xstat, no_servers, t_servers;
  int mul_ret;
  char *buff, *next_buff, *packed_ags;
  Boolean msg_ok = TRUE, machineAlive;
    
  supplied_filter = ((filter->filt_name != NULL) ? TRUE : FALSE);
  rcode = RPC_NOTDONE;

  /* Parameter checks */
  if ((destn->groups_type != GRP_type) &&
      (destn->groups_type != SRVR_type))
  {
    _rpcerr = INVALID_GROUP;
    return(rcode);
  }
  else
  {
    if (destn->groups_type == GRP_type)
    {
      no_servers = (destn->grp_srvr.group).head_gd->sids.no_srs;
      if ((supplied_filter) && (no_servers < filter->no_successes) ||
	  (no_servers < 1))
      {
	_rpcerr = TOO_FEW_SRVRS;
	return(rcode);
      }
    }
  }

  /* Pack action group information to be piggybacked onto call */
  packed_ags = new char [(sizeof(opn_code) + (2 * sizeof(struct MID)) - 8)];

  /* Type field of MID is not piggybacked, but generated at the server */
  buff = next_buff = packed_ags;

  packed_size = sizeof(opn_code);
  opn_code = htonl(NOOP_GRP);
  pack_Int32(opn_code, buff);

  rcode = RPC_NOTDONE;

  getsn(&call_sn); /* Get a sequence number */
  /* Time for receiving result from all servers, sent to server in centisecs */
  call_sn.t_out = timeout;
  call_sn.retry = retry;

  /* Byte Swap */
  call_seqno.sec = htonl(call_sn.sec);    /* Sequence Number */
  call_seqno.msec = htonl(call_sn.msec);
  call_seqno.t_out = htonl(call_sn.t_out);  
  call_seqno.retry = htonl(call_sn.retry);

#ifdef DEBUG
  debug_stream.lock();
  
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "multi_call: call sequence number < " << call_seqno.sec << ", " << call_seqno.msec << " >" << endl;
  debug_stream.unlock();
  
#endif
  
  indicate_opcode = htonl(USER_RPC);

  /* Build Call */  
  req_buf.mc_msg[0].iov_base = (char *) &call_seqno;
  req_buf.mc_msg[0].iov_len  = sizeof(struct RPC_SN);
  req_buf.mc_msg[1].iov_base = (char *) &indicate_opcode;
  req_buf.mc_msg[1].iov_len  = sizeof(indicate_opcode);
  /* Next field, which is group id to join, not used for multi_call */
  req_buf.mc_msg[2].iov_base = (char *) &red_gmid; 
  req_buf.mc_msg[2].iov_len  = sizeof(struct MID);
  req_buf.mc_msg[3].iov_base = (char *) &service->rpc_code;
  req_buf.mc_msg[3].iov_len  = sizeof(service->rpc_code);  
  req_buf.mc_msg[4].iov_base = (char *) packed_ags;
  req_buf.mc_msg[4].iov_len  = (int) packed_size;
  req_buf.mc_msg[5].iov_base = (char *) service->rpc_buff1;
  req_buf.mc_msg[5].iov_len  = (int) service->rpc_length1; 
  req_buf.mc_msg[6].iov_base = (char *) service->rpc_buff2;
  req_buf.mc_msg[6].iov_len  = (int) service->rpc_length2; 
  req_buf.msg_len = 7;
  
  /* Prepare Buffers for Result */

  result_opcode = ZEROCODE;   /* Initialising result opcode */
  result_buf.mc_msg[0].iov_len  = sizeof(struct RPC_SN);
  result_buf.mc_msg[0].iov_base = 0;
  result_buf.mc_msg[1].iov_len  = sizeof(result_opcode);
  result_buf.mc_msg[1].iov_base = 0;
  result_buf.mc_msg[2].iov_len  = 0;
  result_buf.mc_msg[2].iov_base = 0;
  result_buf.mc_msg[3].iov_len  = 0;
  result_buf.mc_msg[3].iov_base = 0;
  result_buf.msg_len = 4;
  ack.next_fs = NULL;
  ack.mc_msg = &result_buf;

#ifdef DEBUG  
  if (destn->groups_type == GRP_type)
  {
      debug_stream.lock();
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "multi_call: group is GRP_type, and no of servers " <<
	  destn->grp_srvr.group.head_gd->sids.no_srs << "\n" << flush;
      debug_stream.unlock();
  }
#endif	 

  if ((destn->groups_type == SRVR_type) || 
      ((destn->groups_type == GRP_type) && 
      ((destn->grp_srvr.group).head_gd->sids.no_srs == 1)))
  {
    /* UNICAST */

    t_out = ((timeout) / (retry + 1) + ROUNDTRIP_TIME);

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "multi_call: must multicast call, timeout per call is " << t_out
		 << " centiseconds\n" << flush;
    debug_stream.unlock();
    
#endif

    machineAlive = TRUE;
    
    do
    {
      if (result_buf.mc_msg[0].iov_base)
      {
	free(result_buf.mc_msg[0].iov_base);
	result_buf.mc_msg[0].iov_base = 0;
      }
      if (result_buf.mc_msg[2].iov_base)
      {
#ifndef GCC_ARRAY_DELETE_BUG
	delete [] result_buf.mc_msg[2].iov_base;
#else
	delete result_buf.mc_msg[2].iov_base;
#endif	
	result_buf.mc_msg[2].iov_base = 0;
      }
      if (result_buf.mc_msg[3].iov_base)
      {
#ifndef GCC_ARRAY_DELETE_BUG
	delete [] result_buf.mc_msg[3].iov_base;
#else
	delete result_buf.mc_msg[3].iov_base;
#endif	
	result_buf.mc_msg[3].iov_base = 0;
      }

      call_seqno.retry = htonl(call_sn.retry);  /* retry value changes */
      
      /* Transmit Call */

      if ((RAJDOOT_USE_BROADCAST) && (msg_ok))
      {
	  if (destn->groups_type == GRP_type)
	      xstat = multicast_long_send(&(destn->grp_srvr.group.head_gd->mymid), &req_buf);
	  else
	      xstat = multicast_long_send(&(destn->grp_srvr.srvr.sr_id), &req_buf);
      }
      else
	  if (msg_ok)
	      xstat = multicast_send_message(objects, &req_buf);

      if (xstat == MC_OK)
      {
#ifdef DEBUG
	  debug_stream.lock();
	  
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "multi_call: sent okay\n" << flush;
	  debug_stream.unlock();
	  
#endif

	/* Receive result - assuming no filter procedure for unicasting */
	if (supplied_filter)
	{
#ifdef DEBUG
	    debug_stream.lock();
	    
	    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	    debug_stream << "multi_call: waiting to unicast recv with supplied filter\n" << flush;
	    debug_stream.unlock();
	    
#endif
	    mc_recv_rcode = multicast_long_receive(&client, &ack,
						   filter->filt_name, t_out);

	    /* filter must set output parameters -  allocated buffer and size received */
	    /* must also delete buffers */

	  if (packed_ags)
#ifndef GCC_ARRAY_DELETE_BUG
	      delete [] packed_ags;
#else
	      delete packed_ags;
#endif

	  return(FILTER_DETERMINED);
	}
	else
	{
	  /* timeout for recv in centisecs */
#ifdef DEBUG
	    debug_stream.lock();
	    
	    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	    debug_stream << "multi_call: waiting to unicast recv with default filter\n" << flush;
	    debug_stream.unlock();
	    
#endif

	    if (objects[0].grps_id.groups_type != SRVR_dead)
	    {
		if (multicast_long_receive(&client, &ack, 0, t_out) > 0)
		{
		    result_seqno = * (struct RPC_SN *) result_buf.mc_msg[0].iov_base;
		    result_opcode = * (Int32 *) result_buf.mc_msg[1].iov_base;
	    
#ifdef DEBUG
		    debug_stream.lock();
	    
		    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
		    debug_stream << "multi_call: received okay\n";
		    debug_stream << "multi_call: seqnos are: <expected> <returned> <" << call_seqno.sec
				 << "," << call_seqno.msec << "> <" << result_seqno.sec << ","
				 << result_seqno.msec << ">\n" << flush;
		    debug_stream.unlock();
	    
#endif
		    /* Byte Swap */
	    
		    /* Check 1st 2 fields of Sequence Number - in network byte order */
		    if (identical_sn(&result_seqno, &call_seqno))
		    { /* Unicasting, only 1 seqno to check, can be done outside filter */
			/* Set output parameters  - allocated buffer and size received */
			result->rpc_buff1 = (char*) result_buf.mc_msg[2].iov_base;
			result->rpc_length1 = result_buf.mc_msg[2].iov_len;
			result->rpc_buff2 = (char*) result_buf.mc_msg[3].iov_base;
			result->rpc_length2 = result_buf.mc_msg[3].iov_len;
			result->rpc_code = result_opcode;  /* User byte swaps opcodes */
			
			rcode = SUCCEEDED;
			msg_ok = TRUE;
		    }
		    else
		    {
			msg_ok = FALSE;
#ifdef DEBUG
			debug_stream.lock();
		
			debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
			debug_stream << "multi_call: seqnos did not match\n" << flush;
			debug_stream.unlock();
#endif
		    }
		}
		else
		{
		    msg_ok = TRUE;
#ifdef DEBUG
		    debug_stream.lock();
	      
		    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
		    debug_stream << "multi_call: receive failed: " << errno << "\n" << flush;
		    debug_stream.unlock();
#endif
		}
	    }
	    else
		rcode = RPC_NOTDONE;

	  if (result_buf.mc_msg[0].iov_base)
	  {
	      free(result_buf.mc_msg[0].iov_base);
	      result_buf.mc_msg[0].iov_base = 0;
	  }
	} /* end else */
      }
      else
      {
	  msg_ok = TRUE;
	  rcode = RPC_UNABLE;

	/*
	 * Something went wrong with the send, so try again.
	 */

#ifdef DEBUG
	error_stream << WARNING << "rpc multi_call: send failed." << endl;
#endif	    

#ifdef DEBUG
	  debug_stream.lock();
	  
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "multi_call: send failed: " << errno << "\n" << flush;
	  debug_stream.unlock();
#endif
      }

      if (!RAJDOOT_USE_BROADCAST)
      {
	  if (rcode != SUCCEEDED)
	  {
	      machineAlive = ping(objects[0].destn_host, timeTaken);    // works because there is only one "group" member.

	      if (machineAlive == FALSE)
	      {
		  objects[0].grps_id.groups_type = SRVR_dead;
		  rcode = RPC_NOTDONE;
	      }
	      else
	      {
		  t_out -= (timeTaken/10000);
		  if (t_out <= 0)
		  {
#ifdef DEBUG
		      error_stream << WARNING << "multi_call::no response from server on available machine " << objects[0].destn_host << endl;
#endif
		      rcode = RPC_UNABLE;  // indicate machine is alive, but no reply received.
		  }
	      }
	  }
      }
    } while ((call_sn.retry -- > 0) && 
	     (((rcode != SUCCEEDED) && (!supplied_filter)) ||
	     ((supplied_filter) && (mc_recv_rcode < 1))) && (machineAlive));
 
    if (packed_ags)
#ifndef GCC_ARRAY_DELETE_BUG
	::delete [] packed_ags;
#else
        ::delete packed_ags;
#endif    

    if (result_buf.mc_msg[0].iov_base)
    {
	free(result_buf.mc_msg[0].iov_base);
	result_buf.mc_msg[0].iov_base = 0;
    }

    return(rcode);
  }
  else /* MULTICAST */
  {
    /* timeout for each call */
    t_out = ((timeout) / (retry + 1) + ROUNDTRIP_TIME);

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "multi_call: Must multicast - timeout per call:" << t_out << ", overall:"
	<< timeout << "\n";
    debug_stream << "multi_call: number of servers in group: " << no_servers << "\n" << flush;
    debug_stream.unlock();
    
#endif    

    /* Set global variables to be shared with filter */
    /* Default successes expected for multi_call is all */
    successes_expected = filter->no_successes;
    if ((successes_expected < 1) || (successes_expected > no_servers))
      successes_expected = no_servers; /* Default successes */

    compare_crit = (((filter->compare != compare_OPCODE) &&
		    (filter->compare != compare_BUFFERS) &&
		    (filter->compare != compare_BOTH))
		    ? compare_NONE : filter->compare);

    total_received = 0;
    filt_replygrp = &(destn->grp_srvr.group);
    init_filtgrp(filt_replygrp, &t_servers);
    machineAlive = TRUE;
    
    do
    {
      if (result_buf.mc_msg[0].iov_base)
      {
        free(result_buf.mc_msg[0].iov_base);
        result_buf.mc_msg[0].iov_base = 0;
      }
      if (result_buf.mc_msg[2].iov_base)
      {
#ifndef GCC_ARRAY_DELETE_BUG
        delete [] result_buf.mc_msg[2].iov_base;
#else
	delete result_buf.mc_msg[2].iov_base;
#endif	
        result_buf.mc_msg[2].iov_base = 0;
      }
      if (result_buf.mc_msg[3].iov_base)
      {
#ifndef GCC_ARRAY_DELETE_BUG
        delete [] result_buf.mc_msg[3].iov_base;
#else
	delete result_buf.mc_msg[3].iov_base;
#endif	
        result_buf.mc_msg[3].iov_base = 0;
      }

      call_seqno.retry= htonl(call_sn.retry);  /* retry value changes */
      /* Transmit Call */
      if ((RAJDOOT_USE_BROADCAST) && (msg_ok))
	  mul_ret = multicast_long_send(&(destn->grp_srvr.group.head_gd->mymid),&req_buf);
      else
	  if (msg_ok)
	      mul_ret = multicast_send_message(objects, &req_buf);
      if (mul_ret == MC_OK)
      {
#ifdef DEBUG
	  debug_stream.lock();
	  
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "multi_call: executing loop - Call: sent okay\n" << flush;
	  debug_stream.unlock();
	  
#endif
	/* Receive result */
	
	if (supplied_filter)
	{
#ifdef DEBUG
	    debug_stream.lock();
	    
	    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	    debug_stream << "multi_call: waiting to multicast recv with supplied filter\n" << flush;
	    debug_stream.unlock();
	    
#endif
	  mc_recv_rcode = multicast_long_receive(&client, &ack,
						 filter->filt_name, t_out);
	  /* timeout for recv in centisecs */
	}        
	else
	{
	  /* number of successful call executions */
	  
#ifdef DEBUG
	    debug_stream.lock();
	    
	    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	    debug_stream << "multi_call: waiting to multicast recv with default filter\n" << flush;
	    debug_stream.unlock();
	    
#endif

	  if ((df_recv_rcode = multicast_long_receive(&client, &ack, default_filter, t_out)) > 0)
	  {
#ifdef DEBUG
	      debug_stream.lock();
	      
	      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	      debug_stream << "multi_call: receive using default filter succeeded\n" << flush;
	      debug_stream.unlock();
	      
#endif
	  }
	  else
	  {
	      msg_ok = TRUE;
#ifdef DEBUG
	      debug_stream.lock();
	      
	      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	      debug_stream << "multi_call: receive using default filter failed: " << errno << "\n" << flush;
	      debug_stream.unlock();
	      
#endif
	  }
	} /* end else */
      }
      else
      {
	  msg_ok = TRUE;
#ifdef DEBUG
	  debug_stream.lock();
	  
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "multi_call: send failed: " << errno << "\n" << flush;
	  debug_stream.unlock();
	  
#endif
      }
      failed = ((((!supplied_filter) && (df_recv_rcode < 1)) || 
		((supplied_filter) && (mc_recv_rcode < 1))) ? TRUE : FALSE);

      if (!RAJDOOT_USE_BROADCAST)
      {
	  if (failed)
	  {
	      for (int i = 0; i < no_servers; i++)
	      {
	          machineAlive = ping(objects[i].destn_host, timeTaken);
		  
		  if (machineAlive == FALSE)
		  {
		       objects[0].grps_id.groups_type = SRVR_dead;
		       df_recv_rcode = mc_recv_rcode = RPC_NOTDONE;
		  }
		  else
		  {
		      t_out -= (timeTaken/10000);
		      if (t_out <= 0)
		      {
#ifdef DEBUG
			  error_stream << WARNING << "multi_call::no response from server on available machine " << objects[i].destn_host << endl;
#endif	  
			  df_recv_rcode = mc_recv_rcode = RPC_UNABLE;  // indicate machine is alive, but no reply received.
		      }
		  }
	      }
	  }
      }
    } while ((call_sn.retry-- > 0) && (failed) && (total_received < no_servers)
	     && (machineAlive) && (df_recv_rcode != ETIMEOUT) && (mc_recv_rcode != ETIMEOUT));
  }

  if (packed_ags)
#ifndef GCC_ARRAY_DELETE_BUG
      delete [] packed_ags;
#else
      delete packed_ags;
#endif

  if (result_buf.mc_msg[0].iov_base)
  {
      free(result_buf.mc_msg[0].iov_base);
      result_buf.mc_msg[0].iov_base = 0;
  }

  /* Even if call fails, this parameter indicates how many replied */
  filter->no_successes = total_received;
  if (failed)
  {
    _rpcerr = FEW_SUCCESSES;
    return(RPC_UNABLE);
  }
  else
  {
    if (!supplied_filter)
    {
      /* Set output parameters  */
      *result = correct_rpc; 

#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "multi_call: received result: opcode " << result->rpc_code << "\n" << flush;
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "and buffer: " << result->rpc_buff1 << " size " << result->rpc_length1
	  << " buffer: " << result->rpc_buff2 << " size " << result->rpc_length2 << "\n" << flush;
      debug_stream.unlock();
      
#endif
    }
    return(SUCCEEDED);
  }
  
}
/* end Multi_Call() */  

/**********************************************/
/* Routine that retries calls to deaf servers */
/**********************************************/

int prodserver (Int32 opn_code, struct MID serverid, struct MID sender,
		struct MID group, struct OBJECT_DESC *objects)
{
  /*
   * Send group management operation request given by 'opn_code' to be
   * performed on group denoted by 'group' to server 'serverid'.  Try a few
   * synchronous calls, and if there is no response, send a request to
   * manager on that server's node to fork a special process that signals
   * the server.
   */
     
  struct mc_msghdr  req_buf, res_buf;
  struct mc_sg_filter ack;
  struct RPC_SN request_sn, result_sn;
  struct MID unithem;
  struct MID    redun_fields;
  char redun_name[OBJECT_NAME_SIZE];
  Boolean failure, finished;
  Int32  rs_code, request_code;
  int   xretry, rretry, xrstat, indx = 0;
  Int32 df_serverport;

  checkEnvironment();

  ::memset(redun_name, '\0', OBJECT_NAME_SIZE);

  /* Parameter range checks */
  if (serverid.hi_part == 0)
    return(INVALID_HOST);
  if ((opn_code < INT_INITIALISE) || (opn_code > INT_LEAVEGRP))
  {
#ifdef DEBUG
      debug_stream.lock();
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "invalid op: " << opn_code << " int init(" << INT_INITIALISE << ") termall("
		   << INT_LEAVEGRP << ")\n" << flush;
      debug_stream.unlock();
#endif
    return(INVALID_OP);
  }
  
  getsn(&request_sn);
  request_sn.sec = htonl(request_sn.sec);  /* Byte swap */
  request_sn .msec = htonl(request_sn .msec);

  opn_code = htonl(opn_code);

  /* Build group management operation Request */
  req_buf.mc_msg[0].iov_base = (char *) &request_sn;
  req_buf.mc_msg[0].iov_len = sizeof(struct RPC_SN);
  req_buf.mc_msg[1].iov_base = (char *) &opn_code;
  req_buf.mc_msg[1].iov_len = sizeof(opn_code);
  req_buf.mc_msg[2].iov_base = (char *) &group;
  req_buf.mc_msg[2].iov_len = sizeof(struct MID);
  req_buf.mc_msg[3].iov_len = 0;
  req_buf.mc_msg[3].iov_base = 0;
  req_buf.mc_msg[4].iov_len = 0;
  req_buf.mc_msg[4].iov_base = 0;
  req_buf.mc_msg[5].iov_len = 0;
  req_buf.mc_msg[5].iov_base = 0;
  req_buf.mc_msg[6].iov_len = 0;
  req_buf.mc_msg[6].iov_base = 0;
  req_buf.msg_len = 7;

  /* Prepare Result Buffer */
  res_buf.mc_msg[0].iov_len = sizeof(struct RPC_SN);
  res_buf.mc_msg[0].iov_base = 0;
  rs_code = ZEROCODE;
  res_buf.mc_msg[1].iov_len = sizeof(rs_code);
  res_buf.mc_msg[1].iov_base = 0;
  res_buf.msg_len = 2;
  ack.next_fs = NULL;
  ack.mc_msg = &res_buf;
  
  /* Send request and await acknowledgement */
  rretry = SYNCH_RETRY;
  failure = FALSE;
  do
  {
    xretry = SYNCH_RETRY;
    xrstat = FAILED;

    do
    {
	if (RAJDOOT_USE_BROADCAST)
	    xrstat = multicast_long_send(&serverid, &req_buf);
	else
	    xrstat = multicast_send_message(objects, &req_buf);
    } while ((xrstat != MC_OK) && (--xretry > 0));

    if (xrstat == MC_OK)
    {
#ifdef DEBUG
	debug_stream.lock();
	
        debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "prodserver: sent okay\n" << flush;
	debug_stream.unlock();
	
#endif
      xrstat = FAILED;
      xrstat = multicast_long_receive(&client, &ack, 0, SYNCH_TOUT);
    }
#ifdef DEBUG
    else
    {
	debug_stream.lock();
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "prodserver: send failed\n" << flush;
	debug_stream.unlock();
    }
#endif

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "prodserver: receive returned with code " << xrstat << "\n" << flush;
    debug_stream.unlock();
    
#endif

    if (xrstat > 0)
    {
      /* Cast result buffer */
      result_sn = * (struct RPC_SN *) res_buf.mc_msg[0].iov_base;
      rs_code = * (Int32 *)            res_buf.mc_msg[1].iov_base;
    }

    if (res_buf.mc_msg[0].iov_base)
    {
#ifndef GCC_ARRAY_DELETE_BUG
	delete [] res_buf.mc_msg[0].iov_base;
#else
        delete res_buf.mc_msg[0].iov_base;
#endif
	res_buf.mc_msg[0].iov_base = 0;
    }
    if (res_buf.mc_msg[1].iov_base)
    {
#ifndef GCC_ARRAY_DELETE_BUG
	delete [] res_buf.mc_msg[1].iov_base;
#else
        delete res_buf.mc_msg[1].iov_base;
#endif
	res_buf.mc_msg[1].iov_base = 0;
    }

    /* Get null sn and opcode - when preparing buffers for receiving */
    failure = (((xrstat <= 0) || (ntohl(rs_code) != DONE) ||
	      (!identical_sn(&result_sn, &request_sn))) ? TRUE : FALSE);

  } while ((failure) && (--rretry > 0));
  
  if (!failure)
  {
#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "prodserver: (!failure1) received okay\n" << flush;
      debug_stream.unlock();
      
#endif
    if (res_buf.mc_msg[0].iov_base)
    {
#ifndef GCC_ARRAY_DELETE_BUG
	delete [] res_buf.mc_msg[0].iov_base;
#else
        delete res_buf.mc_msg[0].iov_base;
#endif
	res_buf.mc_msg[0].iov_base = 0;
    }
    if (res_buf.mc_msg[1].iov_base)
    {
#ifndef GCC_ARRAY_DELETE_BUG
	delete [] res_buf.mc_msg[1].iov_base;
#else
        delete res_buf.mc_msg[1].iov_base;
#endif
	res_buf.mc_msg[1].iov_base = 0;
    }

    return(SUCCEEDED);
  }
  else
  {
#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "prodserver: about to signal server\n" << flush;
      debug_stream.unlock();
      
#endif
    /* Make asynchronous call to server via the manager on its node */

     getsn(&request_sn);
     request_sn.sec = htonl(request_sn.sec);  /* Byte swap */
     request_sn .msec = htonl(request_sn .msec);

    request_code = htonl(GM_SERVER);

    if (RAJDOOT_USE_BROADCAST)
    {
	df_serverport = serverid.lo_part;  /* ids always in network byte order */

	/* Set up manager's address */
	unithem.type = htonl(UID_type);
	unithem.lo_part = htonl(managerAddr);
	unithem.hi_part = sender.hi_part;
	finished = TRUE;
    }
    
    /* Build GETWORKER Request */
    req_buf.mc_msg[0].iov_base = (char *) &request_sn;
    req_buf.mc_msg[0].iov_len = sizeof(struct RPC_SN);
    req_buf.mc_msg[1].iov_base = (char *) &request_code;
    req_buf.mc_msg[1].iov_len = sizeof(request_code);
    req_buf.mc_msg[2].iov_base = (char *) &group;
    req_buf.mc_msg[2].iov_len = sizeof(struct MID);
    req_buf.mc_msg[3].iov_base = (char *) &redun_fields;
    req_buf.mc_msg[3].iov_len = sizeof(struct MID);
    req_buf.mc_msg[4].iov_base = (char *) &redun_fields;
    req_buf.mc_msg[4].iov_len = sizeof(struct MID);
    req_buf.mc_msg[5].iov_base = (char *) &opn_code;
    req_buf.mc_msg[5].iov_len = sizeof(opn_code);
    req_buf.mc_msg[6].iov_base = (char *) &df_serverport;
    req_buf.mc_msg[6].iov_len = sizeof(df_serverport);
    req_buf.mc_msg[7].iov_base = redun_name;
    req_buf.mc_msg[7].iov_len = OBJECT_NAME_SIZE;
    req_buf.mc_msg[8].iov_base = NULL;
    req_buf.mc_msg[8].iov_len = 0; 
    req_buf.msg_len = 9;

    /* Prepare Result Buffer */
    res_buf.mc_msg[0].iov_len = sizeof(struct RPC_SN);
    res_buf.mc_msg[0].iov_base = 0;
    rs_code = ZEROCODE;
    res_buf.mc_msg[1].iov_len = sizeof(rs_code);
    res_buf.mc_msg[1].iov_base = 0;
    res_buf.mc_msg[2].iov_len = sizeof(struct MID);
    res_buf.mc_msg[2].iov_base = 0;
    res_buf.mc_msg[3].iov_len = sizeof(struct MID);
    res_buf.mc_msg[3].iov_base = 0;
    res_buf.msg_len = 4;
    ack.next_fs = NULL;
    ack.mc_msg = &res_buf;

    do
    {
	if (!RAJDOOT_USE_BROADCAST)
	{
	    df_serverport = objects[indx].grps_id.grp_srvr.srvr.sr_id.lo_part;  /* ids always in network byte order */
	    
	    /* Set up manager's address */
	    unithem.type = htonl(UID_type);
	    unithem.lo_part = htonl(managerAddr);
	    unithem.hi_part = objects[indx].destn_host;

	    indx++;
	    finished = ((objects[indx].destn_host == 0) ? TRUE : FALSE);
	}
	    
	/* Send request to manager and await acknowledgement from server */
	rretry = 0; /*ASYNCH_RETRY;  */
	failure = FALSE;
	do
	{
	    xretry = ASYNCH_RETRY;
	    xrstat = FAILED;
      
	    do
	    {
		xrstat = multicast_long_send(&unithem, &req_buf);
	    } while ((xrstat != MC_OK) && (--xretry > 0));
      
	    if (xrstat == MC_OK)
	    {
#ifdef DEBUG
		debug_stream.lock();
		
		debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
		debug_stream << "prodserver: sent okay\n" << flush;
		debug_stream.unlock();
		
#endif
		xrstat = FAILED;
		xrstat = multicast_long_receive(&client, &ack, 0, SYNCH_TOUT);
	    }
#ifdef DEBUG
	    else
	    {
		debug_stream.lock();
		debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
		debug_stream << "prodserver: send failed\n" << flush;
		debug_stream.unlock();
	    }
#endif

	    if (xrstat > 0)
	    {
		/* Cast result buffer */
		result_sn = * (struct RPC_SN *) res_buf.mc_msg[0].iov_base;
		rs_code = * (Int32 *)            res_buf.mc_msg[1].iov_base;

#ifdef DEBUG
		debug_stream.lock();
		debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
		debug_stream << "prodserver: received rcode: " << rs_code << " (" << DONE << "-DONE)\n" << flush;
		debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
		debug_stream << "seqno: sent - <" << result_sn.sec << "," << result_sn.msec << "> expecting - <"
			     << request_sn.sec << "," << request_sn.msec << ">\n" << flush;
		debug_stream.unlock();
#endif
	    }

	    if (res_buf.mc_msg[0].iov_base)
	    {
#ifndef GCC_ARRAY_DELETE_BUG
		delete [] res_buf.mc_msg[0].iov_base;
#else
		delete res_buf.mc_msg[0].iov_base;
#endif	  
		res_buf.mc_msg[0].iov_base = 0;
	    }
	    if (res_buf.mc_msg[1].iov_base)
	    {
#ifndef GCC_ARRAY_DELETE_BUG
		delete [] res_buf.mc_msg[1].iov_base;
#else
		delete res_buf.mc_msg[1].iov_base;
#endif	  
		res_buf.mc_msg[1].iov_base = 0;
	    }
      
	    failure = (((xrstat < 1) || (ntohl(rs_code) != DONE) ||
			(!identical_sn(&result_sn, &request_sn))) ? TRUE : FALSE);

	} while ((failure) && (--rretry > 0));
     
	if (res_buf.mc_msg[0].iov_base)
	{
#ifndef GCC_ARRAY_DELETE_BUG
	    delete [] res_buf.mc_msg[0].iov_base;
#else
	    delete res_buf.mc_msg[0].iov_base;
#endif	
	    res_buf.mc_msg[0].iov_base = 0;
	}
	if (res_buf.mc_msg[1].iov_base)
	{
#ifndef GCC_ARRAY_DELETE_BUG
	    delete [] res_buf.mc_msg[1].iov_base;
#else
	    delete res_buf.mc_msg[1].iov_base;
#endif	
	    res_buf.mc_msg[1].iov_base = 0;
	}
      
	if (!failure)
	{
#ifdef DEBUG
	    debug_stream.lock();
	    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	    debug_stream << "prodserver: received (!failure2) okay\n" << flush;
	    debug_stream.unlock();
#endif
	    return(SUCCEEDED);
	}
	else
	{
#ifdef DEBUG
	    debug_stream.lock();
	    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	    debug_stream << "prodserver: did not receive correct message\n" << flush;
	    debug_stream.unlock();
#endif
	    return(FAILED);
	} /* end else (!failure) */
    } while (!finished);
  }
} /* end prodserver() */

int getpath (pid_t pid, struct PATHS *p_path)
{
/*
 * Invoked by initiate() to obtain rpc path.
 * Read C_LIST file and build the possible rpc path associated with
 * the process identified by `pid'; the rpc path is placed in the
 * variable pointed at by `p_path'. Note that this variable has to
 * be large enough to hold the entire rpc path (at most NSTATION
 * elements of type PATH_EL in this implementation).
 * The integer value returned by `getpath()' indicates the actual
 * number of elements of the rpc path. The return value `-1' indicates
 * that an error has occurred, `errno' contains a diagnostic of that error.
 */
    
  struct C_LIST_EL* clist = NULL;
  Int32    newnode;
  int     pathlength = -1, rstat, retry, i, j, k;
  
  /* Open Clist */
  retry = FILESRETRY; rstat = FAILED;
  do
  {
      rstat = clistopen(clist);
  } while ((rstat != SUCCEEDED) && (retry-- > 0));
  if (rstat != SUCCEEDED)
  {
#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "Error::getpath: open failed. Errno " << errno << "\n" << flush;
      debug_stream.unlock();
      
#endif

  /* clist open failed, so simply "close" clist without writing anything back. */
      
      int openErr = _rpcerr;  // in case we encounter an error on close
      
      (void) clistclose();
#if 0
      /* supress abort */
      
      if (openErr == OPENFAILED)
      {
	  error_stream << FATAL << "clist fatal error." << endl;
	  ArjunaFatal();
      }
#endif      
    return(pathlength);
  }

  /* Construct RPC path */
  pathlength = 0;
  for (j = 0; j < MAXSERVERS; j++)
  {
    if (clist->s_desc[j].process_id == pid)
    {
      pathlength++;
      for (i = 0; i < MAXCLIENTS; i++)
      {
	k= 0;
	while ((k < NSTATION) &&
	       ((newnode = clist->s_desc[j].linkspath[i].rpcpath[k].node) != 0))
	{
	  p_path->node = newnode;
	  p_path->c_count = clist->s_desc[j].linkspath[i].rpcpath[k].c_count;

	  p_path++;  k++;
	}
      }
    }
  }
  
  /* Close clist */
  retry = FILESRETRY;  	rstat = FAILED;
  do
  {
      rstat = clistclose();
  } while ((rstat != SUCCEEDED) && (retry-- > 0));
#ifdef DEBUG
  if (rstat != SUCCEEDED)
  {
      debug_stream.lock();
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "Error::rpcs: close failed. Errno " << errno << "\n" << flush;
      debug_stream.unlock();
  }
#endif  

    return(pathlength);

} /* end getpath() */

Int32 getccount ()    
{
/* Invoked by initiate() to obtain crash count.
 * Read C_COUNT file and return the crash count of the local node,
 * if successful; otherwise -1.
 */

  int ccountfd;

  if (crashcount < 0)
  {
    if ((ccountfd = open(C_COUNT_Q, O_RDWR, 0444)) >= 0)
    {
      struct flock arg;

      arg.l_whence = 0;
      arg.l_start = 0;
      arg.l_len = 0;
      arg.l_type = F_WRLCK;
      if (fcntl(ccountfd, F_SETLKW, &arg) >= 0)
      {         /* Read C_COUNT */
        if(read(ccountfd, (char *) & crashcount, sizeof(crashcount))
	   != sizeof(crashcount))
        {    /* Read failed - reset crashcount to error code */
          crashcount = -1;
        }
	arg.l_type = F_UNLCK;
        fcntl(ccountfd, F_SETLKW, &arg);
      }
      close(ccountfd);
    }
  }    
  return(crashcount);
} /* end getccount */

Int32 getmyhost ()
{
/*
 * This routine returns the node's internetwork host number if successful,
 * otherwise 0.
 */
  struct host_address host_addr;
  static Int32        myhost = 0;

  if (myhost == 0)
  {
      char r_host[MAXHOSTNAMELEN+1];
      ::memset(r_host, '\0', MAXHOSTNAMELEN+1);
      
      if (gethostname(r_host, MAXHOSTNAMELEN) == 0)
      {
	  host_addr.real_host = r_host;
	  if (getloghost(&host_addr) == SUCCEEDED)
	      myhost = host_addr.log_host;
      }               
      else
	  myhost = 0;
  }

  return myhost;
}
