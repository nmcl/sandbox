/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClientRpc.h,v 1.8 1995/02/22 14:55:30 ngdp Exp $
 */

#ifndef CLIENTRPC_H_
#define CLIENTRPC_H_

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
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

#ifndef CLSTATUS_H_
#  include <RPC/ClStatus.h>
#endif

#ifndef SVRERRCODE_H_
#  include <StubGen/SvrErrCode.h>
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

class Buffer;

#if RPC_SYSTEM_TYPE == RPC_RajdootAction_
class ClientRpc : public ClientRajdootAction_rpc
#endif
#if RPC_SYSTEM_TYPE == RPC_Rajdoot_
class ClientRpc : public ClientRajdoot_rpc
#endif
{
public:
    ClientRpc (ArjunaName* AN);
    ClientRpc (const char* const sname = 0);
    ~ClientRpc ();

    RPC_Status call (Int32, Buffer&, Int32&, Buffer&);

};
#endif

