/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ServerRpc.cc,v 1.4 1995/07/18 10:12:51 ngdp Exp $
 */

static const char RCSid[] = "$Id: ServerRpc.cc,v 1.4 1995/07/18 10:12:51 ngdp Exp $";

#include <StubGen/ServerRpc.h>

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

void ServerRpc::getWork ( Int32& opcode, Buffer& work )
{
    Buffer opBuff;
    Buffer* sendBuff[2];
    int number = -1;
    SimpleRpcServer::SOutcome res;

    sendBuff[0] = &opBuff;
    sendBuff[1] = &work;

    res = SimpleRpcServer::getWork(sendBuff, number);

    if (res == DONE)
	opBuff.unpack(opcode);
    
}

void ServerRpc::sendResult ( Int32 errcode, Buffer& result )
{
    Buffer errBuff;
    Buffer* recBuff[2];
    int number = 2;
    SimpleRpcServer::SOutcome res;

    errBuff.pack(errcode);
    
    recBuff[0] = &errBuff;
    recBuff[1] = &result;

    res = SimpleRpcServer::sendResult(recBuff, number);
}
