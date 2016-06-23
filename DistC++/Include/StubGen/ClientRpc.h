/*
 * Copyright (C) 1993-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClientRpc.h,v 1.3 1998/11/20 08:28:41 nmcl Exp $
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
    ClientRpc (ProtocolType = DGRAM);
    ClientRpc (const char* sname, ProtocolType = DGRAM);
    ClientRpc (ClientRpc*);
    virtual ~ClientRpc ();
    
    virtual RPC_Status initiate ();
    virtual RPC_Status terminate ();
    
    virtual RPC_Status call (const Uid&, Int32, Buffer&, Int32&, Buffer&);
};
#endif

