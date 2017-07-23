/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>
#include <fstream.h>
#include <memory.h>

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

#ifndef PASSIVE_H_
#include "Passive.h"
#endif

#ifndef SINGLECOPY_H_
#include "SingleCopy.h"
#endif

#ifndef RANDOMPOLICY_H_
#include "RandomPolicy.h"
#endif

#ifndef MACHINE_H_
#include "Machine.h"
#endif


extern "C"
{
    void sprintf(char*, char*, int);
    int strcmp(const char*, const char*);
    int atoi(char*);
    double atof(char*);
    void _exit(int);
}

Machine* Sites[MAX_MACHINES];
Object* obj = 0;

lock_code lockToAcquire = unlocked;
int NumberOfClients = 0;
int NumberAllowed = 0;
double NumberOfOperationsCompleted = 0.0;

MainBody::MainBody (int argc, char** argv)
{
    if (strcmp(argv[1], "AC") == 0)
	toUse = active;
    else
	if (strcmp(argv[1], "Voting") == 0)
	    toUse = voting;
    else
	if (strcmp(argv[1], "Passive") == 0)
	    toUse = passive;
    else
	toUse = single;

    if (argc > 2)
    {
	NumberAllowed = atoi(argv[2]);
	if (NumberAllowed > MAX_OBJECTS)
	    NumberAllowed = MAX_OBJECTS;
	mean = (double) atoi(argv[3]);
	sd = (double) atoi(argv[4]);
	NumberOfClients = atoi(argv[5]);

	if (NumberOfClients > MaxClients)
	{
	    cerr << "Error - too many clients. Will use maximum allowed < " << MaxClients << " >" << endl;
	    NumberOfClients = MaxClients;
	}
	if (strcmp(argv[6], "write") == 0)
	    lockToAcquire = write;
	else
	    lockToAcquire = read;
    }
    else
    {
	NumberAllowed = MAX_OBJECTS;
	mean = 2000.0;
	sd = 600.0;
	NumberOfClients = 1;
	lockToAcquire = write;
    }

    // Create a Normal Distribution fn from which to draw MTTF/MTTR statistics
    NS = new NormalStream(mean, sd);
}

MainBody::~MainBody () {}

void MainBody::Body ()
{
    Scheduler* sc = new Scheduler();
    int i;
    Client* Users[MaxClients];
    char* fileName[MaxClients];
    ofstream outFiles[MaxClients];
    unsigned int timeT = 0;

    // Generate machines with randomly selected MTTF and MTTR stats
    // Note we assume MTTR is approx 1% of MTTF
    for (i = 0; i < MAX_MACHINES; i++)
    {
	Sites[i] = new Machine(i, (*NS)(), (*NS)() * 0.01);
    }

    for (i = 0; i < MaxClients; i++)
    {
	fileName[i] = 0;
	Users[i] = 0;
    }
    
    // Create clients to run the transactions
    for (i = 0; i < NumberOfClients; i++)
    {
	Users[i] = new Client(i);
	fileName[i] = new char[20];
	::memset(fileName[i], '\0', 20);
	sprintf(fileName[i], "Client_%d_out", i);
	outFiles[i].open(fileName[i], ios::out);
    }

    // Generate the replica consistency protocol managers

    Voting* qc = new Voting(NumberAllowed, NumberAllowed, 0);
    AC* avcp = new AC(1, NumberAllowed, 0);
    Passive* pr = new Passive(1, NumberAllowed, 0);
    SingleCopy* scp = new SingleCopy(1, NumberAllowed, 0);

    // Select a protocol

    switch(toUse)
    {
    case active:
	// use available copies
	obj = new Object(avcp, NumberAllowed);
	break;
    case passive:
	// use passive replication
	obj = new Object(pr, NumberAllowed);
	break;
    case voting:
	// use quorum concensus (majority voting)
	obj = new Object(qc, NumberAllowed);
	break;
    case single:
	// use single copy passive replication
	obj = new Object(scp, NumberAllowed);
	break;
    }

    RandomPolicy RP(Sites, obj, NumberAllowed);
 
    for (int j = 0; j < MAX_MACHINES; j++)
	Sites[j]->Activate();

    for (i = 0; i < NumberOfClients; i++)
	Users[i]->Activate();

    sc->Resume(); // Not a Process, so must call Resume directly

    while (NumberOfOperationsCompleted < 10000 * NumberOfClients)
    {
	Hold(1000);

	cout << "Number of operations performed " << NumberOfOperationsCompleted << endl;
	
	for (i = 0; i < NumberOfClients; i++)
	{
	    outFiles[i] << " " << timeT << " " <<
		Users[i]->OperationsCompleted() * 100 /	Users[i]->OperationsAttempted() << endl;
	}

	timeT++;
    }

    for (i = 0; i < NumberOfClients; i++)
	outFiles[i] << endl;
    
    sc->Suspend();
    for (i = 0; i < NumberOfClients; i++)
	Users[i]->Suspend();

    _exit(0);
}

void MainBody::Await ()
{
    Resume();
    Thread::Self()->Suspend();
}
