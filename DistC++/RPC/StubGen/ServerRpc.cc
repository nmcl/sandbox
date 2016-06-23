/*
 * Copyright (C) 1993-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ServerRpc.cc,v 1.3 1998/11/20 08:28:52 nmcl Exp $
 */

static const char RCSid[] = "$Id: ServerRpc.cc,v 1.3 1998/11/20 08:28:52 nmcl Exp $";

#include <StubGen/ServerRpc.h>
#include <StubGen/ServerConnection.h>

ServerRpc::ServerRpc () : SimpleRpcServer()
{
}

ServerRpc::~ServerRpc ()
{
}

int ServerRpc::initialise ( int argc, char **argv )
{
    return SimpleRpcServer::initialise(argc,argv);
}

ServerConnection* ServerRpc::getWork ( Int32& opcode, Uid& objID, Buffer& work )
{
    Buffer opBuff;
    Buffer* recBuff[2];
    int number = 2;
    SimpleServerConnection* res = (SimpleServerConnection*) 0;

    recBuff[0] = &opBuff;
    recBuff[1] = &work;

    res = SimpleRpcServer::getWork(recBuff, number);

    if (res)
    {
	objID.unpack(opBuff);
	opBuff.unpack(opcode);

	return new ServerConnection(res);
    }
    else
	return 0;
}
