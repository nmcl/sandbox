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

Object* obj;
double NumberOfOperations = 0;
double OperationsPerformed = 0;

MainBody::MainBody (char* name) { policy = name; }

MainBody::~MainBody () {}

void MainBody::Body ()
{
    double availability = 0.80;
    Scheduler* sc = new Scheduler();

    for (int i = 0; i < MAX_MACHINES; i++)
    {
	if ((i == 1) || (i == 4) || (i == 5) || (i == 7) || (i ==8))
	    Sites[i] = new Machine(i, 100, 1);
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

    while (OperationsPerformed < 10000)
    {
#ifndef DEBUG
	Hold(100000);
#else
	Hold(1000);
#endif
	cerr << "OperationsPerformed " << OperationsPerformed << "\n";
    }

    sc->Suspend();
    cl->Suspend();

    cerr << "Operations attempted " << NumberOfOperations << "\n";
    cerr << "Operations performed " << OperationsPerformed << "\n";
    cerr << "Availability is " << (OperationsPerformed / NumberOfOperations) << "\n";
}

void MainBody::Await ()
{
    Resume();
    Thread::Self()->Suspend();
}

    
