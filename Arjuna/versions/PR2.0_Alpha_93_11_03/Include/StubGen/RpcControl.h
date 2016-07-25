/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RpcControl.h,v 1.1 1993/11/03 14:32:13 nmcl Exp $
 */

#ifndef RPCCONTROL_H_
#define RPCCONTROL_H_

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

class RpcControl
{
public:
    RpcControl (ClientRpc * = 0, int = 0);
    RpcControl (const RpcControl&);
    RpcControl (const char *, int = 0);
    virtual ~RpcControl ();

    static RpcControl* createInstance(const char*, int = 0);
    void destroy ();

    RPC_Status Call (long, RpcBuffer&, long&, RpcBuffer&) const;
    
    void rpcAbort () const;
    rpcAbortHandler setHandler(rpcAbortHandler);

    RpcControl& operator= (const RpcControl&);

private:
    rpcAbortHandler abortHandler;
    int deleteMe;
    class RpcImpl *theHandle;
};

#include "RpcControl.n"

#endif
