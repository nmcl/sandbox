#include <iostream.h>

#include "MainBody.h"
#include "Client.h"
#include "Object.h"
#include "AC.h"
#include "Placement.h"

extern "C"
{
    int strcmp(char*, char*);
}

long qtime = 0;
Object* obj;
double NumberOfOperations = 0;
double OperationsPerformed = 0;

MainBody::MainBody(char* name) { policy = name; }

MainBody::~MainBody() {}

void MainBody::Body()
{
    struct timeval stime, ftime;
    double availability = 0.80;
    Scheduler* sc = new Scheduler();

    for (int i = 0; i < MAX_MACHINES; i++)
    {
	if ((i == 1) || (i == 4) || (i == 5) || (i == 7) || (i == 8))
	    Sites[i] = new Machine(i, 1000, 1);
	else
	    Sites[i] = new Machine(i, 100, 10);
    }

    Client* cl = new Client(1);

    AC* avcp = new AC(1, MAX_OBJECTS, 0);
    obj = new Object(avcp, MAX_OBJECTS);
    Placement* pl = new Placement();

    if (strcmp(policy, "Random") == 0)
	pl->RandomPlacementPolicy(Sites, obj);
    else
	pl->SimplePlacementPolicy(Sites, obj, availability, 0.2);

    for (int j = 0; j < MAX_MACHINES; j++)
	Sites[j]->Activate();

    cl->Activate();
    sc->Resume();

    gettimeofday(&stime, 0);
    while (OperationsPerformed < 10000)
    {
	Hold(100000);
	cerr << "OperationsPerformed " << OperationsPerformed << "\n";
    }

    sc->Suspend();
    cl->Suspend();

    gettimeofday(&ftime, 0);

    cerr << "Operations attempted " << NumberOfOperations << "\n";
    cerr << "Operations performed " << OperationsPerformed << "\n";
    cerr << "Availability is " << (OperationsPerformed / NumberOfOperations) << "\n";

    cerr << "Total time taken " << ((ftime.tv_sec*1000000)+ftime.tv_usec) - 
	((stime.tv_sec*1000000)+stime.tv_usec) << "\n";
    cerr << "Time spent in queue " << qtime << "\n";
}

void MainBody::Await()
{
    Resume();
    Thread::Self()->Suspend();
}

    
