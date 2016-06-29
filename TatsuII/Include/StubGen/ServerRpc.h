/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ServerRpc.h,v 1.1 1997/06/09 19:51:57 nmcl Exp $
 */

#ifndef SERVERRPC_H_
#define SERVERRPC_H_

/*
 *
 * Class to handle server side of RPC system
 *     
 */

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#ifndef SVRERRCODE_H_
#  include <StubGen/SvrErrCode.h>
#endif

#ifndef STUBGEN_TYPES_H_
#  include <StubGen/Types.h>
#endif

#ifndef SIMPLERPCSERVER_H_
#  include <RPC/SimpleRpc/SimpleRpcServer.h>
#endif

class ServerRpc : public SimpleRpcServer
{
public:
    ServerRpc ();
    ~ServerRpc ();

    int initialise (int, char**);
    void getWork (Int32&, Buffer&);
    void sendResult (Int32, Buffer&);
};
#endif

