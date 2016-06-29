/*
 * Copyright 1990, 1991, 1992,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: RelPort.cc,v 1.1 1997/06/09 19:52:05 nmcl Exp $
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

#ifndef SETUPBUFFER_H_
#  include "SetupBuffer.h"
#endif


ReliablePort::ReliablePort ()
			   : validStream(FALSE),
			     isConnected(FALSE),
			     sock(INVALID_SOCKET),
			     msgSock(INVALID_SOCKET)
{
    sock = ::socket(AF_INET, SOCK_STREAM, 0);
    validStream = (Boolean) (sock >= 0);
}

ReliablePort::ReliablePort ( Boolean doBind )
			   : validStream(TRUE),
			     isConnected(FALSE),
			     msgSock(INVALID_SOCKET)
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
			     isConnected(FALSE),
			     msgSock(INVALID_SOCKET)
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
    if (msgSock != INVALID_SOCKET)    
#ifndef WIN32
	::close(msgSock);
#else
        closesock(msgSock);
#endif

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

ReliablePort::RPOutcome ReliablePort::acceptConnection (PortAddress& from,
							long timeout)
{
    RPOutcome res = UNKNOWN;
 
    if (msgSock == INVALID_SOCKET)
    {
	if (pollForMessage(timeout) == DONE)
	{
	    int length  = sizeof(struct sockaddr_in);
	
	    msgSock = accept(sock, (struct sockaddr*) &_sockAddr, &length);

	    if (msgSock != INVALID_SOCKET)
	    {
		from.setSockAddr(_sockAddr);
#ifdef DEBUG
		cout << "ReliablePort::acceptConnection - " << *this
		     << " accepting from " << from << endl;
#endif		
		res = DONE;
	    }
	    else
		validStream = FALSE;
	}
	else
	    res = NOTDONE;
    }
    else
	res = DONE;

    return res;
}

ReliablePort::RPOutcome ReliablePort::breakConnection ()
{
    if (msgSock != INVALID_SOCKET)
    {
#ifndef WIN32
	::close(msgSock);
#else	
	closesocket(msgSock);
#endif	
	msgSock = INVALID_SOCKET;
    }

    return DONE;
}

ReliablePort::RPOutcome ReliablePort::setupConnection ( const PortAddress& to )
{
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
		return DONE;
	    }
	    else
	    {
#ifndef WIN32		
		::perror("Connect:");
#else		
		cout << "Connect error - errno " << WSAGetLastError() << endl;
#endif		
		return UNKNOWN;
	    }
	}
	else
	    return UNKNOWN;
    }
    
    return DONE;
}

ReliablePort::RPOutcome ReliablePort::endConnection ()
{
    if (sock != INVALID_SOCKET)
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

ReliablePort::RPOutcome ReliablePort::sendMessage (Buffer* work[],
						   int number)
{
#ifdef DEBUG
    cout << "**ReliablePort::sendMessage - sending " << number
	 << " fields." << endl;
#endif
     
    RPOutcome res;
    SOCKET sockUse = ((msgSock == -1) ? sock : msgSock);

    if ((!validStream) || (number +1 > MSG_MAXIOVLEN))
	res = NOTDONE;
    else
    {
	struct iovec msg[MSG_MAXIOVLEN];
	struct msghdr buffer;
	SetupBuffer _setup;
	int i;
	
	for (i = 0; i < number; i++)
	    _setup.bufferSize[i] = htonl(work[i]->length());

	msg[0].iov_base = (char*) &_setup;
	msg[0].iov_len = sizeof(_setup);

	for (i = 0; i < number; i++)
	{
	    msg[i+1].iov_base = (char *)work[i]->buffer();
	    msg[i+1].iov_len = work[i]->length();
	}
	
#ifdef HAVE_MSGACCRIGHTS
	buffer.msg_accrights = (caddr_t) 0;
	buffer.msg_accrightslen = 0;
#else
	buffer.msg_control = (caddr_t) 0;
	buffer.msg_controllen = 0;
	buffer.msg_flags = 0;	
#endif		
	buffer.msg_namelen = sizeof(struct sockaddr_in);
	buffer.msg_name = (caddr_t) &_sockAddr;
	buffer.msg_iov = (struct iovec*) &msg[0];
	buffer.msg_iovlen = number+1;

	if (::sendmsg(sockUse, &buffer, 0) <= 0)
	    res = UNKNOWN;
	else
	    res = DONE;
    }
    
#ifdef DEBUG
    cout << "ReliablePort::sendMessage - returning " << res << endl;
#endif    
      
    return res;
}

ReliablePort::RPOutcome ReliablePort::receiveMessage ( Buffer* result[],
						       int& number)
{
#ifdef DEBUG
    cout << "ReliablePort::receiveMessage - listening" << endl;
#endif
    
    RPOutcome res = UNKNOWN;
    SOCKET sockUse = ((msgSock == INVALID_SOCKET) ? sock : msgSock);
    char* buff[MSG_MAXIOVLEN];
    int i = 0;
    
    if (!validStream)
	res = NOTDONE;
    else
    {
	struct sockaddr_in SockAddr;
	struct msghdr buffer;
	struct iovec msg[MSG_MAXIOVLEN];
	SetupBuffer _setup;

	msg[0].iov_base = (char*) &_setup;
	msg[0].iov_len = sizeof(_setup);
	
#ifdef HAVE_MSGACCRIGHTS	
	buffer.msg_accrights = (caddr_t) 0;
	buffer.msg_accrightslen = 0;
#else
	buffer.msg_control = (caddr_t) 0;
	buffer.msg_controllen = 0;
	buffer.msg_flags = 0;
#endif
	buffer.msg_namelen = sizeof(struct sockaddr_in);
	buffer.msg_name = (caddr_t) &SockAddr;
	buffer.msg_iov = (struct iovec*) &msg[0];
	buffer.msg_iovlen = 1;

	if (recvmsg(sockUse, &buffer, MSG_PEEK) > 0)
	{
	    long msgSize = sizeof(_setup);
	    int sizeReceived = 0;
	    
	    number = 0;
	    for (i = 0; i < MSG_MAXIOVLEN; i++)
		if (ntohl(_setup.bufferSize[i]) > 0)
		    number++;

#ifdef DEBUG
	    cout << "ReliablePort::receiveMessage - received message with "
		 << number << " fields" << endl;
#endif	    

	    if (number <= 0)
			return NOTDONE;

	    buffer.msg_iovlen = number+1;

	    for (i = 0; i < number; i++)
	    {
		buff[i] = ::new char[ntohl(_setup.bufferSize[i])];
		::memset(buff[i], '\0', (int) ntohl(_setup.bufferSize[i]));
		msg[i+1].iov_base = buff[i];
		msg[i+1].iov_len = (int) ntohl(_setup.bufferSize[i]);
		msgSize += msg[i+1].iov_len;
	    }

	    do
	    {
		sizeReceived = recvmsg(sockUse, &buffer, 0);

		if (sizeReceived > 0)
		{
		    res = DONE;
		    msgSize -= sizeReceived;
		
		    for (i = 0; (i < number+1) && (sizeReceived > 0); i++)
		    {
			if (msg[i].iov_len > 0)
			{
			    int oldSize = msg[i].iov_len;

			    msg[i].iov_len -= sizeReceived;
			    if (msg[i].iov_len < 0)
			    {
				msg[i].iov_len = 0;
				msg[i].iov_base = (char*) 0;
			    }
			    else
				msg[i].iov_base += sizeReceived;
			    
			    sizeReceived -= oldSize;
			}
		    }
		}
		else
		    res = UNKNOWN;
		
	    } while ((msgSize > 0) && (res == DONE));

	    if (res != DONE)
	    {
		for (i = 0; i < number; i++)
		{
		    if (buff[i])
			::delete [] buff[i];
		}
	    }
	    else
	    {
		for (i = 0; i < number; i++)
		{
		    if (result[i] == (Buffer*) 0)
			result[i] = new Buffer((unsigned int) ntohl(_setup.bufferSize[i]), buff[i]);
		    else
			result[i]->setBuffer((unsigned int) ntohl(_setup.bufferSize[i]), buff[i]);
		}
	    }
	}
	else
	    res = NOTDONE;
   }

#ifdef DEBUG
    cout << "ReliablePort::receiveMessage - returning " << res << endl;
#endif    
    
    return res;
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
