/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClientRpc.h,v 1.1 1993/11/03 14:32:07 nmcl Exp $
 */

#ifndef CLIENTRPC_H_
#define CLIENTRPC_H_

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif

#ifdef sun

/*
 *  Pre-include setjmp.h and multicast.h to get around cpp include
 *  stack limit on suns.
 *
 */

#if RPC_SYSTEM_TYPE == RPC_RajdootAction_ ||  RPC_SYSTEM_TYPE == RPC_Rajdoot_
#    ifndef RAJDOOT_MULTICAST_H_
#      include <RPC/Rajdoot/multicast.h>
#    endif
#endif

#  ifndef SETJMP_H_
#    include <System/setjmp.h>
#  endif

#endif

/*
 *
 * Class to handle client side of RPC system
 *     
 */

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#if RPC_SYSTEM_TYPE == RPC_RajdootAction_
#   ifndef RAJDOOT_CLRAJACT_H_
#       include <RPC/Rajdoot/ClRajAct.h>
#   endif
#endif

#if RPC_SYSTEM_TYPE == RPC_Rajdoot_
#   ifndef RAJDOOT_CLRAJ_H_
#       include <RPC/Rajdoot/ClRaj.h>
#   endif
#endif

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_
#   ifndef ANSA_CLANSAACT_H_
#       include <RPC/ANSA/ClANSAAct.h>
#   endif
#endif

#if RPC_SYSTEM_TYPE == RPC_ANSA_
#   ifndef ANSA_CLANSA_H_
#       include <RPC/ANSA/ClANSA.h>
#   endif
#endif

#if RPC_SYSTEM_TYPE == RPC_RajdootAction_
class ClientRpc : public ClientRajdootAction_rpc
#endif
#if RPC_SYSTEM_TYPE == RPC_Rajdoot_
class ClientRpc : public ClientRajdoot_rpc
#endif
#if RPC_SYSTEM_TYPE == RPC_ANSAAction_
class ClientRpc : public ClientANSAAction_rpc
#endif
#if RPC_SYSTEM_TYPE == RPC_ANSA_
class ClientRpc : public ClientANSA_rpc
#endif
{
public:
    ClientRpc (ArjunaName* AN);
    ClientRpc (const char* sname = NULL);
    ~ClientRpc ();
};
#endif

