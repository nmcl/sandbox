/*
 * Copyright (C) 1993-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: rpcuser.h,v 1.15 1996/08/21 10:37:25 nmcl Exp $
 */

#ifndef RAJDOOT_RPCUSER_H_
#define RAJDOOT_RPCUSER_H_

/* include file for users of Rajdoot RPC Mechanism */

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef RAJDOOT_MULTICAST_H_
#  include <RPC/Rajdoot/multicast.h>
#endif

extern int _rpcerr;      /* Error diagnostic variable set by rpc mechanism */


#define PING_SOCKET     4000  /* Socket for ping daemon to listen on */
#define PING_WAIT       1000  /* Wait time for ping in milliseconds */
#define PING_RETRY        10  /* Number of retries for ping */
#define TERMINATE_CODE     0  /* To kill ping daemon */

/*******************************************/
/* Return codes - used in several routines */
/*******************************************/

#ifndef SUCCEEDED
#   define SUCCEEDED      0
#   define FAILED -1
#endif

/********************************/
/* Action ID for crash recovery */
/********************************/

#ifndef PRIORITYACTID
#define PRIORITYACTID 5555
#endif

/**************************************/
/* Remote Procedure Call return codes */
/**************************************/

#ifndef RPC_DONE
#   define RPC_DONE           100	/* RPC executed exactly once */
#endif
#define RPC_NOTDONE           101	/* RPC not executed  */
#define RPC_UNABLE            102	/* RPC perhaps not executed (ignore `result') */
#define FILTER_DETERMINED 103		/* Temporary - outcome determined by user filter */

/****************************************************/
/* Possible values of `_rpcerr' diagnostic variable */
/****************************************************/

#define TOO_FEW_SRVRS  503  /* Not enough servers in group to meet success criterion */
#define INVALID_GROUP  506  /* Group parameter supplied is not valid */
#define FEW_SUCCESSES  509  /* For multicast calls - not enough servers
			       completed call - main return code for multicast */
#define NO_RESPONSE    511  /* For unicast calls - if timeout expires with
			     * no response from servesr */
#define PATH_TOO_LONG  513  /* Error from `getworker()' in `_rpcerr' -
                               the rpc path of this process is too long */
#define C_LIST_FULL    516  /* Error from `getworker()' in `_rpcerr' -
                               the C_LIST of the remote node is full */
#define OPENFAILED     519  /* Unable to open the clist */
#define READFAILED     523  /* Cannot read clist */
#define WRITEFAILED    526  /* Failed to write to clist */
#define CLOSEFAILED    529  /* Error in attempting to close clist */
#define SYSTEMERROR    533  /* System error occurred */
#define LOGERROR       536  /* Server failed to log itself in clist */
#define EUNKNOWN      -999  /* An error was detected and `errno' was not set */

/*********************************************/
/* Other return codes for various primitives */
/*********************************************/

#define INVALID_OP     -113  /* Invalid opcode */
#define INVALID_HOST   -114  /* Invalid host id supplied */
#define TERMNOSRVRS    -115  /* No servers in group to terminate */
#define OLD_PATH       -116  /* Initiate request is from old incarnation of
				client */
#define NOSERVER       -200  /* Server specified in initiate is not present. */
#define SERVERACCESS   -201  /* Access permission problem on server for initiate. */


/************************************/
/* Extra return codes from getwork() */
/************************************/

#define TERMINATE      115  /* Client requested server termination, 
                             * abort all child server processes 
                             */  

#define DIE            116  /* Server initialisation fatal error, or server is
                             * an orphan, commit suicide, but no need to kill
                             * child processes
                             */
/******************************************/
/* Diagnostic return code from sendresult */
/******************************************/

#define WRONG_CLIENT  32747    /* Returned by sendresult, if trying to send to non-existant client */

/***************************************************************************/
/* Return values of Group management primitives - apart from OK and FAILED */
/***************************************************************************/

#define INVALID_ARG       104
#define EMPTY_OBJECT_LIST 117


/* size of buffer for object name */
#define OBJECT_NAME_SIZE   132 

/*******************************/
/* Operation codes for actions */
/*******************************/
#define NOOP_GRP          0
#define BEGIN_GRP         1
  /* used for endgroup() operation */
#define COMMIT_GRP        2  
#define ABORT_GRP         3


/*******************/
/* Types of groups */
/*******************/
#define UNSET_type         -1  /* assigned to type field of group that is not
                                  properly initiated */
#define SRVR_type          501  /* group is of 'server' type - used by rpc
				   mechanism and not by user */
#define GRP_type           503  /*group param is a 'group' type - uniform
				  param used to hide internal details of
				  differences which are only visible to the
				  implementor */
#define SETCOMMITGRP       505  /* Used if initiate is to return a new
				   commit identifier */
#define SRVR_dead          506  /* Used to show server which did not respond
                                   to initiate */

/********************/
/* Types of servers */
/********************/

/* UID_type:   unicast     - in multicast.h
 * MID_type:   multicast   - in multicast.h
 * UNSET_type: same as for 'groups'
 */   


/*************************/
/* Data type definitions */
/*************************/

typedef struct dir_list {
          char *dir_name;
} directory_path;

/****************************************/
/* Structures used for group management */
/****************************************/

struct GROUPIDS
{
  struct GROUPIDS *next_groupid;
  struct MID      g_id;
};

struct GROUP
{
  struct GROUPIDS *head_gids;
  int             no_grps;
};
  
struct SERVERIDS
{
  struct SERVERIDS *next_serverid;
  struct MID       s_id;
  struct MID       sender;   /* Temporary solution */
  Boolean        replied;
};

struct SERVERS
{
  struct SERVERIDS *head_sids;
  int              no_srs;
};

struct GROUP_DESC
{
  struct GROUP_DESC *next_group;
  struct MID        mymid;
  struct GROUP      grps;
  struct SERVERS    sids;
/*   struct REPLN_INFO strategy;  */
};
  
struct GROUP_INFO
{
  struct GROUP_DESC *head_gd;
  int    no_supergrps;
};

struct SRVR_INFO
{
  struct MID  sr_id;
  int         sr_idtype;
};

union GRPNSRVR
{
  struct GROUP_INFO group;
  struct SRVR_INFO  srvr;
};

struct GROUPS
{
  union GRPNSRVR grp_srvr;
  Int32           groups_type;
};

struct UNIQUEID
{
  Int32   host;
  Int32   sec;
  Int32   msec;
};

struct DEADLINE
{
  Boolean reset_server;
  unsigned timeout;
};

struct OBJECT_DESC
{
  Int32             destn_host;
  char             object_name[OBJECT_NAME_SIZE];
  struct UNIQUEID  u_id;
  struct GROUPS    grps_id;
};

struct RPC
{      /* RPC data type - defines service request and result */
    Int32  rpc_code;    /* Op. code in request, return code in result */
    Int32  rpc_length1; /* Length of `rpc_buff1' */

    char   *rpc_buff1;  /* Pointer to parameter list in request,
			  to result buffer in result */
    Int32  rpc_length2; /* Length of `rpc_buff2' */
    char   *rpc_buff2;  /* Pointer to parameter list in request,
			  to result buffer in result */
};

/* Structures and constants concerned with host mappings */

struct host_address
{
  Int32    log_host;
  char    *real_host;
};  
 
# define NO_HOST_MAP     3 /* ltor or rtol host mapping not found */

struct FILTER_INFO
{
#if defined(__STDC__) || defined(__cplusplus)
  int (*filt_name)(struct mc_sg_filter *, Boolean);
#else
  int (*filt_name)();
#endif
  struct GROUPS *expected_group; /* Needed for server's filter - the
				  * destination group is taken as the 
				  * expected_group for the client
				  */  
  int  no_successes;
  int  compare;
};

/***********************/
/* Opcodes for compare */
/***********************/
#define compare_OPCODE   1
#define compare_BUFFERS  2
#define compare_BOTH     3
#define compare_NONE     4

/***********************/

/*
 * External routines
 */

extern int initiate (struct OBJECT_DESC *, struct GROUPS *, int *);
extern int terminate (struct GROUPS *, Boolean, struct OBJECT_DESC *);
extern int multi_call (struct GROUPS *, struct RPC *, struct RPC *,
		       Uint32, Int32, struct FILTER_INFO *,
		       struct OBJECT_DESC *);
extern int getloghost (struct host_address *);
extern int getrealhost (struct host_address *);
extern int init_server (int, char *[]);
extern int getwork (struct GROUPS *, struct RPC *, struct DEADLINE *,
		    struct  FILTER_INFO*);
extern int sendresult (struct GROUPS *, struct RPC *, Boolean = FALSE);
extern void setBurstSize (Uint32);
extern void getBurstSize (Uint32&);
extern void setBurstTime (Uint32);
extern void getBurstTime (Uint32&);
extern void setActionID (struct MID);
extern Boolean ping (Int32, Uint32&, Boolean = FALSE);
extern Boolean ping (const char*, Uint32&, Boolean = FALSE);


#endif
