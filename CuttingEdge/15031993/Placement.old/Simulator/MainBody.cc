/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef MAINBODY_H_
#include "MainBody.h"
#endif

#ifndef CLIENT_H_
#include "Client.h"
#endif

#ifndef OBJECT_H_
#include "Object.h"
#endif

#ifndef AC_H_
#include "AC.h"
#endif

#ifndef VOTING_H_
#include "Voting.h"
#endif

#ifndef PLACEMENT_H_
#include "Placement.h"
#endif

#ifndef MACHINE_H_
#include "Machine.h"
#endif


extern "C"
{
    int strcmp(const char*, const char*);
    int atoi(char*);
    double atof(char*);
    void _exit(int);
}

Object* obj;
double NumberOfOperations = 0;
double OperationsPerformed = 0;

MainBody::MainBody (int argc, char** argv)
{
    // Process args
    policy = argv[1];

    if (strcmp(argv[2], "AC") == 0)
	UseAC = true;
    else
	UseAC = false;

    if (argc > 3)
    {
	NumberOfObjects = atoi(argv[3]);
	if (NumberOfObjects > MAX_OBJECTS)
	    NumberOfObjects = MAX_OBJECTS;
	mean = (double) atoi(argv[4]);
	sd = (double) atoi(argv[5]);
	availability = atof(argv[6]);
    }
    else
    {
	NumberOfObjects = MAX_OBJECTS;
	mean = 2000.0;
	sd = 600.0;
	availability = 0.99;
    }

    // Create a Normal Distribution fn from which to draw MTTF/MTTR statistics
    int ss=0;
    if (argc>7) ss = atoi(argv[7]);
    NS = new NormalStream(mean, sd, ss);
}

MainBody::~MainBody () {}

void MainBody::Body ()
{
    Scheduler* sc = new Scheduler();
    int i;

    // Generate machines with randomly selected MTTF and MTTR stats
    // Note we assume MTTR is approx 1% of MTTF
    for (i = 0; i < MAX_MACHINES; i++)
    {
	Sites[i] = new Machine(i, (*NS)(), (*NS)() * 0.01);
    }

    // Create a client to run the transactions
    Client* cl = new Client(1);

    // Generate the replica consistency protocol managers
    Voting* qc = new Voting(NumberOfObjects, NumberOfObjects, 0);
    AC* avcp = new AC(1, NumberOfObjects, 0);

    // Select a protocol
    if (UseAC)
    {
	// use available copies
	obj = new Object(avcp, NumberOfObjects);
    }
    else
    {
	// use quorum concensus (majority voting)
	obj = new Object(qc, NumberOfObjects);
    }

    // create a placement
    Placement* pl = new Placement();

    cout << "Group availability = " << availability << "\n" << flush;

    if (strcmp(policy, "Random") == 0)
	pl->RandomPlacementPolicy(Sites, obj, NumberOfObjects, availability);
    else
	pl->SimplePlacementPolicy(Sites, obj, NumberOfObjects, availability, 2);

    for (int j = 0; j < MAX_MACHINES; j++)
	Sites[j]->Activate();

    cout << "starting client\n" << flush;
    cl->Activate();
    sc->Resume(); // Not a Process, so must call Resume directly

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
	cerr << "Operations performed " << OperationsPerformed << "\n";
    }

    sc->Suspend();
    cl->Suspend();

    cerr << "Total operations attempted " << NumberOfOperations << "\n";
    cerr << "Total operations completed " << OperationsPerformed << "\n";
    cout << policy << '\t' 
	 << (UseAC?"AC":"QC") << '\t' 
	 << availability << '\t' 
	 << obj->WriteQuorum() << '\t'
	 << (OperationsPerformed / NumberOfOperations) << "\t" 
	 << obj->rw_ratio << "\n"
	 << flush;
    _exit(0);
}

void MainBody::Await ()
{
    Resume();
    Thread::Self()->Suspend();
}
