/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: manager.cc,v 1.70 1993/10/22 13:48:58 nmcl Exp $
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

#include <RPC/Rajdoot/rpcuser.h>
#include "rpc.h"

extern int errno;

static const char RCSid[] = "$Id: manager.cc,v 1.70 1993/10/22 13:48:58 nmcl Exp $";

static struct MID   manager;  
static directory_path search_path[PATH_SIZE];
Boolean debug_mode = FALSE, singleUserMode = FALSE;
struct mc_msghdr    l_work;

void killManager ()
{
#ifdef hpux
    pid_t pid;
#else
    int pid;
#endif
    
    pid = getpid();
    (void) kill(pid, SIGKILL);
}

#ifdef HAVE_INTSIG
int terminateManager (int dummy DOTDOTDOT)
#else
void terminateManager (int dummy DOTDOTDOT)
#endif
{
#if (RAJDOOT_USE_SHARED_MEMORY == NO)
    killManager();
#else
    int rstat, retry = FILESRETRY;
    struct C_LIST_EL clist;

    do
    {
	rstat = clistopen(&clist);
    } while ((rstat != SUCCEEDED) && (retry-- > 0));

    if (rstat != SUCCEEDED)
    {
	(void) clistclose(&clist);
	killManager();
    }

    delete_clist();
    killManager();
#endif
}

#ifdef HAVE_INTSIG
int managerStartUp (int dummy DOTDOTDOT)
#else
void managerStartUp (int dummy DOTDOTDOT)
#endif
{
#ifdef DEBUG
    singleUserMode = !singleUserMode;
#else
    singleUserMode = FALSE;
#endif

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    if (singleUserMode)
	debug_stream << "Manager in single user mode" << endl;
    else
	debug_stream << "Manager in multi-user mode" << endl;
#endif    
}

int init_manager ()
{
  struct host_address host_addr;
  char                localhost[MAXHOSTNAMELEN];
  unsigned short      myport = MANAGER_ADDR;
  
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

int mkcfiles (long *crashcount)
{
/*
 * Create and initialise C_LIST and C_COUNT files.
 * To be invoked at start-up time.
 * Return OK if successful, otherwise FAILED.
 */
    int fd, rcode = FAILED, sm_id;
    time_t currentTimeValue;
    struct C_LIST_EL clist;
    char *start;

    errno = 0;

    /* Create C_LIST and open it for writing */

#if (RAJDOOT_USE_SHARED_MEMORY == NO)
    if((fd = open(C_LIST_Q, O_CREAT | O_RDWR, 0600)) > 0)
	reset_allclients(&clist);
    reset_allservers(&clist);
      
    if((write(fd, (char *) & clist, sizeof(clist)) == sizeof(clist)) &&
       (!chmod(C_LIST_Q, 0666)) && (!close(fd)))
    {
	if((fd = open(C_COUNT_Q, O_CREAT | O_RDWR, 0666)) > 0)
	{
	    currentTimeValue = time(0);
	    if((write(fd, (char *) & currentTimeValue, sizeof(currentTimeValue)) == sizeof(currentTimeValue)) && (!chmod(C_COUNT_Q, 0666)) && (!close(fd)))
	    { 
		rcode = SUCCEEDED;
		*crashcount = (long)currentTimeValue;
	    }   
	}
    }
#else
    sm_id = shmget(MANSHMKEY, sizeof(clist), 0666 | IPC_CREAT | IPC_EXCL);

    if (errno == EEXIST)
    {
	sm_id = shmget(MANSHMKEY, sizeof(clist), 0666);
    }

    if (sm_id >= 0)
    {
	if ((start = (char*) shmat(sm_id, 0, 0)) != (char*) -1)
	{
	    memset(start, '\0', sizeof(clist));
		
	    reset_allclients(&clist);
	    reset_allservers(&clist);
      
	    memcpy(start, (char*) &clist, sizeof(clist));
	    if((fd = open(C_COUNT_Q, O_CREAT | O_RDWR, 0666)) > 0)
	    {
		currentTimeValue = time(0);
		if((write(fd, (char *) & currentTimeValue, sizeof(currentTimeValue)) == sizeof(currentTimeValue)) && (!chmod(C_COUNT_Q, 0666)) && (!close(fd)))
		    { 
			rcode = SUCCEEDED;
			*crashcount = (long)currentTimeValue;
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
	exit(1);
    }  /* End creat C_LIST */
#endif

    return(rcode);
}  /* End mkcfiles */


void SendCcount (struct MID *h_keeper, long c_count, struct RPC_SN *seqno)
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
	    struct UNIQUEID *uid, long *opn_code, long *df_serverport)
{
/*
 * Listen for requests from the network addressed to manager 
 * Calls to LISTEN must be preceded by a call to Init_Manager
 */
 
  struct mc_sg_filter call;
  struct RPC_SN       seqno;
  struct MID          h_keeper;
  Boolean             cont;
  static long         crash_count = -1;
  long                opcode;
  char                *obj_name;
    
  if (crash_count < 0) /* Obtain Crashcount for housekeeper enquiries */
     if (mkcfiles(&crash_count) == FAILED)
     {
#ifdef DEBUG
	 debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	 debug_stream << "Cannot create c_count file.\n" << flush;
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
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "\n\nmanager: about to receive\n" << flush;
#endif
    cont = TRUE;
    while (cont)
    {
      if (multicast_long_receive(&manager, &call, 0, -1) >= 1)
	cont = FALSE;

#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "\n\nmanager: received something\n" << flush;
#endif
    }

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "\n\nmanager: received okay\n" << flush;
#endif

    /* Cast buffers received */
    seqno = * (struct RPC_SN *)    l_work.mc_msg[0].iov_base;
    opcode = * (long *)            l_work.mc_msg[1].iov_base;
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
      *opn_code = * (long *)         l_work.mc_msg[5].iov_base;
      *df_serverport = * (long *)    l_work.mc_msg[6].iov_base;
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

#ifdef DEBUG
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
#ifndef __GNUG__
      delete [] l_work.mc_msg[7].iov_base;
#else
      delete l_work.mc_msg[7].iov_base;
#endif      
      l_work.mc_msg[7].iov_base = 0;
    }
    if (l_work.mc_msg[8].iov_base)
    {
#ifndef __GNUG__
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
		   struct UNIQUEID *uid, long opn_code, long df_serverport)
{
#ifdef HAVE_UNION_WAIT
  union wait status;
#else
  int status;
#endif
  long pid, child_id;
  long  pathlength, buff_size;
  struct PATHS temp_path;
  register int i;
  char argbuff[W_ARGS][ARGSTRING];
  char *argp[W_ARGS], *buff, *nextbuff;
  char *checking_server;
  char *groupman_server;

  checking_server = DUPSERVER_Q;
  groupman_server = GMSERVER_Q;

  if (!(child_id = fork()) )
  {
    if (!(child_id = fork()) )
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "Manager: creatw(): multiaddr: <" << ntohl(multi_addr -> type) << ","
	    << ntohl(multi_addr -> hi_part) << "," << ntohl(multi_addr -> lo_part) << ">\n" << flush;
#endif

      /* Initialise pointers to argument list */
      for (i = 0; i < (W_ARGS - 1); i++)
          argp[i] = &argbuff[i][0];

      argp[1] = object_name;
      sprintf(argp[2], "%ld", client_address -> type);
      sprintf(argp[3], "%ld", client_address -> hi_part);       
      sprintf(argp[4], "%ld", client_address->lo_part);
      sprintf(argp[5], "%ld", call_seqno -> sec);  
      sprintf(argp[6], "%ld", call_seqno -> msec);
      sprintf(argp[7], "%ld", multi_addr -> type);
      sprintf(argp[8], "%ld", multi_addr -> hi_part);      
      sprintf(argp[9], "%ld", multi_addr -> lo_part);            
      sprintf(argp[10], "%ld", actid -> type);
      sprintf(argp[11], "%ld", actid -> hi_part);      
      sprintf(argp[12], "%ld", actid -> lo_part);            
      sprintf(argp[13], "%ld", uid -> host);
      sprintf(argp[14], "%ld", uid -> sec);
      sprintf(argp[15], "%ld", uid -> msec);
      sprintf(argp[16], "%ld", opn_code);            
      sprintf(argp[17], "%ld", df_serverport);            

      i = 19;
      pathlength = 0;
      buff = nextbuff = pwork->rpc_buff1;  /* Initialise buffer pointers */
      buff_size = pwork->rpc_length1;

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "rpcmanager: buff_size: " << buff_size << " and unpacked path: \n" << flush;
#endif

      while (buff_size > 0)
      {
	/* Unpack rpc-path */
	nextbuff = unpack_long(&(temp_path.node), buff);
	buff = nextbuff;
	nextbuff = unpack_long(&(temp_path.c_count), buff);
	buff = nextbuff;

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << " <" << temp_path.node << "," << temp_path.c_count << ">" << flush;
#endif

	sprintf(argp[i++], "%ld", temp_path.node);
	sprintf(argp[i++], "%ld", temp_path.c_count);
	buff_size -= sizeof(struct PATHS);
	pathlength++;
      }
      sprintf(argp[18], "%ld", pathlength);  
      if (debug_mode)
	  argp[i] = "-d"; // debugging
      else
	  argp[i] = "+d"; // no debugging
      argp[i+1] = (char *) NULL;


#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "\nrpcmanager: pathlength: " << pathlength << "\n" << flush;
	debug_stream << "manager: opcode was: " << flush;
	if (pwork -> rpc_code == GET_WORKER)
	    debug_stream << "GET_WORKER" << endl;
	else
	    debug_stream << "GM_SERVER: " << pwork->rpc_code << endl;
#endif

      if (pwork -> rpc_code == GET_WORKER)
      {
	argp[0] = checking_server;
	if (execv(checking_server, argp) == FAILED)
	{
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	    debug_stream << "manager: failed to exec server: " << errno << "\n" << flush;
#endif
	    /* Later - syslog manager diagnostics */
	    exit(1);
	}
      }
      else   /* pwork->rpc_code == GM_SERVER */
      {
	argp[0] = groupman_server;
	if (execv(groupman_server, argp) == FAILED)
	{
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	    debug_stream << "manager: failed to exec server: " << errno << "\n" << flush;
#endif	
        /* Later - syslog manager diagnostics */
	exit(1);
	}
      }
    }     
    exit(child_id < 0 ? errno : 0);
  }    

  if ((pid = wait(&status)) < 0)
       return(FAILED);
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
    struct MID      client_addr;
    struct RPC      work;
    struct MID      multi_addr, actid;
    struct UNIQUEID uid;
    struct RPC_SN   seqno;
    Boolean         invalid_location;
    long            opn_code;
    int             rcode, j, path_number;
    long            df_serverport;
    char            *obj_name, *tmp_name = 0, *location = SERVER_LOCATION_Q;
    struct stat     buf;
    Boolean         valid_path, path_list = FALSE, verboseMode = FALSE;
    char            *vMode = 0, *smMode = 0, *dMode = 0, *pList = 0;
    char            outputFile[OBJECT_NAME_SIZE], errorFile[OBJECT_NAME_SIZE];
#ifndef hpux
    int managerPID = getpid();
#else    
    pid_t managerPID = getpid();
#endif

    if (chdir("/tmp"))
	perror("manager failed to chdir to /tmp\n");

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

    if (init_manager() == FAILED)
    {
	cerr << "init_manager failed" << flush;
	exit(1);
    }

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
	path_number = 0;
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

    path_number = 0;
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

    sprintf(outputFile, "manager_out_%ld", managerPID);
    sprintf(errorFile, "manager_err_%ld", managerPID);
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
    ::signal(SIGUSR1, managerStartUp);

    for(;;)
    {
      work . rpc_code = ZEROCODE;

#ifdef DEBUG
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;

      if (singleUserMode)
	  debug_stream << "Manager in single user mode" << endl;
      else
	  debug_stream << "Manager in multi-user mode" << endl;
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
        if ((rcode == SUCCEEDED) && (!singleUserMode))
        {
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	    debug_stream << "manager: listen ok about to create server\n" << flush;
#endif

        switch(work . rpc_code)
        {
        case GET_WORKER:
        case GM_SERVER:
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	    debug_stream << "MANAGER:: correct Opcode, about to call create_server()\n" << flush;
	    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	    debug_stream << "Manager: with multiaddr: <" << ntohl(multi_addr . type) << ","
		<< ntohl(multi_addr . hi_part) << "," << ntohl(multi_addr . lo_part) << ">\n" << flush;
#endif

#ifdef DEBUG
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
#endif
	  j = 0;
	  invalid_location = FALSE;
	  while ((obj_name[j] != '\0') && (!invalid_location))
	  {
	      if (obj_name[j] == '/')
	      {
		  cerr << "MANAGER:: create_server() FAILED due to invalid server location " << obj_name;
		  invalid_location = TRUE;
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
#ifndef __GNUG__
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
#ifndef __GNUG__
		      delete [] tmp_name;
#else
		      delete tmp_name;
#endif		      
		      tmp_name = 0;
		  }
	      } while ((j <= path_number) && (!valid_path));
	  }

	  if ((!valid_path) || (path_number == 0))
	  {
	      if (tmp_name)
#ifndef __GNUG__
		delete [] tmp_name;
#else
	        delete tmp_name;
#endif	      

	      tmp_name = new char[(strlen(location)+strlen(obj_name)+2)];
	      ::memset(tmp_name, '\0', (strlen(location)+strlen(obj_name)+2));
	      ::strcpy(tmp_name, location);
	      ::strcat(tmp_name, "/");
	      ::strcat(tmp_name, obj_name);
	  }

	  if(create_server(tmp_name, &client_addr, &work, 
			   &seqno, &multi_addr, &actid, &uid,
			   opn_code, df_serverport) != SUCCEEDED)
	      perror("MANAGER:: create_server() FAILED");

          if (tmp_name)
	  {
#ifndef __GNUG__
	      delete [] tmp_name;
#else
	      // g++ does not appear to handle delete [] correctly
	      delete tmp_name;
#endif
	      tmp_name = 0;
	  }

	  break;
#ifdef DEBUG
        default:
	    cerr << "MANAGER:: unrecognised opcode " << work.rpc_code << "\n" << flush;
            break;
#endif
        }  /* end switch */
        } /* end if */

      if (l_work.mc_msg[0].iov_base)
      {
	  free(l_work.mc_msg[0].iov_base);
	  l_work.mc_msg[0].iov_base = 0;
      }
      if (l_work.mc_msg[7].iov_base)
      {
#ifndef __GNUG__
	  delete [] l_work.mc_msg[7].iov_base;
#else
	  delete l_work.mc_msg[7].iov_base;
#endif
	  l_work.mc_msg[7].iov_base = 0;
      }
      if (l_work.mc_msg[8].iov_base)
      {
#ifndef __GNUG__
	  delete [] l_work.mc_msg[8].iov_base;
#else
	  delete l_work.mc_msg[8].iov_base;
#endif
	  l_work.mc_msg[8].iov_base = 0;
      }
    }      /* end for(;;) */
}
