/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TheatreDispatcher.cc,v 1.1 1994/07/01 10:53:49 ngdp Exp $
 */

#include <Config/Configure.h>

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef THEATREDISPATCHER_H_
#  include "TheatreDispatcher.h"
#endif


TheatreDispatcher::TheatreDispatcher ()
{
}

TheatreDispatcher::~TheatreDispatcher ()
{
}

Dispatcher* TheatreDispatcher::create ()
{
    cout << "Creating new theatre" << endl;
    
    return new TheatreDispatcher;
}

Dispatcher::DispatcherStatus TheatreDispatcher::dispatch (Buffer* work[], int nWork,
							  Buffer* result[], int& nResult)
{
    int tmpCode, index = -1;
    TheatreOpcode opcode;
    Buffer resultBuffer;
    Boolean outcome = FALSE;

    if (nWork < 0)
    {
	cerr << "Invalid number of work buffers " << nWork << endl;
	return Dispatcher::NOTDONE;
    }

    if (nResult >= MAXBUFFERS)
    {
	cerr << "Invalid number of result buffers " << nResult << ". Cannot add work done." << endl;
	return Dispatcher::NOTDONE;
    }
    
    work[0]->unpack(tmpCode);
    opcode = (TheatreOpcode) tmpCode;
    work[1]->unpack(index);

    nResult++;
    result[0] = &resultBuffer;

    cout << "theatre opcode " << opcode << endl;
    
    switch (opcode)
    {
    case RESERVE:
	{
	    outcome = theRealObject.reserveSeat(index);
	    resultBuffer.pack(outcome);
	}
	break;
    case UNRESERVE:
	{
	    outcome = theRealObject.unreserveSeat(index);
	    resultBuffer.pack(outcome);
	}
	break;
    case INSPECT:
	{
	    int status;

	    outcome = theRealObject.inspectSeat(index, status);
	    resultBuffer.pack(outcome);
	    resultBuffer.pack(status);
	}
	break;
    default:
	{
	    cerr << "Error. Unknown opcode." << endl;
	    resultBuffer.pack(outcome);
	}
	break;
    }
    
    return Dispatcher::DONE;
}
