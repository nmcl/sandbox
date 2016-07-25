/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: groupsrvr.cc,v 1.19 1993/04/27 14:45:50 ngdp Exp $
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

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef RAJDOOT_RPCUSER_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif

#ifndef RPC_H_
#  include "rpc.h"
#endif

static const char RCSid[] = "$Id: groupsrvr.cc,v 1.19 1993/04/27 14:45:50 ngdp Exp $";

extern char* clistfd;
extern int errno;

Boolean get_sr_pid (struct C_LIST_EL *clist, long df_serverport, struct MID *serverid, long *serverpid)
{
  /* Map server's multicast id into its process id - if server is logged
   * in clist.  Return code indicates whether server was found or not.
   */
  
  Boolean found = FALSE;
  int i;

#ifdef DEBUG
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "gmoperations: server port - given: " << df_serverport << "\n" << flush;
#endif  

  if (df_serverport > 0)
  {
    i = 0;
    while ((!found) && (i < MAXSERVERS))
    {
      if (df_serverport == ((clist->s_desc[i].myunicastid).lo_part))
      {
#ifdef DEBUG
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "gmoperations: server port - in clist: " << clist->s_desc[i].myunicastid.lo_part
	      << "\n" << flush;
#endif  
	found = TRUE;
	*serverpid = clist -> s_desc[i] . process_id;
	*serverid  = clist -> s_desc[i] . myunicastid;
#ifdef DEBUG
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "gmoperations: found in clist processid: " << *serverpid << "\n" << flush;
	  debug_stream << "about to send to " << flush;
	  printgid(*serverid);
#endif  
      }
      i++;
    }
  }
    
  return(found);

} /* end get_sr_pid() */
  
int server_signal (int  argc, char **argv)
{

  struct mc_sg_filter ack;
  struct mc_msghdr    req_buf, res_buf;
  struct MID          multi_addr, myself, serverid;
  struct RPC_SN       n_lastrpcsn, request_sn, result_sn;
  struct MID       h_client_addr, n_client_addr;
  struct C_LIST_EL    clist;
  Boolean             found, done;
  long                client_port;
  long                sropcode, rs_code, error, serverpid;
  int                 rretry, retry, xretry, xrstat, rstat;
  long                df_serverport;

#if (RAJDOOT_USE_SHARED_MEMORY == NO)
  int                 f_clistfd;
#endif
  
  errno = _rpcerr = 0;

  if (argc < 17)
      perror("Error: server_signal failed to find sufficient parameters.");

  /* Unpack Call and Byte Swap */
  sscanf(argv[2], "%ld", & n_client_addr . type);  /* Get client address */
  sscanf(argv[3], "%ld", & n_client_addr . hi_part);
  sscanf(argv[4], "%ld",  & client_port);
  n_client_addr . lo_part = (short) client_port; 
  h_client_addr . type = ntohl(n_client_addr . type);
  h_client_addr . hi_part = ntohl(n_client_addr . hi_part);
  h_client_addr . lo_part = ntohs(n_client_addr . lo_part);

  sscanf(argv[5], "%ld", & n_lastrpcsn.sec);       /* Get sequence number */
  sscanf(argv[6], "%ld", & n_lastrpcsn.msec);
  h_lastrpcsn . sec = ntohl(n_lastrpcsn . sec);    /* Byte Swap */
  h_lastrpcsn . msec = ntohl(n_lastrpcsn . msec);  
  
  sscanf(argv[7], "%ld", & multi_addr . type); /* Get multicast identifier */
  sscanf(argv[8], "%ld", & multi_addr . hi_part);
  sscanf(argv[9], "%ld", & multi_addr . lo_part);

  /* Ignore other fields - e.g. action id, uid & path - not needed here */
  
  sscanf(argv[16], "%ld", & sropcode);
  sscanf(argv[17], "%ld", & df_serverport);
     
  multicast_get_mid(&myself);  /* Get an identifier for receiving messages */
  
  /* Open clist */
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
  {  /* Could not read clist after several attempts */
#ifdef DEBUG
      cerr << "gmserver: open failed" << flush;
#endif
    error = SYSTEMERROR;
    senderror(&n_client_addr, error);
    return(FAILED);   /* group management operation failed */
  }
  
  /* Map server's port number into its process id */
  /* ids are always in network byte order - no need to byte swap */
  found  = get_sr_pid(&clist, df_serverport, &serverid, &serverpid);

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
  {  /* Could not write clist after several attempts */
#ifdef DEBUG
      cerr << "gmserver: close failed" << flush;
#endif
    error = SYSTEMERROR;
    senderror(&n_client_addr, error);
    return (FAILED);   /* group management operation failed */
  }
  
  if (!found)
  {
#ifdef DEBUG
      cerr << "gmserver: Server not found\n" << flush;
#endif
    senderror(&n_client_addr, SRVR_NOT_FOUND);
  }
  else
  {
    /* Signal deaf server to listen to gm operation request*/
    kill ((int) serverpid, INTERRUPT_SERVER);
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "server_signal: about to signal server " << serverpid << "\n" << flush;
#endif    
      
    /* Send call to server to perform required group management operation */
    getsn(&request_sn);
    request_sn.sec = htonl(request_sn.sec);    /* Byte Swap */
    request_sn.msec = htonl(request_sn.msec);
    
    /* Build Request */
    req_buf.mc_msg[0].iov_base = (char *) &request_sn;
    req_buf.mc_msg[0].iov_len  = sizeof(struct RPC_SN);
    req_buf.mc_msg[1].iov_base = (char *) &sropcode;
    req_buf.mc_msg[1].iov_len  = sizeof(sropcode);
    req_buf.mc_msg[2].iov_base = (char *) &multi_addr;
    req_buf.mc_msg[2].iov_len  = sizeof(struct MID);
    req_buf.mc_msg[3].iov_len  = 0;
    req_buf.mc_msg[4].iov_len  = 0;
    req_buf.mc_msg[5].iov_len  = 0;
    req_buf.mc_msg[6].iov_len  = 0;
    req_buf.msg_len = 7; 

    /* Prepare Result Buffer */
    rs_code = ZEROCODE; /* Initialise result opcode */
    res_buf.mc_msg[0].iov_len  = sizeof(rs_code);
    res_buf.mc_msg[1].iov_len  = sizeof(struct RPC_SN);
    res_buf.msg_len = 2;
    ack . next_fs = NULL;
    ack . mc_msg = &res_buf;
    
    /* Send call and await ack, use timeout and retries */
    rretry = 0;  /*SERVERRETRY; */
    xrstat = FAILED;

    done = FALSE;
    do
    {
      xretry = SERVERRETRY;
      xrstat = FAILED;
      do
      {
	xrstat = multicast_long_send(&serverid, &req_buf);
      } while ((xrstat != MC_OK) && (--xretry > 0));
      
      if (xrstat == MC_OK)
      {
#ifdef DEBUG
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "gmserver: send ok, waiting to recv\n" << flush;
#endif
	xrstat = multicast_long_receive(&myself, &ack, 0, SERVERTOUT);
#ifdef DEBUG
	  if (xrstat > 0)
	  {
	      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	      debug_stream << "gmserver: received from server ok\n" << flush;
	  }
	  else
	      cerr << "gmserver: failed to receive from server\n" << flush;
#endif
      }
#ifdef DEBUG
      else
	  cerr << "gmserver: send failed: " << errno << flush;
#endif
      
      if (xrstat > 0)
      {
	/* Cast buffers received */
	rs_code = * (long *)            res_buf.mc_msg[0].iov_base;
	result_sn = * (struct RPC_SN *) res_buf.mc_msg[1].iov_base;
      }

      done = (((xrstat >= 1) && (ntohl(rs_code) == TERM_OK) &&
	      (identical_sn(&request_sn, &result_sn)))? TRUE : FALSE);
      
    } while ((!done) && (--rretry > 0));
    
    if (!done)
      senderror(&n_client_addr, FAILED);
    else
      sendok(&n_client_addr, DONE);
  } /* end else */

  /* should not get here */

  return FAILED;
} /* end server_signal() */


int main (int argc, char **argv)
{
#ifdef DEBUG
  _arjuna_debug->set_debuglevel(FUNCTIONS);
  long          myid;
  char          stdoutfn[OBJECT_NAME_SIZE];
  char          stderrfn[OBJECT_NAME_SIZE];

  myid = getpid();
  sprintf(stdoutfn, "gmserverout_%ld", myid);
  sprintf(stderrfn, "gmservererr_%ld", myid);

  ofstream target1(stdoutfn);
  if (!target1)
      printf("groupmanserver: cannot open file gmserverout\n");
  else
      cout = target1;

  ofstream target2(stderrfn);
  if (!target2)
      printf("groupmanserver: cannot open file gmservererr\n");
  else
      cerr = target2;

  printf("groupmanserver: about to signal server\n");
#endif

  server_signal(argc, argv);
  /* server_signal does all the work; server no longer required */
     
#ifdef DEBUG
  printf("groupmanserver: completed work, about to exit\n");
#endif

  exit(0);
  
}
