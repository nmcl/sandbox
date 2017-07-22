#include <iostream.h>

#include "Client.h"

extern Object* obj;
extern double NumberOfOperations;
extern double OperationsPerformed;

Client::Client (long id)
{
    rate = 200;
    ClientID = id;
    ArrivalTime = new ExponentialStream(rate);
    ProcessTime = new ExponentialStream(rate*0.05);
    WaitTime = new UniformStream(1, 5);
    OperationType = new Draw(0.2);
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
	{
#ifdef DEBUG
	    cout << "Cannot perform operation\n" << flush;
#endif
	    Hold((*WaitTime)());
	}
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
	lockas = write;
#endif

	MakeCall(lockas);
	finished = false;
	while (!finished)
	{
	    Hold(holdtime);

#ifdef DEBUG
	    cout << "Client " << ClientID << " woken up\n" << flush;
#endif

	    if (x != CurrentTime())
	    {
		if (!obj->CheckQuorum(lockas))
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
#ifdef DEBUG
		cout << "Client " << ClientID << " successfully completed call\n" << flush;
#endif

		MakeCall(unlocked);
		finished = true;
	    }
	}
    }
}
