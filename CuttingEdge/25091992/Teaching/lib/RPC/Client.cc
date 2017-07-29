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

#include <iostream.h>

#include <RPC/Client.h>

Client::Client(char* service)
{
/**** ?????? ****/
}

Client::Client(PortAddress& portAddress)
{
    serverAddr = portAddress;
}

Client::~Client()
{
}

Client::OutcomeC Client::Call(Buffer& parameter, Buffer& result)
{
    Port::Outcome outcome;

    if (port.Valid() && serverAddr.Valid())
    {
	outcome = port.SendMessage(serverAddr, parameter);

	if (outcome == Port::Done)
	{
	    outcome = port.ReceiveMessage(serverAddr, result);
	    if (outcome == Port::NotDone)
		outcome = Port::Unknown;
	}

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
