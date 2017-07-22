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
 * $Id: ServerRPC.cc,v 1.2 1992/01/28 11:48:25 nmcl Exp $
 */

#include <iostream.h>

#include <ServerRPC.h>

ServerRPC::ServerRPC()
{
    Manager = new ServerManager;
    ClientAddr = new PortAddress;
    ServerAddr = new PortAddress(0);
}

ServerRPC::~ServerRPC()
{
    delete Manager;
    delete ClientAddr;
    delete ServerAddr;
}

boolean ServerRPC::Initialise(int argc, char *argv[])
{
    Manager->Initial_Server(argc, argv, *ClientAddr);
    Port CommsPort(*ServerAddr);

    if (!CommsPort.Valid())
    {
	perror("Invalid CommsPort ");
	return false;
    }

    Buffer Buff1;
    ServerAddr->pack(Buff1);

    if (CommsPort.SendMessage(*ClientAddr, Buff1) != Done)
    {
	perror("SendMessage ");
	return false;
    }
    else
	return true;
}

void ServerRPC::GetWork(long& opcode, Buffer& call)
{
}

void ServerRPC::GetWork(long& opcode, Buffer& call1, Buffer& call2)
{
}

void ServerRPC::SendResult(long errcode, Buffer& result)
{
}

void ServerRPC::SendResult(long errcode, Buffer& result1, Buffer& result2)
{
}
