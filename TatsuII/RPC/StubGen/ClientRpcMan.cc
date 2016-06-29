/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClientRpcMan.cc,v 1.1 1997/06/09 19:52:06 nmcl Exp $
 */

static const char RCSid[] = "$Id: ClientRpcMan.cc,v 1.1 1997/06/09 19:52:06 nmcl Exp $";

/*
 * Class to handle RPC system initiation and termination for stub
 * generated clients. 
 *
 */

#include <System/strstream.h>

/*
 * Do NOT re-order the following includes
 */

#include <StubGen/ClientRpc.h>
#include <StubGen/ClientRpcMan.h>
#include <StubGen/RpcBuffer.h>

/*
 * Private class that handles the actual RPC connection
 *
 */


class RpcImpl
{
public:
    RpcImpl (ClientRpc *, int);
    ~RpcImpl ();
    RPC_Status call (Int32, RpcBuffer&, Int32&, RpcBuffer&);

    void ref ();
    void unref ();
    
    ClientRpc *const getRpcHandle () const;
    
private:
    int initiated;
    int refCount;
    ClientRpc *rpcHandle;
    char doDelete;
};

/*
 * PUBLIC operations
 */

ClientRpcManager::ClientRpcManager ( ClientRpc *clientHandle, int byDefault )
				   : abortHandler(0),
				     deleteMe(byDefault),
				     theHandle(0)
{
    if (clientHandle == 0)
	theHandle = new RpcImpl(new ClientRpc("/tmp/server"), 1);
    else
	theHandle = new RpcImpl(clientHandle, 0);
}

ClientRpcManager::ClientRpcManager ( const ClientRpcManager& other )
				   : deleteMe(0)
{
    abortHandler = other.abortHandler;
    theHandle = other.theHandle;
    theHandle->ref();
}

ClientRpcManager::ClientRpcManager ( const char * const serverName,
				     int byDefault )
				   : abortHandler(0),
				     deleteMe(byDefault),
				     theHandle(0)
{
    theHandle = new RpcImpl(new ClientRpc(serverName), 1);
}

ClientRpcManager::~ClientRpcManager ()
{
    if (theHandle)
	theHandle->unref();
}

void ClientRpcManager::destroy ()
{
    if (deleteMe)
	delete this;
}

ClientRpc *const ClientRpcManager::getRpcHandle () const
{
    if (theHandle)
	return theHandle->getRpcHandle();
    else
	return 0;
}

RPC_Status ClientRpcManager::call ( Int32  opcode, RpcBuffer& call,
				    Int32& errcode, RpcBuffer& result) const

{
    if (theHandle)
	return theHandle->call(opcode, call, errcode, result);
    else
	return OPER_NOTDONE;
}

void ClientRpcManager::rpcAbort ( RPC_Status rpcStat, Int32 svrStat ) const
{
       if ((abortHandler == 0) || (*abortHandler)(rpcStat,svrStat) == TRUE)
	   ::rpcAbort(rpcStat,svrStat);
}

rpcAbortHandler ClientRpcManager::setHandler ( rpcAbortHandler newHandler )
{
    rpcAbortHandler oldHandler = abortHandler;
    
    abortHandler = newHandler;
    return oldHandler;
}

ClientRpcManager& ClientRpcManager::operator= ( const ClientRpcManager& other )
{
    /* watch for rc = rc */

    if (this == &other)
	return *this;
    
    abortHandler = other.abortHandler;
    theHandle->unref();
    theHandle = other.theHandle;
    theHandle->ref();
    
    return *this;
}

RpcImpl::RpcImpl ( ClientRpc *clientHandle, int byDefault )
                 : initiated(0),
		   refCount(1),
		   rpcHandle(clientHandle),
		   doDelete(byDefault)
{
    if (rpcHandle->initiate() == OPER_DONE)
        initiated = 1;
}

RpcImpl::~RpcImpl ()
{
    if (initiated)
	rpcHandle->terminate();

    if (doDelete)
	delete rpcHandle;
}

ClientRpc *const RpcImpl::getRpcHandle () const
{
    return rpcHandle;
}

RPC_Status RpcImpl::call ( Int32  opcode, RpcBuffer& call,
			   Int32& errcode, RpcBuffer& result)

{
    if (initiated && (rpcHandle))
	return rpcHandle->call(opcode, call, errcode, result);
    else
	return OPER_NOTDONE;
}

void RpcImpl::ref ()
{
    refCount++;
}

void RpcImpl::unref ()
{
    refCount--;
    if (refCount == 0)
	delete this;
}

#ifdef NO_INLINES
#  define CLIENTRPCMAN_CC_
#  include "StubGen/ClientRpcMan.n"
#  undef CLIENTRPCMAN_CC_
#endif
