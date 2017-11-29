/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: manager.cc,v 1.39.6.1 1996/10/10 12:26:14 ngdp Exp $
 */

/*
 * The following code uses the manager functions 'init_manager', 'listen' 
 * and 'create_server' to construct a general purpose manager.
 * 
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#   include <Common/Debug.h>
#endif

#ifndef PROTOS_H_
#   include <System/protos.h>
#endif

#ifndef STDLIB_H_
#   include <System/stdlib.h>
#endif

#ifndef STRING_H_
#   include <System/string.h>
#endif

#ifndef UNISTD_H_
#   include <System/unistd.h>
#endif

#ifndef MALLOC_H_
#   include <System/malloc.h>
#endif

#ifndef MEMORY_H_
#   include <System/memory.h>
#endif

#ifndef SIGNAL_H_
#   include <System/signal.h>
#endif

#ifndef SYS_WAIT_H_
#   include <System/sys/wait.h>
#endif

#ifndef SYS_PARAM_H_
#   include <System/sys/param.h>
#endif

#ifndef FSTREAM_H_
#   include <System/fstream.h>
#endif

#ifndef NETDB_H_
#   include <System/netdb.h>
#endif

#ifndef NETINET_IN_H_
#   include <System/netinet/in.h>
#endif

#ifndef STDIO_H_
#   include <System/stdio.h>
#endif

#ifndef ERRNO_H_
#   include <System/errno.h>
#endif

#ifndef TIME_H_
#   include <System/time.h>
#endif

#ifndef SYS_TIME_H_
#   include <System/sys/time.h>
#endif

#ifndef FCNTL_H_
#   include <System/fcntl.h>
#endif

#ifndef SYS_FILE_H_
#   include <System/sys/file.h>
#endif

#ifndef SYS_TYPES_H_
#   include <System/sys/types.h>
#endif

#ifndef SYS_STAT_H_
#   include <System/sys/stat.h>
#endif

#ifndef IPC_PRIVATE
#   ifndef SYS_IPC_H_
#       include <System/sys/ipc.h>
#   endif
#endif

#ifndef SYS_SHM_H_
#   include <System/sys/shm.h>
#endif

#include <Common/Error.h>

#include <RPC/Rajdoot/rpcuser.h>
#include "rpc.h"

extern void checkEnvironment();

extern Int32 managerAddr;
extern int managerShm;
extern int managerSem;
extern int errno;
extern char* clistfd;

static const char RCSid[] = "$Id: manager.cc,v 1.39.6.1 1996/10/10 12:26:14 ngdp Exp $";

static struct MID manager;  
static directory_path search_path[PATH_SIZE];
Boolean debug_mode = FALSE, singleUserMode = FALSE, verboseMode = FALSE;
Boolean crashRecoveryMessage = FALSE, doReply = FALSE;
struct mc_msghdr l_work;
struct MID client_addr;
struct C_LIST_EL clist;


void respondToClient (struct MID& clientAddr, Int32 opn_code)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "void respondToClient (struct MID& client_addr, Int32 opn_code)" << endl;
    debug_stream.unlock();
    
#endif

    if ((opn_code == GET_WORKER) || (opn_code == GM_SERVER) || (opn_code == MANAGER_DIE)
	|| (opn_code == SINGLEUSER) || (opn_code == MULTIUSER))
    {
	struct mc_msghdr reply;
	struct RPC_SN seqno;
	struct MID _mid;
	int rcode;
	Int32 dummyCode = opn_code == GM_SERVER;
	
#ifdef DEBUG
	debug_stream.lock();
	
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "Manager received opcode " << dummyCode << endl;
	debug_stream.unlock();
	
#endif
	
	getnullmid(&_mid);
	getsn(&seqno);
	reply.mc_msg[0].iov_base = (char*) &seqno;
	reply.mc_msg[0].iov_len = sizeof(seqno);
	reply.mc_msg[1].iov_base = (char*) &dummyCode;
	reply.mc_msg[1].iov_len = sizeof(dummyCode);
	reply.mc_msg[2].iov_base = (char*) &_mid;
	reply.mc_msg[2].iov_len = sizeof(_mid);
	reply.mc_msg[3].iov_base = (char*) &_mid;
	reply.mc_msg[3].iov_len = sizeof(_mid);
	reply.msg_len = 4;

	rcode = multicast_long_send(&clientAddr, &reply);
    }
}

void setSingleUser (Boolean value, int opcode)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "void setSingleUser ( " << ((value == TRUE) ? "TRUE" : "FALSE") << " )" << endl;
    debug_stream.unlock();
    
#endif
    
    singleUserMode = value;

    if (verboseMode)
	cout << "Manager in " << ((value == TRUE) ? "single-user" : "multi-user") << " mode" << endl;
    
    respondToClient(client_addr, opcode);
}

void terminateAllServers (struct C_LIST_EL* clist)
{
    for (int i = 0; i < MAXSERVERS; i++)
    {
	if (clist->s_desc[i].no_links > 0)
	{
#ifdef DEBUG
	    debug_stream.lock();
	    
	    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	    debug_stream << "manager: terminateAllServers - sending signal to " << clist->s_desc[i].process_id << endl;
	    debug_stream.unlock();
	    
#endif
	    if (verboseMode)
		cout << "Manager killing process " << clist->s_desc[i].process_id << endl;
	    
	    (void) kill((int) clist->s_desc[i].process_id, TERMINATE_SERVER);
	}
    }
}
	
void killManager ()
{
#ifdef hpux
    pid_t pid;
#else
    int pid;
#endif

    if (verboseMode)
	cout << "Manager terminating." << endl;
    
    if (doReply)
	respondToClient(client_addr, MANAGER_DIE);
    
    pid = getpid();
    (void) kill(pid, SIGKILL);
}

#ifdef HAVE_INTSIG
int terminateManager (int dummy DOTDOTDOT)
#else
void terminateManager (int dummy DOTDOTDOT)
#endif
{
    int rstat, retry = FILESRETRY;
    struct C_LIST_EL* dummyclist = NULL;

    do
    {
	/*
	 * Prevent clistopen from doing an attach.
	 */
	
	rstat = clistopen(dummyclist, FALSE);
    } while ((rstat != SUCCEEDED) && (retry-- > 0));

    if (rstat != SUCCEEDED)
    {
	(void) clistclose();
	killManager();
    }
    else
    {
	::memcpy((char*) &clist, clistfd, sizeof(clist));
	terminateAllServers(&clist);
	delete_clist();
	killManager();
    }
}

#ifdef HAVE_INTSIG
int managerMultiOn (int dummy DOTDOTDOT)
#else
void managerMultiOn (int dummy DOTDOTDOT)
#endif
{
    singleUserMode = FALSE;

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "Manager in multi-user mode" << endl;
    debug_stream.unlock();
    
#endif
}

#ifdef HAVE_INTSIG
int managerMultiOff (int dummy DOTDOTDOT)
#else
void managerMultiOff (int dummy DOTDOTDOT)
#endif
{
    singleUserMode = TRUE;

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "Manager in single user mode" << endl;
    debug_stream.unlock();
    
#endif    
}

int init_manager ()
{
  struct host_address host_addr;
  char                localhost[MAXHOSTNAMELEN];

  unsigned short myport = (unsigned short) managerAddr;

  gethostname(localhost, MAXHOSTNAMELEN);
  host_addr . real_host = localhost;
  getloghost(&host_addr);

  if (((client_ipc = initialise_inet_ipc(&network_addr, myport)) < 0) ||
      (bind(client_ipc,(struct sockaddr*)&network_addr, sizeof(struct sockaddr_in)) == -1) ||
      (fcntl(client_ipc, F_SETFD, 1)))
    return(FAILED);

  manager . type = htonl(UID_type);
  manager . hi_part = host_addr . log_host;
  manager . lo_part = htonl(myport);

  return(SUCCEEDED);
  
} /* end init_manager() */

int mkcfiles (Int32 *crashcount)
{
/*
 * Create and initialise C_LIST and C_COUNT files.
 * To be invoked at start-up time.
 * Return OK if successful, otherwise FAILED.
 */
    int fd, rcode = FAILED, sm_id;
    Int32 currentTimeValue;

    errno = 0;

    /* Create C_LIST and open it for writing */

    sm_id = shmget(managerShm, sizeof(clist), 0666 | IPC_CREAT | IPC_EXCL);

    if (errno == EEXIST)
    {
	sm_id = shmget(managerShm, sizeof(clist), 0666);
    }

    if (sm_id >= 0)
    {
	if ((clistfd = (char*) shmat(sm_id, 0, 0)) != (char*) -1)
	{
	    memset(clistfd, '\0', sizeof(clist));
		
	    reset_allclients(&clist);
	    reset_allservers(&clist);
      
	    memcpy(clistfd, (char*) &clist, sizeof(clist));
	    if((fd = open(C_COUNT_Q, O_CREAT | O_RDWR, 0666)) > 0)
	    {
		currentTimeValue = (Int32) time(0);
		
		if ((write(fd, (char *) &currentTimeValue, sizeof(currentTimeValue)) == sizeof(currentTimeValue))
		    && (!chmod(C_COUNT_Q, 0666)) && (!close(fd)))
		    { 
			rcode = SUCCEEDED;
			*crashcount = currentTimeValue;
		    }   
	    }
	}
	else
	{
	    printf("Failed to attach to shared memory. Error code is %d\n",errno);
	    exit(1);
	}
    }
    else
    {
	printf("Failed to get shared memory. Error code is %d\n",errno);
	if (errno == EINVAL)
	    printf("Possible problem with specified size of shared memory: %d bytes.\n", sizeof(clist));
	exit(1);
    }  /* End creat C_LIST */

    return(rcode);
}  /* End mkcfiles */


void SendCcount (struct MID *h_keeper, Int32 c_count, struct RPC_SN *seqno)
{
  /* Transmit the node's crash count to enquiring housekeeper */

  struct mc_msghdr req_buf;
  int xrstat, xretry;

  
  /* Prepare buffers for transmission */
  req_buf . mc_msg[0] . iov_base = (char *) seqno;
  req_buf . mc_msg[0] . iov_len = sizeof(struct RPC_SN);
  req_buf . mc_msg[1] . iov_base = (char *) &c_count;
  req_buf . mc_msg[1] . iov_len = sizeof(c_count);
  req_buf . msg_len = 2;

  /* Transmit buffer containing crash count */
  xretry = HK_REPLYRETRY;
  do
  {
    xrstat = multicast_long_send(h_keeper, &req_buf);
  } while ((xrstat != MC_OK) && (--xretry > 0));

} /* end SendCcount() */


int listen (struct MID *client_address, char **object_name, struct RPC *u_work,
	    struct RPC_SN *call_seqno, struct MID *multi_addr, struct MID *actid,
	    struct UNIQUEID *uid, Int32 *opn_code, Int32 *df_serverport)
{
/*
 * Listen for requests from the network addressed to manager 
 * Calls to LISTEN must be preceded by a call to Init_Manager
 */
 
  struct mc_sg_filter call;
  struct RPC_SN       seqno;
  struct MID          h_keeper;
  Boolean             cont;
  static Int32         crash_count = -1;
  Int32                opcode;
  char                *obj_name;
    
  if (crash_count < 0) /* Obtain Crashcount for housekeeper enquiries */
     if (mkcfiles(&crash_count) == FAILED)
     {
#ifdef DEBUG
	 debug_stream.lock();
	 
	 debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	 debug_stream << "Cannot create c_count file.\n" << flush;
	 debug_stream.unlock();
	 
#endif
        return(MANAGER_FAILED);
     }

  do
  {
    /* Set up Buffers for Receiving RPC */
    l_work . mc_msg[0] . iov_len = sizeof(struct RPC_SN);
    l_work.mc_msg[0].iov_base = 0;
    if (!u_work -> rpc_code)
      opcode = ZEROCODE;  /* Initialise operation code */
    l_work . mc_msg[1] . iov_len = sizeof(opcode);
    l_work.mc_msg[1].iov_base = 0;
    l_work . mc_msg[2] . iov_len = sizeof(struct MID);
    l_work.mc_msg[2].iov_base = 0;
    l_work . mc_msg[3] . iov_len = sizeof(struct MID);
    l_work.mc_msg[3].iov_base = 0;
    l_work . mc_msg[4] . iov_len = sizeof(struct UNIQUEID);
    l_work.mc_msg[4].iov_base = 0;
    l_work . mc_msg[5] . iov_len = sizeof(*opn_code);
    l_work.mc_msg[5].iov_base = 0;
    l_work . mc_msg[6] . iov_len = sizeof(*df_serverport);
    l_work.mc_msg[6].iov_base = 0;
    l_work . mc_msg[7] . iov_len = OBJECT_NAME_SIZE;
    l_work.mc_msg[7].iov_base = 0;
    l_work . mc_msg[8] . iov_len = 0;
    l_work.mc_msg[8].iov_base = 0;

    l_work . msg_len = 9;
    call . next_fs = NULL;
    call . mc_msg = &l_work;

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "\n\nmanager: about to receive\n" << flush;
    debug_stream.unlock();
    
#endif
    cont = TRUE;
    while (cont)
    {
      if (multicast_long_receive(&manager, &call, 0, -1) >= 1)
	cont = FALSE;

#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "\n\nmanager: received something\n" << flush;
      debug_stream.unlock();
      
#endif
    }

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "\n\nmanager: received okay\n" << flush;
    debug_stream.unlock();
    
#endif

    /* Cast buffers received */
    seqno = * (struct RPC_SN *)    l_work.mc_msg[0].iov_base;
    opcode = * (Int32 *)            l_work.mc_msg[1].iov_base;
    opcode = ntohl(opcode);
    if (opcode == GETMECCOUNT)
    {
      h_keeper = call.source;
      SendCcount(&h_keeper, crash_count, &seqno);
    }
    else
    {
      *multi_addr = * (struct MID *) l_work.mc_msg[2].iov_base;
      *actid = * (struct MID *)      l_work.mc_msg[3].iov_base;
      *uid = * (struct UNIQUEID *)   l_work.mc_msg[4].iov_base;
      *opn_code = * (Int32 *)         l_work.mc_msg[5].iov_base;
      *df_serverport = * (Int32 *)    l_work.mc_msg[6].iov_base;
      obj_name =                     (char*) l_work.mc_msg[7].iov_base;
      u_work->rpc_length1 =          l_work.mc_msg[8].iov_len;
      if (u_work->rpc_length1 > 0)
	u_work->rpc_buff1 =          (char*) l_work.mc_msg[8].iov_base; 
      
      /* Byte Swap  and Assign Output Parameters */
      /* Sequence Number */
      call_seqno -> sec = seqno.sec;
      call_seqno -> msec = seqno . msec;
      /* t_out and retry only sent for RPC, which are not sent to manager */
      
      /* Byte swap operation code: rpc_code: gives operation code for manager -
       * opn_code gives code for group management operation to be performed by
       * server
       */

      u_work->rpc_code = opcode;
      *opn_code = ntohl(*opn_code);
      /* df_serverport: like all ids should be in network byte order */
      
      *object_name = obj_name;   /* Object Name */
      *client_address = call . source;   /* Client address */

      if ((actid->type == PRIORITYACTID) && (actid->hi_part == PRIORITYACTID) &&
	  (actid->lo_part == PRIORITYACTID))
      {
	  actid->type = actid->hi_part = actid->lo_part = 0;
	  crashRecoveryMessage = TRUE;
      }
      else
	  crashRecoveryMessage = FALSE;
      
#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "\ndf_serverport: " << ntohl(*df_serverport) << "\n" << flush;
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "manager: actid " << flush;
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      printgid(*actid);
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "and uid: " << flush;
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      printgid(*(struct MID *)uid);
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "objectname: " << obj_name << "\n" << flush;
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "Manager: listen(): multiaddr: " << flush;
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      printgid(*multi_addr);
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "client address received " << flush;
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      printgid(*client_address);
      debug_stream.unlock();
      
#endif

      return(SUCCEEDED);
    } /* end else */

    if (l_work.mc_msg[0].iov_base)
    {
      free(l_work.mc_msg[0].iov_base);
      l_work.mc_msg[0].iov_base = 0;
    }
    if (l_work.mc_msg[7].iov_base)
    {
#ifndef GCC_ARRAY_DELETE_BUG
      delete [] l_work.mc_msg[7].iov_base;
#else
      delete l_work.mc_msg[7].iov_base;
#endif      
      l_work.mc_msg[7].iov_base = 0;
    }
    if (l_work.mc_msg[8].iov_base)
    {
#ifndef GCC_ARRAY_DELETE_BUG
      delete [] l_work.mc_msg[8].iov_base;
#else
      delete l_work.mc_msg[8].iov_base;
#endif      
      l_work.mc_msg[8].iov_base = 0;
    }

  } while (opcode == GETMECCOUNT);

  return (SUCCEEDED);
} /* end Listen() */


int create_server (char *object_name, struct MID *client_address, struct RPC *pwork,
		   struct RPC_SN *call_seqno, struct MID *multi_addr, struct MID *actid,
		   struct UNIQUEID *uid, Int32 opn_code, Int32 df_serverport)
{
#ifdef HAVE_UNION_WAIT
  union wait status;
#else
  int status;
#endif
  pid_t pid, child_id;
  Int32  pathlength, buff_size;
  struct PATHS temp_path;
  register int i;
  char argbuff[W_ARGS][ARGSTRING];
  char *argp[W_ARGS], *buff, *nextbuff;
  char *checking_server = DUPSERVER_Q;
  char *groupman_server = GMSERVER_Q;

  errno = 0;

  if (!(child_id = fork()) )
  {
    if (!(child_id = fork()) )
    {
#ifdef DEBUG
	debug_stream.lock();
	
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "Manager: creatw(): multiaddr: <" << ntohl(multi_addr -> type) << ","
		     << ntohl(multi_addr -> hi_part) << "," << ntohl(multi_addr -> lo_part) << ">\n" << flush;
	debug_stream.unlock();
	
#endif

      /* Initialise pointers to argument list */
      for (i = 0; i < (W_ARGS - 1); i++)
          argp[i] = &argbuff[i][0];

      argp[1] = object_name;
      sprintf(argp[2], "%d", client_address -> type);
      sprintf(argp[3], "%d", client_address -> hi_part);       
      sprintf(argp[4], "%d", client_address->lo_part);
      sprintf(argp[5], "%d", call_seqno -> sec);  
      sprintf(argp[6], "%d", call_seqno -> msec);
      sprintf(argp[7], "%d", multi_addr -> type);
      sprintf(argp[8], "%d", multi_addr -> hi_part);      
      sprintf(argp[9], "%d", multi_addr -> lo_part);            
      sprintf(argp[10], "%d", actid -> type);
      sprintf(argp[11], "%d", actid -> hi_part);      
      sprintf(argp[12], "%d", actid -> lo_part);
      sprintf(argp[13], "%d", uid -> host);
      sprintf(argp[14], "%d", uid -> sec);
      sprintf(argp[15], "%d", uid -> msec);
      sprintf(argp[16], "%d", opn_code);            
      sprintf(argp[17], "%d", df_serverport);            

      i = 19;
      pathlength = 0;
      buff = nextbuff = pwork->rpc_buff1;  /* Initialise buffer pointers */
      buff_size = pwork->rpc_length1;

#ifdef DEBUG
      debug_stream.lock();
	
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "rpcmanager: buff_size: " << buff_size << " and unpacked path: \n" << flush;
      debug_stream.unlock();
#endif

      while (buff_size > 0)
      {
	/* Unpack rpc-path */
	nextbuff = unpack_Int32(&(temp_path.node), buff);
	buff = nextbuff;
	nextbuff = unpack_Int32(&(temp_path.c_count), buff);
	buff = nextbuff;

#ifdef DEBUG
	debug_stream.lock();
	
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << " <" << temp_path.node << "," << temp_path.c_count << ">" << flush;
	debug_stream.unlock();
#endif

	sprintf(argp[i++], "%d", temp_path.node);
	sprintf(argp[i++], "%d", temp_path.c_count);
	buff_size -= sizeof(struct PATHS);
	pathlength++;
      }
      sprintf(argp[18], "%d", htonl(pathlength));  
      if (debug_mode)
	  argp[i] = "-d"; // debugging
      else
	  argp[i] = "+d"; // no debugging
      argp[i+1] = (char *) NULL;

#ifdef DEBUG
      debug_stream.lock();
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "\nrpcmanager: pathlength: " << pathlength << "\n" << flush;
      debug_stream << "manager: opcode was: " << flush;
      if (pwork -> rpc_code == GET_WORKER)
	  debug_stream << "GET_WORKER" << endl;
      else
	  debug_stream << "GM_SERVER: " << pwork->rpc_code << endl;
      debug_stream.unlock();
#endif

      if (pwork -> rpc_code == GET_WORKER)
      {
	argp[0] = checking_server;
#ifdef DEBUG
	debug_stream.lock();
	
	debug_stream << "manager: exec args are :";

	for (i=0; argp[i] != 0; i++)
	   debug_stream << argp[i] << " ";

	debug_stream << endl; 	
	debug_stream.unlock();
#endif

	if (execv(checking_server, argp) == FAILED)
	{
	    error_stream << FATAL << "manager: failed to exec server " << checking_server
		 << " : " << errno << "\n" << flush;	    
	    /* Later - syslog manager diagnostics */
	    exit(1);
	}
      }
      else   /* pwork->rpc_code == GM_SERVER */
      {
	argp[0] = groupman_server;
	if (execv(groupman_server, argp) == FAILED)
	{
	    error_stream << FATAL << "manager: failed to exec server " << groupman_server
		 << " : " << errno << "\n" << flush;
	    /* Later - syslog manager diagnostics */
	    exit(1);
	}
      }
    }     
    exit(child_id < 0 ? errno : 0);
  }    

  if (errno != 0)
  {
      error_stream << WARNING << "Manager create_server fork error. Errno " << errno << endl;
      return FAILED;
  }
  
  if ((pid = wait(&status)) < 0)
  {
      error_stream << WARNING << "Manager create_server wait error. Errno " << errno << endl;
      return FAILED;
  }

  if (verboseMode)
      cout << "Server creation succeeded." << endl;
  
  return(SUCCEEDED);

} /* create_server */

void check_files (char *name, struct stat *buf)
{
    if (stat(name, buf) == -1)
    {
	printf("manager failed to find %s\n", name);
	exit(1);
    }
    else
	if ((buf->st_mode & 0111) != 0111)
	{
	    printf("Executable %s does not have execute permission\n", name);
	    exit(1);
	}
}

int main (int argc, char **argv)
{
    struct RPC      work;
    struct MID      multi_addr, actid;
    struct UNIQUEID uid;
    struct RPC_SN   seqno;
    Boolean         invalid_location;
    Int32            opn_code;
    int             rcode, j, path_number = 0;
    Int32            df_serverport;
    char            *obj_name, *tmp_name = 0, *location = SERVER_LOCATION_Q;
    struct stat     buf;
    Boolean         valid_path, path_list = FALSE;
    char            *vMode = 0, *smMode = 0, *dMode = 0, *pList = 0;
    char            outputFile[OBJECT_NAME_SIZE], errorFile[OBJECT_NAME_SIZE];
#ifndef hpux
    int managerPID = getpid();
#else    
    pid_t managerPID = getpid();
#endif

    checkEnvironment();

    if (chdir("/tmp"))
	perror("manager failed to chdir to /tmp\n");

    if (init_manager() == FAILED)
    {
	cerr << "init_manager failed" << endl;
	exit(1);
    }

    if (stat(MANPID_FILE_Q, &buf) != -1)
	unlink(MANPID_FILE_Q);

    ofstream target(MANPID_FILE_Q);
    if (!target)
	perror("manager failed to open pid file\n");
    else
    {
	target << managerPID;
	target.close();
    }
    
    if (stat(C_LIST_Q, &buf) != -1)
	unlink(C_LIST_Q);

    if (stat(C_COUNT_Q, &buf) != -1)
	unlink(C_COUNT_Q);

    if (stat(INSTALL_TOPDIR_DEFAULT_Q, &buf) == -1)
    {
	printf("manager failed to locate %s directory\n", INSTALL_TOPDIR_DEFAULT_Q);
	exit(1);
    }
    else
	if ((buf.st_mode & 0555) != 0555)
	{
	    printf("Directory %s is not readable and executable by everyone\n", INSTALL_TOPDIR_DEFAULT_Q);
	    exit(1);
	}

    check_files(DUPSERVER_Q, &buf);
    check_files(GMSERVER_Q, &buf);

    /*
     * Get manager options from environment first. Command line
     * overrides this.
     */

    vMode = ::getenv("MANAGER_VERBOSE");
    if (vMode)
	verboseMode = (Boolean) (::strcmp(vMode, "ON") == 0);
    
    smMode = ::getenv("MANAGER_MODE");
    if (smMode)
	singleUserMode = (Boolean) (::strcmp(smMode, "SINGLE") == 0);
    
    dMode = ::getenv("MANAGER_DEBUG");
    if (dMode)
	debug_mode = (Boolean) (::strcmp(dMode, "ON") == 0);
    
    pList = ::getenv("MANAGER_PATH");
    if (pList)
    {
	Boolean finishedScan = FALSE, firstGo = TRUE;
	
	while (!finishedScan)
	{
	    if (path_number >= PATH_SIZE)
	    {
		printf("manager: Too many search paths. Maximum is %d\n",PATH_SIZE);
		exit(1);
	    }

	    if (firstGo)
	    {
		location = ::strtok(pList, " ");
		firstGo = FALSE;
	    }
	    else
		location = ::strtok(0, " ");
	    
	    if (location == 0)
		finishedScan = TRUE;
	    else
	    {
		search_path[path_number].dir_name = location;
		if (stat(location, &buf) == -1)
		{
		    printf("Manager failed to find %s\n", location);
		    exit(1);
		}

		if ((buf.st_mode & 0111) != 0111)
		{
		    printf("Directory %s does not have correct permissions\n", location);
		    exit(1);
		}

		path_number++;
		path_list = TRUE;
	    }
	}
    }

    j = 1;
    if (argc > 1)
    {
	while (j < argc)
	{
	    if ((argv[j][0] == '-'))
	    {
		switch (argv[j][1])
		{
		case 'v':
		    verboseMode = TRUE;
		    break;
		case 'm':
		    singleUserMode = FALSE;
		    break;
		case 's':
		    singleUserMode = TRUE;
		    break;
		case 'd':
		    debug_mode = TRUE;
		    break;
		case 'p':
		    if (path_number >= PATH_SIZE)
		    {
			printf("manager: Too many search paths. Maximum is %d\n",PATH_SIZE);
			exit(1);
		    }
		    location = (char*) argv[j+1];
		    search_path[path_number].dir_name = location;
		    if (stat(location, &buf) == -1)
		    {
			printf("Manager failed to find %s\n", location);
			exit(1);
		    }

		    if ((buf.st_mode & 0111) != 0111)
		    {
			printf("Directory %s does not have correct permissions\n", location);
			exit(1);
		    }

		    path_number++;
		    j++;
		    path_list = TRUE;
		    break;
		case 'h':
		    printf("Usage: manager [-h] [-v] [-s] [-p] [-d]\n");
		    return 0;
		    break;
		default:
		    printf("manager: argument not recognised '%s'\n", argv[j]);
		    exit(1);
		}
	    }
	    else
	    {
		printf("manager: argument not recognised '%s'\n", argv[j]);
		exit(1);
	    }
	    j++;
	}
	path_number -= 1;
    }

    if (!path_list)
    {
	if (stat(SERVER_LOCATION_Q, &buf) == -1)
	{
	    printf("manager failed to locate %s directory\n", SERVER_LOCATION_Q);
	    exit(1);
	}
	else
	    if ((buf.st_mode & 0111) != 0111)
	    {
		printf("Directory %s does not have correct permissions\n", SERVER_LOCATION_Q);
		exit(1);
	    }
    }

    sprintf(outputFile, "manager_out_%d", managerPID);
    sprintf(errorFile, "manager_err_%d", managerPID);
    ofstream target1(outputFile);
    if (!target1)
	printf("manager: Cannot open file %s\n", outputFile);
    else
    {
	if (debug_mode)
	    cout = target1;
	else
	{
	    target1.close();
	    ::unlink(outputFile);
	}
    }
	
    ofstream target2(errorFile);
    if (!target2)
	printf("manager: Cannot open file %s\n", errorFile);
    else
    {
	if (debug_mode)
	    cerr = target2;
	else
	{
	    target2.close();
	    ::unlink(errorFile);
	}
    }

    if (verboseMode)
    {
	if (singleUserMode)
	    cout << "Manager in single user mode" << endl;
	else
	    cout << "Manager in multi-user mode" << endl;
    }

    ::signal(SIGINT, terminateManager);
    ::signal(SIGUSR1, managerMultiOn);
    ::signal(SIGUSR2, managerMultiOff);

    for(;;)
    {
      work . rpc_code = ZEROCODE;

#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;

      if (singleUserMode)
	  debug_stream << "Manager in single user mode" << endl;
      else
	  debug_stream << "Manager in multi-user mode" << endl;
      debug_stream.unlock();
      
#endif

      if ((rcode = listen(&client_addr, &obj_name, &work, 
			  &seqno, &multi_addr, &actid, &uid, &opn_code,
			  &df_serverport)) == MANAGER_FAILED)
      {
	  cerr << "manager: listen failed\n" << flush;
	  cerr << "Check that c_count can be created\n" << flush;

	  exit(1);
      }
      else
	  if (rcode == SUCCEEDED)
	  {
	      if ((singleUserMode) && (!crashRecoveryMessage))
		  respondToClient(client_addr, work.rpc_code);
	      else
	      {
#ifdef DEBUG
		  debug_stream.lock();
		  
		  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
		  debug_stream << "manager: listen ok" << endl;
		  debug_stream.unlock();
		  
#endif
		  switch(work.rpc_code)
		  {
		  case SINGLEUSER:
		      setSingleUser(TRUE, SINGLEUSER);
		      break;
		  case MULTIUSER:
		      setSingleUser(FALSE, MULTIUSER);
		      break;
		  case MANAGER_DIE:
		      doReply = TRUE;
		      terminateManager(0);
		      break;
		  case GET_WORKER:
		  case GM_SERVER:
#ifdef DEBUG
		      debug_stream.lock();
		      
		      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
		      debug_stream << "MANAGER:: correct Opcode, about to call create_server()\n" << flush;
		      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
		      debug_stream << "Manager: with multiaddr: <" << ntohl(multi_addr . type) << ","
				   << ntohl(multi_addr . hi_part) << "," << ntohl(multi_addr . lo_part) << ">\n" << flush;
		      debug_stream.unlock();
		      
#endif

#ifdef DEBUG
		      debug_stream.lock();
		      
		      if (work.rpc_code == GM_SERVER)
		      {
			  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
			  debug_stream << "manager: About to create_server for group management\n" << flush;
		      }
	    
		      if (work.rpc_code == GET_WORKER)
		      {
			  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
			  debug_stream << "manager: with object name; " << obj_name << "\n" << flush;
			  debug_stream << "manager: and opcode: " << work.rpc_code << "\n" << flush;
		      }
		      debug_stream.unlock();
		      
#endif
		      j = 0;
		      invalid_location = FALSE;
		      while ((obj_name[j] != '\0') && (!invalid_location))
		      {
			  if (obj_name[j] == '/')
			  {
			      error_stream << WARNING << "MANAGER:: create_server() FAILED due to invalid server location " << obj_name << endl;
			      invalid_location = TRUE;
			      senderror(&client_addr, NOSERVER);
			  }
			  else
			      j++;
		      }
		      if (invalid_location)
			  break;

		      valid_path = FALSE;
		      if (path_number >= 1)
		      {
			  j = 1;
			  do
			  {
			      if (tmp_name)
#ifndef GCC_ARRAY_DELETE_BUG
			      delete [] tmp_name;
#else
			      delete tmp_name;
#endif		  
			      tmp_name = new char [(strlen(search_path[j].dir_name)+strlen(obj_name)+2)];
			      ::memset(tmp_name, '\0', (strlen(search_path[j].dir_name)+strlen(obj_name)+2));
			      ::strcpy(tmp_name, search_path[j].dir_name);
			      ::strcat(tmp_name, "/");
			      ::strcat(tmp_name, obj_name);

			      if (stat(tmp_name, &buf) == 0)
				  valid_path = TRUE;
			      else
			      {
				  j++;
#ifndef GCC_ARRAY_DELETE_BUG
				  ::delete [] tmp_name;
#else
				  ::delete tmp_name;
#endif		      
				  tmp_name = 0;
			      }
			  } while ((j <= path_number) && (!valid_path));
		      }

		      if ((!valid_path) || (path_number == 0))
		      {
			  if (tmp_name)
#ifndef GCC_ARRAY_DELETE_BUG
			      ::delete [] tmp_name;
#else
			      ::delete tmp_name;
#endif	      
			  tmp_name = new char[(strlen(location)+strlen(obj_name)+2)];
			  ::memset(tmp_name, '\0', (strlen(location)+strlen(obj_name)+2));
			  ::strcpy(tmp_name, location);
			  ::strcat(tmp_name, "/");
			  ::strcat(tmp_name, obj_name);
		      }

		      if (verboseMode)
			  cout << "Manager: attempting to create server " << tmp_name << endl;
			  
		      if (create_server(tmp_name, &client_addr, &work, 
					&seqno, &multi_addr, &actid, &uid,
					opn_code, df_serverport) != SUCCEEDED)
			  perror("MANAGER:: create_server() FAILED");

		      if (tmp_name)
		      {
#ifndef GCC_ARRAY_DELETE_BUG
			  ::delete [] tmp_name;
#else
			  // g++ does not appear to handle delete [] correctly
			  ::delete tmp_name;
#endif
			  tmp_name = 0;
		      }

		      break;
#ifdef DEBUG
		  default:
		      error_stream << WARNING << "MANAGER:: unrecognised opcode " << work.rpc_code << "\n" << flush;
		      break;
#endif
		  }  /* end switch */
	      } /* end if */
	  }

      if (l_work.mc_msg[0].iov_base)
      {
	  free(l_work.mc_msg[0].iov_base);
	  l_work.mc_msg[0].iov_base = 0;
      }
      if (l_work.mc_msg[7].iov_base)
      {
#ifndef GCC_ARRAY_DELETE_BUG
	  delete [] l_work.mc_msg[7].iov_base;
#else
	  delete l_work.mc_msg[7].iov_base;
#endif
	  l_work.mc_msg[7].iov_base = 0;
      }
      if (l_work.mc_msg[8].iov_base)
      {
#ifndef GCC_ARRAY_DELETE_BUG
	  delete [] l_work.mc_msg[8].iov_base;
#else
	  delete l_work.mc_msg[8].iov_base;
#endif
	  l_work.mc_msg[8].iov_base = 0;
      }
    }      /* end for(;;) */

    return 0;
}