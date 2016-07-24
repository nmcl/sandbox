/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: rpc.h,v 1.1 1993/11/03 14:41:34 nmcl Exp $
 */

#ifndef RPC_H_
#define RPC_H_

/* include file for RPC Protocol */

#include <Configure.h>
#include "longmltcst.h"
#include <RPC/Rajdoot/rpcuser.h>

/*************/
/* RPC types */
/*************/

#define NEWRPC            0    /* New RPC received (getwork()) */
#define RETRYRPC          1    /* RPC retry received (getwork()) */
#define DUPRPC            2    /* RPC duplicate received (getwork()) */

#define MANSHMKEY            1000
#define MANSEMKEY            2000

/***********************************************************************/
/* Timeout values for protocols  - all in centiseconds for consistency */
/***********************************************************************/

#define IDLE_TO         30000     /* Idle Time Out (5 mins - 30000) getwork() */
#define CL_TERM_TO      250       /* (2.5 secs) for client to receive ack from server */

#ifdef DEBUG
#define SERVERTOUT       2000     /* (20 secs) For a server to get ack from
				     another server - increased for debugging
				     tests */
#else
#define SERVERTOUT       1000
#endif

#define RETRYT_OUT       500      /* (5 secs) For client to send retries, expected by server */

#define GWTIMEOUT       500       /* timeout for getting server */

#define SYNCH_TOUT       250      /* For client to get response from server */
#define HK_TIMEOUT       3000     /* to receive crashcount from manager */

/* sleep interval for housekeeper - in SECONDS */
#define HK_INTERVAL     1200      /* time in SECONDS */

#define ROUNDTRIP_TIME    2       /* Approx roundtrip time in centi-seconds for ipc */

/******************************/
/* Retry values for protocols */
/******************************/

#define HK_MAXRETRY     5      /* for housekeeper to send request to manager  */
#define HK_REPLYRETRY   5      /* for manager to send crash count to housekeeper */
#define CL_TERM_RETRY   5      /* for client to *send* terminate message to server */
#define GWRETRY         5      /* Number of retries internal to initiate() */

#define SYNCH_RETRY     5      /* For client to get response from deaf server */
#define ASYNCH_RETRY    8      /* For client to get response from deaf server via manager */
#define FILESRETRY      5      /* To open, lock, close, read and write to clist */
#define SERVERRETRY     2      /* To prod a server to initialise */

/*********************************/
/* Operation Codes for protocols */
/*********************************/

#define GET_WORKER    32750    /* Opcode for unicast and multicast getworker request */
#define GM_SERVER     32753    /* Opcode for unicast and multicast get group
				* management serverrequest */
#define GW_OK         32755    /* Opcode: server->client: getworker okay */
#define GW_ERROR      32760    /* Error indication in getworker protocol (see `getworker()') */
#define MANAGER_FAILED 32765   /* Outcome if manager fails to initialise ccount or clist files */
#define INIT_DONE     32775    /* Code returned by server to another server */

#define USER_RPC      32786    /* Opcode when sending a user call */
#define TERM_OK       32787    /* opcode for server's ack - on abort - to client */
#define SRVR_ERROR    32788    /* opcode for server's ack - on abort - to client */
#define GETNEWSERVER  32735    /* Used by server, when a new server is required */
#define GETMECCOUNT   32790    /* Opcode for Housekeeper's crash count enquiry */
#define CCOUNTSENT    32795    /* Return code for housekeeper's crash count enquiry */
#define ZEROCODE      -999     /* Used to prevent UDS interface from confusing 0 with NULL */

/************************************************************************/
/* Opcodes for group management primitives  - offset of 5 from original */
/* group management opcodes, for interrupt opcodes                      */
/************************************************************************/
#define GMCODEOFFSET         5    /* Offset of interrupt opcode from ordinary
				   * group management primitive */
#define INITIALISE       32900    /* Opcode to server waiting in getwork to
				   * initiate */
#define INT_INITIALISE   32905    
#define TERMONESRVR      32910    /* opcode for client's terminate (1 client
				   * link) message to server */
#define INT_TERMONESRVR  32915
#define TERMALLSRVRS     32920    /* opcode for client's terminate (all client
				   * links) message to server */
#define INT_TERMALLSRVRS 32925
#define JOINGRP          32930    /* join opcode for server in getwork */
#define INT_JOINGRP      32935
#define LEAVEGRP         32940    /* leave opcode for server in getwork */
#define INT_LEAVEGRP     32945

/******************************/
/* Return codes for protocols */
/******************************/

#define ACK_CLIENT    32745    /* Used by server, when an initiate retry is received */
#define DONE          32748    /* Return code from check_client to getwork() */
#define PROD_SERVER   32740    /* Used by server, when an object manager exists */
#define NO_SERVERS    32825    /* Used by client, when server list is empty */
#define CLEANUP       32950    /* Returned by removelink routine */
#define MNGRBOOT      32955    /* Returned by removelink routine */


/**********************************/
/* Signals used by rpc primitives */
/**********************************/

#define ORPHAN_TIMER     SIGALRM /* signal on which server logs itself as
				      a potential orphan */
#define ABORT      SIGALRM /* signal on which server resets its environment */
#define INTERRUPT_SERVER SIGUSR1 /* User generated signal to interrupt deaf server */
#define ORPHAN_SIGNAL    SIGUSR2 /* Signal passed to orphan server */
#define TERMINATE_SERVER SIGKILL /* User generated signal to kill (terminate) busy server */


/*******************/
/* Other constants */
/*******************/

#ifndef MANAGER_ADDR
#define MANAGER_ADDR 3980          /* Well known manager port number */
#endif

#define NOARG           0            /* Null arg used for buffer list */

#define MAXREJECT      10            /* Max. no. of rejected requests (getwork()) */

#define W_ARGS   (NSTATION * 2) + 6 /* Max. no. of arguments to activate worker */
#define ARGSTRING     26            /* Size of buffer to store an argument for starting up a worker */
/*#define ARGSTRING     16*/
/*****************/
/* c_list bounds */
/*****************/

#define NSTATION         20    /* Max. depth of a remote call */ 
#define MAXSERVERS       30    /* Max. No. of servers per node */
#define MAXCLIENTS       30    /* Max. no. of clients per server */

#define S_GROUPSIZE    50      /* Number of groups that a server can join */
  
/***************************************/
/* Constants used for group management */
/***************************************/

#define SUGRP_REMOVED         5
#define INVALID_GRP          -5
#define EMPTY_SUPERGRP       -10
#define SRVR_NOT_FOUND       -15
#define GRP_NOT_FOUND        -20
#define SUPERGRP_NOT_REMOVED -25
#define NOSRVRSTOTERM        -30  


/*************************************************************************/
/* Size and indices of array of groupids of actions maintained by server */
/*************************************************************************/
#define S_ACTGRPSIZE   2
#define S_CURRENTACT   0
#define S_PARENTACT    1


/********************/
/* Data  Structures */
/********************/


struct MESSAGES
{
    long          opcode;
    struct MID    server;
    struct MID    sender;
};
 
struct RPC_SN
{                             /* RPC header */
    long          sec;        /* seconds */
    long          msec;       /* microseconds */
    long          retry;      /* no. of retry */
    long          t_out;      /* time out value */
};

struct S_WORK
{
  struct MID     client_address;
  struct RPC_SN  seq_no;
  struct RPC     result;
};

/* Used for saving latest work received by server for each client */
typedef struct S_WORK SERVERSWORK[MAXCLIENTS];

struct PATH_EL
{                             /* RPC path element */
    long  node;               /* Client node number */
    long  c_count;            /* Crash count value at that node */
    int   s_indx[MAXSERVERS]; /* Index to server info array */
};

struct PATHS
{
  long node;
  long c_count;
};

struct FULL_PATH
{
    struct PATHS  rpcpath[NSTATION];
    struct RPC_SN timestamp; /* To detect retries with same timestamp */
    struct MID    groupid;
};    

struct SERVER_DESC
{
    long             process_id;
    Boolean        poflag;
    struct MID       myunicastid;
    struct MID       sender;
    struct MID       action_id;
    struct UNIQUEID  u_id;
    int              no_links;
    struct FULL_PATH linkspath[MAXCLIENTS];
};

struct C_LIST_EL
{                                             /* C_LIST file element */
    struct PATH_EL      client[NSTATION];     /* path element */
    struct SERVER_DESC  s_desc[MAXSERVERS];   /* Array of server info */
};

extern struct RPC_SN  h_lastrpcsn; /* Sequence number of last received call
				      for use from server routines */


/************************************************************************/
/* Structure for linked list of RPC structures - used by default filter */
/************************************************************************/

struct RPC_LIST
{
  struct RPC_LIST *next_rpc;
  struct RPC      info;
};

struct HEAD_RPCLIST
{
  struct RPC_LIST *head_rpc;
};

extern long getccount ();
extern int clistclose (struct C_LIST_EL *);
extern int clistopen (struct C_LIST_EL *);
extern void delete_clist ();
extern int dec_filtgrp (struct GROUP_INFO *, struct MID);
extern Boolean empty_grp (struct GROUP_DESC *);
extern int f_clistclose (int *, struct C_LIST_EL *);
extern int f_clistopen (int *, struct C_LIST_EL *);
extern Boolean find_retry (struct C_LIST_EL *, int, long, struct RPC_SN);
extern Boolean found_sharecrit (struct MID, struct UNIQUEID,
				  struct MID, struct UNIQUEID);
extern int get_filtserver (struct GROUP_INFO *, struct MID *, struct MID *);
extern int get_clientindx (struct C_LIST_EL *, long);
extern void get_groups (struct MID [S_ACTGRPSIZE], char *, long);
extern int get_noservers (struct GROUP_INFO *);
extern void getnullmid (struct MID *);
extern int getsn (struct RPC_SN *);
extern Boolean identical_sn (struct RPC_SN *, struct RPC_SN *);
extern int init_filtgrp (struct GROUP_INFO *, int *);
extern int insert_groups (struct GROUP_INFO *, struct MID, struct MID);
extern void insert_mergedgroup (struct ACTION_IDS *, struct GROUPS *);
extern void insert_newmsg (struct mc_sg_filter *, struct HEAD_RPCLIST *);
extern int insert_servers (struct GROUP_INFO *, struct MID,
			   struct MID, struct MID);
extern int insert_super_group (struct GROUP_INFO *, struct MID);
extern Boolean isempty_filtgrp (struct GROUP_INFO *);
extern Boolean identical_group (struct MID *, struct MID *);
extern Boolean isnull_id (struct MID *);
extern int match_clients (struct MID  *,struct MID  *);
extern void multicast_new_mid (struct MID *);
extern char *pack_long (long, char *_buff);
extern char *pack_HPID (struct MID, char *);
extern void printgid (struct MID);
extern int remove_group (struct GROUP_INFO *, struct GROUP_DESC *,
			 struct MID, Boolean, Boolean);
extern void remove_rpc_list (struct HEAD_RPCLIST *, Boolean);
extern int remove_server (struct GROUP_INFO *, struct GROUP_DESC *,
			  struct MID, Boolean, Boolean);
extern void reset_allclients (struct C_LIST_EL *);
extern void reset_allservers (struct C_LIST_EL *);
extern void reset_path (struct FULL_PATH *);
extern void reset_pathel (struct PATH_EL *);
extern void reset_server_info (struct SERVER_DESC *);
extern void RmClientFromPath (struct C_LIST_EL *, int, long);
extern void rm_group (struct ACTION_IDS *);
extern int rmall_supergrps (struct GROUP_INFO *);
extern void senderror (struct MID *, long); 
extern int sendok (struct MID *, long); 
extern void sendmyport (struct MID *, struct MID, struct MID);
extern void setalarm (unsigned long);
extern void set_grp_info (struct GROUPS *, struct MID);
extern void set_rpcinfo (struct RPC *, long, long, char *, long, char *);
extern void traverseall_supergrps (struct GROUP_INFO *);
extern void traverse_grp (struct ACTIONS *);
extern void traverse_supergrp (struct GROUP_DESC *);
extern char *unpack_long (long *, char *_buff);
extern char *unpack_HPID (struct MID *,char *);
extern Boolean unset_sharecrit (struct MID, struct UNIQUEID);
extern int vote_on_results (struct HEAD_RPCLIST *, int, int, struct RPC *);

#endif
