/*
 * Copyright 1990, 1991, 1992,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: PortAddress.cc,v 1.1 1997/06/09 19:52:05 nmcl Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef SYS_TYPES_H_
# include <System/sys/types.h>
#endif

#ifndef SYS_SOCKET_H_
#  include <System/sys/socket.h>
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

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#include "PortAddress.h"

PortAddress::PortAddress ()
{
    validAddr = FALSE;
}

PortAddress::PortAddress ( unsigned short port )
{
    validAddr = TRUE;
    char localhostname[MAXHOSTNAMELEN];

    if (::gethostname(localhostname, MAXHOSTNAMELEN) != 0)
	validAddr = FALSE;
    else
    {
	struct hostent *hp;
	hp = ::gethostbyname(localhostname);

	if (hp == NULL)
	    validAddr = FALSE;
	else
	{
	    ::memset((void *) &sockAddr, '\0', sizeof(struct sockaddr_in));
	    ::memcpy((char *) &sockAddr.sin_addr, (char *) hp->h_addr, hp->h_length);
	    sockAddr.sin_family = AF_INET;
	    sockAddr.sin_port = htons(port);
	}
    }
}

PortAddress::PortAddress ( const char* host, unsigned short port )
{
    validAddr = TRUE;

    struct hostent *hp;
    hp = gethostbyname(host);

    if (hp == NULL)
	validAddr = FALSE;
    else
    {
	::memset((void *) &sockAddr, '\0', sizeof(struct sockaddr_in));
	::memcpy((char *) &sockAddr.sin_addr, (char *) hp->h_addr, hp->h_length);
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(port);
    }
}

PortAddress::~PortAddress ()
{
}

void PortAddress::operator= ( const PortAddress& Addr )
{
    validAddr = Addr.validAddr;

    if (Addr.validAddr)
    {
	sockAddr.sin_family      = Addr.sockAddr.sin_family;
        sockAddr.sin_port        = Addr.sockAddr.sin_port;
        sockAddr.sin_addr.s_addr = Addr.sockAddr.sin_addr.s_addr;
    }
}

Boolean PortAddress::operator== ( const PortAddress& Addr ) const
{
    Boolean res;

    if (validAddr != Addr.validAddr)
        res = FALSE;
    else if (validAddr == FALSE)
        res = TRUE;
    else 
	res = ((sockAddr.sin_family == Addr.sockAddr.sin_family) &&
               (sockAddr.sin_port == Addr.sockAddr.sin_port) &&
	       (sockAddr.sin_addr.s_addr == Addr.sockAddr.sin_addr.s_addr));

    return res;
}

Boolean PortAddress::operator!= (const PortAddress& Addr) const
{
    return ((*this == Addr) ? FALSE : TRUE);
}

Boolean PortAddress::pack ( Buffer& Buff ) const
{
    return (Buff.pack(validAddr) && Buff.pack(sockAddr.sin_family) &&
            Buff.pack(ntohs(sockAddr.sin_port)) &&
	    Buff.pack(ntohl(sockAddr.sin_addr.s_addr)));
}

Boolean PortAddress::unpack ( Buffer& Buff )
{
    Boolean res = (Boolean) (Buff.unpack(validAddr) && Buff.unpack(sockAddr.sin_family) &&
                             Buff.unpack(sockAddr.sin_port) && Buff.unpack(sockAddr.sin_addr.s_addr));

    sockAddr.sin_port = htons(sockAddr.sin_port);
    sockAddr.sin_addr.s_addr = htonl(sockAddr.sin_addr.s_addr);

    return res;
}

Boolean PortAddress::getSockAddr ( struct sockaddr_in& addr_sock ) const
{
    Boolean res = TRUE;

    if (! validAddr)
        res = FALSE;
    else
    {
	addr_sock.sin_family      = sockAddr.sin_family;
        addr_sock.sin_port        = sockAddr.sin_port;
	addr_sock.sin_addr.s_addr = sockAddr.sin_addr.s_addr;
    }

    return res;
}

void PortAddress::setSockAddr ( struct sockaddr_in addr_sock )
{
    validAddr = TRUE;

    sockAddr.sin_family      = addr_sock.sin_family;
    sockAddr.sin_port        = addr_sock.sin_port;
    sockAddr.sin_addr.s_addr = addr_sock.sin_addr.s_addr;
}

Boolean PortAddress::getHost ( char*& host ) const
{
    Boolean res = TRUE;

    if (! validAddr)
        res = FALSE;
    else
    {
	struct hostent *hp;
	hp = gethostbyaddr((const char*) &sockAddr.sin_addr, sizeof(sockAddr.sin_addr), AF_INET);

	if (hp == NULL)
	    res = FALSE;
	else
	{
	    if (host != NULL)
		delete host;

	    host = ::strdup(hp->h_name);
	}
    }

    return res;
}

Boolean PortAddress::getPort ( unsigned short& port ) const
{
    port = ntohs(sockAddr.sin_port);

    return validAddr;
}

Boolean PortAddress::valid() const
{
    return validAddr;
}

ostream& PortAddress::print ( ostream& strm ) const
{
    if (validAddr)
    {
       strm << "<PortAddress:" << sockAddr.sin_family
            << ',' << ntohs(sockAddr.sin_port)
            << ',' << ntohl(sockAddr.sin_addr.s_addr) << '>';
    }
    else
       strm << "<PortAddress:Invalid>";

    return strm;
}

ostream& operator<< ( ostream& strm, const PortAddress& pa )
{
    return pa.print(strm);
}

