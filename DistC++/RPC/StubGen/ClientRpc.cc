/*
 * Copyright (C) 1993-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClientRpc.cc,v 1.4 1998/11/20 08:28:51 nmcl Exp $
 */

static const char RCSid[] = "$Id: ClientRpc.cc,v 1.4 1998/11/20 08:28:51 nmcl Exp $";

#include <System/iostream.h>

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#include <StubGen/ClientRpc.h>

ClientRpc::ClientRpc (ProtocolType p)
		     : SimpleRpcClient(p)
{
}

ClientRpc::ClientRpc ( const char* sname, ProtocolType p )
		     : SimpleRpcClient(sname, p)
{
}

ClientRpc::ClientRpc (ClientRpc* doner)
		     : SimpleRpcClient(doner)
{
}

ClientRpc::~ClientRpc ()
{
}

RPC_Status ClientRpc::initiate ()
{
    return SimpleRpcClient::initiate();
}

RPC_Status ClientRpc::terminate ()
{
    return SimpleRpcClient::terminate();
}

RPC_Status ClientRpc::call ( const Uid& objID, Int32 opcode, Buffer& call,
			     Int32& errcode, Buffer& result )
{
    if (call.readState() == BUFFER_GOOD)
    {
	Buffer *sendBuffer[2];
	Buffer *recBuffer[2];
        Buffer opBuff, errBuff;
        int number = 2;
        RPC_Status res;

	objID.pack(opBuff);
        opBuff.pack(opcode);

        sendBuffer[0] = &opBuff;
        sendBuffer[1] = &call;
        recBuffer[0] = &errBuff;
        recBuffer[1] = &result;

	cout << "call" << endl;
	
	res = SimpleRpcClient::call(sendBuffer, 2, recBuffer, number);

	cout << "end of call" << endl;
	
	if ((res == OPER_DONE) && (number == 2))
            errBuff.unpack(errcode);

        return res;
    }
    else
	return OPER_NOTDONE;
}
