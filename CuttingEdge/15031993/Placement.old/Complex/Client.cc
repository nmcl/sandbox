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
			 ResponseTime(0.0),
			 intervalOperations(0.0),
			 totalRT(0.0),
			 intervalRT(0.0)
{
    ArrivalTime = new ExponentialStream(10.0); // time between operations
    ProcessTime = new ExponentialStream(1.48); // time for operation
    WaitTime = new UniformStream(1.0, 4.0); // time between retries
    OperationType = new Draw(0.2); // chance that operation is write
}

Client::~Client ()
{
    delete ArrivalTime;
    delete ProcessTime;
}

double Client::GetResponseTime ()
{
    double tempVal = (ResponseTime/intervalOperations);
    totalRT += tempVal;
    intervalRT++;
    ResponseTime = 0.0;
    intervalOperations = 0.0;

#ifdef AVERAGE
    return (totalRT/intervalRT);
#else
    return tempVal;
#endif
}

double Client::OperationsAttempted () { return NumberOfOperations; }

double Client::OperationsCompleted () { return OperationsPerformed; }

boolean Client::MakeCall (lock_code locktype)
{
    Opcode result;
    
    if (locktype != unlocked)
	NumberOfOperations++;

    for (;;)
    {
	result = obj->PerformOperation(locktype, this);

	if (result == lockconflict)
	{
#ifdef DEBUG
	    cout << "Cannot perform operation due to lock conflict\n" << flush;
#endif
	    NumberOfOperations--;
	    intervalOperations++;
	    Hold((*WaitTime)());
	    NumberOfOperations++;
	}
	else
	    if (result == failed)
		return false;
	else
	{
	    if (locktype != unlocked)
	    {
#ifdef DEBUG
		cout << "\nobjects in use by client " << ClientID << " " << obj->GetReplicasInUse(this) << endl;
		cout << "object load " << obj->GetLoad(this) << endl;
#endif
		ResponseTime += obj->GetReplicasInUse(this) * obj->GetLoad(this);
		intervalOperations++;
#ifdef DEBUG
		cout << "interval operations " << intervalOperations << endl;
#endif
		OperationsPerformed++;
		NumberOfOperationsCompleted++;
	    }
	    
	    return true;
	}
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
