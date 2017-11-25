/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ServerRpc.h,v 1.4 1995/02/22 14:55:31 ngdp Exp $
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

#if RPC_SYSTEM_TYPE == RPC_RajdootAction_
#   ifndef RAJDOOT_SVRAJACT_H_
#       include <RPC/Rajdoot/SvRajAct.h>
#   endif
#endif

#if RPC_SYSTEM_TYPE == RPC_Rajdoot_
#   ifndef RAJDOOT_SVRAJ_H_
#       include <RPC/Rajdoot/SvRaj.h>
#   endif
#endif

#if RPC_SYSTEM_TYPE == RPC_RajdootAction_
class ServerRpc : public ServerRajdootAction_rpc
#endif
#if RPC_SYSTEM_TYPE == RPC_Rajdoot_
class ServerRpc : public ServerRajdoot_rpc
#endif
{
public:
    ServerRpc ();
    ~ServerRpc ();
};
#endif

