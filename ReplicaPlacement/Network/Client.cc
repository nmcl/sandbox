/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef CLIENT_H_
#include "Client.h"
#endif

extern Object* obj;
extern double NumberOfOperationsCompleted;
extern lock_code lockToAcquire;

Client::Client (long id)
                       : ClientID(id),
			 NumberOfOperations(0.0),
			 OperationsPerformed(0.0),
			 primary(-1)
{
    ArrivalTime = new ExponentialStream(10.0); // time between operations
    ProcessTime = new ExponentialStream(1.48); // time for operation
    OperationType = new Draw(0.2); // chance that operation is write
}

Client::~Client ()
{
    delete ArrivalTime;
    delete ProcessTime;
    delete OperationType;
}

double Client::OperationsAttempted () { return NumberOfOperations; }

double Client::OperationsCompleted () { return OperationsPerformed; }

boolean Client::MakeCall (lock_code locktype)
{
    Opcode result;
    
    if (locktype != unlocked)
	NumberOfOperations++;

    result = obj->PerformOperation(locktype, this);

    if ((result == lockconflict) || (result == failed))
	return false;
    else
    {
	if (locktype != unlocked)
	{
	    OperationsPerformed++;
	    NumberOfOperationsCompleted++;
	}
	
	return true;
    }
}

void Client::Body ()
{
    double x, holdtime;
    lock_code lockas;
    boolean finished;

    for (;;)
    {
	Hold((*ArrivalTime)());

	holdtime = (*ProcessTime)();

#ifdef DEBUG
	cout << "Client " << ClientID << " holding for " << holdtime << "\n" << flush;
#endif

	x = holdtime + CurrentTime();

#ifdef RANDOP
	if ((*OperationType)())
	{
#ifdef DEBUG
	    cout << "Client " << ClientID << " making read operation\n" << flush;
#endif
	    lockas = read;
	}
	else
	{
#ifdef DEBUG
	    cout << "Client " << ClientID << " making write operation\n" << flush;
#endif
	    lockas = write;
	}
#else
	lockas = lockToAcquire;
#endif
	
	if (MakeCall(lockas))
	{
	    finished = false;

	    while (!finished)
	    {
		Hold(holdtime);

#ifdef DEBUG
		cout << "Client " << ClientID << " woken up\n" << flush;
#endif

		if (x != CurrentTime())
		{
		    if (!obj->CheckQuorum(lockas, this))
		    {
#ifdef DEBUG
			cout << "Client " << ClientID << " aborting\n" << flush;
#endif
			(void) MakeCall(unlocked);
			finished = true;
		    }
		    else
			holdtime = x - CurrentTime();
		}
		else
		{
#ifdef DEBUG
		    cout << "Client " << ClientID << " successfully completed call\n" << flush;
#endif

		    (void) MakeCall(unlocked);
		    finished = true;
		}
	    }
	}
    }
}
