/*
 * Copyright (C) 1993-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: serverops.cc,v 1.8 1995/09/12 12:18:59 nmcl Exp $
 */

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef SYS_FILE_H_
#  include <System/sys/file.h>
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

extern int errno;

static const char RCSid[] = "$Id: serverops.cc,v 1.8 1995/09/12 12:18:59 nmcl Exp $";

int sendmessage (struct MID *to, struct MESSAGES *m_sg)
{
/* Transmit message 'm_sg' to client at address `to'.
 * Return values:
 * SUCCEEDED: result successfully transmitted;
 * FAILED: an error occured;  
 * rpcerr set to:-
 * errno: system error (see `errno.h');
 * EUNKNOWN: system error but `errno' was not set.
 */

    struct RPC_SN         seqno;
    struct mc_msghdr      res_buf;
    int                   rcode;

    /* Byte Swap */
    seqno.sec = htonl(h_lastrpcsn.sec);
    seqno.msec = htonl(h_lastrpcsn.msec);

    /* Build result for transmission */
    res_buf . mc_msg[0] . iov_base = (char *) &seqno; 
    res_buf . mc_msg[0] . iov_len = sizeof(seqno);
    res_buf . mc_msg[1] . iov_base = (char *) &(m_sg -> opcode);
    res_buf . mc_msg[1] . iov_len = sizeof(m_sg -> opcode);
    res_buf . mc_msg[2] . iov_base = (char *) &(m_sg -> server);
    res_buf . mc_msg[2] . iov_len = sizeof(struct MID);
    res_buf . mc_msg[3] . iov_base = (char *) &(m_sg -> sender);
    res_buf . mc_msg[3] . iov_len = sizeof(struct MID);
    res_buf . msg_len = 4;
    
    /* Later: could try sending several times */
    if ((rcode = multicast_long_send(to, &res_buf)) != MC_OK)
    {
      _rpcerr = (errno == 0) ? EUNKNOWN : errno;
#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "sendmessage: failed to send to client errno " << errno << ", code "
	  << rcode << "\n" << flush;
      debug_stream.unlock();
      
#endif
      return(FAILED);
    }
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "sendmessage: sent to client ok\n" << flush;
    debug_stream.unlock();
    
#endif
    return(SUCCEEDED);

} /* End sendmessage */

void senderror (struct MID *client_addr, Int32 error)
{
  struct MESSAGES m_sg;
  
  m_sg . opcode = htonl(error);
  m_sg . server . type = htonl(error);  
  sendmessage(client_addr, &m_sg);

} /* end senderror() */ 

int sendok (struct MID *client_addr, Int32 code)
{
  struct MESSAGES m_sg;
  
  m_sg . opcode = htonl(code); 
  m_sg . server . type = htonl(code);
  return sendmessage(client_addr, &m_sg);

} /* end sendok() */ 

void sendmyport (struct MID *client_addr, struct MID serverid, struct MID senderid)
{
  struct MESSAGES m_sg;

  m_sg . opcode = htonl((Int32) GW_OK);
  m_sg . server = serverid;
  m_sg . sender = senderid;
  sendmessage(client_addr, &m_sg);
  
  /* If sendmessage fails, client will retry; Operation is idempotent */
} /* end sendmyport() */


Boolean unset_sharecrit (struct MID actid, struct UNIQUEID uid)
{
  return (((actid.type == 0) && (actid.hi_part == 0) && (actid.lo_part == 0)
	   && (uid.host == 0) && (uid.sec == 0) && (uid.msec == 0)) ? TRUE : FALSE);
} /* end unset_sharecrit() */

Boolean found_sharecrit (struct MID given_actid, struct UNIQUEID given_uid, struct MID clist_actid, struct UNIQUEID clist_uid)
{

  return (((given_actid.type == clist_actid.type) &&
	  (given_actid.hi_part == clist_actid.hi_part) &&
	  (given_actid.lo_part == clist_actid.lo_part) &&
	  (given_uid.host == clist_uid.host) &&
	  (given_uid.sec == clist_uid.sec) &&
	  (given_uid.msec == clist_uid.msec)) ? TRUE : FALSE);
} /* end found_sharecrit() */


Boolean find_retry (struct C_LIST_EL *clist, int indx, Int32 clientnode, struct RPC_SN timestamp)
{
  /*
   * Determine whether server with identical share criterion as the one
   * that is about to log is the result of a retry - or requires that
   * an object manager be shared: (if the share criterion is set), or
   * requires a distinct server: (if share criterion is not set).
   */

  Boolean retry = FALSE;
  int     m = 0;

  while ((m < MAXCLIENTS) && (!retry))
  {
      int path_indx = 0;

      while ((clist->s_desc[indx].linkspath[m].rpcpath[path_indx].node != 0) && (!retry))
      {

#ifdef DEBUG
	  debug_stream.lock();
	  
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "find_retry: comparing " << clist->s_desc[indx].linkspath[m].rpcpath[path_indx].node << " and " << clientnode << endl;
	  debug_stream.unlock();
	  
#endif

	  if ((clist->s_desc[indx].linkspath[m].rpcpath[path_indx].node == clientnode) &&
	      (identical_sn(&(clist->s_desc[indx].linkspath[m].timestamp),
			    &timestamp)))
	  {
	      retry = TRUE;
#ifdef DEBUG
	      debug_stream.lock();
	      
	      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	      debug_stream << "This was a retry\n" << flush;
	      debug_stream.unlock();
	      
#endif
	  }
	  else
	      path_indx++;
      }
      
      m++;
  } /* end while */

  return(retry);
  
} /* end find_retry() */

void create_newlist (struct MID oldlist[S_ACTGRPSIZE], struct MID newlist[S_ACTGRPSIZE], Int32 opcode)
{
  int current_elt;
  
  switch (ntohl(opcode))
  {
  case BEGIN_GRP:
    /* Join current action group group */
    if (!isnull_id(&(newlist[S_CURRENTACT])))
      multicast_join(&(newlist[S_CURRENTACT]));
    break;

  case COMMIT_GRP:
    /* Leave current action group, join parent action group */
    if (!isnull_id(&(oldlist[S_CURRENTACT])))
      multicast_leave(&(oldlist[S_CURRENTACT]));
    if (!isnull_id(&(oldlist[S_PARENTACT])))
      multicast_join(&(oldlist[S_PARENTACT]));

    break;

  case ABORT_GRP:
    /* Leave current action group */
    if (!isnull_id(&(oldlist[S_CURRENTACT])))
      multicast_leave(&(oldlist[S_CURRENTACT]));
    break;

    /* default case should not arise; server does nothing to action
       groups if it does */

  }

  for (current_elt = 0; current_elt < S_ACTGRPSIZE; current_elt++)
    oldlist[current_elt] = newlist[current_elt];

} /* end create_newlist() */

void get_groups (struct MID grouplist[S_ACTGRPSIZE], char *start_buff, Int32 buff_size)
{
  /* Unpack groupids given by 'start_buff' and create a newlist of groupids
   * consisting of those groups; Call a merge routine to merge (and join/
   * leave relevant groups) the newlist obtained and the oldlist - given by
   * 'grouplist', return the new merged list via 'grouplist'
   */

  struct MID newlist[S_ACTGRPSIZE];
  Int32 opcode;
  int current_elt;
  char *buff, *next_buff;

  for (current_elt = 0; current_elt < S_ACTGRPSIZE; current_elt++)
    getnullmid(&(newlist[current_elt]));

  buff = next_buff = start_buff;

  next_buff = unpack_Int32(&opcode, buff);
  buff = next_buff;
  buff_size -= sizeof(opcode);

  if (ntohl(opcode) != NOOP_GRP)
  {
    if (buff_size > 0)
    {
      /* Unpack at most 2 MIDs, which is the maximum number transmitted */
      current_elt = 0;
      do
      {
	next_buff = unpack_HPID(&(newlist[current_elt++]), buff);
	buff_size -= (sizeof(struct MID) - 4);
	buff = next_buff;
      } while (buff_size > 0);

    }

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "get_groups: before creating newlist: \n" << flush;
    debug_stream << "\nOLDLIST: \n" << flush;
    for (current_elt = 0; current_elt < S_ACTGRPSIZE; current_elt++)
	printgid(grouplist[current_elt]);
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "\nNEWLIST: \n" << flush;
    for (current_elt = 0; current_elt < S_ACTGRPSIZE; current_elt++)
	printgid(newlist[current_elt]);
    debug_stream.unlock();
    
#endif

    create_newlist(grouplist, newlist, opcode);

  } /* end if */

} /* end get_groups() */

