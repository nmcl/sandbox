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
 *  $Id: ClientManager.cc,v 1.2 1992/01/28 11:48:19 nmcl Exp $
 */

#include <iostream.h>

#include <Manager.h>
#include <ClientManager.h>

ClientManager::ClientManager(char* hostname) { ManagerAddr = new PortAddress(hostname, MANAGER_PORT); }

ClientManager::~ClientManager() { delete ManagerAddr; }

PortOutcome ClientManager::Create_Server(const char* service, PortAddress& ClientAddr, PortAddress& ServerAddr)
{
    Buffer Buff1, Buff2;
    Port CommsPort, ServerPort(ClientAddr);

    if (!CommsPort.Valid())
    {
	perror("MANAGER: Valid CommsPort ");
	return NotDone;
    }

    Buff1.pack(Do_Create_Server);
    Buff1.pack(service);
    ClientAddr.pack(Buff1);

    if (CommsPort.SendMessage(*ManagerAddr, Buff1) != Done)
    {
	perror("MANAGER: SendMessage ");
	return NotDone;
    }

    if (ServerPort.ReceiveMessage(ServerAddr, Buff2) != Done)
    {
	perror("SERVER: ReceiveMessage ");
	return NotDone;
    }
    else
	ServerAddr.unpack(Buff2);

    return Done;
}

PortOutcome ClientManager::Quit()
{
    Buffer Buff1;
    Port CommsPort;

    if (!CommsPort.Valid())
    {
	perror("MANAGER: Valid CommsPort ");
	return NotDone;
    }

    Buff1.pack(Do_Quit);
    if (CommsPort.SendMessage(*ManagerAddr, Buff1) != Done)
    {
	perror("MANAGER: SendMessage to Quit ");
	return NotDone;
    }
    else
	return Done;
}
