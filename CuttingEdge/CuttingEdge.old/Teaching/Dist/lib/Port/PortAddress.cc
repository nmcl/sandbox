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
 * $Id: PortAddress.cc,v 1.1 1992/02/21 10:26:11 nmcl Exp $
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdlib.h>
#include <sysent.h>
#include <string.h>

#include <iostream.h>

#include <Port/PortAddress.h>

PortAddress::PortAddress ()
{
    ValidAddr = FALSE;
}

PortAddress::PortAddress ( unsigned short port)
{
    ValidAddr = TRUE;
    char localhostname[64];

    if (gethostname(localhostname, 64) != 0)
	ValidAddr = FALSE;
    else
    {
	struct hostent *hp;
	hp = gethostbyname(localhostname);

	if (hp == NULL)
	    ValidAddr = FALSE;
	else
	{
	    bzero((void *) &sock_addr, sizeof(struct sockaddr_in));
	    bcopy((char *) hp->h_addr, (char *) &sock_addr.sin_addr, hp->h_length);
	    sock_addr.sin_family      = AF_INET;
	    sock_addr.sin_port        = htons(port);
	}
    }
}

PortAddress::PortAddress ( const char* host, unsigned short port)
{
    ValidAddr = TRUE;

    struct hostent *hp;
    hp = gethostbyname(host);

    if (hp == NULL)
	ValidAddr = FALSE;
    else
    {
	bzero((void *) &sock_addr, sizeof(struct sockaddr_in));
	bcopy((char *) hp->h_addr, (char *) &sock_addr.sin_addr, hp->h_length);
	sock_addr.sin_family      = AF_INET;
	sock_addr.sin_port        = htons(port);
    }
}

PortAddress::~PortAddress ()
{
}

void PortAddress::operator= (const PortAddress& Addr)
{
    ValidAddr = Addr.ValidAddr;

    if (Addr.ValidAddr)
    {
	sock_addr.sin_family      = Addr.sock_addr.sin_family;
        sock_addr.sin_port        = Addr.sock_addr.sin_port;
        sock_addr.sin_addr.s_addr = Addr.sock_addr.sin_addr.s_addr;
    }
}

Boolean PortAddress::operator== (const PortAddress& Addr) const
{
    Boolean res;

    if (ValidAddr != Addr.ValidAddr)
        res = FALSE;
    else if (ValidAddr == FALSE)
        res = TRUE;
    else 
	res = ((sock_addr.sin_family == Addr.sock_addr.sin_family) &&
               (sock_addr.sin_port == Addr.sock_addr.sin_port) &&
	       (sock_addr.sin_addr.s_addr == Addr.sock_addr.sin_addr.s_addr));

    return res;
}

Boolean PortAddress::pack (Buffer& Buff) const
{
    return (Buff.pack(ValidAddr) && Buff.pack(sock_addr.sin_family) && Buff.pack(ntohs(sock_addr.sin_port)) && Buff.pack(sock_addr.sin_addr.s_addr));
}

Boolean PortAddress::unpack (Buffer& Buff)
{
    Boolean res = (Boolean) (Buff.unpack(ValidAddr) && Buff.unpack(sock_addr.sin_family) && Buff.unpack(sock_addr.sin_port) && Buff.unpack(sock_addr.sin_addr.s_addr));

    sock_addr.sin_port = htons(sock_addr.sin_port);

    return res;
}

Boolean PortAddress::GetSockAddr (struct sockaddr_in& addr_sock) const
{
    Boolean res = TRUE;

    if (! ValidAddr)
        res = FALSE;
    else
    {
	addr_sock.sin_family      = sock_addr.sin_family;
        addr_sock.sin_port        = sock_addr.sin_port;
	addr_sock.sin_addr.s_addr = sock_addr.sin_addr.s_addr;
    }

    return res;
}

void PortAddress::SetSockAddr (struct sockaddr_in addr_sock)
{
    ValidAddr = TRUE;

    sock_addr.sin_family      = addr_sock.sin_family;
    sock_addr.sin_port        = addr_sock.sin_port;
    sock_addr.sin_addr.s_addr = addr_sock.sin_addr.s_addr;
}

Boolean PortAddress::GetHost(char*& host) const
{
    Boolean res = TRUE;

    if (! ValidAddr)
        res = FALSE;
    else
    {
	struct hostent *hp;
	hp = gethostbyaddr((const char*) &sock_addr.sin_addr, sizeof(sock_addr.sin_addr), AF_INET);

	if (hp == NULL)
	    res = FALSE;
	else
	{
	    if (host != NULL)
		delete host;

	    host = strdup(hp->h_name);
	}
    }

    return res;
}

Boolean PortAddress::GetPort(unsigned short& port) const
{
    port = sock_addr.sin_port;

    return ValidAddr;
}

Boolean PortAddress::Valid() const
{
    return ValidAddr;
}

ostream& PortAddress::print (ostream& strm) const
{
    strm << ValidAddr;
    strm << " " << sock_addr.sin_family;
    strm << " " << sock_addr.sin_port;
    strm << " " << sock_addr.sin_addr.s_addr;

    return strm;
}

ostream& operator<< (ostream& strm, const PortAddress& pa)
{
    return pa.print(strm);
}

