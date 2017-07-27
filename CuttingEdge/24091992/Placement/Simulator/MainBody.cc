#include <iostream.h>

#include "MainBody.h"
#include "Client.h"
#include "Object.h"
#include "AC.h"
#include "Voting.h"
#include "Placement.h"
#include "Machine.h"

extern "C"
{
    int strcmp(char*, char*);
    int atoi(char*);
}

Object* obj;
double NumberOfOperations = 0;
double OperationsPerformed = 0;

MainBody::MainBody (char* name, char* tocreate, char* m, char* s, char* av, char* n)
{
    if (tocreate)
    {
	NumberOfObjects = atoi(tocreate);
	if (NumberOfObjects > MAX_OBJECTS)
	    NumberOfObjects = MAX_OBJECTS;
	mean = (double) atoi(m);
	sd = (double) atoi(s);
	availability = (double) (atoi(av))/100;
	NumberOfGoodNodes = atoi(n);
	if (NumberOfGoodNodes > MAX_MACHINES)
	    NumberOfGoodNodes = MAX_MACHINES;
    }
    else
    {
	NumberOfObjects = MAX_OBJECTS;
	mean = 2000;
	sd = 40;
	availability = 0.70;
	NumberOfGoodNodes = 5;
    }

    NS = new NormalStream(mean, sd);
    policy = name;
}

MainBody::~MainBody () {}

void MainBody::Body ()
{
    Scheduler* sc = new Scheduler();
    UniformStream* US = new UniformStream(0, 9);
    boolean GoodMachines[MAX_MACHINES];
    int i;

    for (i = 0; i < MAX_MACHINES; i++)
	GoodMachines[i] = false;
	
    i = 0;
    do
    {
	int l = (int) (*US)();
	if (!GoodMachines[l])
	{
	    GoodMachines[l] = true;
	    i++;
	}
    } while (i < NumberOfGoodNodes);

    for (i = 0; i < MAX_MACHINES; i++)
    {
	if (GoodMachines[i])
	    Sites[i] = new Machine(i, (*NS)(), (*NS)()/1000);
	else
	    Sites[i] = new Machine(i, (*NS)()/8, (*NS)()/250);
    }

    Client* cl = new Client(1);

    Voting* qc = new Voting(NumberOfObjects, NumberOfObjects, 0);
    AC* avcp = new AC(1, NumberOfObjects, 0);
    obj = new Object(avcp, NumberOfObjects);

    Placement* pl = new Placement();

    cout << "Group availability = " << availability << "\n" << flush;

    if (strcmp(policy, "Random") == 0)
	pl->RandomPlacementPolicy(Sites, obj, NumberOfObjects, availability);
    else
	pl->SimplePlacementPolicy(Sites, obj, NumberOfObjects, availability, 0.2);

    for (int j = 0; j < MAX_MACHINES; j++)
	Sites[j]->Activate();

    cl->Activate();
    sc->Resume();

#ifndef FAST
    while (OperationsPerformed < 10000)
#else
    while (OperationsPerformed < 1000)
#endif
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

    
