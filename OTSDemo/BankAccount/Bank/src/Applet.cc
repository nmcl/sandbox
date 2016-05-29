/*
 * Copyright (C) 1996, 1997,
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

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#include <TCPConnection.h>

#ifndef OPCODE_H_
#  include "Opcode.h"
#endif

#ifndef APPLET_H_
#  include "Applet.h"
#endif

/*
 * Public constructors and destructors
 */

Applet::Applet (char* id, TCPConnection* port, Bank::Outcome outcome)
	       : tid(0),
		 _port(port),
		 cashAmount(-1),
		 _result(outcome),
		 errorStatus(NO_ERROR)
{
    tid = ::new char[::strlen(id)+1];
    ::strcpy(tid, id);
}

Applet::Applet (TCPConnection* port, ErrorStatus err)
	       : tid(0),
		 _port(port),
		 cashAmount(-1),
		 _result(Bank::UNKNOWN),
		 errorStatus(err)
{
}

Applet::~Applet ()
{
    if (tid)
	::delete [] tid;
}

Boolean Applet::abort ()
{
    return transmitOutcome(ABORT);
}

Boolean Applet::commit (int cash)
{
    cashAmount = cash;
    return transmitOutcome(COMMIT);
}

Boolean Applet::prepare ()
{
    return transmitOutcome(PREPARE);
}

Boolean Applet::transmitOutcome (BankActionStatus status)
{
    Boolean res = FALSE;
    Buffer buff1, buff2;
    Buffer* work[2];
    int number = 0;
    
    /*
     * Send back money regardless of operation!
     * We send 2 buffers for commit/abort - the status and the result.
     */

    if (buff1.pack(status) && buff1.packNonMappedString(tid))
    {
	res = TRUE;
	
	if (status == COMMIT)
	{
	    res = buff2.pack(_result) && buff2.pack(cashAmount);
	    number = 2;
	    work[0] = &buff1;
	    work[1] = &buff2;
	}
	else
	{
	    if (status == ABORT)
	    {
		if (errorStatus != NO_ERROR)
		    res = buff2.pack(errorStatus);
		else
		    res = buff2.pack(_result);
		
		number = 2;
		work[0] = &buff1;
		work[1] = &buff2;
	    }
	    else
	    {
		number = 1;  // only one buffer for prepare
		work[0] = &buff1;
	    }
	}

	if (res)
	{
	    res = FALSE;

	    if (_port->sendMessage(work, number) == TCPConnection::DONE)
	    {
		Buffer* result[1];
		Buffer buff3;

		result[0] = &buff3;
		number = 0;
		
		/*
		 * We're not really bothered about what the reply is, only
		 * that we get one.
		 */

		if (_port->receiveMessage(result, number) == TCPConnection::DONE)
		    res = TRUE;
	    }
	}
    }

    return res;
}
