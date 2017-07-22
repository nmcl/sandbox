#include <iostream.h>

#include "Client.h"

extern Object* obj;
extern double NumberOfOperations;
extern double OperationsPerformed;

Client::Client (long id)
{
    ClientID = id;
    ArrivalTime = new ExponentialStream(100);
    ProcessTime = new ExponentialStream(20);
    WaitTime = new UniformStream(1, 5);
}

Client::~Client ()
{
    delete ArrivalTime;
    delete ProcessTime;
}

void Client::MakeCall (lock_code locktype)
{
    boolean done = false;

    while (!done)
    {
	if (locktype != unlocked)
	    NumberOfOperations++;
	if (!obj->PerformOperation(locktype, this))
	    Hold((*WaitTime)());
	else
	{
	    done = true;
	    if (locktype != unlocked)
		OperationsPerformed++;
	}
    }
}

void Client::Body ()
{
    double x, holdtime;
    boolean finished;

    for (;;)
    {
	Hold((*ArrivalTime)());

	holdtime = (*ProcessTime)();
	x = holdtime + CurrentTime();

#ifdef DEBUG
	cout << "Client " << ClientID << " making call\n" << flush;
#endif

	MakeCall(write);
	
	finished = false;
	while (!finished)
	{
	    Hold(holdtime);

#ifdef DEBUG
	    cout << "Client " << ClientID << " woken up\n" << flush;
#endif

	    if (x != CurrentTime())
	    {
		if (!obj->CheckQuorum(write))
		{
#ifdef DEBUG
		    cout << "Client " << ClientID << " aborting\n" << flush;
#endif

		    MakeCall(unlocked);
		    finished = true;
		}
		else
		    holdtime = x - CurrentTime();
	    }
	    else
	    {
		MakeCall(unlocked);
		finished = true;
	    }
	}
    }
}
