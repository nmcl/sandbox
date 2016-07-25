/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RpcControl.cc,v 1.1 1993/11/03 14:46:40 nmcl Exp $
 */

static const char RCSid[] = "$Id: RpcControl.cc,v 1.1 1993/11/03 14:46:40 nmcl Exp $";

/*
 * Class to handle RPC system initiation and termination for stub
 * generated clients. 
 *
 */

#include <strstream.h>

/*
 * Do NOT re-order the following includes
 */

#include <StubGen/ClientRpc.h>
#include <StubGen/RpcControl.h>
#include <StubGen/RpcBuffer.h>

/*
 * Private class that handles the actual RPC connection
 *
 */


class RpcImpl
{
public:
    RpcImpl(ClientRpc *, int);
    ~RpcImpl();
    RPC_Status Call (long, RpcBuffer&, long&, RpcBuffer&);

    void ref ();
    void unref ();
    
private:
    int initiated;
    int refCount;
    ClientRpc *rpcHandle;
    char doDelete;
};

/*
 * PUBLIC operations
 */

RpcControl::RpcControl ( ClientRpc *clientHandle, int byDefault )
                       : abortHandler(0),
			 deleteMe(byDefault),
			 theHandle(0)
{
    if (clientHandle == 0)
	theHandle = new RpcImpl(new ClientRpc("/tmp/server"), 1);
    else
	theHandle = new RpcImpl(clientHandle, 0);
}

RpcControl::RpcControl ( const RpcControl& other )
                       : deleteMe(0)
{
    abortHandler = other.abortHandler;
    theHandle = other.theHandle;
    theHandle->ref();
}

RpcControl::RpcControl ( const char *serverName, int byDefault )
                       : abortHandler(0),
			 deleteMe(byDefault),
			 theHandle(0)
{
    theHandle = new RpcImpl(new ClientRpc(serverName), 1);
}

RpcControl::~RpcControl ()
{
    if (theHandle)
	theHandle->unref();
}

void RpcControl::destroy ()
{
    if (deleteMe)
	delete this;
}

RPC_Status RpcControl::Call ( long  opcode, RpcBuffer& call,
			      long& errcode, RpcBuffer& result) const

{
    if (theHandle)
	return theHandle->Call(opcode, call, errcode, result);
    else
	return OPER_NOTDONE;
}

void RpcControl::rpcAbort () const
{
       if ((abortHandler == 0) || (*abortHandler)() == TRUE)
	   ::rpcAbort();
}

rpcAbortHandler RpcControl::setHandler ( rpcAbortHandler newHandler )
{
    rpcAbortHandler oldHandler = abortHandler;
    
    abortHandler = newHandler;
    return oldHandler;
}

RpcControl& RpcControl::operator= ( const RpcControl& other )
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
                 : doDelete(byDefault),
		   rpcHandle(clientHandle)
{
    refCount = 1;

    if (rpcHandle->Initiate() == OPER_DONE)
        initiated = 1;
}

RpcImpl::~RpcImpl ()
{
    if (initiated)
	rpcHandle->Terminate();

    if (doDelete)
	delete rpcHandle;
}

RPC_Status RpcImpl::Call ( long  opcode, RpcBuffer& call,
			   long& errcode, RpcBuffer& result)

{
    if (rpcHandle)
	return rpcHandle->Call(opcode, call, errcode, result);
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
#  define RPCCONTROL_CC_
#  include "StubGen/RpcControl.n"
#  undef RPCCONTROL_CC_
#endif
