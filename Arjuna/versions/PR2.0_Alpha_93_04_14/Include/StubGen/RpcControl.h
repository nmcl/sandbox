/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RpcControl.h,v 1.4 1993/03/22 09:38:34 ngdp Exp $
 */

#ifndef RPCCONTROL_H_
#define RPCCONTROL_H_

/*
 *
 * Class to handle RPC system initiation and termination for stub
 * generated clients. Is inherited public virtual to ensure a single
 * instance exists regardless of the true inheritance graph
 *     
 */

class ClientRpc;
class RpcBuffer;

class RpcControl
{
public:
    RpcControl (ClientRpc * = 0);
    RpcControl (const char *);
    virtual ~RpcControl ();

    RPC_Status Call (long, RpcBuffer&, long&, RpcBuffer&);
    
private:
    int initiated;
    ClientRpc *rpc_handle;    
};

#endif
