/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: multicast.cc,v 1.1 1993/11/03 14:41:51 nmcl Exp $
 */

/*
 *
 * Multicast primitive functions
 *
 * The following functions provide the ability to multicast and
 * unicast network messages. The technique employed to multicast is
 * very simple - a daemon (called mcrdaemon) is used to maintain the
 * members of the multicast set and distribute incoming messages to
 * the multicast set specified by a field in the message.
 *
 * Clients join a set by invoking the "join" operation passing a
 * multicast identifier as an argument. A corresponding function
 * called "leave" is provided to leave a multicast set.
 *
 * Messages are sent using the "multicast_send" function, with the
 * 'type' field of the multicast identifier passed as an argument
 * to this function defining whether the message should be multicast
 * or unicast. A unicast message is sent to the <host, port> pair
 * which is held in remaining fields of a unicast version of a
 * multicast identifier. Multicast messages are sent to the
 * multicast receive daemon by broadcasting the message on the network.
 *
 * Replies to a multicast message are directed to the client responsible
 * for the multicast using a unicast message (sent by the "multicast_send"
 * function). Unicast messages (replies) or multicast messages (passed
 * by the mcrdaemon) are received by a server using the "multicast_receive"
 * function.
 *
 */

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifdef _AIX
#  ifndef SYS_SELECT_H_
#    include <System/sys/select.h>
#  endif
#endif

#ifndef SYS_TIME_H_
#  include <System/sys/time.h>
#endif

#ifndef ARPA_INET_H_
#  include <System/arpa/inet.h>
#endif

#ifndef FCNTL_H_
#  include <System/fcntl.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef RAJ_COMMON_H_
#  include "common.h"
#endif

#ifndef _SC_OPEN_MAX
#define _SC_OPEN_MAX 5
#endif

#ifdef SERVER
#  include <System/syslog.h>
#endif

#ifndef RAJDOOT_MULTICAST_H_
#  include <RPC/Rajdoot/multicast.h>
#endif

static const char RCSid[] = "$Id: multicast.cc,v 1.1 1993/11/03 14:41:51 nmcl Exp $";


int mcrdaemon_ipc = -1;	/* File descriptor for the socket to the mcrdaemon */
int client_ipc = -1;	/* File descriptor for the socket to the client */
struct hostent *hp = 0; /* host entry for the local host */

long fieldNumber = 0;

struct sockaddr_in mcrdaemon_addr; /* internet address of mcrdaemon */
struct sockaddr_in network_addr;   /* internet address of broadcast network */

struct multicast_header mc_hdr;

/* Function to open a socket and initialise the internet address
   passed as an argument, setting the port number of the internet
   address to the final argument. The result is the file descriptor
   of the socket, any sort of failure returns a -1 (MC_FAILED). */

int initialise_inet_ipc (struct sockaddr_in *address, unsigned short portno)
{
    int ipc;
    char localhost[MAXHOSTNAMELEN+1];

    ::memset(localhost, '\0', MAXHOSTNAMELEN+1);

    if ((ipc = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
#ifdef SERVER
	syslog(LOG_ERR|LOG_LOCAL7, "initialise_inet_ipc socket call: %m");
#else
	perror("initialise_inet_ipc socket call ");
#endif
	return(MC_FAILED);
    }

    if (hp == 0 && (gethostname(localhost, sizeof(localhost)) == -1 ||
		    (hp = gethostbyname(localhost)) == 0))
    {
#ifdef SERVER
	syslog(LOG_ERR|LOG_LOCAL7,
	       "initialise_inet_ipc gethostname or gethostbyname: %m");
#else
	perror("initialise_inet_ipc gethostname or gethostbyname ");
#endif
	return(MC_FAILED);
    }

    (void) memset((caddr_t) address, '\0', sizeof(struct sockaddr_in));
    (void) memcpy((caddr_t) &(address->sin_addr), hp->h_addr, hp->h_length);
    address->sin_port = htons(portno);
    address->sin_family = hp->h_addrtype;
    return(ipc);
}

/* Function to initialise a internet address to that needed to
   send and receive broadcasts. Returns MC_FAILED if the getnetbyname
   call fails. */

int initialise_broadcast_addr (struct sockaddr_in *broadcast_address)
{
    struct netent *net;

    if ((net = getnetbyname(BROADCAST_NET)) == 0)
    {
#ifdef SERVER
	syslog(LOG_ERR|LOG_LOCAL7,"initialise_broadcast_addr getnetbyname; %m");
#else
	perror("initialise_broadcast_addr getnetbyname ");
#endif
	return(MC_FAILED);
    }

    (void) memset((caddr_t) broadcast_address, '\0', sizeof(struct sockaddr_in));
    broadcast_address->sin_family = net->n_addrtype;
    broadcast_address->sin_addr = inet_makeaddr((int) net->n_net, (int) INADDR_ANY);
    broadcast_address->sin_port = htons(NET_PORT);

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "broadcast internet address" << flush;
    debug_stream << " " << ((broadcast_address->sin_addr.s_addr>>24)&0x000000ff) << "."
	<< ((broadcast_address->sin_addr.s_addr>>16)&0x000000ff) << "."
	    << ((broadcast_address->sin_addr.s_addr>>8)&0x000000ff) << "."
		<< ((broadcast_address->sin_addr.s_addr&0x000000ff)) << flush;
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "\n" << flush;
#endif

    return(MC_OK);
}

/* Function to create an ipc channel to the mcrdaemon. The channel is
   a socket, and the address required to communicate with the mcrdaemon
   is held in the global variable mcrdaemon_addr. If any of the system
   calls fail then MC_FAILED is returned. */

int initialise_mcrdaemon_ipc ()
{
    if (mcrdaemon_ipc == -1)
    {
	/* set up the internet address for the mcrdaemon */

	return((mcrdaemon_ipc = 
		initialise_inet_ipc(&mcrdaemon_addr, MCR_DAEMON)));
    }
    return(MC_OK);
}

/* Function to create and ipc channel for the client. The channel is
   a socket, and the socket is bound to an address on the local host.
   In addition, the internet address required to broadcast is initialised
   in the global varibale network_addr. Any form of failure results in
   a MC_FAILED return code. */

int initialise_client_ipc ()
{
    struct sockaddr_in uni_name;
    int uni_len;

    if (client_ipc == -1)
    {
	/* get details about the local host */

	if ((client_ipc = initialise_inet_ipc(&network_addr, 0)) == MC_FAILED)
	{
#ifdef SERVER
	    syslog(LOG_ERR|LOG_LOCAL7, "initialise_inet_ipc call");
#else
	    perror("initialise_inet_ipc call ");
#endif
	    return(MC_FAILED);
	}

	/* try and bind to this network address */

	if (bind(client_ipc, (struct sockaddr *)&network_addr, sizeof(struct sockaddr_in)) == -1)
	{
#ifdef SERVER
	    syslog(LOG_ERR|LOG_LOCAL7, "initialise_client_ipc bind call: %m");
#else
	    perror("initialise_client_ipc bind call ");
#endif
	    return (MC_FAILED);
	}

	/* now set up the broadcast address in network_addr */

	if (RAJDOOT_USE_BROADCAST)
	{
	    if (initialise_broadcast_addr(&network_addr) == MC_FAILED)
		return(MC_FAILED);
	}

	/* and get the port number of the bound socket */

	uni_len = sizeof(struct sockaddr_in);
	getsockname(client_ipc, (struct sockaddr *)&uni_name, &uni_len);

	/* set up the header used by all multicast message */

	mc_hdr.source.type = htonl(UID_type);
	mc_hdr.source.hi_part = (unsigned long) uni_name.sin_addr.s_addr;
	mc_hdr.source.lo_part = htonl(ntohs(uni_name.sin_port));

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "Socket " << client_ipc << " port number " << ntohs(uni_name.sin_port) << "\n" << flush;
#endif
    }

    return (MC_OK);
}

/* The following function disassociates a server from the controlling
   terminal of its invoker. */

void multicast_terminal_closedown (char *logfile)
{
#ifndef DEBUG
    int i;

    for (i = sysconf(_SC_OPEN_MAX) - 1; i >= 0; --i)
	close(i);

    open("/dev/null", O_RDONLY);
    dup2(0, 1);
    dup2(0, 2);

    i = open("/dev/tty", O_RDWR);
    if (i >= 0)
    {
	tcsetpgrp(i, 0);  /* use instead of ioctl(i, TIOCNOTTY, 0); */
	close(i);
    }
#endif

#ifdef SERVER
    openlog(logfile, LOG_PID, LOG_USER);
    syslog(LOG_ERR|LOG_LOCAL7,
	   "multicast_terminal_closedown openlog %s: %m", logfile);
#endif
}

/* The following primitive attempts to join the multicast set
   identified by the argument. If the type of the multicast id is
   anything other then NOT_JOINED (or JOINED from a previous call)
   then the primtive returns ECANTJOIN. */

int multicast_join (struct MID *midptr)
{
    int ret;
    char ack;
    int fromlen = sizeof(struct sockaddr_in);

    if (RAJDOOT_USE_BROADCAST)
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "Within Multicast Join.\n" << flush;
#endif
	switch (ntohl(midptr->type))
	{
	case NOT_JOINED:
	    if (initialise_mcrdaemon_ipc() == MC_FAILED)
		return(ECANTJOIN);
	    
	    midptr->type = htonl(JOIN_type);
	    
	    if (sendto(mcrdaemon_ipc, (char *)midptr, sizeof(struct MID), 0,
#if !defined(__GNUG__) || defined(SYSV) || defined(__linux__)
		       (struct sockaddr *)&mcrdaemon_addr,
#else
		       (void *)&mcrdaemon_addr,
#endif
		       sizeof(struct sockaddr_in)) != sizeof(struct MID))
	    {
#ifdef DEBUG
		debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
		debug_stream << "Multicast Join failed.\n" << flush;		
#endif
	
#ifdef SERVER
		syslog(LOG_ERR|LOG_LOCAL7, "multicast_join sendto: %m");
#else
		perror("multicast_join sendto ");
#endif
		return(ECANTJOIN);
	    }

	    ret = recvfrom(mcrdaemon_ipc, &ack, sizeof(ack), 0, (struct sockaddr *)&mcrdaemon_addr, &fromlen);

	    midptr->type = htonl(JOINED);
	    
	    return (MC_OK);
	    
	case JOINED:
	    return (MC_OK);
	    
	default:
	    return (ECANTJOIN);
	}
    }
    else
	return (MC_OK);
}

/* The following primitive attempts to leave the multicast set
   identified by the argument.  If the type of the multicast id is
   anything other then JOINED then a error code is returned. */

int multicast_leave (struct MID *midptr)
{
    switch (ntohl(midptr->type))
    {
	case JOINED:
	    if (initialise_mcrdaemon_ipc() == MC_FAILED)
		return(ECANTLEAVE);

	    midptr->type = htonl(LEAVE_type);

	    if (sendto(mcrdaemon_ipc, (char *)midptr, sizeof(struct MID), 0,
#if !defined(__GNUG__) || defined(SYSV) || defined(__linux__)
		       (struct sockaddr *)&mcrdaemon_addr,
#else
		       (char *)&mcrdaemon_addr,
#endif
		       sizeof(struct sockaddr_in)) != sizeof(struct sockaddr_in))
		return(ECANTLEAVE);

	    midptr->type = htonl(NOT_JOINED);
	    return (MC_OK);

	case NOT_JOINED:
	    return (ENOTHINGTOLEAVE);

	default:
	    return (ECANTLEAVE);
    }
}

/* The following primitive implements the multicast send function.
   The message passed as an argument is sent to the multicast 
   group identified by the MID. If the type field of the MID is
   JOINED or NOT_JOINED then this is a valid multicast in so that
   the message is broadcast on the network. If the type field is
   UID_type then this is a unicast id and the remaining two fields
   are the host and port ids of the receipient. */

int multicast_send (struct MID *midptr, struct mc_msghdr *msg)
{
    struct sockaddr_in hostport;
    struct msghdr buffer;

    if (initialise_client_ipc() == MC_FAILED)
	return(EMCASTSENDFAILED);

    buffer.msg_accrights = (caddr_t) 0;
    buffer.msg_accrightslen = 0;
    buffer.msg_namelen = sizeof(struct sockaddr_in);
    buffer.msg_iovlen = (int) msg->msg_len;

    switch (ntohl(midptr->type))
    {
	case JOINED:
	case NOT_JOINED:
	    mc_hdr.mid.hi_part = midptr->hi_part;
	    mc_hdr.mid.lo_part = midptr->lo_part;
	    msg->mc_address.iov_base = (caddr_t) &mc_hdr;
	    msg->mc_address.iov_len = sizeof(struct multicast_header);

	    buffer.msg_name = (caddr_t) &network_addr;
	    buffer.msg_iov = (struct iovec *) msg;
	    buffer.msg_iovlen++;
	    break;
	case UID_type:
	    (void) memset((caddr_t) &hostport, '\0', sizeof(struct sockaddr_in));
	    hostport.sin_addr.s_addr = midptr->hi_part;
	    hostport.sin_port = htons(ntohl(midptr->lo_part));
	    hostport.sin_family = AF_INET;

	    buffer.msg_name = (caddr_t) &hostport;
	    buffer.msg_iov = (struct iovec *) &msg->mc_msg[0];
	    break;
	default:		/* Cannot send to anything other than
				 * UID_type, JOINED or NOT_JOINED */
	    return (ECANTSEND);
    }

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "multicast_send to address " << flush;
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << " " << ((((struct sockaddr_in *)buffer.msg_name)->sin_addr.s_addr>>24)&0x000000ff) << "."
	<< ((((struct sockaddr_in *)buffer.msg_name)->sin_addr.s_addr>>16)&0x000000ff) << "."
	    << ((((struct sockaddr_in *)buffer.msg_name)->sin_addr.s_addr>>8)&0x000000ff) << "."
		<< ((((struct sockaddr_in *)buffer.msg_name)->sin_addr.s_addr&0x000000ff)) << flush;
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "\n" << flush;
#endif

    if (sendmsg(client_ipc, &buffer, 0) < 0)
    {
#ifdef SERVER
	syslog(LOG_ERR|LOG_LOCAL7, "multicast_send sendmsg: %m");
#else
	perror("multicast_send sendmsg ");
#endif

	if (ntohl(midptr->type) == UID_type)
	    return (EUCASTSENDFAILED);
	else
	    return (EMCASTSENDFAILED);
    }

    return (MC_OK);
}

/* multicast_receive waits for one or more (defined by the filter
   set by the user) unicast or multicast messages to be received.
   Unicast messages come from the client_ipc socket and are likely
   to be replies to a multicast message made by the client.
   Multicast messages come from the multicast receive daemon.
   The function either waits until a message arrives or the
   timeout passed as an argument is met. If no timeout value
   is passed, the function blocks waiting for a message.

   If a filter is passed as one of the arguments, multicast_receive
   loops while the filter returns a zero value. Any value other than
   zero is returned as the result of the function.
   If no filter is passed, then only a single message is collected, and
   a zero value returned. */

int multicast_receive (struct MID *midptr, struct mc_sg_filter *msg_ptr, int (*filter) (struct mc_sg_filter *, Boolean), long toval)
{
    register int filter_result = 0;
    struct timeval timeptr;
    fd_set watch;
    struct sockaddr_in from;
    struct msghdr buffer;
    int bytes_recv;

    if (midptr->type == htonl(NOT_JOINED))
	return (ENOTJOINED);

    if (toval > 0)  /* set up the timeval structure */
    {
	timeptr.tv_sec = toval / 1000000;
	timeptr.tv_usec = toval % 1000000;
    }

    FD_ZERO(&watch);

    for(;;)
    {
	if (mcrdaemon_ipc != -1)
	    FD_SET(mcrdaemon_ipc, &watch);
	FD_SET(client_ipc, &watch);

#ifdef hpux
	if (select(FD_SETSIZE, (int*) &watch, 0, 0, ((toval <= 0)?0:&timeptr)) == 0)
#else
	if (select(FD_SETSIZE, &watch, 0, 0, ((toval <= 0)?0:&timeptr)) == 0)
#endif
	{   
	    /* select timed out */

	    if ((int) filter != 0)
		return((*filter) (msg_ptr, TRUE));
	    else
		return (ENOMSGRECEIVED);
	}

	if (msg_ptr->mc_msg == (struct mc_msghdr *) 0)
	    return (ENOSGARRAY);

	buffer.msg_name = (caddr_t) &from;
	buffer.msg_namelen = sizeof(struct sockaddr_in);
	buffer.msg_accrights = (caddr_t) 0;
	buffer.msg_iovlen = (int) msg_ptr->mc_msg->msg_len;

	if (FD_ISSET(client_ipc, &watch))  /* Standard socket receive */
	{
	    buffer.msg_iov = (struct iovec *) &msg_ptr->mc_msg->mc_msg[0];

	    if ((bytes_recv = recvmsg(client_ipc, &buffer, 0)) < 0)
		return (EUCASTRECVFAILED);

	    msg_ptr->source.type = htonl(UID_type);
	    msg_ptr->source.hi_part = (unsigned long) from.sin_addr.s_addr;
	    msg_ptr->source.lo_part = htonl(ntohs(from.sin_port));
	}
	else /* must be FD_ISSET(mcrdaemon_ipc, &watch) */
	{    /* so a multicast message sent by mcrdaemon is available */

	    msg_ptr->mc_msg->mc_address.iov_base = (caddr_t) &msg_ptr->source;
	    msg_ptr->mc_msg->mc_address.iov_len = sizeof(struct MID);
	    buffer.msg_iov = (struct iovec *) &msg_ptr->mc_msg->mc_address;
	    buffer.msg_iovlen++;

	    if ((bytes_recv = recvmsg(mcrdaemon_ipc, &buffer, 0)) < 0)
		return (EMCASTRECVFAILED);
	}

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "received " << bytes_recv << " bytes from " <<
	    ((FD_ISSET(client_ipc, &watch))?"client_ipc":"mcrdaemon_ipc")
		<< ", address " << flush;
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << " " << ((from.sin_addr.s_addr>>24)&0x000000ff) << "."
	    << ((from.sin_addr.s_addr>>16)&0x000000ff) << "."
		<< ((from.sin_addr.s_addr>>8)&0x000000ff) << "."
		    << ((from.sin_addr.s_addr&0x000000ff)) << flush;
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << ", port " << ntohs(from.sin_port) << "\n" << flush;
#endif

	if ((int) filter != 0)
	{
	    if ((filter_result = (*filter)(msg_ptr, FALSE)) != 0)
		break;
	}
	else
	    break;
    }

    return (filter_result);
}

/* The following function creates a new multicast indentifier
   which is assigned to the MID parameter. */

void multicast_new_mid (struct MID *newmidptr)
{
    time_t currentTime = time(0) + fieldNumber;
    fieldNumber++;

    newmidptr->type = htonl(NOT_JOINED);
    newmidptr->hi_part = htonl((get_machine_id() << 16) | (getpid() & 0xffff));
    newmidptr->lo_part = htonl(currentTime); /* time in microseconds */
}

/* The following function assigns the internet address of the
   ipc channel belonging to the client to the MID structure
   passed as an argument */

void multicast_get_mid (struct MID *myaddress)
{
    if (initialise_client_ipc() == MC_FAILED)
#ifdef SERVER
	syslog(LOG_ERR|LOG_LOCAL7,
	       "initialise_client_ipc multicast_get_mid call");
#else
	perror("initialise_client_ipc multicast_get_mid call ");
#endif
    else
    {
	/* initialise_client_ipc fills the global mc_hdr with the
	   client's address so retrieve it from there... */

	myaddress->type = htonl(UID_type);
	myaddress->hi_part = mc_hdr.source.hi_part;
	myaddress->lo_part = mc_hdr.source.lo_part;
    }
}
