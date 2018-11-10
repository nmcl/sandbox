/*
 * Copyright 1995, 1996,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 *
 * $Id: Port.cc,v 1.18 1996/04/04 08:51:47 nmcl Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef SYS_SELECT_H_
#  include <System/sys/select.h>
#endif

#ifndef SYS_TYPES_H_
# include <System/sys/types.h>
#endif

#ifndef SYS_SOCKET_H_
#  include <System/sys/socket.h>
#endif

#ifndef SYS_UIO_H_
#  include <System/sys/uio.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef SYS_TIME_H_
#  include <System/sys/time.h>
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

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef PORT_H_
#  include "Port.h"
#endif

#ifndef SETUPBUFFER_H_
#  include "SetupBuffer.h"
#endif

#ifndef BUFFERMAP_H_
#  include "BufferMap.h"
#endif

#define MAX_PACKET 1448

Port::Port ()
	   : validPort(TRUE),
	     sock(0),
	     sendBuffers(0),
	     receiveBuffers(0),
	     burstTimeout(10000),
	     burstSize(2)
{
    sock = ::socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0)
        validPort = FALSE;
    else
    {
	struct linger l;

	l.l_onoff = 0;
	l.l_linger = 0;

	setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*) &l, sizeof(l));
	
	struct sockaddr_in sockAddr;

	sockAddr.sin_addr.s_addr = INADDR_ANY;
	sockAddr.sin_family      = AF_INET;
	sockAddr.sin_port        = 0;
	if (::bind(sock, (struct sockaddr *) &sockAddr, sizeof(sockAddr)) < 0)
	    validPort = FALSE;
    }
}

Port::Port ( PortAddress& Addr )
	   : validPort(TRUE),
	     sock(0),
	     sendBuffers(0),
	     receiveBuffers(0),
	     burstTimeout(10000),
	     burstSize(2)	     
{
    sock = ::socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0)
        validPort = FALSE;
    else
    {
	struct sockaddr_in sockAddr;

	validPort = Addr.getSockAddr(sockAddr);
	sockAddr.sin_addr.s_addr = INADDR_ANY;

        if ((!validPort) ||
	    (::bind(sock, (struct sockaddr *) &sockAddr, sizeof(sockAddr)) < 0))
	    validPort = FALSE;
    }
}

Port::~Port ()
{
    if (sock >= 0)
	::close(sock);

    if (sendBuffers)
	::delete [] sendBuffers;
    if (receiveBuffers)
	::delete [] receiveBuffers;
}

Boolean Port::valid () const
{
    return validPort;
}

void Port::mapSendBuffers (Buffer* work[], int number)
{
    sendBuffers = ::new BufferMap* [number];

    for (int i = 0; i < number; i++)
    {
	sendBuffers[i] = new BufferMap;
	sendBuffers[i]->bufferPtr = work[i]->buffer();
	sendBuffers[i]->sizeLeft = work[i]->length();
    }
}

int Port::createReceiveBuffers (const SetupBuffer& _setup)
{
    int numberOfBuffers = 0;
    int i;

    for (i = 0; i < MSG_MAXIOVLEN; i++)
    {
	if (ntohl(_setup.bufferSize[i]) != 0)
	    numberOfBuffers++;
    }

    receiveBuffers = ::new BufferMap* [numberOfBuffers];

    for (i = 0; i < numberOfBuffers; i++)
    {
	receiveBuffers[i] = new BufferMap;
	receiveBuffers[i]->bufferSize = ntohl(_setup.bufferSize[i]);
	receiveBuffers[i]->sizeLeft = receiveBuffers[i]->bufferSize;
	receiveBuffers[i]->buffer = ::new char[receiveBuffers[i]->bufferSize];
	receiveBuffers[i]->bufferPtr = receiveBuffers[i]->buffer;
    }

    return numberOfBuffers;
}

Port::POutcome Port::sendMessage ( PortAddress Addr,
				   Buffer* work[], int number)
{
    POutcome res = NOTDONE;
    int i = 0;
    
#ifdef DEBUG
    cout << "Port::sendMessage - sender " << *this << " sending "
	 << number << " buffers to " << Addr << endl;
#endif
    
    if ((validPort) && (number < MSG_MAXIOVLEN-1) && (number > 0))
    {
	struct sockaddr_in sockAddr;
	struct iovec msg[MSG_MAXIOVLEN];
	struct msghdr buffer;
	SetupBuffer _setup;
	int buffersSent = 0;
	size_t packetNumber = 0;

	if (!Addr.getSockAddr(sockAddr))
	    return NOTDONE;
	
	for (i = 0; i < number; i++)
	    _setup.bufferSize[i] = htonl(work[i]->length());

	mapSendBuffers(work, number);
	
	msg[0].iov_base = (char*) &_setup;
	msg[0].iov_len = sizeof(_setup);

	res = DONE;
	
	while ((buffersSent != number) && (res == DONE))
	{
	    size_t packetSize = sizeof(_setup);
	    
	    for (i = buffersSent; (packetSize < MAX_PACKET) && (buffersSent != number); i++)
	    {
		size_t buffSize = sendBuffers[buffersSent]->sizeLeft;

		if (buffSize + packetSize > MAX_PACKET)
		    buffSize = MAX_PACKET - packetSize;
		
		msg[i+1].iov_base = (char*) sendBuffers[i]->bufferPtr;
		msg[i+1].iov_len = buffSize;

		sendBuffers[i]->bufferPtr += buffSize;
		sendBuffers[i]->sizeLeft -= buffSize;

		packetSize += buffSize;

		if (sendBuffers[i]->sizeLeft == 0)
		    buffersSent++;

		if (buffersSent == number)
		    _setup.endOfMessage = htonl(1);
	    }

	    packetNumber++;
	    _setup.packetNumber = htonl(packetNumber);

#ifdef HAVE_MSGACCRIGHTS
	    buffer.msg_accrights = (caddr_t) 0;
	    buffer.msg_accrightslen = 0;
#else
	    buffer.msg_control = (caddr_t) 0;
	    buffer.msg_controllen = 0;
	    buffer.msg_flags = 0;
#endif	    
	    buffer.msg_namelen = sizeof(struct sockaddr_in);
	    buffer.msg_iovlen = i+1;

	    buffer.msg_name = (caddr_t) &sockAddr;
	    buffer.msg_iov = (struct iovec*) &msg[0];

	    if (::sendmsg(sock, &buffer, 0) > 0)
	    {
		res = DONE;

		if (packetNumber%burstSize == 0)
		    usleep((unsigned int) burstTimeout);
	    }
	    else
		res = UNKNOWN;
	}
    }

#ifdef DEBUG
    cout << "Port::sendMessage - returning " << res << endl;
#endif

    if (sendBuffers)
    {
	for (i = 0; i < number; i++)
	    if (sendBuffers[i])
		delete sendBuffers[i];

	::delete [] sendBuffers;
	sendBuffers = 0;
    }
    
    return res;
}

Port::POutcome Port::receiveMessage ( PortAddress& Addr,
				      Buffer* result[],
				      int& number, long timeout)
{
    POutcome res = NOTDONE;
    Boolean isMessage = FALSE;
    char* buff[MSG_MAXIOVLEN];
    int i = 0;
    
    if (validPort)
    {
	size_t packetNumber = 0;
	Boolean finished = FALSE;

	res = DONE;
	
	while ((res == DONE) && (!finished))
	{
	    if (pollForMessage(isMessage, timeout) == DONE)
	    {
		struct sockaddr_in sockAddr;
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
		buffer.msg_name = (caddr_t) &sockAddr;
		buffer.msg_iov = (struct iovec*) &msg[0];
		buffer.msg_iovlen = 1;

		if (::recvmsg(sock, &buffer, MSG_PEEK) > 0)
		{
		    Addr.setSockAddr(sockAddr);

#ifdef DEBUG
		    cout << "Port::receiveMessage - received message from "
			 << Addr << endl;
#endif
		    int thisPacket = ntohl(_setup.packetNumber);

		    if (thisPacket == 1)
			number = createReceiveBuffers(_setup);
		    else
		    {
			if (thisPacket != packetNumber +1)
			{
			    if (receiveBuffers)
			    {
				for (i = 0; i < number; i++)
				    if (receiveBuffers[i])
					delete receiveBuffers[i];
			    
				::delete [] receiveBuffers;
				receiveBuffers = 0;
			    }
			    
			    return UNKNOWN;
			}
		    }

		    packetNumber = thisPacket;
		    buffer.msg_iovlen = number+1;

#ifdef DEBUG
		    cout << "Port::receiveMessage - " << Addr << " received "
			 << number << " buffers." << endl;
#endif	    
		    
		    for (i = 0; i < number; i++)
		    {
			msg[i+1].iov_base = (char*) receiveBuffers[i]->bufferPtr;
			msg[i+1].iov_len = receiveBuffers[i]->sizeLeft;
		    }

		    if (::recvmsg(sock, &buffer, 0) > 0)
		    {
			size_t packetSize = sizeof(_setup);
			
			for (i = 0; (i < number) && (packetSize < MAX_PACKET); i++)
			{
			    size_t sizeUsed = receiveBuffers[i]->sizeLeft;

			    if (sizeUsed + packetSize > MAX_PACKET)
				sizeUsed = MAX_PACKET - packetSize;
			    
			    receiveBuffers[i]->sizeLeft -= sizeUsed;
			    receiveBuffers[i]->bufferPtr += sizeUsed;
			}

			if (ntohl(_setup.endOfMessage) == 1)
			    finished = TRUE;
		    }
		    else
			res = UNKNOWN;
		}
		else
		    res = UNKNOWN;
	    }
	    else
		res = UNKNOWN;
	}
	
	if (finished)
	{
	    for (i = 0; i < number; i++)
	    {
		if (result[i] == (Buffer*) 0)
		    result[i] = new Buffer(receiveBuffers[i]->bufferSize, receiveBuffers[i]->buffer);
		else
		    result[i]->setBuffer(receiveBuffers[i]->bufferSize, receiveBuffers[i]->buffer);

		receiveBuffers[i]->buffer = 0;
	    }
	}
   }
    
#ifdef DEBUG
    cout << "Port::receiveMessage - returning " << res << endl;
#endif

    if (receiveBuffers)
    {
	for (i = 0; i < number; i++)
	    if (receiveBuffers[i])
		delete receiveBuffers[i];
	
	::delete [] receiveBuffers;
	receiveBuffers = 0;
    }
	    
    return res;
}

/*
 * Timeout given in milliseconds.
 */

Port::POutcome Port::pollForMessage ( Boolean& isMess, long timeout ) const
{
    POutcome outcome;
    struct timeval* waitTime = 0;
    
    if (timeout >= 0)
    {
	waitTime = new timeval;
	waitTime->tv_sec = (int) (timeout/1000);
	waitTime->tv_usec = (timeout - waitTime->tv_sec*1000)*1000;
    }

    isMess = FALSE;
    if (!validPort)
	outcome = NOTDONE;
    else
    {
	fd_set inmask;
	int res;

	FD_ZERO(&inmask);
	FD_SET(sock, &inmask);

#ifdef hpux
	res = select(FD_SETSIZE, (int *) &inmask, 0, 0, waitTime);
#else
	res = select(FD_SETSIZE, &inmask, 0, 0, waitTime);
#endif
	
	isMess = (Boolean) (res > 0);
	
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

Port::POutcome Port::getAddress ( PortAddress& Addr ) const
{
    POutcome res;

    if (! validPort)
	res = NOTDONE;
    else
    {
	struct sockaddr_in sockAddr;
	int    length;

	length = sizeof(sockAddr);
	if (getsockname(sock, (struct sockaddr *) &sockAddr, &length) < 0)
            res = NOTDONE;
	else
	{
	    char localhostname[MAXHOSTNAMELEN];

	    if (::gethostname(localhostname, MAXHOSTNAMELEN) != 0)
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

void Port::setBurstTimeout (long b)
{
    burstTimeout = b;
}

void Port::getBurstTimeout (long& b) const
{
    b = burstTimeout;
}

void Port::setBurstSize (long b)
{
    burstSize = b;
}

void Port::getBurstSize (long& b) const
{
    b = burstSize;
}

ostream& Port::print ( ostream& strm ) const
{
    if (validPort)
    {
        PortAddress Addr;

        strm << "<Port:" << sock << ',';
        if (getAddress(Addr) == DONE)
            strm << Addr << '>';
        else
            strm << "???? >";
    }
    else
        strm << "<Port:Invalid>";

    return strm;
}

ostream& operator<< (ostream& strm, const Port& pt)
{
    return pt.print(strm);
}
