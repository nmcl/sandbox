/*
 * Copyright 1998,
 * Computing Laboratory,
 * University of Newcastle upon Tyne,
 * UK.
 *
 * $Id: Ping.cc,v 1.1 1998/11/20 08:28:49 nmcl Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef PING_H_
#  include <RPC/SimpleRpc/Ping.h>
#endif

#ifndef PORT_H_
#  include "Port.h"
#endif

#ifndef PORTADDRESS_H_
#  include "PortAddress.h"
#endif

Ping::Ping (const char* host, unsigned short port)
	   : pingPort(new Port),
	     pingAddr(0)
{
    if (host == 0)
	pingAddr = new PortAddress(port);
    else
	pingAddr = new PortAddress(host, port);
}

Ping::Ping (const PortAddress& addr, unsigned short port)
	   : pingPort(new Port),
	     pingAddr(0)
{
    char* host = 0;

    addr.getHost(host);
    
    pingAddr = new PortAddress(host, port);

    if (host)
	::delete [] host;
}

Ping::Ping (const Ping& toCopy)
	   : pingPort(new Port),
	     pingAddr(0)
{
    if (toCopy.pingAddr)
	pingAddr = new PortAddress(*toCopy.pingAddr);
}

Ping::~Ping ()
{
    if (pingAddr)
	delete pingAddr;
    if (pingPort)
	delete pingPort;
}

Boolean Ping::getAddr (PortAddress& addr) const
{
    if (pingAddr)
    {
	addr = *pingAddr;
	return TRUE;
    }
    else
	return FALSE;
}

Boolean Ping::setAddr (const PortAddress& addr)
{
    if (pingAddr)
	*pingAddr = addr;
    else
	pingAddr = new PortAddress(addr);
}

Boolean Ping::isAlive ()
{
    int retry = PING_RETRY;
    Boolean alive = FALSE;
    Buffer* buf[1];
    Buffer request;

    request.pack(ARE_YOU_ALIVE);

    buf[0] = &request;
    
    do
    {
	if (pingPort->sendMessage(*pingAddr, buf, 1) == Port::DONE)
	{
	    int rNumb = 1;	    
	    Buffer* reply[1];
	    Buffer response;
	    PortAddress from;
	    
	    reply[0] = &response;

	    if (pingPort->receiveMessage(from, reply, rNumb, PING_WAIT) == Port::DONE)
	    {
		if (from == *pingAddr)  // not really interested in contents of message
		    return TRUE;
	    }
	}
    } while (retry-- > 0);

    return FALSE;
}

Boolean Ping::shutdown ()
{
    int retry = PING_RETRY;
    Boolean alive = FALSE;
    Buffer* buf[1];
    Buffer request;

    request.pack(TERMINATE_CODE);

    buf[0] = &request;
    
    do
    {
	if (pingPort->sendMessage(*pingAddr, buf, 1) == Port::DONE)
	{
	    int rNumb = 1;	    
	    Buffer* reply[1];
	    Buffer response;
	    PortAddress from;
	    
	    reply[0] = &response;

	    if (pingPort->receiveMessage(from, reply, rNumb, PING_WAIT) == Port::DONE)
	    {
		if ((from == *pingAddr) && (rNumb == 1))
		{
		    Boolean res;

		    response.unpack(res);

		    if (res)
			return TRUE;
		}
	    }
	}
    } while (retry-- > 0);

    return FALSE;    
}
