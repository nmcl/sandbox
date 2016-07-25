/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: dupserver.cc,v 1.35 1993/10/22 13:21:09 nmcl Exp $
 */

/* VERSION with duplicate server logging initiate requests in clist */

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

#ifndef STRING_H_
#  include <System/string.h>
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

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#include <RPC/Rajdoot/rpcuser.h>
#include "rpc.h"

static const char RCSid[] = "$Id: dupserver.cc,v 1.35 1993/10/22 13:21:09 nmcl Exp $";

extern char* clistfd;
extern int errno;

static int clist_slot;
Boolean debug_mode = FALSE;

int check_path (struct C_LIST_EL* clist, struct PATHS path[NSTATION])
{
  int client_indx, freeslot, j, k, ptindx;
  Boolean found, non_empty;

  ptindx = 0;
  while((ptindx < NSTATION) && (path[ptindx].node != 0))
  {
    client_indx = 0;
    freeslot = -1;
    found = FALSE;
    while ((client_indx < NSTATION) && (!found))
    {
      found = ((clist->client[client_indx].node == path[ptindx].node)? TRUE : FALSE);
      /* Mark a free slot if found */
      if ((freeslot < 0) && (clist->client[client_indx].node == 0))
	freeslot = client_indx;
      if (!found)
	client_indx++;
    } /* end while */

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "\nDUP: CHECKPATH(): c_count: given: " << path[ptindx].c_count << ", in clist "
	<< clist->client[client_indx].c_count << "\n" << flush;
    if (found)
    {
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "\nDUP: CHECKPATH(): client node exists in clist\n" << flush;
	debug_stream << "\nDUP: given node: " << path[ptindx].node << " and client node:  "
	    << clist->client[client_indx].node << " and index " << client_indx << "\n" << flush;
	debug_stream << "\nDUP: CHECKPATH: servers (>= 0): and their processids \n" << flush;
      for (int srindx = 0; srindx < MAXSERVERS; srindx++)
      {
	if (clist -> s_desc[srindx] . process_id > 0)
	{
	    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	    debug_stream << ":" << clist->client[client_indx].s_indx[srindx];
	    debug_stream << ":" << clist -> s_desc[srindx] . process_id;
	}
      }
    }
#endif

    if (!found)
    {
      if (freeslot < 0)
      {
	/* Reclaim client slots that have not been used recently */
	j = 0;
	while ((j < NSTATION) && (freeslot < 0))
	{
	  k = 0;
	  non_empty = FALSE;
	  while ((k < MAXSERVERS) && (!non_empty))
	  {
	    non_empty = ((clist->client[j].s_indx[k] >= 0)? TRUE : FALSE);
	    k++;
	  }

	  if (!non_empty)  /* empty slot found */
	    freeslot = j;
	  else
	    j++;
	} /* end while */
      } /* end if */

      if (freeslot < 0)
	return(C_LIST_FULL);
      else
      {
	/* Insert client entry at free slot in clist */
	clist->client[freeslot].node = path[ptindx].node;
	clist->client[freeslot].c_count = path[ptindx].c_count;
      }
    } 
    else
    {
      /* Entry for given client node exists in clist */
      if (clist->client[client_indx].c_count >  path[ptindx].c_count)
	/* Reject initiate request since it is from a previous incarnation
	   of client */
	return(OLD_PATH);
      else
	if (clist->client[client_indx].c_count < path[ptindx].c_count)
	{
	  /* Note: if the housekeeper does not receive replies to its
	   * request for crash count, it assumes the client node is dead,
	   * sets its crash count to 0, and checks for orphans - in which
	   * case the duplicateserver does not need to check for orphans for
	   * that client, it simply sets the new crash count for the client.
	   */
	  
	  if (clist->client[client_indx].c_count > 0)
	    /* Information in clist is from old incarnation of client; remove
	       all such entries */
	    RmClientFromPath(clist, client_indx,
			     clist->client[client_indx].node);

	  /* Log in new client's crash count */
	  clist->client[client_indx].c_count = path[ptindx].c_count;
	}
    } /* end else */

    /* No need to adjust anything if the client node crash count is
       identical to that logged in the clist for that client */
    
    ptindx++;
    
  } /* end while */
  return(SUCCEEDED);

} /* end check_path() */

void set_srvrindx (struct C_LIST_EL* clist, struct PATHS* path, int server_indx)
{
  /* All client entries in path exist in clist.  Log in server_indx
   * fields for clients in rpcpath
   */

  struct PATHS *temp_path;
  Boolean found, existsinlist;
  int client_indx, freeslot, j;

  temp_path = path;  /*Initialise temporary path pointer */

  while (temp_path->node != 0)
  {
    client_indx = 0;
    found = FALSE;
    while ((client_indx < NSTATION) && (!found))
    {
      if (found = ((clist->client[client_indx].node == temp_path->node)?TRUE:FALSE))
      {
	/* Log that server_indx field if it is not already present */
	existsinlist = FALSE;
	freeslot = -1;
	j = 0;
	while ((!existsinlist) && (j < MAXSERVERS))
	{
	  existsinlist = ((clist->client[client_indx].s_indx[j] == server_indx)?TRUE:FALSE);
	  if ((freeslot < 0) && (clist->client[client_indx].s_indx[j] < 0))
	    freeslot = j;
	  j++;
	} /* end while */

	/* Free slot should exist for server entry */
	if (!existsinlist)  /* log server_indx in clist */
	  clist->client[client_indx].s_indx[freeslot] = server_indx;
      } /* end if */
      client_indx++;
    } /* end while */
    temp_path++;
  } /* end while */

#ifdef DEBUG
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "\nDUP: SETSRVRINDX: servers (>= 0): and their processids \n" << flush;
  for (client_indx = 0; client_indx < 5; client_indx++)
  {
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "for client " << client_indx << "\n" << flush;
      for (int srindx = 0; srindx < MAXSERVERS; srindx++)
      {
	if (clist -> s_desc[srindx] . process_id > 0)
	{
	    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	    debug_stream << ":" << clist->client[client_indx].s_indx[srindx];
	    debug_stream << ":" << clist -> s_desc[srindx] . process_id << "\n" << flush;
	}
      }
    }
#endif  
    
} /* end set_srvrindx() */

void log_newserver (struct C_LIST_EL *clist, struct MID actid, struct UNIQUEID uid, int server_indx)
{
  /* Log information regarding the new server in clist: Note rpcpath 
   * is logged separately.
   */

  long mypid;
  
  mypid = getpid();
  clist->s_desc[server_indx].process_id = mypid;
  clist->s_desc[server_indx].action_id = actid;
  clist->s_desc[server_indx].u_id = uid;
  clist->s_desc[server_indx].no_links = 1;

} /* end log_newserver() */

void log_path (struct C_LIST_EL *clist, struct PATHS path[NSTATION], int server_indx, int link_indx, struct RPC_SN timestamp, struct MID client_addr)
{
  int pt_indx;

  pt_indx = 0;
  while (path[pt_indx].node != 0)
  {
    clist->s_desc[server_indx].linkspath[link_indx].rpcpath[pt_indx] =
	      path[pt_indx];
    pt_indx++;
  }
  clist->s_desc[server_indx].linkspath[link_indx].timestamp = timestamp;
  clist->s_desc[server_indx].linkspath[link_indx].groupid = client_addr;

} /* end log_path() */

int inspect_clist (struct C_LIST_EL *clist, struct RPC_SN timestamp, struct MID actid, struct UNIQUEID uid,
		   long clientnode, struct MID *serverid, struct MID *sender, long *serverpid,
		   struct PATHS path[NSTATION], struct MID client_addr)
{
  /* Check in clist whether a new server is required, and log further
   * information about the server - if a new one is not required
   */
  
  Boolean  found, retry, inserted, unset;
  register int i, m, indx, rcode;
  int  freeslot;

#ifdef DEBUG
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "dup: inspecting clist\n" << flush;
#endif

  /* Check for orphan servers in clist */
  if ((rcode = check_path(clist, path)) != SUCCEEDED)
    return(rcode);
  
  /* If share criterion (uid and actionid) not set, need to get a new server */
  unset = unset_sharecrit(actid, uid);

#ifdef DEBUG
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  if (unset)
      debug_stream << "dup: share crit is unset\n" << flush;
  else
      debug_stream << "dup: share crit is set\n" << flush;
#endif
  
  freeslot = -1;
  i = 0;
  while ((i < MAXSERVERS) && (freeslot < 0))
  {
    if (clist -> s_desc[i] . process_id < 0)
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "dup: slotexists\n" << flush;
#endif
      freeslot = i;
      clist_slot = freeslot;
    }
    i++;
  }
 
  /* Check whether new server is required and whether call was a retry */
  i = 0;
  retry = FALSE;
  found = FALSE;
  while ((i < MAXSERVERS) && (((!unset) && (!found)) ||
	 ((unset) && (!retry))))
  {
    if (clist -> s_desc[i] . process_id > 0)
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "dup: pid for " << i << " is " << clist->s_desc[i].process_id << "\n" << flush;
	debug_stream << "dup comparing share_crits <inclist,supplied>\n" << flush;
#endif
      
      found = found_sharecrit(actid, uid, clist->s_desc[i].action_id,
			      clist->s_desc[i].u_id);

      if (found)
      {
	/* server with identical share criterion found - is this a retry? */
#ifdef DEBUG
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "dup: found share crit in clist\n" << flush;
#endif	
	retry = find_retry(clist, i, clientnode, timestamp);
      } /* end if */
      
    } /* end if */
    i++;
    
  } /* end  while */
  
  indx = --i;
  if (retry)
  {
    /* Set server id whose port is to be sent to client */
    /* Should be in Network order; ensure proper server logs in correctly */
    *serverid = (clist -> s_desc[indx]) . myunicastid;
    *sender = (clist -> s_desc[indx]) . sender;
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "dup: this was a retry\n" << flush;
    debug_stream << "dup: inspect_clist set serverid at indx " << indx << " to <"
	<< ntohl((clist -> s_desc[indx]) . myunicastid . type) << ","
	    << ntohl((clist -> s_desc[indx]) . myunicastid . hi_part) << ","
		<< ntohl((clist -> s_desc[indx]) . myunicastid . lo_part) << ">\n" << flush;
#endif
    return(ACK_CLIENT);
  }
  else
  {
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "dup: not a rety - unset: " << unset << " found: " << found << "\n" << flush;
#endif    
    if (unset || (! found))
    {
      if (freeslot < 0)
	return(C_LIST_FULL);
      else
      {
	/* Log new server in clist */
	log_newserver(clist, actid, uid, freeslot);

	/* Insert path in clist */
	log_path(clist, path, freeslot, 0, timestamp, client_addr);

	/* Set server index for all clients in server's rpcpath */
	set_srvrindx(clist, path, freeslot);
	/* Check whether 'freeslot' is correct server indx in this context */

	return(GETNEWSERVER);
      }

    }
    else
    {
      /* Update Path */
      m = 0; inserted = FALSE;
      while ((m < MAXCLIENTS) && (!inserted))
      {
	if (clist->s_desc[indx].linkspath[m].rpcpath[0].node == 0)
	{
	  /* Set server index for all clients in server's rpcpath */
	  set_srvrindx(clist, path, indx);
	  /* Insert path in clist */
	  log_path(clist, path, indx, m, timestamp, client_addr);

	  clist->s_desc[indx] . no_links++;  /* Update server's #links */
	  clist->s_desc[indx].poflag = FALSE;

	  /* Set result parameters */
	  *serverid  = (clist -> s_desc[indx]) . myunicastid;
	  *sender  = (clist -> s_desc[indx]) . sender;
	  *serverpid = clist -> s_desc[indx] . process_id;
	  inserted = TRUE;          
#ifdef DEBUG
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "dup: inspect_clist set serverid at indx " << i << " to <"
	      << ntohl((clist -> s_desc[indx]) . myunicastid . type) << ","
		  << ntohl((clist -> s_desc[indx]) . myunicastid . hi_part) << ","
		      << ntohl((clist -> s_desc[indx]) . myunicastid . lo_part) << ">\n" << flush;
	  debug_stream << "dup: this was not a retry\n" << flush;
	  debug_stream << "dup: inserted in indx: node " << indx << ", path " << m << "\n" << flush;
#endif
	}
	m++;
      } /* end while */
      
      if (!inserted)
	return(C_LIST_FULL);
      else
	return(PROD_SERVER);
      
    } /* end else */
  } /* end else (!retry) */
  
} /* end inspect_clist() */

int modify_clist (int f_clistfd, struct C_LIST_EL& clist)
{
#if (RAJDOOT_USE_SHARED_MEMORY == NO)
    if (lseek(f_clistfd, 0, L_SET) < 0)
    {
	_rpcerr = WRITEFAILED;
	return(FAILED);
    }
	
    if (write(f_clistfd, (char*) &clist, sizeof(clist)) != sizeof(clist))
    {
	_rpcerr = WRITEFAILED;
	return(FAILED);
    }
#else
    memcpy(clistfd, (char*) &clist, sizeof(clist));
#endif

    return(SUCCEEDED);
}

int duplicate_check (int  argc,char **argv)
{
  struct mc_sg_filter ack;
  struct mc_msghdr    req_buf, res_buf;
  struct MID          multi_addr, myself, serverid, actid, sender;
  struct MID          h_client_addr, n_client_addr, serversendid;
  struct RPC_SN       n_lastrpcsn, request_sn, result_sn;
  struct UNIQUEID     uid;
  struct C_LIST_EL    clist;
  struct PATHS        *ptr, path[NSTATION];
  Boolean             done;
  long                client_port, pathlength;
  long                sropcode, rs_code, error, serverpid, junk;
  int                 rcode, f_clistfd = -1;
  int                 rretry, retry, xretry, xrstat, rstat;
  register int        i;
  char                *full_name, *server_name;
  char                *argp[W_ARGS], argbuff[W_ARGS][ARGSTRING];

  errno = _rpcerr = 0;
  /* Unpack Call and Byte Swap */
  full_name = argv[1];
  sscanf(argv[2], "%ld", & n_client_addr . type);  /* Get client address */
  sscanf(argv[3], "%ld", & n_client_addr . hi_part);
  sscanf(argv[4], "%ld",  & client_port);
  n_client_addr . lo_part = client_port; 
  h_client_addr . type = ntohl(n_client_addr . type);
  h_client_addr . hi_part = ntohl(n_client_addr . hi_part);
  h_client_addr . lo_part = ntohl(n_client_addr . lo_part);

  sscanf(argv[5], "%ld", & n_lastrpcsn.sec);       /* Get sequence number */
  sscanf(argv[6], "%ld", & n_lastrpcsn.msec);
  h_lastrpcsn . sec = ntohl(n_lastrpcsn . sec);    /* Byte Swap */
  h_lastrpcsn . msec = ntohl(n_lastrpcsn . msec);  

  sscanf(argv[7], "%ld", & multi_addr . type); /* Get multicast identifier */
  sscanf(argv[8], "%ld", & multi_addr . hi_part);
  sscanf(argv[9], "%ld", & multi_addr . lo_part);

  sscanf(argv[10], "%ld", &actid . type );
  sscanf(argv[11], "%ld", &actid . hi_part);
  sscanf(argv[12], "%ld", &actid . lo_part);
  sscanf(argv[13], "%ld", &uid . host);
  sscanf(argv[14], "%ld", &uid . sec);
  sscanf(argv[15], "%ld", &uid . msec);

  /* Buffers 16 and 17 reserved for group man opn_code and deaf server */
  sscanf(argv[16], "%ld", & junk);
  sscanf(argv[17], "%ld", & junk);
  
  /* Get rpc-path length */
  sscanf(argv[18], "%ld", & pathlength);
  pathlength = ntohl(pathlength);
  i = 19;
  ptr = path;
  while(i < argc -1)
  {   /* Get rpc-path */
    sscanf(argv[i++], "%ld", & ptr->node);
    sscanf(argv[i++], "%ld", & ptr->c_count);
    ptr->node = ntohl(ptr->node);
    ptr->c_count = ntohl(ptr->c_count);
    ptr++;
  }
  ptr->node = 0; /* Terminate received rpcpath */

#ifndef __linux__
  // g++ on linux has *severe* problems with debug_streams!

#ifdef DEBUG
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "dup: actionid " << flush;
  printgid(actid);
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "dup: uid <" << uid.host << "," << uid.sec << "," << uid.msec << ">" << flush;
  debug_stream << "dup: pathlength " << pathlength << " about to log path " << flush;
    for (int j = 0; j < pathlength; j++)
    {
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "<" << path[j].node << "," << path[j].c_count << ">" << flush;
    }

  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "\n" << flush;
#endif

#endif

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
      cerr << "dup: open failed" << flush;
#endif
    error = SYSTEMERROR;
    senderror(&n_client_addr, error);
    return (FAILED);   /* Initialise failed */
  }
  
  /* Check if new server is required */
  rcode = inspect_clist(&clist, h_lastrpcsn, actid, uid,
                        h_client_addr.hi_part, &serverid, &sender,
                        &serverpid, path, multi_addr);

#if (RAJDOOT_USE_SHARED_MEMORY == NO)
  if (rcode == GETNEWSERVER)
  {  /* Make changes - but do not release lock on clist */

      if(lseek(f_clistfd, 0, L_SET) < 0)
      {
	  _rpcerr = WRITEFAILED;
	  rstat = FAILED;
      }
    
      if (write(f_clistfd, (char *) &clist, sizeof(clist)) != sizeof(clist))
      {
	  _rpcerr = WRITEFAILED;
	  rstat = FAILED;
      }
  }
  else
  {  /* Close clist */
      retry = FILESRETRY;  	rstat = FAILED;
      do
      {
	  rstat = f_clistclose(&f_clistfd, &clist);
      } while ((rstat != SUCCEEDED) && (retry-- > 0));
  }
#else
  if (rcode == GETNEWSERVER)
  {  /* Make changes - but do not release lock on clist */
    memcpy(clistfd, (char*) &clist, sizeof(clist));
  }
  else
  {  /* Close clist */
      retry = FILESRETRY;  	rstat = FAILED;
      do
      {
	  rstat = clistclose(&clist);
      } while ((rstat != SUCCEEDED) && (retry-- > 0));
  }
#endif
  
  if (rstat != SUCCEEDED)
  {  /* Could not write clist after several attempts */
#ifdef DEBUG
      cerr << "dup: close failed" << flush;
#endif
    error = SYSTEMERROR;
    senderror(&n_client_addr, error);
    return (FAILED);   /* Initialise failed */
  }
  
  switch(rcode)
  {
  case OLD_PATH:
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "dup: rcode was old_path\n" << flush;
#endif
                      break;

  case C_LIST_FULL:
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "dup: rcode was clistfull\n" << flush;
#endif
                      senderror(&n_client_addr, rcode);
                      break;
    case ACK_CLIENT:
                      /* Send server's address to client */
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "dup: rcode was ackclient to " << flush;
      printgid(n_client_addr);
#endif
                      sendmyport(&n_client_addr, serverid, sender);
                      break;
    case PROD_SERVER: 
		      /* Send server's address to client, server already
		       * exists - but prod server to join given id first,
		       * else client and server will be out of synch
		       */
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "dup: rcode was prodserver\n" << flush;
#endif

                      /* Send call to server to join MID supplied */
                      
                      getsn(&request_sn);
                      /* Byte Swap */
                      request_sn.sec = htonl(request_sn.sec);
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
                      res_buf.mc_msg[0].iov_len  = sizeof(struct RPC_SN);
                      res_buf.mc_msg[1].iov_len  = sizeof(rs_code);
                      res_buf.msg_len = 2;
                      ack . next_fs = NULL;
                      ack . mc_msg = &res_buf;
                      
                      /* Setting server's address, must be of type MID */
                      serversendid = serverid;
                      
                      /* Send call and await ack, use timeout and retries */
                      rretry = SERVERRETRY;
                      xrstat = FAILED;
                      sropcode = htonl(INITIALISE);  /* Operation Code */

#ifdef DEBUG
      if (!RAJDOOT_USE_BROADCAST)
      {
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "dup: sending to sender <" <<  ntohl(sender.type) << ","
		       << ntohl(sender.hi_part) << "," << ntohl(sender.lo_part) << ">\n" << flush;
      }
      else
      {
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "dup: sending to serverid <" <<  ntohl(serverid.type) << ","
		       << ntohl(serverid.hi_part) << "," << ntohl(serverid.lo_part) << ">\n" << flush;
      }
#endif		      
		      done = FALSE;
                      do
                      {
                        xretry = SERVERRETRY;
                        xrstat = FAILED;
                        do
                        {
			    if (!RAJDOOT_USE_BROADCAST)
				xrstat = multicast_long_send(&sender, &req_buf);
			    else
				xrstat = multicast_long_send(&serverid, &req_buf);
                        } while ((xrstat != MC_OK) && (--xretry > 0));
                        if (xrstat == MC_OK)
                        {
#ifdef DEBUG
			    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
			    debug_stream << "duplicate_check: send ok, waiting to recv\n" << flush;
#endif
                          xrstat = multicast_long_receive(&myself, &ack, 0, 2000);
#ifdef DEBUG
                          if (xrstat > 0)
			  {
			      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
			      debug_stream << "dup: received from server ok\n" << flush;
			  }
#endif
                        }
#ifdef DEBUG
                        else
			    cerr << "duplicate_check: send failed" << flush;
#endif

			if (xrstat > 0)
			{
			  /* Cast buffers received */
			  result_sn = * (struct RPC_SN *)
			    res_buf.mc_msg[0].iov_base;
			  rs_code = * (long *) res_buf.mc_msg[1].iov_base;
			}
			
			done = (((xrstat >= 1) && (ntohl(rs_code) == INIT_DONE)
				&& (identical_sn(&request_sn, &result_sn))) ? TRUE : FALSE);

			if (res_buf.mc_msg[0].iov_base)
#ifndef __GNUG__
			  delete [] res_buf.mc_msg[0].iov_base;
#else
	    		  delete res_buf.mc_msg[0].iov_base;
#endif
			if (res_buf.mc_msg[1].iov_base)
#ifndef __GNUG__
			  delete [] res_buf.mc_msg[1].iov_base;
#else
                          delete res_buf.mc_msg[1].iov_base;
#endif			

                      } while ((!done) && (--rretry > 0));
		      
                      /* No need to compare sender with expected server -
                         since duplicateserver would only contact one server */
                      /* Also no need to check sequence number, since operation
                         is idempotent */

		      /* old myaddress should not be needed */
		      /*multicast_get_mid(&myaddress);  */
                     if ((!done) && (serverpid > 0))
                     {
                      kill ((int) serverpid, INTERRUPT_SERVER);

                      /* Send call and await ack, use timeout and retries */
                      rretry = SERVERRETRY;
                      xrstat = FAILED;
                      sropcode = htonl(INT_INITIALISE);  /* Operation Code */

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
			    debug_stream << "duplicate_check: send ok, waiting to recv\n" << flush;
#endif
                          xrstat = FAILED;   
                          xrstat = multicast_long_receive(&myself, &ack, 0,
					   SERVERTOUT);
#ifdef DEBUG
                          if (xrstat > 0)
			  {
			      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
			      debug_stream << "dup: received from server ok\n" << flush;
			  }
#endif
                        }
#ifdef DEBUG
                        else
			    cerr << "duplicate_check: send failed" << flush;
#endif

			if (xrstat > 0)
			{
			  /* Cast buffers received */
			  result_sn = * (struct RPC_SN *)
			    res_buf.mc_msg[0].iov_base;
			  rs_code = * (long *) res_buf.mc_msg[1].iov_base;
			}

			done = (((xrstat >= 1) && (ntohl(rs_code) == INIT_DONE)
				&& (identical_sn(&request_sn, &result_sn)))?TRUE : FALSE);

			if (res_buf.mc_msg[0].iov_base)
#ifndef __GNUG__
			  delete [] res_buf.mc_msg[0].iov_base;
#else
			  delete res_buf.mc_msg[0].iov_base;
#endif			
			if (res_buf.mc_msg[1].iov_base)
#ifndef __GNUG__
			  delete [] res_buf.mc_msg[1].iov_base;
#else
			  delete res_buf.mc_msg[1].iov_base;
#endif			

                      } while ((!done) && (--rretry > 0));

                      if (!done)
                        senderror(&n_client_addr, FAILED);
                      else
			sendmyport(&n_client_addr, serverid, sender);
                      /* Do this at start of switch, when running with RPC */
                     }
                     else
		       sendmyport(&n_client_addr, serverid, sender);

		      break;
    case GETNEWSERVER:
                      /* Build argp list and exec server code */
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "dup: rcode was getnewserver\n" << flush;
#endif
                      for (i = 0; i < (W_ARGS - 1); i++)
                           argp[i] = &argbuff[i][0];

                      server_name = ((char*) strrchr(full_name, '/')+1);
#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "dup: about to exec server with name: " << server_name << "\n" << endl;
#endif

                      argp[0] = server_name;
                      sprintf(argp[1], "%ld", n_client_addr . type);
                      sprintf(argp[2], "%ld", n_client_addr . hi_part);
                      sprintf(argp[3], "%ld", client_port);
                      sprintf(argp[4], "%ld", n_lastrpcsn . sec);
                      sprintf(argp[5], "%ld", n_lastrpcsn . msec);
                      sprintf(argp[6], "%ld", multi_addr . type);
                      sprintf(argp[7], "%ld", multi_addr . hi_part);
                      sprintf(argp[8], "%ld", multi_addr . lo_part);
                      sprintf(argp[9], "%ld", f_clistfd);
                      if (debug_mode)
			  argp[10] = "-d";
                      else
			  argp[10] = 0;
		      argp[11] = (char *) NULL;

#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "dup: about to exec [" << full_name << "]\n" << flush;
#endif
                      if (execv(full_name, argp) == FAILED)
		      {
                        senderror(&n_client_addr, FAILED);
			reset_pathel(&(clist.client[clist_slot]));
			reset_server_info(&(clist.s_desc[clist_slot]));
			modify_clist(f_clistfd, clist);
#ifdef DEBUG
			cerr << "duplicateserver: failed to exec server - errno " << errno << "\n" << flush;
#endif
		      }
		      /* Later - syslog duplicateserver diagnostics */
                      break;
  } /* end switch */

  /* should never get here */
  return FAILED;
} /* end duplicate_check() */  

int main (int  argc, char **argv)
{
#ifdef DEBUG
  _arjuna_debug->set_debuglevel(FUNCTIONS);
#endif

  long myid;
  int res;
  char stdoutfn[OBJECT_NAME_SIZE], stderrfn[OBJECT_NAME_SIZE];
  char tempOfn[OBJECT_NAME_SIZE], tempEfn[OBJECT_NAME_SIZE];
  char* objName = ::strrchr(argv[1], '/');
  char *fileName = 0;

  if (objName == 0)
      objName = argv[1];
  else
      objName++;

  fileName = ::new char[::strlen(objName)+3];
  ::memset(fileName, '\0', ::strlen(objName)+3);
  ::strcpy(fileName, objName);
  ::strcpy(fileName+::strlen(objName), "%s");

  myid = getpid();
  sprintf(tempOfn, "_out_%ld", myid);
  sprintf(tempEfn, "_err_%ld", myid);
  sprintf(stdoutfn, fileName, tempOfn);
  sprintf(stderrfn, fileName, tempEfn);

#ifndef __GNUG__
  ::delete [] fileName;
#else
  ::delete fileName;
#endif

  if (strcmp(argv[argc-1], "-d") == 0)
  {
      ofstream target1(stdoutfn);
      ofstream target2(stderrfn);

      debug_mode = TRUE;

      if (!target1)
	  printf("duplicateserver: cannot open file %s\n", stdoutfn);
      else
      {	
	  res = dup2(target1.rdbuf()->fd(), 1);
	  if (res == -1)
	      printf("duplicateserver: cannot assign output file descriptor\n");
      }


      if (!target2)
	  printf("duplicateserver: cannot open file %s\n", stderrfn);
      else
      {
	  res = dup2(target2.rdbuf()->fd(), 2);
	  if (res == -1)
	      printf("duplicateserver: cannot assign error file descriptor\n");
      }
  }

#ifdef DEBUG
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "duplicateserver: about to check for duplicates\n" << endl;
#endif

  duplicate_check(argc, argv);
  /* Duplicate_check does all the work; server no longer required */
     
#ifdef DEBUG
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "duplicateserver: completed work, about to exit\n" << endl;
#endif

  _exit(0);
}
