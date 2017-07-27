/*
 * Copyright 1990, 1991,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 *
 * Permission to use, copy, modify and distribute this program for
 * evaluation, teaching and/or research purposes only and without fee is
 * hereby granted, providing that this copyright and permission notice
 * appear on all copies and supporting documentation, and that similar
 * conditions are imposed on any individual or organization to whom the
 * program is distributed. The University of Newcastle upon Tyne makes no
 * representation about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 */

/*
 * $Id: ClientRPC.cc,v 1.2 1992/01/28 11:48:20 nmcl Exp $
 */

#include <iostream.h>

extern "C"
{
    int strlen(const char*);
}

#include <ClientRPC.h>

ClientRPC::ClientRPC(const char* serviceName)
{
    if (serviceName != 0)
    {
	service = (char*) malloc(strlen(serviceName));
	bcopy(serviceName, service, strlen(serviceName));
    }
    else
	service = 0;

    char *hostname = 0;
    ServerAddr = new PortAddress;
    ClientAddr = new PortAddress(0);
    Manager = new ClientManager("ulgham");
}

ClientRPC::~ClientRPC()
{
    delete service;
    delete Manager;
    delete ClientAddr;
    delete ServerAddr;
}

RPC_Status ClientRPC::Initiate()
{
    if (service == 0)
    {
	perror("INITIATE: null service name ");
	return Oper_NotDone;
    }

    if (Manager->Create_Server(service, *ClientAddr, *ServerAddr) != Done)
	return Oper_NotDone;
    else
	return Oper_Done;
}

RPC_Status ClientRPC::Call(long  opcode,  Buffer& call, long& errcode, Buffer& result)
{
    Buffer Buff1, Buff2;
    Port CommsPort(*ClientAddr);

    Buff1.pack(opcode);
    Buff1.buffer_pack(call);
    ClientAddr->pack(Buff1);

    if (CommsPort.SendMessage(*ServerAddr, Buff1) != Done)
	return Oper_NotDone;

    if (CommsPort.ReceiveMessage(*ServerAddr, Buff2) != Done)
	return Oper_NotDone;

    Buff2.unpack(errcode);
    Buff2.buffer_unpack(result);

    return Oper_Done;
}

RPC_Status ClientRPC::Terminate()
{
    // Not done yet.
}
