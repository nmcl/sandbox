/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: multicast.h,v 1.11 1993/03/18 14:33:44 nmcl Exp $
 */

#ifndef RAJDOOT_MULTICAST_H_
#define RAJDOOT_MULTICAST_H_

/* 
 * 
 * Multicast constant and struture definitions
 *
 */

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SYS_UIO_H_
#  include <System/sys/uio.h>
#endif

#ifndef SYS_SOCKET_H_
#  include <System/sys/socket.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifdef DEBUG
#define NETADDR(strm,addr) fprintf(strm,"%u.%u.%u.%u", \
				   ((addr>>24)&0x000000ff), \
				   ((addr>>16)&0x000000ff), \
				   ((addr>>8)&0x000000ff), \
				   (addr&0x000000ff))
#endif

#ifndef BROADCAST_NET
#   define BROADCAST_NET "ncl"     /* local broadcast network - NB there must
				      be an entry for this network in the
				      /etc/networks file
				   */
#endif

#define MCX_DAEMON 9994         /* port number of multicast transmit daemon */
#define MCR_DAEMON 9995         /* port number of multicast receive daemon */
#define NET_PORT   9996         /* port number of network socket */

/* multicast return codes */

#define MC_OK               0
#define MC_FAILED          -1
#define ENOTHINGTOLEAVE    -2
#define ECANTLEAVE         -3
#define EIDNOTFOUND        -4
#define ECANTOPENSOCKET    -5
#define EMCASTSENDFAILED   -6
#define EUCASTSENDFAILED   -7
#define EWRONGSIZE         -8
#define ECANTSEND          -9
#define ENOMSGRECEIVED     -10
#define EUCASTRECVFAILED   -11
#define EMCASTRECVFAILED   -12
#define ENOTJOINED         -13
#define ECANTCREATE        -14
#define ENULLNAME          -15
#define ECANTREMOVE        -16
#define EDONTKNOWSELF      -17
#define ENOSGARRAY         -18
#define ECANTJOIN          -19
#define EPARTIALSEND       -25

/* struct MID is the multicast identifier used to name multicast groups when
   the type field is other than UID_type. When it is UID_type then the
   hi_part field is the host id, and the lo_part is the port number, of a
   server. */

struct MID
{
    long type;
    long hi_part;
    long lo_part;
};

/* the values the field type in the above structure can have are as follows */

#define UID_type    0
#define MID_type    1
#define JOINED      2
#define NOT_JOINED  3
#define JOIN_type   4
#define LEAVE_type  5

/* the header sent when a message is broadcast on the net */

struct multicast_header
{
    struct MID mid;
    struct MID source;
};

#define MSG_SIZE 1448 /* maximum no. of bytes that can be sent or received */

/* the data broadcast consists of the following structure */

struct multicast_msg
{
    struct multicast_header hdr;
    char message[MSG_SIZE];
};

/* mc_msghdr is the structure used by the multicast_send implmentation
   when calling the scatter/gather sendmsg system call. A global variable
   of this type is used, and initalised during the first multicast_send. */

struct mc_msghdr
{
    struct iovec  mc_address;
    struct iovec  mc_msg[MSG_MAXIOVLEN-1];
    unsigned long msg_len;
};

struct mc_sg_filter
{
    struct mc_sg_filter *next_fs;
    struct MID source;
    struct mc_msghdr *mc_msg;
};

extern int client_ipc;
extern struct sockaddr_in network_addr;

extern int initialise_broadcast_addr (struct sockaddr_in *);
extern int initialise_inet_ipc (struct sockaddr_in *, unsigned short);
extern void multicast_get_mid (struct MID *);
extern int multicast_join (struct MID *);
extern int multicast_leave (struct MID *);
extern int multicast_receive (struct MID *, struct mc_sg_filter *,
			      int (*filter)(struct mc_sg_filter *, Boolean),
			      long);
extern int multicast_send (struct MID *, struct mc_msghdr *);
extern void multicast_terminal_closedown (char *);

#endif
