/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClientRpcMan.h,v 1.5 1995/01/20 10:18:01 ngdp Exp $
 */

#ifndef CLIENTRPCMAN_H_
#define CLIENTRPCMAN_H_

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef ABORT_H_
#  include <StubGen/Abort.h>
#endif

#ifndef CLSTATUS_H_
#  include <RPC/ClStatus.h>
#endif

/*
 *
 * Class to handle RPC system initiation and termination for stub
 * generated clients. 
 *     
 */

class ClientRpc;
class RpcBuffer;
class RpcImpl;

class ClientRpcManager
{
public:
    ClientRpcManager (ClientRpc * = 0, int = 0);
    ClientRpcManager (const ClientRpcManager&);
    ClientRpcManager (const char *const, int = 0);
    virtual ~ClientRpcManager ();

    static ClientRpcManager* createInstance(const char* const, int = 0);
    void destroy ();

    ClientRpc *const getRpcHandle () const;
    RPC_Status call (Int32, RpcBuffer&, Int32&, RpcBuffer&) const;
    
    void rpcAbort (RPC_Status, Int32) const;
    rpcAbortHandler setHandler(rpcAbortHandler);

    ClientRpcManager& operator= (const ClientRpcManager&);

private:
    rpcAbortHandler abortHandler;
    int deleteMe;
    RpcImpl *theHandle;
};

#include "ClientRpcMan.n"

#endif
