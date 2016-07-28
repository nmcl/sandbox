/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: shutdown_ping.cc,v 1.3 1994/11/03 15:06:25 ngdp Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef  SYS_TIME_H_
#  include <System/sys/time.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifdef SYSV

#ifndef SYS_TIMES_H_
#  include <System/sys/times.h>
#endif

#ifndef LIMITS_H_
#  include <System/limits.h>
#endif

#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef SYS_UIO_H_
#  include <System/sys/uio.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef SYS_SOCKET_H_
#  include <System/sys/socket.h>
#endif

#ifndef SYS_SELECT_H_
#  include <System/sys/select.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef ARPA_INET_H_
#  include <System/arpa/inet.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef RPC_H_
#  include "rpc.h"
#endif

#ifndef RAJDOOT_RPCUSER_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif


int main (int argc, char** argv)
{
#ifdef SYSV
    clock_t sendTime = 0, receiveTime = 0;
#else    
    struct timeval sendTime, receiveTime;
#endif
    static char* hostName = (char*) 0;
    struct sockaddr_in sockAddr;
    struct hostent* hp = 0;
    int retry = PING_RETRY;   
    int sock = socket(AF_INET, SOCK_DGRAM, 0);

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: shutdown_ping [-host] [-help]" << endl;
	    return 0;
	}
	if (::strcmp(argv[i], "-host") == 0)
	    hostName = argv[i+1];
    }
    

    if (hostName == (char*) 0)
    {
	hostName = ::new char[MAXHOSTNAMELEN+1];
	::memset(hostName, '\0', MAXHOSTNAMELEN+1);
	::gethostname(hostName, MAXHOSTNAMELEN);
    }

    if (sock >= 0)
    {
	hp = gethostbyname(hostName);

	if (hp)
	{
	    Boolean ignoreSend = FALSE; // if we receive an "old" message then we will not resend to avoid flooding
#ifdef SYSV
	    sendTime = TERMINATE_CODE;
#else    
	    sendTime.tv_sec = sendTime.tv_usec = TERMINATE_CODE;
#endif
	    ::memset((void *) &sockAddr, '\0', sizeof(struct sockaddr_in));
	    ::memcpy((char *) &sockAddr.sin_addr, (char *) hp->h_addr, hp->h_length);
	    sockAddr.sin_family = AF_INET;
	    sockAddr.sin_port = htons(PING_SOCKET);

	    do
	    {
		if ((sendto(sock, (char*) &sendTime, sizeof(sendTime), 0,
			   (struct sockaddr*) &sockAddr, sizeof(struct sockaddr_in)) == sizeof(sendTime)) ||
		    (ignoreSend))
		{
		    struct timeval waitTime;
		    fd_set inmask;
		    int res;
		    
		    waitTime.tv_sec = (int) (PING_WAIT/1000);
		    waitTime.tv_usec = (PING_WAIT - waitTime.tv_sec*1000)*1000;
		
		    FD_ZERO(&inmask);
		    FD_SET(sock, &inmask);
#ifdef hpux
		    res = select(FD_SETSIZE, (int *) &inmask, 0, 0, &waitTime);
#else
		    res = select(FD_SETSIZE, &inmask, 0, 0, &waitTime);
#endif
		    ignoreSend = FALSE;
		    
		    if (res > 0)
		    {
			struct sockaddr_in from;
			int length = 0;

			if (recvfrom(sock, (char*) &receiveTime, sizeof(receiveTime), 0, (struct sockaddr*) &from, &length) >= 0)
			{
#ifdef SYSV
			    if (receiveTime == sendTime)
			    {
				cout << "Received acknowledgement from pingdaemon." << endl;
				return 0;
			    }
#else
			    if ((receiveTime.tv_sec == sendTime.tv_sec) && (receiveTime.tv_usec == sendTime.tv_usec))
			    {
				cout << "Received acknowledgement from pingdaemon." << endl;
				return 0;
			    }
#endif				
			    retry++;
			    ignoreSend = TRUE;
			}
			else
			    error_stream << WARNING << "shutdown_ping: receive problem. Errno " << errno << endl;
		    }
		}
		else
		    error_stream << WARNING << "shutdown_ping: send problem. Errno " << errno << endl;
	    } while (retry-- > 0);

	    cout << "Did not receive acknowledgment from pingdaemon." << endl;
	}
	else
	    error_stream << WARNING << "shutdown_ping: could not get host information for " << hostName << ". Errno " << errno << endl;
    }
    else
	error_stream << WARNING << "shutdown_ping: could not get socket. Errno " << errno << endl;

    return -1;
}
