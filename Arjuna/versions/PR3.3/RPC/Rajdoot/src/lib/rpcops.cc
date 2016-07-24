/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: rpcops.cc,v 1.23 1996/08/21 10:37:28 nmcl Exp $
 */

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef MALLOC_H_
#  include <System/malloc.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef FCNTL_H_
#  include <System/fcntl.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#ifndef SYS_FILE_H_
#  include <System/sys/file.h>
#endif

#ifndef SYS_TIME_H_
#  include <System/sys/time.h>
#endif
 
#ifndef SYS_TIMES_H_
#  include <System/sys/times.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
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

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

#include <RPC/Rajdoot/rpcuser.h>
#include "rpc.h"

#ifndef F_ULOCK
#define F_ULOCK 0
#endif

#ifndef F_LOCK
#define F_LOCK  1
#endif

extern Uint32 get_machine_id();

void construct_altlists (struct HEAD_RPCLIST *, int *, struct HEAD_RPCLIST *,
			int *, int);
void create_rpc_list (struct HEAD_RPCLIST *, struct HEAD_RPCLIST *);
void insert_rpc_head (struct HEAD_RPCLIST *);
void insert_rpc_nextlist (struct HEAD_RPCLIST *, int *, struct RPC_LIST *,
			 struct RPC_LIST *);
void insert_rpcinfo (struct HEAD_RPCLIST *, Int32, Int32, char *, Int32, char *);
void remove_rpc_memb (struct RPC_LIST *);
char *pack_MID (struct MID, char *);
void traverse_rpclist (struct HEAD_RPCLIST *);
char *unpack_MID (struct MID *, char *);

static const char RCSid[] = "$Id: rpcops.cc,v 1.23 1996/08/21 10:37:28 nmcl Exp $";

char *clistfd = 0;
int sem_id = -1, sm_id = -1;
Int32 seqNoField = 0;

Int32 managerAddr = MANAGER_ADDR;
int managerShm = MANSHMKEY;
int managerSem = MANSEMKEY;
Boolean checked = FALSE;

void checkEnvironment ()
{
    if (checked)
	return;

    char *addr = 0, *shmKey = 0, *semKey = 0;

    addr = ::getenv("MANAGER_ADDR");
    if (addr)
	managerAddr = (Int32) atoi(addr);

    shmKey = ::getenv("MANSHMKEY");
    if (shmKey)
	managerShm = atoi(shmKey);

    semKey = ::getenv("MANSEMKEY");
    if (semKey)
	managerSem = atoi(semKey);

    checked = TRUE;
}

char *pack_group (struct GROUPS *group, int *packedsize)
{
  /* Pack structure of type 'GROUPS' in contiguous form and return the
   * buffer, and its size in bytes via 'packedsize'
   */

  struct GROUP_DESC *current_sg;
  struct GROUPIDS *current_g;
  struct SERVERIDS *current_s;
  int    no_supergrps, no_groups, no_servers;
  int    sgindx, gindx, sindx;
  int offset;
  char *start_buff, *buff;

  switch (group->groups_type)
  {
  case UNSET_type:

    *packedsize = sizeof(group->groups_type);
    start_buff = (char*) malloc(*packedsize);
    buff = start_buff;
    pack_Int32(group->groups_type, buff);
    break;

  case SRVR_type:

    *packedsize = sizeof(group->groups_type) + sizeof(struct MID);
    start_buff = (char*) malloc(*packedsize);
    buff = start_buff;
    buff = pack_Int32(group->groups_type, buff);
    pack_MID(group->grp_srvr.srvr.sr_id, buff);
    break;

  case GRP_type:

#ifdef DEBUG
      debug_stream.lock();
      
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "group being packed\n";
    traverseall_supergrps(&(group->grp_srvr.group));
    debug_stream << flush;
      debug_stream.unlock();
      
#endif
    offset = 0;
    *packedsize = sizeof(group->groups_type) * 2;
    start_buff = (char*) malloc(*packedsize);  
    buff = start_buff;

    no_supergrps = group->grp_srvr.group.no_supergrps;

    
    buff = pack_Int32(group->groups_type, buff);
    buff = pack_Int32(no_supergrps, buff);
    offset += (sizeof(group->groups_type) * 2);

    current_sg =  group->grp_srvr.group.head_gd;
    /* Pack group descriptor */
    for (sgindx = 0; sgindx < no_supergrps; sgindx++)
    {
      *packedsize += (sizeof(struct MID) + sizeof(Int32));

      if ((start_buff = (char*) realloc((malloc_t) start_buff, *packedsize)) == NULL)
	return(NULL);
      buff = start_buff + offset;

      buff = pack_MID(current_sg->mymid, buff);
      offset += (sizeof(struct MID));

      /* Pack groups in group descriptor */
      no_groups = current_sg->grps.no_grps;
      current_g = current_sg->grps.head_gids;

      *packedsize += (sizeof(struct MID) * no_groups);
      buff = pack_Int32(no_groups, buff);
      offset += (sizeof(no_groups));

      if ((start_buff = (char*) realloc((malloc_t) start_buff, *packedsize)) == NULL)
	return(NULL);
      buff = start_buff + offset;

      for (gindx = 0; gindx < no_groups; gindx++)
      {
	buff = pack_MID(current_g->g_id, buff);
	offset += (sizeof(struct MID));
	current_g = current_g->next_groupid;
      }

      /* Pack servers in group descriptor */
      no_servers = current_sg->sids.no_srs;
      current_s = current_sg->sids.head_sids;

      *packedsize += (sizeof(Int32) + (sizeof(struct MID) * 2 * no_servers));

      if ((start_buff = (char*) realloc((malloc_t) start_buff, *packedsize)) == NULL) 
	return(NULL);
      buff = start_buff + offset;

      buff = pack_Int32(no_servers, buff);
      offset += (sizeof(no_servers));

      for (sindx = 0; sindx < no_servers; sindx++)
      {
	buff = pack_MID(current_s->s_id, buff);
	buff = pack_MID(current_s->sender, buff);
	offset += ((sizeof(struct MID) * 2));
	current_s = current_s->next_serverid;
      }

      current_sg = current_sg->next_group;

    } /* end for */
    break;

  default:

    return(NULL);

  } /* end switch */

  return(start_buff);

} /* end pack_group() */

struct GROUPS *unpack_group (char *group, int  packedsize)
{
  /* Unpack group supplied in buffer 'group' and whose size is given by
   * by 'packedsize', and return it in the GROUPS linked list structure
   */

  struct GROUPS *unpackedgroup;
  struct GROUP_DESC *current_sg;
  struct MID tmp_mid, tmp_sid;
  Int32 no_supergrps, no_groups, no_servers;
  int sgindx, gindx, sindx;
  char * buff;

  if (packedsize < sizeof(Int32))
    return(NULL);

  buff = group; /* Initialise temporary buffer */
  unpackedgroup = (struct GROUPS *) malloc(sizeof(struct GROUPS));
  
  buff = unpack_Int32(&(unpackedgroup->groups_type), buff);

  switch(unpackedgroup->groups_type)
  {
  case SRVR_type:

    unpack_MID(&(unpackedgroup->grp_srvr.srvr.sr_id), buff);
    break;

  case GRP_type:

    buff =  unpack_Int32(&no_supergrps, buff);
    buff = unpack_MID(&tmp_mid, buff);

    unpackedgroup->grp_srvr.group.head_gd = NULL;
    unpackedgroup->grp_srvr.group.no_supergrps = 0;
    insert_super_group(&(unpackedgroup->grp_srvr.group), tmp_mid);
    
    current_sg = unpackedgroup->grp_srvr.group.head_gd;
    sgindx = 0;
    do
    {
      /* Unpack groups in group descriptor */
      buff = unpack_Int32(&no_groups, buff);

      for (gindx = 0; gindx < no_groups; gindx++)
      {
	buff = unpack_MID(&tmp_mid, buff);
	insert_groups(&(unpackedgroup->grp_srvr.group),
		      current_sg->mymid, tmp_mid);
      }
      
      /* Unpack servers in group descriptor */
      buff = unpack_Int32(&no_servers, buff);

      for (sindx = 0; sindx < no_servers; sindx++)
      {
	buff = unpack_MID(&tmp_mid, buff);
	buff = unpack_MID(&tmp_sid, buff);
	insert_servers(&(unpackedgroup->grp_srvr.group),
		       current_sg->mymid,
		       tmp_mid, tmp_sid);
      }

      if (++sgindx < no_supergrps)
      {
	buff = unpack_MID(&tmp_mid, buff);
	insert_super_group(&(unpackedgroup->grp_srvr.group), tmp_mid);
	current_sg = current_sg->next_group;
      }

    } while (sgindx < no_supergrps);
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "unpacked group\n";
    traverseall_supergrps(&(unpackedgroup->grp_srvr.group));
    debug_stream << flush;
    debug_stream.unlock();
    
#endif    

    break;

  } /* end switch */

  return(unpackedgroup);

} /* end unpack_group() */

Boolean identical_group (struct MID *group1, struct MID *group2)
{
  return (((group1 -> hi_part == group2 -> hi_part ) &&
	  (group1 -> lo_part == group2 -> lo_part)) ? TRUE : FALSE);

} /* end identical_group() */

int match_clients (struct MID *client1, struct MID *client2)
{
  if ((client1 -> type == client2 -> type) &&
      (client1 -> hi_part == client2 -> hi_part) &&
      (client1 -> lo_part == client2 -> lo_part))
    return(SUCCEEDED);
  else
      return(FAILED);
} /* end match_clients() */

Boolean identical_sn (struct RPC_SN *sn1, struct RPC_SN *sn2)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "comparing <" << sn1->sec << ", " << sn1->msec << ">\n";
    debug_stream << "with <" << sn2->sec << ", " << sn2->msec << ">" << endl;
    debug_stream.unlock();
    
#endif

  return (((sn1 -> sec == sn2 -> sec) &&
	  (sn1 -> msec == sn2 -> msec)) ? TRUE : FALSE);
} /* end identical_sn() */


/* Put sequence number in the structure pointed at by the output
 * parameter `seq_no'
 */

int getsn (struct RPC_SN *seq_no)
{
    time_t currentTime = time(0) + seqNoField;
    seqNoField++;

    seq_no->sec = (get_machine_id() << 16) | (getpid() & 0xffff);
    seq_no->msec = (Int32) currentTime; /* time in microseconds */
    return(SUCCEEDED);
}

int getloghost (struct host_address *host_addr)
{
/* map real host name to internetwork host address 
 * returns: SUCCEEDED if host mapping found, with internetwork host field set
 *          or NO_HOST_MAP if host mapping not found
 */
 
  struct hostent *h_desc;

  if ((h_desc = gethostbyname(host_addr -> real_host)) == NULL)
      return(NO_HOST_MAP);

  (void) memcpy((char *) &(host_addr->log_host), h_desc->h_addr, h_desc->h_length);

  return(SUCCEEDED);
  
} /* end getloghost() */

int getrealhost (struct host_address *host_addr)
{    
/* map internetwork host address to real host name 
 * returns: SUCCEEDED if host mapping found, with real host field set
 *          or NO_HOST_MAP if host mapping not found
 */
 
  struct hostent *h_desc;
  
  if ((h_desc = gethostbyaddr((char *) & (host_addr ->log_host),
			      sizeof(struct in_addr), AF_INET)) == NULL) 
      return(NO_HOST_MAP);
  
  host_addr->real_host = ::new char[MAXHOSTNAMELEN+1];
  ::memset(host_addr->real_host, '\0', MAXHOSTNAMELEN+1);
  ::strcpy(host_addr->real_host, h_desc->h_name);

  return(SUCCEEDED);
  
} /* end getrealhost() */    

void setalarm (Uint32 limit)
{
/* set real time clock to expire after 'limit' centiseconds */

  struct itimerval clock_value;

  clock_value.it_interval.tv_sec = 0;
  clock_value.it_interval.tv_usec = 0;
 
  if (limit == 0)
  {
    clock_value.it_value.tv_sec = 0;
    clock_value.it_value.tv_usec = 0;
  }  
  else
  {
    clock_value.it_value.tv_sec = limit / 100;     /* convert centisecs to secs */
    clock_value.it_value.tv_usec = (limit % 100) * 1000000; /*convert centisecs to microsecs */
  }

  setitimer(0, &clock_value, 0);  

} 

Boolean isnull_id (struct MID *multi_addr)
{
  return (((multi_addr -> hi_part == htonl(0)) &&
	   (multi_addr -> lo_part == htonl(0))) ? TRUE : FALSE);
}

void getnullmid (struct MID *multi_id)
{
  multi_id->type = multi_id->hi_part = multi_id->lo_part = htonl(0);
}

char *pack_Int32 (Int32  _val, char *_buff)
{
	*(Int32 *) _buff = _val;
	
	return(_buff + 4);
}

char *unpack_Int32 (Int32 *_val, char *_buff)
{
	*_val = *(Int32 *) _buff;

	return(_buff + 4);
}

char *pack_MID (struct MID _val, char *_buff)
{
	char *_buf;

	_buf = _buff;
	_buf = pack_Int32(_val.type, _buf); 
	_buf = pack_Int32(_val.hi_part, _buf);
	_buf = pack_Int32(_val.lo_part, _buf);
	
	return(_buf);
}

char *unpack_MID (struct MID *_val, char *_buff)
{
	char *_buf;

	_buf = _buff;
	_buf = unpack_Int32(&_val->type, _buf); 
	_buf = unpack_Int32(&_val->hi_part, _buf);
	_buf = unpack_Int32(&_val->lo_part, _buf);
	
	return(_buf);
}

char *pack_HPID (struct MID _val, char       *_buff)
{
	char *_buf;

	_buf = _buff;
	
	_buf = pack_Int32(_val.hi_part, _buf);
	_buf = pack_Int32(_val.lo_part, _buf);
	
	return(_buf);
}

char *unpack_HPID (struct MID  *_val, char        *_buff)
{
	char *_buf;

	_buf = _buff;
	
	_buf = unpack_Int32(&_val->hi_part, _buf);
	_buf = unpack_Int32(&_val->lo_part, _buf);
	_val->type = htonl(NOT_JOINED);
	
	return(_buf);
}

char *pack_PATHS (struct PATHS _val, char *_buff)
{
  char *_buf;

  _buf = _buff;
  _buf = pack_Int32(_val.node, _buf);
  _buf = pack_Int32(_val.c_count, _buf);

  return(_buf);

}

char *unpack_PATHS (struct PATHS *_val, char *_buff)
{
  char *_buf;

  _buf = _buff;
  _buf = unpack_Int32(&_val->node, _buf);
  _buf = unpack_Int32(&_val->c_count, _buf);

  return(_buf);

}

#ifdef DEBUG
void printgid (struct MID gid)
{
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "<" << ntohl(gid.type) << "," << ntohl(gid.hi_part) << "," << ntohl(gid.lo_part) << ">\n" << flush;
    debug_stream.unlock();
}
#else
void printgid (struct MID)
{
}
#endif

void set_rpcinfo (struct RPC *new_rpc, Int32 opcode, Int32 blength1, char *buffer1, Int32 blength2, char *buffer2)
{
  new_rpc->rpc_code = opcode;
  new_rpc->rpc_length1 = blength1;
  new_rpc->rpc_buff1 = buffer1;
  new_rpc->rpc_length2 = blength2;
  new_rpc->rpc_buff2 = buffer2;

} /* end set_rpcinfo() */

int clistopen (struct C_LIST_EL*& clist, Boolean doAttach)
{
  int rretry = FILESRETRY;
  int waitTime = USLEEPWAIT;

  checkEnvironment();
  
  _rpcerr = 0;
  errno = 0;

  if (sm_id < 0)
      sm_id = shmget(managerShm, sizeof(clist), 0666);

  if (sm_id >= 0)
  {
      do
      {
	  if (sem_id < 0)
	      sem_id = semget(managerSem, 1, 0600 | IPC_CREAT | IPC_EXCL);
	  if ((sem_id < 0) && (rretry -1 > 0))
	  {
#ifdef DEBUG
	      debug_stream.lock();
	      
	      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	      debug_stream << "rpcops: semget conflict. Backing off" << endl;
	      debug_stream.unlock();
#endif	      
	      // do an exponential backoff if we get a conflict.
	      
	      usleep(waitTime);
	      waitTime = (int) (waitTime * EXP_VALUE);
	      if (waitTime > MAXWAITTIME)
		  waitTime = MAXWAITTIME;
	  }
      } while ((sem_id < 0) && (--rretry > 0));

      if (sem_id < 0)
      {
	  cerr << "Failed to get shared memory semaphore. Errno " << errno << endl;
	  _rpcerr = READFAILED;
	  return FAILED;
      }

      /*
       * The manager attaches independently, so we cannot do this again.
       */
      
      if (doAttach)
      {
	  errno = 0;
	  
	  if ((clistfd = (char*)shmat(sm_id, 0, 0)) != (char*) -1)
	      clist = (struct C_LIST_EL*) clistfd;
	  else
	  {
	      cerr << "Failed to attach to shared memory for c_list. Errno " << errno << "\n" << flush;
	      cerr << "Check manager is running\n" << flush;
	      _rpcerr = READFAILED;
	      return(FAILED);
	  }
      }
  }
  else
  {
      cerr << "Failed to get shared memory segment for c_list. Errno " << errno << "\n" << flush;

      if (errno == EINVAL)
	  cerr << "Possible problem with requested size for shared memory: " << sizeof(clist) << " bytes" << endl;
      else
	  cerr << "Check manager is running\n" << flush;
      _rpcerr = OPENFAILED;
      return(FAILED);
  }

  return(SUCCEEDED);
} /* end clistopen() */

int clistclose ()
{
#if !defined(hpux) && !defined(_AIX)
  union semun arg;
#endif
  int sem_ctl;
  int result;

  checkEnvironment();
  
  _rpcerr = 0;

  if ((clistfd == 0) || (clistfd == (char*) -1) || (sem_id < 0))
      return SUCCEEDED;
  
  result = shmdt(clistfd);

#ifdef DEBUG
  if (result == -1)
      cerr << "Failed to detach shared memory segment. Errno " << errno << endl;
#endif

#if defined(hpux) || defined(_AIX)
  sem_ctl = semctl(sem_id, managerSem, IPC_RMID, 0);
#else
  arg.val = 0;

#ifndef __linux__
  sem_ctl = semctl(sem_id, managerSem, IPC_RMID, arg);
#else
  sem_ctl = semctl(sem_id, 0, IPC_RMID, arg);
#endif

#endif

  if (sem_ctl == -1)
  {
      cerr << "Failed to remove lock. sem_id " << sem_id << ". Errno " << errno << "\n" << flush;
      _rpcerr = CLOSEFAILED;
      return(FAILED);
  }

  sem_id = -1;
  sm_id = -1;
  
  return(SUCCEEDED);
}/* end clistclose() */

void delete_clist ()
{
#if defined(hpux) || defined(_AIX)
    int myUid;
#else
    union semun arg;
    uid_t myUid;
#endif
    int result, sem_ctl;
    struct shmid_ds buf;

    checkEnvironment();
    
    result = shmdt(clistfd);

    myUid = getuid();
    buf.shm_perm.uid = myUid;
    
    sem_ctl = shmctl(sm_id, IPC_RMID, &buf);

#if defined(hpux) || defined(_AIX)
    sem_ctl = semctl(sem_id, managerSem, IPC_RMID, 0);
#else
    arg.val = 0;

#ifndef __linux__
    sem_ctl = semctl(sem_id, managerSem, IPC_RMID, arg);
#else
    sem_ctl = semctl(sem_id, 0, IPC_RMID, arg);
#endif

#endif
}
    
/***************************************************/
/* Routines for initialising contents of the clist */
/***************************************************/

void reset_mid (struct MID *givenmid)
{
  givenmid->type = -1;
  givenmid->hi_part = -1;
  givenmid->lo_part = -1;
}

void reset_uid (struct UNIQUEID *givenuid)
{
  givenuid->host = -1;
  givenuid->sec = -1;
  givenuid->msec = -1;
}

void reset_rpcsn (struct RPC_SN *givenrpcsn)
{
  givenrpcsn->sec = -1;
  givenrpcsn->msec = -1;
  givenrpcsn->retry = -1;
  givenrpcsn->t_out = -1;

}

void clear_path (struct FULL_PATH *givenpath)
{
  /* This routine clears up those rpcpaths that are associated with
   * orphan links
   */
  int i;
  
  reset_mid(&(givenpath->groupid));
  reset_rpcsn(&(givenpath->timestamp));
  i = 0;
  /* No holes exist */
  while ((givenpath->rpcpath[i].node != 0) && (i < NSTATION))
  {
    givenpath->rpcpath[i].node = 0;
    givenpath->rpcpath[i].c_count = -1;
    i++;
  }
} /* end clear_path() */

void reset_path (struct FULL_PATH *givenpath)
{
  /* Routine to initialise rpcpaths */
  int i;
  
  reset_mid(&(givenpath->groupid));
  reset_rpcsn(&(givenpath->timestamp));
  for (i = 0; i < NSTATION; i++)
  {
    givenpath->rpcpath[i].node = 0;
    givenpath->rpcpath[i].c_count = -1;
  }
} /* end reset_path() */

void reset_allpaths (struct FULL_PATH links[MAXCLIENTS])
{
  int j;

  for (j = 0; j < MAXCLIENTS; j++)
    reset_path(&(links[j]));
} /* end reset_allpaths */

void reset_server_info (struct SERVER_DESC *s_desc)
{
  s_desc->process_id = -1;
  s_desc->poflag = FALSE;
  reset_mid(&(s_desc->myunicastid));
  reset_mid(&(s_desc->sender));
  reset_mid(&(s_desc->action_id));
  reset_uid(&(s_desc->u_id));
  s_desc->no_links = 0;
  reset_allpaths(s_desc->linkspath);
} /* end reset_server_info() */

void reset_pathel (struct PATH_EL *givenpathel)
{
  int i;
  
  givenpathel->node = 0;
  givenpathel->c_count = -1;
  for (i = 0; i < MAXSERVERS; i++)
    givenpathel->s_indx[i] = -1;
}

void reset_allservers (struct C_LIST_EL *clist)
{
  int j;

  for (j = 0; j < MAXSERVERS; j++)
    reset_server_info(&(clist->s_desc[j]));
}

void reset_allclients (struct C_LIST_EL *clist)
{
  int i;

  for (i = 0; i < NSTATION; i++)
    reset_pathel(&(clist->client[i]));
}

int get_clientindx (struct C_LIST_EL *clist, Int32 client_node)
{
  /* Return the client indx in the clist where the node given by
   * client_node is held, if one such slot exists
   */

  int client_indx = -1, i;

  i = 0;
  while ((i < NSTATION) && (client_indx < 0))
  {
    if (clist->client[i].node == client_node)
      client_indx = i;
    i++;
  }
  return(client_indx);

}

/**************************************************************************/
/* Routine used by duplicateserver and housekeeper - for orphan detection */
/**************************************************************************/

void RmClientFromPath (struct C_LIST_EL *clist, int client_indx, Int32 client_node)
{
  struct FULL_PATH *current_link;
  Boolean found;
  int check_sindx, srindx, fullptindx, ptindx;

#ifdef DEBUG
  debug_stream.lock();
  
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "\nDUP: about to RMCLIENTFROMPATH\n" << flush;
  debug_stream.unlock();
  
#endif  
  
  /* Examine each server serving given client node */
  srindx = 0;
  check_sindx = clist->client[client_indx].s_indx[srindx];
  while (srindx < MAXSERVERS)
  {
    if (check_sindx >= 0)
    {
      /* Remove all links with given client */
      fullptindx = 0;
      while ((fullptindx < MAXCLIENTS) &&
	     (clist->s_desc[check_sindx].no_links > 0))
      {
	current_link = &(clist->s_desc[check_sindx].linkspath[fullptindx]);

	/* Look for client in RPC-path */
	ptindx = 0;
	found = FALSE;
	while ((current_link->rpcpath[ptindx].node != 0) &&
	       (ptindx < NSTATION) && (!found))
	  found = (current_link->rpcpath[ptindx++].node == client_node)
	      ? TRUE : FALSE;

	/* Remove that link if client entry exists in it
	   and decrement #links */
	if (found)
	{
#ifdef DEBUG
	    debug_stream.lock();
	    
	    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	    debug_stream << "\nDUP: RMCLIENTFROMPATH: client entry found in linkspath\n" << flush;
	    debug_stream.unlock();
	    
#endif
	  clear_path(current_link);
	  clist->s_desc[check_sindx].no_links--;
	}
	
	fullptindx++;
      } /* end while */

      /* If #links = 0, that server is an orphan */
      if (clist->s_desc[check_sindx].no_links == 0)
      {
#ifdef DEBUG
	  debug_stream.lock();
	  
	  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	  debug_stream << "\nDUP: RMCLIENTFROMPATH: about to kill process "
		       << clist->s_desc[check_sindx].process_id << "\n" << flush;
	  debug_stream.unlock();
	  
#endif
	kill((int) clist->s_desc[check_sindx].process_id, ORPHAN_SIGNAL);   

	/* Clean up clist and signal orphan process */
	reset_server_info(&(clist->s_desc[check_sindx]));
      }

    } /* end if */

    if (++srindx < MAXSERVERS)
      check_sindx = clist->client[client_indx].s_indx[srindx];

  } /* end while */

} /* end RmClientFromPath() */

/**************************************************************************/
/* Routines that manipulate reply group expected by the client and server*/
/**************************************************************************/

int dec_filtgrp (struct GROUP_INFO *filt_grp, struct MID server)
{
  /* Set replied field of server in list - if server exists in group */

  struct SERVERIDS *current_server;
  Boolean found = FALSE;

  if ((filt_grp->head_gd == NULL) ||
      (filt_grp->head_gd->sids.head_sids == NULL))
    return(EMPTY_SUPERGRP);

#ifdef DEBUG
  debug_stream.lock();
  
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "\ndec_filtgrp: received from server: ";
  printgid(server);
  debug_stream << flush;
  debug_stream.unlock();
  
#endif
  current_server = filt_grp->head_gd->sids.head_sids;
  do
  {
#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "\ndec_filtgrp: servers in list: " ;
      printgid(current_server->sender);
      debug_stream << flush;
      debug_stream.unlock();
      
#endif
    if ((!current_server->replied) &&
	(identical_group(&(current_server->sender), &server)))
      found = current_server->replied = TRUE;
    else
      current_server = current_server -> next_serverid;

  } while ((current_server != NULL) && (!found));

  return((found) ? SUCCEEDED : FAILED);

} /* end dec_filtgrp() */

int vote_on_results (struct HEAD_RPCLIST *msgs_recvd, int successes_expected, int compare_code, struct RPC *correct_rpc)
{
  /* Vote on messages received ('msgs_recvd') according to comparison
   * criteria given by 'compare_code', with the requirement that
   * 'successes_expected' succeed the voting process.  Return code is
   * SUCCEEDED or FAILED, depending on the outcome of the vote, if FAILED,
   * 'correct_rpc' is not set, but if SUCCEEDED, the correct rpc is returned 
   * via 'correct_rpc'
   */

  struct HEAD_RPCLIST *currentlist, *nextlist;
  Boolean stop;
  int sizeofcurrent, sizeofnext, rcode;

  /* Create a separate list for manipulation, since the original list is
     to be undisturbed if the vote fails */
  currentlist = new HEAD_RPCLIST;
  nextlist = new HEAD_RPCLIST;

  create_rpc_list(msgs_recvd, currentlist);

#ifdef DEBUG
  debug_stream.lock();
  
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "after creating rpclist: currentlist ";
  traverse_rpclist(currentlist);
  debug_stream << flush;
  debug_stream.unlock();
  
#endif  
  stop = FALSE;

  if (currentlist != NULL)
  {
    construct_altlists(currentlist, &sizeofcurrent,
		       nextlist, &sizeofnext, compare_code);

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "\nafter constructing alt lists: currentlist: ";
    traverse_rpclist(currentlist);
    debug_stream << flush;

    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "next list ";
    traverse_rpclist(nextlist);
    debug_stream << flush;

    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "SIZES: current " << sizeofcurrent << " next " << sizeofnext <<
	" successed_expected " << successes_expected << "\n" << flush;
    debug_stream.unlock();
    
#endif
    if (sizeofcurrent >= successes_expected)
    {
      rcode = SUCCEEDED;
      stop = TRUE;
    }
    else
    {
      if (sizeofnext >= successes_expected)
	stop = FALSE;
      else
      {
	rcode = FAILED;
	stop = TRUE;
      }
    }
    if (!stop)
    {
      /* continue to divide list */
      remove_rpc_list(currentlist, FALSE);
      currentlist = nextlist;
    }
  } while (!stop);
  
  if (rcode == SUCCEEDED)
  {
    *correct_rpc = currentlist->head_rpc->info;
    currentlist->head_rpc = currentlist->head_rpc->next_rpc;
    remove_rpc_list(currentlist, TRUE);
    remove_rpc_list(nextlist, TRUE);    
  }
  else
  {
    remove_rpc_list(currentlist, FALSE);
    remove_rpc_list(nextlist, FALSE);
  }
  return(rcode);

} /* end vote_on_results() */

void remove_rpc_list (struct HEAD_RPCLIST *rpclist, Boolean all)
{
  /* Free all storage associated with rpclist */
  
  struct RPC_LIST *currentptr, *nextptr;

  currentptr = rpclist->head_rpc;
  nextptr = NULL;

  while (currentptr != NULL)
  {
    nextptr = currentptr->next_rpc;
    if (all)
      remove_rpc_memb(currentptr);
    delete currentptr;
    currentptr = nextptr;
  }
  rpclist->head_rpc = NULL;

} /* end remove_rpc_list() */

void insert_newmsg (struct mc_sg_filter *new_msg, struct HEAD_RPCLIST *new_rpc)
{
  /* Extract the RPC field of the message given by 'new_msg', and insert
   * it at the head of the linked RPC-list given by 'new_rpc'
   */

  insert_rpc_head(new_rpc);

  /* Set info field of new_rpc */
  insert_rpcinfo(new_rpc,
		 * (Int32 *) new_msg->mc_msg->mc_msg[1].iov_base,
		 new_msg->mc_msg->mc_msg[2].iov_len,
		 (char*) new_msg->mc_msg->mc_msg[2].iov_base, 
		 new_msg->mc_msg->mc_msg[3].iov_len,
		 (char*) new_msg->mc_msg->mc_msg[3].iov_base);

} /* end insert_newmsg() */

Boolean isempty_filtgrp (struct GROUP_INFO *filt_grp)
{
  /* Check whether all servers have replied to Multi_Call */

  struct SERVERIDS *current_server;
  Boolean          set = FALSE;

  if ((filt_grp->head_gd == NULL) ||
      (filt_grp->head_gd->sids.head_sids == NULL))
    return(FALSE);

  current_server = filt_grp->head_gd->sids.head_sids;
  do
  {
    set = current_server -> replied;
    current_server = current_server -> next_serverid;

  } while ((current_server != NULL) && (set));

  return(set);
} /* end isempty_filtgrp() */

int init_filtgrp (struct GROUP_INFO *filt_grp, int *no_servers)
{
  /* Initialise reply entries for all servers in call group */

  struct SERVERIDS *current_server;

  if ((filt_grp->head_gd == NULL) ||
      (filt_grp->head_gd->sids.head_sids == NULL))
    return(EMPTY_SUPERGRP);

  *no_servers = filt_grp->head_gd->sids.no_srs;

  current_server = filt_grp->head_gd->sids.head_sids;
  do
  {
    current_server->replied = FALSE;
    current_server = current_server -> next_serverid;
  } while (current_server != NULL);

  return(SUCCEEDED);

} /* end init_filtgrp() */

void create_rpc_list (struct HEAD_RPCLIST *oldlist, struct HEAD_RPCLIST *newlist)
{
  struct RPC_LIST *oldlistptr;
  struct HEAD_RPCLIST *tmplist;

  oldlistptr = oldlist->head_rpc;

  tmplist = new HEAD_RPCLIST;
  tmplist->head_rpc = NULL;

  while (oldlistptr != NULL)
  {
    insert_rpc_head(tmplist);
    insert_rpcinfo(tmplist, oldlistptr->info.rpc_code,
		   oldlistptr->info.rpc_length1,
		   oldlistptr->info.rpc_buff1,
		   oldlistptr->info.rpc_length2,
		   oldlistptr->info.rpc_buff2);

    oldlistptr = oldlistptr->next_rpc;
  }

  *newlist = *tmplist;
  
  delete tmplist;

} /* end create_rpc_list() */

void remove_rpc_memb (struct RPC_LIST *rpcmemb)
{
  /* Free all storage associated with rpclist */
  
  /* Free separately allocated rpc buffers */
  if (rpcmemb->info.rpc_length1 > 0)
    free(rpcmemb->info.rpc_buff1);
  if (rpcmemb->info.rpc_length2 > 0)
    free(rpcmemb->info.rpc_buff2);
  
} /* end remove_rpc_memb() */

void construct_altlists (struct HEAD_RPCLIST *currentlist, int *sizeofcurrent, struct HEAD_RPCLIST *nextlist, int *sizeofnext, int compare)
{
  /* Search the 'currentlist' of rpcs and remove all entries whose comparison
   * criteria does not match with that of the first element in 'currentlist',
   * inserting all non-matching elements into the alternative list given by
   * 'nextlist'.  Return the size of the two resultant lists by the two size
   * parameters.  'compare' gives the comparison criterion required.
   */

  struct RPC_LIST *currentptr, *prevptr;
  Boolean removed;

  *sizeofcurrent = *sizeofnext = 0;

  /* Initialise pointers */
  prevptr = currentlist->head_rpc;
  if (prevptr != NULL)
  {
    (*sizeofcurrent)++;
    currentptr = prevptr->next_rpc;
  }
  nextlist->head_rpc = NULL;

  while (currentptr != NULL)
  {
    (*sizeofcurrent)++;
    removed = FALSE;
    
    if ((compare == compare_OPCODE) || (compare == compare_BOTH))
    {
      if ((removed = ((prevptr->info.rpc_code != currentptr->info.rpc_code)? TRUE:FALSE)))
	/* Insert element in nextlist and remove it from currentlist */
	insert_rpc_nextlist(nextlist, sizeofnext, currentptr, prevptr);
	
    }
    
    if ((!removed) && ((compare == compare_BUFFERS) ||
	(compare == compare_BOTH)))
    {
      if ((prevptr->info.rpc_length1 == currentptr->info.rpc_length1) &&
	  (prevptr->info.rpc_length2 == currentptr->info.rpc_length2))
      {

	if ((memcmp(prevptr->info.rpc_buff1,
		    currentptr->info.rpc_buff1,
		    (int) prevptr->info.rpc_length1) != 0) ||
	    (memcmp(prevptr->info.rpc_buff2,
		   currentptr->info.rpc_buff2,
		   (int) prevptr->info.rpc_length2) != 0))
	{
	  removed = TRUE;
	  
	  /* Insert element in nextlist and remove it from currentlist */
	  insert_rpc_nextlist(nextlist, sizeofnext, currentptr, prevptr);
	}
	
      }
      else
      {
	removed = TRUE;
	
	/* Insert element in nextlist and remove it from currentlist */
	insert_rpc_nextlist(nextlist, sizeofnext, currentptr, prevptr);
      }
      
    } /* end if */
 
    if (removed)
      currentptr = prevptr->next_rpc;
    else
    {
      prevptr = currentptr;
      currentptr = currentptr->next_rpc;
    }
    
  } /* end while */
  
  (*sizeofcurrent) -= (*sizeofnext);

} /* end construct_altlists() */

void traverse_rpclist (struct HEAD_RPCLIST *list)
{
  struct RPC_LIST *ptr;

  ptr = list->head_rpc;

  while (ptr != NULL)
  {
#ifdef DEBUG
      debug_stream.lock();
      
      debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
      debug_stream << "code " << ptr->info.rpc_code << " buffs " << ptr->info.rpc_buff1
	  << " " << ptr->info.rpc_buff2 << "\n" << flush;
      debug_stream.unlock();
      
#endif
      ptr = ptr->next_rpc;
  }
}

void insert_rpcinfo (struct HEAD_RPCLIST *new_rpc, Int32 opcode, Int32 blength1, char *buffer1, Int32 blength2, char *buffer2)
{
  set_rpcinfo(&(new_rpc->head_rpc->info), opcode,
	      blength1, buffer1, blength2, buffer2);

} /* end insert_rpcinfo() */

void insert_rpc_head (struct HEAD_RPCLIST *rlist)
{
  /* Allocate memory for a new element at the head of the rpclist given by
   * 'list' and return the new structure via the same list
   */

  struct RPC_LIST *ptr;

  ptr = new RPC_LIST;
  ptr->next_rpc = rlist->head_rpc;
  rlist->head_rpc = ptr;

}

void insert_rpc_nextlist (struct HEAD_RPCLIST *nextlist, int *sizeofnext, struct RPC_LIST *currentptr, struct RPC_LIST *prevptr)
{
  /* insert element from rpclist pointed to by 'currentptr' into 'nextlist'
   * and remove that element from the original list.  Increment the size of
   * 'nextlist', and return the new size via 'sizeofnext'
   */
  
  /* Insert element in nextlist */
  insert_rpc_head(nextlist);
  insert_rpcinfo(nextlist, currentptr->info.rpc_code,
		 currentptr->info.rpc_length1,
		 currentptr->info.rpc_buff1,
		 currentptr->info.rpc_length2,
		 currentptr->info.rpc_buff2);

  (*sizeofnext)++;
  
  prevptr->next_rpc = currentptr->next_rpc;
  /* Do not remove element from currentlist - since only one copy exists */

} /* end insert_rpc_nextlist() */
