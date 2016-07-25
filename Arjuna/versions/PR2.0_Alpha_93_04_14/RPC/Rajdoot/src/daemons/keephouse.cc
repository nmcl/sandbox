/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: keephouse.cc,v 1.19 1993/03/18 15:37:52 nmcl Exp $
 */

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef FSTREAM_H_
#  include <System/fstream.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef RAJDOOT_RPCUSER_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif

#ifndef RPC_H_
#  include "rpc.h"
#endif

static const char RCSid[] = "$Id: keephouse.cc,v 1.19 1993/03/18 15:37:52 nmcl Exp $";

struct MID h_keeper;

extern char* clistfd;
extern int sem_id;

Boolean IsNodeDead (struct PATHS *path, long *newccount)
{
  struct MID manager;
  struct RPC_SN rq_sn, rs_sn;
  struct mc_msghdr req_buf, res_buf;  
  struct mc_sg_filter ack;
  Boolean crashed_node = TRUE, wrong_call;
  long rq_code;
  long manager_port = MANAGER_ADDR;
  int rretry, xretry, xrstat;

  /* Transmit call to client node's manager process */

  /* Set up Manager's unicast address */
  manager.type = htonl((long)UID_type);
  manager.lo_part = htons((unsigned short) manager_port);
  manager.hi_part = path->node;

  getsn(&rq_sn);  /* Obtain a sequence number for request */
  rq_code = GETMECCOUNT;    /* operation code to get crashcount */

  /* Byte swap */
  rq_sn.sec = htonl(rq_sn.sec);
  rq_sn.msec = htonl(rq_sn.msec);
  rq_code = htonl(rq_code);

  /* Build Request buffer */
  req_buf . mc_msg[0] . iov_base = (char *) &rq_sn;
  req_buf . mc_msg[0] . iov_len = sizeof(struct RPC_SN);
  req_buf . mc_msg[1] . iov_base = (char *) &rq_code;
  req_buf . mc_msg[1] . iov_len = sizeof(rq_code);
  req_buf . mc_msg[2] . iov_len = 0;
  req_buf . mc_msg[3] . iov_len = 0;
  req_buf . mc_msg[4] . iov_len = 0;
  req_buf . mc_msg[5] . iov_len = 0;
  req_buf . mc_msg[6] . iov_len = 0;
  req_buf . mc_msg[7] . iov_len = 0;
  req_buf . mc_msg[8] . iov_len = 0;
/*  req_buf . mc_msg[2] . iov_base = (char *) */
  req_buf . msg_len = 9;
  
  /* Prepare result buffer */
  res_buf . mc_msg[0] . iov_len = sizeof(struct RPC_SN);
  *newccount = -1;
  res_buf . mc_msg[1] . iov_len = sizeof(*newccount);
  res_buf . msg_len = 2;
  ack . next_fs = NULL;
  ack . mc_msg = &res_buf;

  rretry =  HK_MAXRETRY;
  xrstat = FAILED;
  do
  {
    xretry =  HK_MAXRETRY;
    xrstat = FAILED;
    do
    {
      xrstat = multicast_long_send(&manager, &req_buf);
    } while ((xrstat != MC_OK) && (--xretry > 0));
    if (xrstat == MC_OK)
    {
      xrstat = FAILED;
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "Housekeeper: sent message - awaiting reply\n" << flush;
#endif
      xrstat = multicast_long_receive(&h_keeper, &ack, 0, HK_TIMEOUT); 
    }
#ifdef DEBUG
    else
    {
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "Housekeeper: send failed" << flush;
    }
#endif

    if (xrstat > 0)
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "Housekeeper: received reply" << flush;
#endif
      /* Cast result buffer */
      rs_sn = * (struct RPC_SN *)   res_buf.mc_msg[0].iov_base;
      *newccount = * (long *)        res_buf.mc_msg[1].iov_base;
      /* Byte swap crash count - sequence numbers are in network byte order */
      *newccount = ntohl(*newccount);
    }

    wrong_call = (((!identical_sn(&rs_sn, &rq_sn)) || (xrstat < 1))?TRUE : FALSE);

  } while ((wrong_call) && (--rretry > 0));
    
  if (!wrong_call)
    crashed_node = (path->c_count < *newccount) ? TRUE : FALSE;

  return(crashed_node);

} /* end IsNodeDead() */

int check_for_orphans ()
{
  struct C_LIST_EL clist;
  struct SERVER_DESC *srvr_desc;
  struct PATHS *current_path;
  Boolean crashed_node;
  long client_node, newccount;
  int retry, rstat;
  int server_indx, link_indx, path_indx, client_indx;

#if (RAJDOOT_USE_SHARED_MEMORY == NO)
  int                 f_clistfd;
#endif

  retry = FILESRETRY; rstat = FAILED;
  do
  {
#if (RAJDOOT_USE_SHARED_MEMORY == NO)
      rstat = f_clistopen(&f_clistfd, &clist);
#else
      rstat = clistopen(&clist);
#endif
  } while ((rstat != SUCCEEDED) && (retry-- > 0));
  if (rstat != SUCCEEDED)
  {
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "housekeeper: failed to open clist\n" << flush;
#endif
    return(FAILED);
  }

  for (server_indx = 0; server_indx < MAXSERVERS; server_indx++)
  {
    if (clist.s_desc[server_indx].poflag)
    {
      srvr_desc = &(clist.s_desc[server_indx]);
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "housekeeper: server " << srvr_desc->process_id << " is a potential orphan\n" << flush;
#endif
      /* Check crash counts of all nodes in server's links - by
	 contacting the managers at those nodes */

      link_indx = 0;
      do
      {
	if (!isnull_id(&(srvr_desc->linkspath[link_indx].groupid)))
	{
	  path_indx = 0;
	  crashed_node = FALSE;
	  current_path = &(srvr_desc->linkspath[link_indx].rpcpath[path_indx]);

	  while ((current_path->node != 0) && (!crashed_node))
	  {
	    /* Transmit call to client node's manager and check
	       its crash count */

	    if (crashed_node = IsNodeDead(current_path, &newccount))
	    {
	      client_node = current_path->node;
	      client_indx = get_clientindx(&clist, client_node);
	      RmClientFromPath(&clist, client_indx, client_node);

	      /* Log in client node's new crash count */
	      clist.client[client_indx].c_count = newccount;
	    }

	    path_indx++; 
	    current_path = &(srvr_desc->linkspath[link_indx].rpcpath[path_indx]);
	  } /* end while */
	} /* end if */
	
	link_indx++;
      } while ((link_indx < MAXCLIENTS) && (srvr_desc->no_links > 0));
    } /* end if */
  } /* end for */

  /* Close clist */
  retry = FILESRETRY;  	rstat = FAILED;
  do
  {
#if (RAJDOOT_USE_SHARED_MEMORY == NO)
      rstat = f_clistclose(&f_clistfd, &clist);
#else
      rstat = clistclose(&clist);
#endif
  } while ((rstat != SUCCEEDED) && (retry-- > 0));
  if (rstat != SUCCEEDED)
  {
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "housekeeper: failed to close clist" << flush;
#endif
    return(FAILED);
  }

  return(SUCCEEDED);

} /* end check_for_orphans() */
  
int main ()
{
#ifdef DEBUG
  _arjuna_debug->set_debuglevel(FUNCTIONS);
#endif

   if (chdir("/tmp"))
       perror("housekeeper failed to chdir to /tmp\n");

   ofstream target1("housekeeper_out");
   if (!target1)
       printf("housekeeper: Cannot open file housekeeper_out\n"); 
   else
       cout = target1;

   ofstream target2("housekeeper_err");
   if (!target2)
       printf("housekeeper: Cannot open file housekeeper_err\n");
   else
       cerr = target2;
  
  multicast_get_mid(&h_keeper); /* housekeeper's port for communication */

  for (;;)
  {
      sleep(10);
//    sleep(HK_INTERVAL);
    check_for_orphans();
  }

} 
   
