/*
 * Copyright 1990, 1991,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 *
 * Permission to use, copy, modify and distribute this program for
 * evaluation, teaching and/or research purposes only and without fee is
 * hereby granted, providing that this copyright and permission notice
 * appear on all copies and supporting documentation, and that similar
 * conditions are imposed on any individual or organization to whom the
 * program is distributed. The University of Newcastle upon Tyne makes no
 * representation about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 */

/*
 * $Id: Port.cc,v 1.3 1992/02/06 17:35:12 nmcl Exp $
 */

#include <iostream.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stddef.h>
#include <sysent.h>
#include <string.h>

#include <Std/boolean.h>
#include <Arjuna/Buffer.h>

#include "Port.h"

#define MAX_PACKET 1024
#define EndMessage "End Of Buffer Stream"

MultiBuffer::MultiBuffer()
{
    CurrBuff = 0;
    next = 0;
}

MultiBuffer::~MultiBuffer() {}

Port::Port ()
{
    ValidPort = TRUE;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        ValidPort = FALSE;
    else
    {
	struct sockaddr_in sock_addr;

	sock_addr.sin_addr.s_addr = INADDR_ANY;
	sock_addr.sin_family      = AF_INET;
	sock_addr.sin_port        = 0;
	if (bind(sock, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0)
	    ValidPort = FALSE;
    }
}

Port::Port (PortAddress& Addr)
{
    unsigned short portnum;
    bool init = FALSE;
    ValidPort = TRUE;

    Addr.GetPort(portnum);
    if (portnum == 0)
	init = TRUE;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        ValidPort = FALSE;
    else
    {
	struct sockaddr_in sock_addr;

	ValidPort = Addr.GetSockAddr(sock_addr);
	sock_addr.sin_addr.s_addr = INADDR_ANY;

        if ((! ValidPort) || (bind(sock, (struct sockaddr *) &sock_addr, sizeof(sock_addr)) < 0))
	    ValidPort = FALSE;
	else
	{
	    if (init)
	    {
		int length = sizeof(sock_addr);

		if (getsockname(sock, (struct sockaddr *) &sock_addr, &length) < 0)
		    ValidPort = FALSE;
		else
		{
		    cerr << "Created " << sock_addr.sin_port << "\n";
		    Addr.SetPort(sock_addr.sin_port);
		}
	    }
	}
    }
}

Port::~Port ()
{
    if (sock >=  0)
	close(sock);
}


PortOutcome Port::ReceiveMessage (PortAddress& Addr, Buffer& message)
{
    PortOutcome res;

    if (! ValidPort)
	res = NotDone;
    else
    {
	struct sockaddr_in SockAddr;
	int    length = sizeof(struct sockaddr_in);

	int   bufflen = MAX_PACKET;
	char* buff    = new char[bufflen];

	if ((bufflen = recvfrom(sock, buff, bufflen, 0, (struct sockaddr *) &SockAddr, &length)) >= 0)
	{
	    cerr << "Received from " << SockAddr.sin_port << "\n";
	    Addr.SetSockAddr(SockAddr);
	    if (message.set_buffer(bufflen, buff))
		res = Done;
	    else
	    {
		if (buff != 0)
		    delete buff;
		res = Unknown;
	    }
	}
	else
	    res = NotDone;
    }

    return res;
}

PortOutcome Port::ReceiveLongMessage (PortAddress& Addr, MultiBuffer& message)
{
    PortOutcome res;
    Buffer dummy;

    if (!ValidPort)
	res = NotDone;
    else
    {
	MultiBuffer ptr = message;
	struct sockaddr_in SockAddr;
	int length = sizeof(struct sockaddr_in);
	long dlen = sizeof(EndMessage);
	char* string;

	int bufflen = MAX_PACKET;
	char* buff = new char[bufflen];
	bool finished = FALSE, first = TRUE;

	while (!finished)
	{
	    if ((bufflen = recvfrom(sock, buff, bufflen, 0, (struct sockaddr *) &SockAddr, &length)) >= 0)
	    {
		Addr.SetSockAddr(SockAddr);

		if (bufflen == dlen)
		{
		    dummy.set_buffer(bufflen, buff);
		    dummy.unpack(string);

		    if (strcmp(string, EndMessage) == 0)
		    {
			delete buff;
			finished = TRUE;
		    }
		}

		if (!finished)
		{
		    if (!first)
		    {
			ptr.next = new MultiBuffer;
			ptr = *ptr.next;
		    }

		    if (ptr.CurrBuff.set_buffer(bufflen, buff))
		    {
			first = FALSE;
			res = Done;
		    }
		    else
		    {
			if (buff != 0)
			    delete buff;
			res = Unknown;
		    }
		}
	    }
	    else
	    {
		finished = TRUE;
		res = NotDone;
	    }
	}
    }

    return res;
}

PortOutcome Port::SendLongMessage (PortAddress Addr, MultiBuffer& message)
{
    PortOutcome res;
    MultiBuffer ptr = message;

    if (!ValidPort)
	res = NotDone;
    else
    {
	bool finished = FALSE;
	struct sockaddr_in SockAddr;
	int length = sizeof(struct sockaddr_in);
	int bufflen;
	char* buff;

	if (Addr.GetSockAddr(SockAddr))
	{
	    Buffer dummy;
	    dummy.pack(EndMessage);
	    int dlen = dummy.length();

	    do
	    {
		bufflen = (int) ptr.CurrBuff.length();
		buff = ptr.CurrBuff.buffer();

		if (sendto(sock, buff, bufflen, 0, (struct sockaddr *) &SockAddr, length) == bufflen)
		    res = Done;
		else
		{
		    res = Unknown;
		    finished = TRUE;
		}

		if (ptr.next != 0)
		    ptr = *ptr.next;
		else
		{
		    finished = TRUE;
		    
		    if (sendto(sock, dummy.buffer(), dlen, 0, (struct sockaddr *) &SockAddr, length) == dlen)
			res = Done;
		    else
			res = Unknown;
		}
	    } while (!finished);
	}
	else
	    res = Unknown;
    }

    return res;
}


PortOutcome Port::SendMessage (PortAddress Addr, Buffer& message)
{
    PortOutcome res;
    
    if (! ValidPort)
	res = NotDone;
    else
    {
	struct sockaddr_in SockAddr;
	int    length = sizeof(struct sockaddr_in);
	
	int   bufflen = message.length();
	char* buff    = message.buffer();
	
	if (Addr.GetSockAddr(SockAddr))
	{
	    if (sendto(sock, buff, bufflen, 0, (struct sockaddr *) &SockAddr, length) == bufflen)
		res = Done;
	    else
		res = Unknown;
	}
	else
	    res = Unknown;
    }
    
    return res;
}

PortOutcome Port::PollForMessage (bool& isMess)
{
    PortOutcome res;

    isMess = FALSE;
    if (! ValidPort)
	res = NotDone;
    else
    {
    }

    return res;
}

bool Port::Valid()
{
    return ValidPort;
}

PortOutcome Port::GetAddress (PortAddress& Addr) const
{
    PortOutcome res;

    if (! ValidPort)
	res = NotDone;
    else
    {
	struct sockaddr_in sock_addr;
	int    length;

	length = sizeof(sock_addr);
	if (getsockname(sock, (struct sockaddr *) &sock_addr, &length) < 0)
            res = NotDone;
	else
	{
	    char localhostname[32];

	    if (gethostname(localhostname, 32) != 0)
		res = NotDone;
	    else
	    {
		struct hostent *hp;
		hp = gethostbyname(localhostname);

		if (hp == NULL)
		    res = NotDone;
		else
		{
		    bcopy((char *) hp->h_addr, (char *) &sock_addr.sin_addr, hp->h_length);
		    Addr.SetSockAddr(sock_addr);
		    res = Done;
		}
	    }
	}
    }

    return res;
}
