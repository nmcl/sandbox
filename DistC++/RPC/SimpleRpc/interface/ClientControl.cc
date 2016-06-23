/*
 * Copyright 1990, 1991, 1992-1998,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 */

/*
 * $Id: ClientControl.cc,v 1.4 1998/11/20 08:28:49 nmcl Exp $
 */

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef WIN32

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef SIMPLERPCCLIENT_H_
#  include <RPC/SimpleRpc/SimpleRpcClient.h>
#endif

#ifndef CLIENTCONTROL_H_
#  include <RPC/SimpleRpc/ClientControl.h>
#endif

#ifndef THREADMUTEX_H_
#  include <Thread/ThreadMutex.h>
#endif

const char* defaultServer = "server";
char* ClientControl::myHost = 0;


ClientControl::ClientControl ()
			     : service(0),
			       hostname(0),
			       objectID(NIL_UID),
			       timeout(DEFAULT_TIMEOUT),
			       retry(DEFAULT_RETRY),
			       itTimeout(INITTERM_TIMEOUT),
			       itRetry(INITTERM_RETRY),
			       burstSize(BURST_SIZE),
			       burstTimeout(BURST_TIMEOUT),
			       protocol(DGRAM),
			       port(0),
			       _mutex(ThreadMutex::create())
{
    if (ClientControl::myHost == 0)
    {
	ClientControl::myHost = ::new char[MAXHOSTNAMELEN+1];
	::memset(ClientControl::myHost, '\0', MAXHOSTNAMELEN+1);
	::gethostname(ClientControl::myHost, MAXHOSTNAMELEN);
    }

    setServiceName(defaultServer);
    setHost(ClientControl::myHost);
}

ClientControl::ClientControl (const ClientControl& toCopy)
			     : service(0),
			       hostname(0),
			       serverID(toCopy.serverID),
			       objectID(toCopy.objectID),
			       timeout(toCopy.timeout),
			       retry(toCopy.retry),
			       itTimeout(toCopy.itTimeout),
			       itRetry(toCopy.itRetry),
			       burstSize(toCopy.burstSize),
			       burstTimeout(toCopy.burstTimeout),
			       protocol(toCopy.protocol),
			       port(toCopy.port),
			       _mutex(ThreadMutex::create())
{
    if (toCopy.service != 0)
    {
	service = ::new char[::strlen(toCopy.service)+1];
	::strcpy(service, toCopy.service);
    }

    if (toCopy.hostname != 0)
    {
	hostname = ::new char[::strlen(toCopy.hostname)+1];
	::strcpy(hostname, toCopy.hostname);
    }
}

ClientControl::~ClientControl ()
{
    if (service != 0)
    {
	::delete [] service;
	service = 0;
    }

    if (hostname != 0)
    {
	::delete [] hostname;
	hostname = 0;
    }

    if (_mutex)
	delete _mutex;
}

ClientControl& ClientControl::operator= (const ClientControl& toCopy)
{
    if (this == &toCopy)
	return *this;
    else
    {
	if (service != 0)
	{
	    ::delete [] service;
	    service = 0;
	}

	if (hostname != 0)
	{
	    ::delete [] hostname;
	    hostname = 0;
	}
	
	if (toCopy.service != 0)
	{
	    service = ::new char[::strlen(toCopy.service)+1];
	    ::strcpy(service, toCopy.service);
	}

	if (toCopy.hostname != 0)
	{
	    hostname = ::new char[::strlen(toCopy.hostname)+1];
	    ::strcpy(hostname, toCopy.hostname);
	}
	
	serverID = toCopy.serverID;
	objectID = toCopy.objectID;
	timeout = toCopy.timeout;
	retry = toCopy.retry;
	itTimeout = toCopy.itTimeout;
	itRetry = toCopy.itRetry;
	burstSize = toCopy.burstSize;
	burstTimeout = toCopy.burstTimeout;
	protocol = toCopy.protocol;
	port = toCopy.port;
    }
}

Boolean ClientControl::valid () const
{
    Boolean res = FALSE;

    if (_mutex->lock())
    {
	if ((!service) || (!hostname))
	    res = FALSE;
	else
	    res = TRUE;

	_mutex->unlock();
    }

    return res;
}

RPC_Status ClientControl::setProtocol ( ProtocolType p )
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {
	protocol = p;
	res = OPER_DONE;

	_mutex->unlock();
    }

    return res;
}

RPC_Status ClientControl::getProtocol ( ProtocolType& p ) const
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {
	p = protocol;
	res = OPER_DONE;

	_mutex->unlock();
    }

    return res;
}

RPC_Status ClientControl::setServerID ( const Uid& id )
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	serverID = id;
	res = OPER_DONE;

	_mutex->unlock();
    }

    return res;
}

RPC_Status ClientControl::getServerID ( Uid& id ) const
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	id = serverID;
	res = OPER_DONE;

	_mutex->unlock();
    }

    return res;
}

RPC_Status ClientControl::setPort (unsigned short p)
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {
	port = p;
	res = OPER_DONE;

	_mutex->unlock();
    }

    return res;
}

RPC_Status ClientControl::getPort (unsigned short& p) const
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {
	if (port != 0)
	{
	    p = port;
	    res = OPER_DONE;
	}

	_mutex->unlock();
    }

    return res;
}

RPC_Status ClientControl::setObjectUid ( const Uid& id )
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	objectID = id;
	res = OPER_DONE;

	_mutex->unlock();
    }

    return res;
}

RPC_Status ClientControl::getObjectUid ( Uid& id ) const
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	id = objectID;
	res = OPER_DONE;

	_mutex->unlock();
    }

    return res;
}

RPC_Status ClientControl::setServiceName ( const char* serv )
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	if (serv)
	{
	    if (service)
		::delete [] service;
    
	    service = ::new char[::strlen(serv)+1];
	    ::memset(service, '\0', ::strlen(serv)+1);
	    ::memcpy(service, serv, ::strlen(serv));
	    res = OPER_DONE;
	}

	_mutex->unlock();
    }

    return res;    
}

RPC_Status ClientControl::getServiceName ( char*& serv ) const
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	if (service)
	{
	    serv = ::new char[::strlen(service)+1];
	    ::memset(serv, '\0', ::strlen(service)+1);
	    ::memcpy(serv, service, ::strlen(service));
	    res = OPER_DONE;
	}

	_mutex->unlock();
    }

    return res;    
}

RPC_Status ClientControl::setHost ( const char* host )
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	if (host)
	{
	    if (hostname)
		::delete [] hostname;

	    hostname = ::new char[::strlen(host)+1];
	    ::memset(hostname, '\0', ::strlen(host)+1);
	    ::memcpy(hostname, host, ::strlen(host));
	    res = OPER_DONE;
	}

	_mutex->unlock();
    }

    return res;    
}

RPC_Status ClientControl::getHost ( char*& host ) const
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	if (hostname)
	{
	    host = ::new char[::strlen(hostname)+1];
	    ::memset(host, '\0', ::strlen(hostname)+1);
	    ::memcpy(host, hostname, ::strlen(hostname));
	    res = OPER_DONE;
	}

	_mutex->unlock();
    }

    return res;    
}

RPC_Status ClientControl::setTimeout ( long t_out )
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	timeout = t_out;
	res = OPER_DONE;

	_mutex->unlock();
    }

    return res;    
}

RPC_Status ClientControl::getTimeout ( long& t_out ) const
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	t_out = timeout;
	res = OPER_DONE;

	_mutex->unlock();
    }

    return res;    
}

RPC_Status ClientControl::setInitTermTimeout ( long t )
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	itTimeout = t;
	res = OPER_DONE;

	_mutex->unlock();
    }

    return res;    
}

RPC_Status ClientControl::getInitTermTimeout ( long& t ) const
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	t = itTimeout;
	res = OPER_DONE;

	_mutex->unlock();
    }

    return res;    
}

RPC_Status ClientControl::setInitTermRetry ( long r )
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	itRetry = r;
	res = OPER_DONE;

	_mutex->unlock();
    }

    return res;    
}

RPC_Status ClientControl::getInitTermRetry ( long& r ) const
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	r = itRetry;
	res = OPER_DONE;

	_mutex->unlock();
    }

    return res;    
}

RPC_Status ClientControl::setRetry ( long r )
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	retry = r;
	res = OPER_DONE;

	_mutex->unlock();
    }

    return res;    
}

RPC_Status ClientControl::getRetry ( long& r ) const
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	r = retry;
	res = OPER_DONE;

	_mutex->unlock();
    }

    return res;    
}

RPC_Status ClientControl::setBurstTimeout (long b)
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	burstTimeout = b;
	res = OPER_DONE;

	_mutex->unlock();
    }

    return res;    
}

RPC_Status ClientControl::getBurstTimeout (long& b) const
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	b = burstTimeout;
	res = OPER_DONE;

	_mutex->unlock();
    }

    return res;    
}

RPC_Status ClientControl::setBurstSize (long b)
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	burstSize = b;
	res = OPER_DONE;

	_mutex->unlock();
    }

    return res;    
}

RPC_Status ClientControl::getBurstSize (long& b) const
{
    RPC_Status res = OPER_NOTDONE;

    if (_mutex->lock())
    {    
	b = burstSize;
	res = OPER_DONE;

	_mutex->unlock();
    }

    return res;    
}
