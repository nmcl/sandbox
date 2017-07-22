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
 *  $Id: ServerManager.cc,v 1.2 1992/01/28 11:48:23 nmcl Exp $
 */

#include <iostream.h>

#include <Manager.h>
#include <ServerManager.h>

ServerManager::ServerManager() {}

ServerManager::~ServerManager() { delete ManagerAddr; }

PortOutcome ServerManager::Initial_Server(int argc, char* argv[], PortAddress& Client)
{
    int portnum1 = 0, portnum2 = 0;
    PortAddress* C_Addr;

    if (argc < 4)
    {
	perror("SERVERMANAGER: Too few initialization parameters ");
	return NotDone;
    }

    char *hostname1 = argv[1], *hostname2 = argv[3];
    char *portname1 = argv[2], *portname2 = argv[4];

    sscanf(portname1, "%d", &portnum1);
    sscanf(portname2, "%d", &portnum2);

    ManagerAddr = new PortAddress(hostname2, (unsigned short) portnum2);
    C_Addr = new PortAddress(hostname1, (unsigned short) portnum1);
    Client = *C_Addr;
}

PortOutcome ServerManager::Quit()
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
