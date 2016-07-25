/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClientRpc_.cc,v 1.1 1993/11/03 14:46:34 nmcl Exp $
 */

/*
 * $Id: ClientRpc_.cc,v 1.1 1993/11/03 14:46:34 nmcl Exp $
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
