/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ServerRpc.h,v 1.5 1993/03/22 09:38:36 ngdp Exp $
 */

#ifndef SERVERRPC_H_
#define SERVERRPC_H_

/*
 *
 * Class to handle server side of RPC system
 *     
 */
#ifndef CONFIGURE_H_
#  include <Configure.h>
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

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_
#   ifndef ANSA_SVANSAACT_H_
#       include <RPC/ANSA/SvANSAAct.h>
#   endif
#endif

#if RPC_SYSTEM_TYPE == RPC_ANSA_
#   ifndef ANSA_SVANSA_H_
#       include <RPC/ANSA/SvANSA.h>
#   endif
#endif

#if RPC_SYSTEM_TYPE == RPC_RajdootAction_
class ServerRpc : public ServerRajdootAction_rpc
#endif
#if RPC_SYSTEM_TYPE == RPC_Rajdoot_
class ServerRpc : public ServerRajdoot_rpc
#endif
#if RPC_SYSTEM_TYPE == RPC_ANSAAction_
class ServerRpc : public ServerANSAAction_rpc
#endif
#if RPC_SYSTEM_TYPE == RPC_ANSA_
class ServerRpc : public ServerANSA_rpc
#endif
{
public:
    ServerRpc ();
    ~ServerRpc ();
};
#endif

