/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClientRpc.h,v 1.3 1994/12/12 14:59:24 ngdp Exp $
 */

#ifndef CLIENTRPC_H_
#define CLIENTRPC_H_

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

/*
 *
 * Class to handle client side of RPC system
 *     
 */

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef CLSTATUS_H_
#  include <RPC/ClStatus.h>
#endif

#ifndef STUBGEN_TYPES_H_
#  include <StubGen/Types.h>
#endif

#ifndef SVRERRCODE_H_
#  include <StubGen/SvrErrCode.h>
#endif

#ifndef SIMPLERPCCLIENT_H_
#  include <RPC/SimpleRpc/SimpleRpcClient.h>
#endif

class Buffer;

class ClientRpc : public SimpleRpcClient
{
public:
    ClientRpc ();
    ClientRpc (const char* sname = 0);
    ~ClientRpc ();

    RPC_Status initiate ();
    RPC_Status call (Int32, Buffer&, Int32&, Buffer&);
    virtual RPC_Status terminate ();
};
#endif

