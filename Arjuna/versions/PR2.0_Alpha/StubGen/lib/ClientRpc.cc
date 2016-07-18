/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClientRpc.cc,v 1.4 1993/05/06 14:49:38 nsmw Exp $
 */

static const char RCSid[] = "$Id: ClientRpc.cc,v 1.4 1993/05/06 14:49:38 nsmw Exp $";

#include <StubGen/ClientRpc.h>
#if RPC_SYSTEM_TYPE == RPC_RajdootAction_
ClientRpc::ClientRpc (const char* sname) : ClientRajdootAction_rpc(sname)
#endif
#if RPC_SYSTEM_TYPE == RPC_Rajdoot_
ClientRpc::ClientRpc (const char* sname) : ClientRajdoot_rpc(sname)
#endif
#if RPC_SYSTEM_TYPE == RPC_ANSAAction_
ClientRpc::ClientRpc (const char* sname) : ClientANSAAction_rpc(sname)
#endif
#if RPC_SYSTEM_TYPE == RPC_ANSA_
ClientRpc::ClientRpc (const char* sname) : ClientANSA_rpc(sname)
#endif
{
}

ClientRpc::~ClientRpc ()
{
}
