/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id$
 */

#include <System/errno.h>
#include <System/unistd.h>
#include <System/sys/param.h>
#include <System/protos.h>
#include <System/netdb.h>
#include <System/sys/socket.h>
#include <System/netinet/in.h>
#include <System/string.h>
#include <System/iostream.h>
#include <System/fstream.h>
#include <System/memory.h>
#include <System/stdio.h>

#if defined(DEBUG ) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef OPCODE_H_
#  include "Opcode.h"
#endif

#ifndef APPLET_H_
#  include "Applet.h"
#endif

/*
 * Public constructors and destructors
 */

Applet::Applet (ReliablePort& port, int outcome)
	       : _port(port),
		 cashAmount(-1),
		 _result(outcome)
{
    // cout << "Creating Applet proxy." << endl;
}

Applet::~Applet ()
{
}

Boolean Applet::abort ()
{
    return transmitOutcome(OP_ABORT);
}

Boolean Applet::commit (int cash)
{
    cashAmount = cash;
    return transmitOutcome(OP_COMMIT);
}

Boolean Applet::prepare ()
{
    return transmitOutcome(OP_PREPARE);
}

Boolean Applet::transmitOutcome (int outcome)
{
    Boolean res = FALSE;
    Buffer buff1, buff2;
    Buffer* work[2];
    int number = 0;
    
    /*
     * Send back money regardless of operation!
     * We send 2 buffers for commit/abort - the outcome and the result.
     */

    if (buff1.pack(outcome))
    {
	if (outcome == OP_COMMIT)
	{
	    res = buff2.pack(_result) && buff2.pack(cashAmount);
	    number = 2;
	    work[0] = &buff1;
	    work[1] = &buff2;
	}
	else
	{
	    if (outcome == OP_ABORT)
	    {
		res = buff2.pack(_result);
		number = 2;
		work[0] = &buff1;
		work[1] = &buff2;
	    }
	    else
	    {
		res = TRUE;
		number = 1;  // only one buffer for prepare
		work[0] = &buff1;
	    }
	}

	if (res)
	{
	    cout << "Sending message to applet." << endl;
	    
	    if (_port.sendMessage(work, number) == ReliablePort::DONE)
	    {
		Buffer* result[1];
		Buffer buff3;

		result[0] = &buff3;
		number = 0;
		
		/*
		 * We're not really bothered about what the reply is, only
		 * that we get one.
		 */

		cout << "Receiving reply from applet." << endl;
		
		if (_port.receiveMessage(result, number) == ReliablePort::DONE)
		    res = TRUE;
	    }
	}
    }
    
    return res;
}
