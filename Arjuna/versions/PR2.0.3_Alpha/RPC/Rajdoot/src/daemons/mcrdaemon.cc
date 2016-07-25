/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: mcrdaemon.cc,v 1.17 1993/07/06 13:23:17 nmcl Exp $
 */

/*
 *
 * Multicast receive daemon
 *
 */

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SYS_TIME_H_
#  include <System/sys/time.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef SYSLOG_H_
#  include <System/syslog.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifdef _AIX
#  ifndef SYS_SELECT_H_
#    include <System/sys/select.h>
#  endif
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef RAJDOOT_MULTICAST_H_
#  include <RPC/Rajdoot/multicast.h>
#endif

#define LOCALHOST 0
#define BROADCASTHOST 1

static const char RCSid[] = "$Id: mcrdaemon.cc,v 1.17 1993/07/06 13:23:17 nmcl Exp $";

struct list_el
{
    struct MID mid;
    struct sockaddr_in who;
    struct list_el *next_el;
};

struct list_el *start;

int proc_sock;
int net_sock;
extern int errno;

/* socket_open calls initialise_inet_ipc to open a socket and
   initialise the internet address structure passed as an
   argument, if the host type is BROADCASTHOST then a call is
   made to initialise_broadcast_addr to change the address to the
   broadcast address. The function then binds the socket to the
   requested port number and returns the descriptor for the
   socket. */

int socket_open (int host, int port)
{
    int new_socket = -1;
    struct sockaddr_in new_address;

    if ((new_socket = initialise_inet_ipc(&new_address, port)) == -1)
    {
	syslog(LOG_ERR|LOG_LOCAL7, "socket_open initialise_inet_ipc", 0);
	return(-1);
    }

    if (host == BROADCASTHOST && initialise_broadcast_addr(&new_address) == -1)
    {
	syslog(LOG_ERR|LOG_LOCAL7, "socket_open initialise_broadcast_addr", 0);
	return(-1);
    }

#ifdef DEBUG
    fprintf(stdout, "socket %d, internet address ", new_socket);
    NETADDR(stdout, new_address.sin_addr.s_addr);
    fprintf(stdout, ", port %d\n", port);
#endif

    if (bind(new_socket, (struct sockaddr*)&new_address, sizeof(struct sockaddr_in)) == -1)
    {
	syslog(LOG_ERR|LOG_LOCAL7, "socket_open bind: %m", 0);
	return(-1);
    }

    return(new_socket);
}

/* The following function reads the incoming message and sends it to 
   the clients that have joined the multicast set defined by the
   multicast id of the message. */

void from_network ()
{
    struct multicast_msg data;
    struct sockaddr_in from;
    int i;
    register struct list_el *loop;
    register long mid_hi, mid_lo;
    int fromlen = sizeof(struct sockaddr_in);

    /* just read the multicast_header of the message to determine the
       multicast identifier and source of the incoming message */

    if ((i = recvfrom(net_sock, (char *)&data,
		      sizeof(struct multicast_msg), 0, (struct sockaddr *)&from, &fromlen)) > 0)
    {
	i -= sizeof(struct MID);
	mid_hi = data.hdr.mid.hi_part;
	mid_lo = data.hdr.mid.lo_part;

#ifdef DEBUG
	fprintf(stdout, "%d bytes received from host ", i);
	NETADDR(stdout, data.hdr.source.hi_part);
	fprintf(stdout, ", port %d, type %d\n",
		data.hdr.source.lo_part, data.hdr.source.type);
#endif

	/* scan for the multicast identifier in the list of ids */

	for (loop = start; loop != (struct list_el *) 0; loop = loop->next_el)
	{
	    if ((loop->mid.hi_part == mid_hi) && (loop->mid.lo_part == mid_lo))
	    {
		/* a match - so sent the message to this client */

		if (sendto(proc_sock, (char *)&data.hdr.source, i, 0,
#if !defined(__GNUG__) || defined(SYSV) || defined(__linux__)
			   (struct sockaddr*)&loop->who,
#else
			   (char*)&loop->who,
#endif
			   sizeof(struct sockaddr_in)) != i)
		    syslog(LOG_ERR|LOG_LOCAL7, "mcrdaemon sento call to client: %m", 0);
	    }
	}
    }
    else
	syslog(LOG_ERR|LOG_LOCAL7, "from_network recvfrom: %m", 0);
}

void from_process ()
{
    register struct list_el *previous, *loop, *new_el;
    struct MID mid_no;
    struct sockaddr_in from;
    int fromlen = sizeof(struct sockaddr_in);
    int ret;
    char ack = 'Z';

    if (recvfrom(proc_sock, (char *)&mid_no, sizeof(struct MID), 0, (struct sockaddr *)&from, &fromlen)
	!= sizeof(struct MID))
    {
	syslog(LOG_ERR|LOG_LOCAL7, "from_process recvfrom: %m", 0);
	return;
    }

#ifdef DEBUG
    fprintf(stdout,"host "),
    NETADDR(stdout,from.sin_addr.s_addr);
    fprintf(stdout,", port %d, mid <%d,%d,%d>, start %d\n",
	    ntohs(from.sin_port), mid_no.type, mid_no.hi_part, mid_no.lo_part, (int) start);
#endif

    switch (ntohl(mid_no.type))
    {
	case JOIN_type:
	    new_el = new list_el;
	    new_el->mid.hi_part = mid_no.hi_part;
	    new_el->mid.lo_part = mid_no.lo_part;
	    memcpy((caddr_t) &new_el->who, (caddr_t) &from, sizeof(struct sockaddr_in));
	    new_el->next_el = start;
	    start = new_el;

	    ret = sendto(proc_sock, (char *)&ack, sizeof(ack), 0,
#if  !defined(__GNUG__) || defined(SYSV) || defined(__linux__)
			 (struct sockaddr*)&from,
#else
			 (char*)&from,
#endif
			 fromlen);

	    break;

	case LEAVE_type:
	    previous = (struct list_el *) 0;
	    loop = start;

	    while (loop != (struct list_el *) 0)
	    {
		if ((loop->who.sin_port == from.sin_port) &&
		    (loop->mid.hi_part == mid_no.hi_part) &&
		    (loop->mid.lo_part == mid_no.lo_part))
		{
		    if (previous == (struct list_el *) 0)
			start = loop->next_el;
		    else
			previous->next_el = loop->next_el;
		    delete loop;
		    loop = (struct list_el *) 0;
		}
		else
		{
		    previous = loop;
		    loop = loop->next_el;
		}
	    }
	    break;

	default:
	    syslog(LOG_ERR|LOG_LOCAL7, "from_process failure: %u",(int) ntohl(mid_no.type));
	    close(proc_sock);
	    proc_sock = socket_open(LOCALHOST, MCR_DAEMON);
    }
}

int main ()
{
    FILE            *strptra;
    FILE            *strptrb;

    fd_set inmask;

    if (chdir("/tmp"))
	perror("mcrdaemon failed to chdir to /tmp\n");

    if ((strptra = freopen("mcrdaemon_out", "a+", stdout)) != NULL)
        setbuf(strptra, NULL);
    else
        printf("mcrdaemon: Cannot open file mcrdaemon_out\n");

    if ((strptrb = freopen("mcrdaemon_err", "a+", stderr)) != NULL)
        setbuf(strptrb, NULL);
    else
        printf("mcrdaemon: Cannot open file mcrdaemon_err\n");

    multicast_terminal_closedown("mcrdaemon");

    net_sock = socket_open(BROADCASTHOST, NET_PORT);
    proc_sock = socket_open(LOCALHOST, MCR_DAEMON);

    if (net_sock != -1 && proc_sock != -1)
    {
	start = (struct list_el *) 0;

	syslog(LOG_INFO|LOG_LOCAL7, "mcrdaemon started", 0);

	for (;;)
	{
	    FD_ZERO(&inmask);
	    FD_SET(net_sock, &inmask);
	    FD_SET(proc_sock, &inmask);

#ifdef hpux
	    if (select(FD_SETSIZE, (int*) &inmask, 0, 0, 0) == -1)
#else
	      if (select(FD_SETSIZE, &inmask, 0, 0, 0) == -1)
#endif
	    {
		syslog(LOG_ERR|LOG_LOCAL7, "mcrdaemon select: %m", 0);
		closelog();
		exit(0);
	    }

	    if (FD_ISSET(net_sock, &inmask))
		from_network();

	    if (FD_ISSET(proc_sock, &inmask))
		from_process();
	}
    }
    else
	syslog(LOG_ERR|LOG_LOCAL7, "mcrdaemon failed to start", 0);
}
