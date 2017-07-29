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
 * $Id: Port.cc,v 1.1 1992/02/21 10:26:09 nmcl Exp $
 */

#include <iostream.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdlib.h>
#include <sysent.h>

#include <Common/Boolean.h>
#include <Common/Buffer.h>

#include <Port/Port.h>

#define MAX_PACKET 1024

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
    ValidPort = TRUE;

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
    }
}

Port::~Port ()
{
    if (sock >= 0)
	close(sock);
}


Boolean Port::Valid() const
{
    return ValidPort;
}

Port::Outcome Port::ReceiveMessage (PortAddress& Addr, Buffer& message) const
{
    Outcome res;

    if (! ValidPort)
	res = Port::NotDone;
    else
    {
	struct sockaddr_in SockAddr;
	int    length = sizeof(struct sockaddr_in);

	int   bufflen = MAX_PACKET;
	char* buff    = new char[bufflen];

	if ((bufflen = recvfrom(sock, buff, bufflen, 0, (struct sockaddr *) &SockAddr, &length)) >= 0)
	{
	    Addr.SetSockAddr(SockAddr);
	    if (message.set_buffer(bufflen, buff))
		res = Port::Done;
	    else
	    {
		if (buff != 0)
		    delete buff;
		res = Port::Unknown;
	    }
	}
	else
	    res = Port::NotDone;
    }

    return res;
}

Port::Outcome Port::SendMessage (PortAddress Addr, Buffer& message) const
{
    Outcome res;
    
    if (! ValidPort)
	res = Port::NotDone;
    else
    {
	struct sockaddr_in SockAddr;
	int    length = sizeof(struct sockaddr_in);
	
	int   bufflen = message.length();
	char* buff    = message.buffer();
	
	if (Addr.GetSockAddr(SockAddr))
	{
	    if (sendto(sock, buff, bufflen, 0, (struct sockaddr *) &SockAddr, length) == bufflen)
		res = Port::Done;
	    else
		res = Port::Unknown;
	}
	else
	    res = Port::Unknown;
    }
    
    return res;
}

Port::Outcome Port::PollForMessage (Boolean& isMess) const
{
    Outcome res;

    isMess = FALSE;
    if (! ValidPort)
	res = Port::NotDone;
    else
    {
	fd_set inmask;
	int res;

	FD_ZERO(&inmask);
	FD_SET(sock, &inmask);

	res = select(FD_SETSIZE, &inmask, 0, 0, 0);
	if (res < 0)
	{
	    isMess = FALSE;
	    res = Port::Unknown;
	}
	else
	{
	    isMess = TRUE;
	    res = Port::Done;
	}
    }

    return res;
}

Port::Outcome Port::GetAddress (PortAddress& Addr) const
{
    Outcome res;

    if (! ValidPort)
	res = Port::NotDone;
    else
    {
	struct sockaddr_in sock_addr;
	int    length;

	length = sizeof(sock_addr);
	if (getsockname(sock, (struct sockaddr *) &sock_addr, &length) < 0)
            res = Port::NotDone;
	else
	{
	    char localhostname[32];

	    if (gethostname(localhostname, 32) != 0)
		res = Port::NotDone;
	    else
	    {
		struct hostent *hp;
		hp = gethostbyname(localhostname);

		if (hp == NULL)
		    res = Port::NotDone;
		else
		{
		    bcopy((char *) hp->h_addr, (char *) &sock_addr.sin_addr, hp->h_length);
		    Addr.SetSockAddr(sock_addr);
		    res = Port::Done;
		}
	    }
	}
    }

    return res;
}

ostream& Port::print (ostream& strm) const
{
    strm << ValidPort;
    strm << " " << sock;

    return strm;
}

ostream& operator<< (ostream& strm, const Port& pt)
{
    return pt.print(strm);
}
