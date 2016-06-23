/*
 * Copyright 1990-1997, 1998,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: TCPConnection.cc,v 1.3 1998/11/20 08:28:43 nmcl Exp $
 */

#ifdef WIN32
#  include <System/winsock.h>
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

#ifndef THREADMUTEX_H_
#  include <Thread/ThreadMutex.h>
#endif

#ifndef RELPORT_H_
#  include "RelPort.h"
#endif

#ifndef SETUPBUFFER_H_
#  include "SetupBuffer.h"
#endif

#ifndef TCPCONNECTION_H_
#  include "TCPConnection.h"
#endif

TCPConnection::TCPConnection (SOCKET sock, ReliablePort* handle, const PortAddress& address)
			     : msgSock(sock),
			       msgHandle(handle),
			       addr(address),
			       mutex(ThreadMutex::create())
{
}

TCPConnection::TCPConnection (const TCPConnection& conn)
			     : msgSock(conn.msgSock),
			       msgHandle(conn.msgHandle),
			       addr(conn.addr),
			       mutex(ThreadMutex::create())
{
}

TCPConnection::~TCPConnection ()
{
    if (msgSock != INVALID_SOCKET)
    {
#ifndef WIN32
	::close(msgSock);
#else
        closesock(msgSock);
#endif
    }

    if (msgHandle)
	msgHandle->endConnection();

    if (mutex)
	delete mutex;
}

TCPConnection::Outcome TCPConnection::terminateConnection ()
{
    if (mutex->lock())
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

	msgHandle->endConnection();

	mutex->unlock();
	
	return DONE;
    }

    return NOTDONE;
}

TCPConnection::Outcome TCPConnection::sendMessage ( Buffer* work[], int number )
{
#ifdef DEBUG
    cout << "**TCPConnection::sendMessage - sending " << number
	 << " fields." << endl;
#endif
     
    Outcome res = NOTDONE;

    if (number <= 0)
	return TCPConnection::NOTDONE;
    
    if (mutex->lock())
    {
	if (number +1 <= MSG_MAXIOVLEN)
	{
	    struct iovec msg[MSG_MAXIOVLEN];
	    struct msghdr buffer;
	    SetupBuffer _setup;
	    int i = 0;

	    for (i = 0; i < number; i++)
	    {
		_setup.bufferSize[i] = htonl(work[i]->length());
	    }

	    msg[0].iov_base = (char*) &_setup;
	    msg[0].iov_len = sizeof(_setup);

	    for (i = 0; i < number; i++)
	    {
		msg[i+1].iov_base = (char*) work[i]->buffer();
		msg[i+1].iov_len = work[i]->length();
	    }

	    struct sockaddr_in _sockAddr;

	    addr.getSockAddr(_sockAddr);
	    
#ifdef HAVE_MSGCONTROL
	    buffer.msg_control = (caddr_t) 0;
	    buffer.msg_controllen = 0;
	    buffer.msg_flags = 0;
#else		
	    buffer.msg_accrights = (caddr_t) 0;
	    buffer.msg_accrightslen = 0;
#endif	
	    buffer.msg_namelen = sizeof(struct sockaddr_in);
	    buffer.msg_name = (caddr_t) &_sockAddr;
	    buffer.msg_iov = (struct iovec*) &msg[0];
	    buffer.msg_iovlen = number+1;

	    if (::sendmsg(msgSock, &buffer, 0) <= 0)
		res = UNKNOWN;
	    else
		res = DONE;
	}

	mutex->unlock();
    }
    
#ifdef DEBUG
    cout << "TCPConnection::sendMessage - returning " << res << endl;
#endif    
      
    return res;
}

TCPConnection::Outcome TCPConnection::receiveMessage ( Buffer* result[],
						       int& number)
{
#ifdef DEBUG
    cout << "TCPConnection::receiveMessage - listening" << endl;
#endif

    Outcome res = UNKNOWN;

    if (number <= 0)
	return NOTDONE;

    int recNumb = 0;
    
    if (mutex->lock())
    {
	char* buff[MSG_MAXIOVLEN];
	int i = 0;
	struct sockaddr_in SockAddr;
	struct msghdr buffer;
	struct iovec msg[MSG_MAXIOVLEN];
	SetupBuffer _setup;

	msg[0].iov_base = (char*) &_setup;
	msg[0].iov_len = sizeof(_setup);

#ifdef HAVE_MSGCONTROL
	buffer.msg_control = (caddr_t) 0;
	buffer.msg_controllen = 0;
	buffer.msg_flags = 0;
#else		
	buffer.msg_accrights = (caddr_t) 0;
	buffer.msg_accrightslen = 0;
#endif	
	buffer.msg_namelen = sizeof(struct sockaddr_in);
	buffer.msg_name = (caddr_t) &SockAddr;
	buffer.msg_iov = (struct iovec*) &msg[0];
	buffer.msg_iovlen = 1;

	if (recvmsg(msgSock, &buffer, MSG_PEEK) > 0)
	{
	    long msgSize = sizeof(_setup);
	    int sizeReceived = 0;

	    /*
	     * Number of buffers to receive is the last none-zero index.
	     */
	    
	    for (i = 0; i < MSG_MAXIOVLEN; i++)
	    {
		if (ntohl(_setup.bufferSize[i]) != 0)
		    recNumb = i+1;
	    }
	
#ifdef DEBUG
	    cout << "TCPConnection::receiveMessage - received message with "
		 << recNumb << " fields" << endl;
#endif	    

	    if (recNumb <= 0)
	    {
		mutex->unlock();
		return NOTDONE;
	    }

	    buffer.msg_iovlen = recNumb+1;

	    for (i = 0; i < recNumb; i++)
	    {
		if (ntohl(_setup.bufferSize[i]) > 0)
		{
		    buff[i] = ::new char[ntohl(_setup.bufferSize[i])];
		    ::memset(buff[i], '\0', (int) ntohl(_setup.bufferSize[i]));
		}
		else
		    buff[i] = 0;
		    
		msg[i+1].iov_base = buff[i];		    
		msg[i+1].iov_len = (int) ntohl(_setup.bufferSize[i]);
		msgSize += msg[i+1].iov_len;
	    }

	    do
	    {
		sizeReceived = recvmsg(msgSock, &buffer, 0);

		if (sizeReceived > 0)
		{
		    res = DONE;
		    msgSize -= sizeReceived;
		
		    for (i = 0; (i < recNumb+1) && (sizeReceived > 0); i++)
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
		for (i = 0; i < recNumb; i++)
		{
		    if (buff[i])
			::delete [] buff[i];
		}
	    }
	    else
	    {
		int loop = number;

		if (recNumb < number)
		    loop = recNumb;
		
		for (i = 0; i < loop; i++)
		{
		    if (result[i] == (Buffer*) 0)
			result[i] = new Buffer((unsigned int) ntohl(_setup.bufferSize[i]), buff[i]);
		    else
			result[i]->setBuffer((unsigned int) ntohl(_setup.bufferSize[i]), buff[i]);
		}

		number = recNumb;
	    }
	}

	mutex->unlock();
    }

#ifdef DEBUG
    cout << "TCPConnection::receiveMessage - returning " << res << endl;
#endif    
    
    return res;
}

TCPConnection& TCPConnection::operator= (const TCPConnection& c)
{
    if (this == &c)
	return *this;
    else
    {
	msgSock = c.msgSock;
	msgHandle = c.msgHandle;
	addr = c.addr;

	return *this;
    }
}

PortAddress TCPConnection::getAddress () const
{
    return addr;
}
