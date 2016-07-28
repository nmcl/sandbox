/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClientRpc.cc,v 1.4 1995/02/22 14:55:41 ngdp Exp $
 */

static const char RCSid[] = "$Id: ClientRpc.cc,v 1.4 1995/02/22 14:55:41 ngdp Exp $";

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#include <StubGen/ClientRpc.h>

#if RPC_SYSTEM_TYPE == RPC_RajdootAction_
#define PARENT ClientRajdootAction_rpc
ClientRpc::ClientRpc (const char* const sname) : ClientRajdootAction_rpc(sname)
#endif
#if RPC_SYSTEM_TYPE == RPC_Rajdoot_
#define PARENT ClientRajdoot_rpc
ClientRpc::ClientRpc (const char* const sname) : ClientRajdoot_rpc(sname)
#endif
{
}

ClientRpc::~ClientRpc ()
{
}

RPC_Status ClientRpc::call ( Int32 opcode, Buffer& call, Int32& errcode,
			     Buffer& result )

{
    if (call.readState() == BUFFER_GOOD)
	return PARENT::call(opcode,call,errcode,result);
    else
	return OPER_NOTDONE;
}
