/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClientRpc_.cc,v 1.2 1995/02/22 14:55:42 ngdp Exp $
 */

/*
 * $Id: ClientRpc_.cc,v 1.2 1995/02/22 14:55:42 ngdp Exp $
 */

#include <StubGen/ClientRpc.h>

#if RPC_SYSTEM_TYPE == RPC_RajdootAction_
ClientRpc::ClientRpc (ArjunaName* AN) : ClientRajdootAction_rpc(AN)
#endif
#if RPC_SYSTEM_TYPE == RPC_Rajdoot_
ClientRpc::ClientRpc (ArjunaName* AN) : ClientRajdoot_rpc(AN)
#endif
{
}
