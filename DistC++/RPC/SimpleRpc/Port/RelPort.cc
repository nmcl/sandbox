/*
 * Copyright 1990, 1991, 1992-1998,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: RelPort.cc,v 1.2 1998/11/20 08:28:43 nmcl Exp $
 */

#ifdef WIN32
#include <System/winsock.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef SYS_TYPES_H_
# include <System/sys/types.h>
#endif

#ifndef SYS_SELECT_H_
#  include <System/sys/select.h>
#endif

#ifndef SYS_SOCKET_H_
#  include <System/sys/socket.h>
#endif

#ifndef SYS_UIO_H_
#  include <System/sys/uio.h>
#endif

#ifndef WIN32

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef SYS_TIME_H_
#  include <System/sys/time.h>
#endif

#else

#include <System/winsock.h>
#include <System/time.h>

#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef RELPORT_H_
#  include "RelPort.h"
#endif

#ifndef TCPCONNECTION_H_
#  include "TCPConnection.h"
#endif

ReliablePort::ReliablePort ()
			   : validStream(FALSE),
			     isConnected(FALSE),
			     sock(INVALID_SOCKET)
{
    sock = ::socket(AF_INET, SOCK_STREAM, 0);
    validStream = (Boolean) (sock >= 0);
}

ReliablePort::ReliablePort ( Boolean doBind )
			   : validStream(TRUE),
			     isConnected(FALSE)
{
    sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        validStream = FALSE;
    else
    {
	if (doBind)
	{
	    struct sockaddr_in sockAddr;

	    sockAddr.sin_addr.s_addr = INADDR_ANY;
	    sockAddr.sin_family      = AF_INET;
	    sockAddr.sin_port        = 0;
	    if (::bind(sock, (struct sockaddr *) &sockAddr, sizeof(sockAddr)) < 0)
		validStream = FALSE;
	    else
		::listen(sock, 5);
	}
    }
}

ReliablePort::ReliablePort ( PortAddress& Addr )
			   : validStream(TRUE),
			     isConnected(FALSE)
{
    sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
        validStream = FALSE;
    else
    {
	struct sockaddr_in sockAddr;

	validStream = Addr.getSockAddr(sockAddr);
	sockAddr.sin_addr.s_addr = INADDR_ANY;

        if ((!validStream) ||
	    (::bind(sock, (struct sockaddr *) &sockAddr, sizeof(sockAddr)) < 0))
	    validStream = FALSE;
	else
	    ::listen(sock, 5);
    }
}

ReliablePort::~ReliablePort ()
{
    if (sock != INVALID_SOCKET)
#ifndef WIN32	
	::close(sock);
#else
        closesocket(sock);
#endif
}

Boolean ReliablePort::valid () const
{
    return validStream;
}

TCPConnection* ReliablePort::acceptConnection (long timeout)
{
    TCPConnection* toReturn = 0;
 
    if (pollForMessage(timeout) == DONE)
    {
	int length  = sizeof(struct sockaddr_in);
	SOCKET msgSock = accept(sock, (struct sockaddr*) &_sockAddr, &length);

	if (msgSock != INVALID_SOCKET)
	{
	    PortAddress from(_sockAddr);

#ifdef DEBUG
	    cout << "ReliablePort::acceptConnection - " << *this
		 << " accepting from " << from << endl;
#endif		
	    toReturn = new TCPConnection(msgSock, this, from);
	}
	else
	    validStream = FALSE;
    }

    return toReturn;
}

TCPConnection* ReliablePort::setupConnection ( const PortAddress& to )
{
    TCPConnection* toReturn = 0;
    
    if ((!isConnected) && (validStream))
    {
	int length  = sizeof(struct sockaddr_in);

#ifdef DEBUG
	cout << "ReliablePort::setupConnection - " << *this
	     << " setting connection to " << to << endl;
#endif
	
	if (to.getSockAddr(_sockAddr))
	{
	    int res = ::connect(sock, (struct sockaddr*) &_sockAddr, length);

	    if (res == 0)
	    {
		isConnected = TRUE;

		toReturn = new TCPConnection(sock, this, to);
	    }
	    else
	    {
#ifndef WIN32		
		::perror("Connect:");
#else		
		cout << "Connect error - errno " << WSAGetLastError() << endl;
#endif		
	    }
	}
    }
    
    return toReturn;
}

ReliablePort::RPOutcome ReliablePort::endConnection ()
{
    if ((sock != INVALID_SOCKET) && (isConnected))
    {
#ifndef WIN32
	::close(sock);
#else	
	closesocket(sock);
#endif
	sock = ::socket(AF_INET, SOCK_STREAM, 0);
	validStream = (Boolean) (sock != INVALID_SOCKET);
	isConnected = FALSE;
    }
    
    return DONE;
}

/*
 * Timeout given in milliseconds.
 */

ReliablePort::RPOutcome ReliablePort::pollForMessage (long timeout) const
{
    RPOutcome outcome;
    struct timeval* waitTime = 0;
    
    if (!validStream)
	outcome = NOTDONE;
    else
    {
	fd_set inmask;
	int res;

	FD_ZERO(&inmask);
	FD_SET(sock, &inmask);

	if (timeout != -1)
	{
	    waitTime = new timeval;
	    waitTime->tv_sec = timeout/1000000;
	    waitTime->tv_usec = timeout%1000000;
	}
	
#ifdef hpux
	res = ::select(FD_SETSIZE, (int *) &inmask, 0, 0, waitTime);
#else
	res = ::select(FD_SETSIZE, &inmask, 0, 0, waitTime);
#endif

	if (res < 0)
	    outcome = UNKNOWN;
	else
	{
	    if (res == 0)
		outcome = NOTDONE;
	    else
		outcome = DONE;
	}
    }

    if (waitTime)
		delete waitTime;
    
    return outcome;
}

ReliablePort::RPOutcome ReliablePort::getAddress ( PortAddress& Addr ) const
{
    RPOutcome res;

    if (!validStream)
	res = NOTDONE;
    else
    {
	struct sockaddr_in sockAddr;
	int    length;

	length = sizeof(sockAddr);
	if (::getsockname(sock, (struct sockaddr *) &sockAddr, &length) < 0)
            res = NOTDONE;
	else
	{
	    char localhostname[MAXHOSTNAMELEN];

	    if (gethostname(localhostname, MAXHOSTNAMELEN) != 0)
		res = NOTDONE;
	    else
	    {
		struct hostent *hp;
		hp = ::gethostbyname(localhostname);

		if (hp == NULL)
		    res = NOTDONE;
		else
		{
		    ::memcpy((char *) &sockAddr.sin_addr, (char *) hp->h_addr, hp->h_length);
		    Addr.setSockAddr(sockAddr);
		    res = DONE;
		}
	    }
	}
    }

    return res;
}

ostream& ReliablePort::print ( ostream& strm ) const
{
    if (validStream)
    {
        PortAddress Addr;

        strm << "<ReliablePort:" << sock << ',';
        if (getAddress(Addr) == DONE)
            strm << Addr << '>';
        else
            strm << "???? >";
    }
    else
        strm << "<ReliablePort:Invalid>";

    return strm;
}

ostream& operator<< ( ostream& strm, const ReliablePort& rp )
{
    return rp.print(strm);
}
