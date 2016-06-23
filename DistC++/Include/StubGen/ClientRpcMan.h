/*
 * Copyright (C) 1993-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClientRpcMan.h,v 1.3 1998/11/20 08:28:41 nmcl Exp $
 */

#ifndef CLIENTRPCMAN_H_
#define CLIENTRPCMAN_H_

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
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
    ClientRpcManager (ClientRpc * = 0, int = 0, const Uid& objID = NIL_UID);
    ClientRpcManager (const char *const, int = 0, const Uid& objID = NIL_UID);
    ClientRpcManager (const ClientRpcManager&);
    virtual ~ClientRpcManager ();

    static ClientRpcManager* createInstance (const char* const, int = 0);

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
