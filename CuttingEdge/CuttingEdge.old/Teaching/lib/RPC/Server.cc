/*
 * Copyright 1990, 1991, 1992,
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
 * $Id$
 */

#include <stdlib.h>
#include <iostream.h>

#include <RPC/Server.h>

Server::Server(PortAddress& portAddress) : port(portAddress)
{
}

Server::Server(int argc, char *argv[])
{
    if (argc == 3)
    {
	PortAddress creatorAddr(argv[1], atoi(argv[2]));

	Buffer buff;

	port.SendMessage(creatorAddr, buff);
    }
}

Server::~Server()
{
}

Server::OutcomeS Server::GetWork(Buffer& parameter)
{
    Port::Outcome outcome;

    if (port.Valid())
    {
	outcome = port.ReceiveMessage(clientAddr, parameter);

	if (outcome == Port::Done)
	    return Done;
	else if (outcome == Port::NotDone)
	    return NotDone;
	else
	    return Unknown;
    }
    else
	return NotDone;
}

Server::OutcomeS Server::SendResult(Buffer& result)
{
    Port::Outcome outcome;

    if (port.Valid() && clientAddr.Valid())
    {
	outcome = port.SendMessage(clientAddr, result);

	if (outcome == Port::Done)
	    return Done;
	else if (outcome == Port::NotDone)
	    return NotDone;
	else
	    return Unknown;
    }
    else
	return NotDone;
}
