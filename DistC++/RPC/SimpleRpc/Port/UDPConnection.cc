/*
 * Copyright 1999-1997, 1998,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 *
 * $Id: UDPConnection.cc,v 1.3 1998/11/20 08:28:44 nmcl Exp $
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

#ifndef PORT_H_
#  include "Port.h"
#endif

#ifndef PORTADDRESS_H_
#  include "PortAddress.h"
#endif

#ifndef UDPCONNECTION_H_
#  include "UDPConnection.h"
#endif

UDPConnection::UDPConnection (Port* p, const PortAddress& addr, Boolean canDelete)
			     : _port(p),
			       _addr(addr),
			       _connected(TRUE),
			       _canDelete(canDelete)
{
}

UDPConnection::UDPConnection (const UDPConnection& conn)
			     : _port(conn._port),
			       _addr(conn._addr),
			       _connected(TRUE),
			       _canDelete(FALSE)
{
}

UDPConnection::~UDPConnection ()
{
    if (_port && _canDelete)
	delete _port;
}

UDPConnection::Outcome UDPConnection::sendMessage ( Buffer* work[], int number )
{
    Port::POutcome res = Port::NOTDONE;

    cout << "sending reply" << endl;
    
    if (_port && _port->valid() && _connected)
    {
	res = _port->sendMessage(_addr, work, number);
    }

    cout << "reply sent" << endl;
    
    switch (res)
    {
    case Port::DONE:
	return DONE;
    case Port::NOTDONE:
	return NOTDONE;
    default:
	return UNKNOWN;
    }
}

/*
 * Return an error if this message is not from the address we were
 * given at creation time.
 *
 * Leave it to the caller to delete the buffers though.
 */

UDPConnection::Outcome UDPConnection::receiveMessage ( Buffer* result[],
						       int& number, long timeout)
{
    Port::POutcome res = Port::NOTDONE;

    if (_port && _port->valid() && _connected)
    {
	PortAddress from;
	
	res = _port->receiveMessage(from, result, number);

	if (from != _addr)
	    res = Port::NOTDONE;
    }

    switch (res)
    {
    case Port::DONE:
	return DONE;
    case Port::NOTDONE:
	return NOTDONE;
    default:
	return UNKNOWN;
    }
}

UDPConnection::Outcome UDPConnection::terminateConnection ()
{
    _connected = FALSE;

    return UDPConnection::DONE;
}

UDPConnection& UDPConnection::operator= (const UDPConnection& conn)
{
    if (this == &conn)
	return *this;
    else
    {
	_port = conn._port;
	_addr = conn._addr;
    }

    return *this;
}

PortAddress UDPConnection::getAddress () const
{
    return _addr;
}
