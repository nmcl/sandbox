/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClientRpc_.cc,v 1.3 1993/05/06 14:49:39 nsmw Exp $
 */

/*
 * $Id: ClientRpc_.cc,v 1.3 1993/05/06 14:49:39 nsmw Exp $
 */

#include <StubGen/ClientRpc.h>

#if RPC_SYSTEM_TYPE == RPC_RajdootAction_
ClientRpc::ClientRpc (ArjunaName* AN) : ClientRajdootAction_rpc(AN)
#endif
#if RPC_SYSTEM_TYPE == RPC_Rajdoot_
ClientRpc::ClientRpc (ArjunaName* AN) : ClientRajdoot_rpc(AN)
#endif
#if RPC_SYSTEM_TYPE == RPC_ANSAAction_
ClientRpc::ClientRpc (ArjunaName* AN) : ClientANSAAction_rpc(AN)
#endif
#if RPC_SYSTEM_TYPE == RPC_ANSA_
ClientRpc::ClientRpc (ArjunaName* AN) : ClientANSA_rpc(AN)
#endif
{
}
