/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RpcControl.cc,v 1.3 1993/05/06 14:49:48 nsmw Exp $
 */

static const char RCSid[] = "$Id: RpcControl.cc,v 1.3 1993/05/06 14:49:48 nsmw Exp $";

/*
 * Class to handle RPC system initiation and termination for stub
 * generated clients. Is inherited public virtual to ensure a single
 * instance exists regardless of the true inheritance graph
 *
 */

#include <strstream.h>

/*
 * Do NOT re-order the following includes
 */

#include <StubGen/ClientRpc.h>
#include <StubGen/RpcControl.h>
#include <StubGen/RpcBuffer.h>

/*
 * PUBLIC operations
 */

RpcControl::RpcControl ( ClientRpc *client_handle )
                       : initiated(0),
			 rpc_handle(client_handle)
{
    if (rpc_handle == 0)
	rpc_handle = new ClientRpc("/tmp/server");

    if (rpc_handle->Initiate() == OPER_DONE)
	initiated = 1;
}

RpcControl::RpcControl ( const char *server_name )
                       : initiated(0),
			 rpc_handle(0)
{
    rpc_handle = new ClientRpc(server_name);

    if (rpc_handle->Initiate() == OPER_DONE)
	initiated = 1;
}

RpcControl::~RpcControl ()
{
    if (initiated)
	rpc_handle->Terminate();

    delete rpc_handle;
}

RPC_Status RpcControl::Call ( long  opcode, RpcBuffer& call,
			      long& errcode, RpcBuffer& result)

{
    if (rpc_handle)
	return rpc_handle->Call(opcode, call, errcode, result);
    else
	return OPER_NOTDONE;
}

