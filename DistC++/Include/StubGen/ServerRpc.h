/*
 * Copyright (C) 1993-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ServerRpc.h,v 1.3 1998/11/20 08:28:41 nmcl Exp $
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

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef SIMPLERPCSERVER_H_
#  include <RPC/SimpleRpc/SimpleRpcServer.h>
#endif

#ifndef SERVERCONNECTION_H_
#  include <StubGen/ServerConnection.h>
#endif

class ServerRpc : public SimpleRpcServer
{
public:
    ServerRpc ();
    virtual ~ServerRpc ();

    int initialise (int, char**);
    
    ServerConnection* getWork (Int32&, Uid&, Buffer&);
};

#endif

