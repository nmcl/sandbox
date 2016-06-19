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
#  include "MainBody.h"
#endif

#ifndef CLIENT_H_
#  include "Client.h"
#endif

#ifndef OBJECT_H_
#  include "Object.h"
#endif

#ifndef AC_H_
#  include "AC.h"
#endif

#ifndef VOTING_H_
#  include "Voting.h"
#endif

#ifndef PLACEMENT_H_
#  include "Placement.h"
#endif

#ifndef MACHINE_H_
#  include "Machine.h"
#endif


const char* nodeFile = "nodeFile";
const char* machineFile = "machineFile";


extern "C"
{
    void sprintf(char*, char*, int);
    int strcmp(const char*, const char*);
    int atoi(char*);
    double atof(char*);
    void _exit(int);
}

Machine* Sites[MAX_MACHINES];
Placement* pl = 0;
Object* obj = 0;

lock_code lockToAcquire = unlocked;
int NumberOfClients = 0;
int NumberAllowed = 0;
double availability = 0.0;
double NumberOfOperationsCompleted = 0.0;
ofstream machineStatsFile;


MainBody::MainBody (int argc, char** argv)
{
    // Process args
    policy = argv[1];

    if (strcmp(argv[2], "AC") == 0)
	UseAC = TRUE;
    else
	UseAC = FALSE;

    if (argc > 3)
    {
	NumberAllowed = atoi(argv[3]);
	if (NumberAllowed > MAX_OBJECTS)
	    NumberAllowed = MAX_OBJECTS;
	mean = (double) atoi(argv[4]);
	sd = (double) atoi(argv[5]);
	availability = atof(argv[6]);
	read_write = atof(argv[7]);
	NumberOfClients = atoi(argv[8]);

	if (NumberOfClients > MaxClients)
	{
	    cerr << "Error - too many clients. Will use maximum allowed < " << MaxClients << " >" << endl;
	    NumberOfClients = MaxClients;
	}
	if (strcmp(argv[10], "write") == 0)
	    lockToAcquire = write;
	else
	    lockToAcquire = read;
    }
    else
    {
	NumberAllowed = MAX_OBJECTS;
	mean = 2000.0;
	sd = 600.0;
	availability = 1.0;
	read_write = 10.0; // expected read/write ratio
	NumberOfClients = 1;
	lockToAcquire = write;
    }

    // Create a Normal Distribution fn from which to draw MTTF/MTTR statistics
    int ss = 0;
    if (argc > 9) ss = atoi(argv[9]);
    NS = new NormalStream(mean, sd, ss);
}

MainBody::~MainBody () {}

void MainBody::Body ()
{
    Scheduler* sc = new Scheduler();
    int i;
    Boolean UseRandom;
    Client* Users[MaxClients];
    char* fileName[MaxClients];
    ofstream outFiles[MaxClients];
    ofstream nodesDataFile;

#ifdef RUNINFO
    double oldOpsPerformed[MaxClients], oldOpsAttempted[MaxClients];
#endif    

    // Generate machines with randomly selected MTTF and MTTR stats
    // Note we assume MTTR is approx 1% of MTTF
    for (i = 0; i < MAX_MACHINES; i++)
    {
	Sites[i] = new Machine(i, (*NS)(), (*NS)() * 0.01);
    }

    for (i = 0; i < MaxClients; i++)
    {
#ifdef RUNINFO
	oldOpsPerformed[i] = oldOpsAttempted[i] = 0.0;
#endif
	fileName[i] = 0;
	Users[i] = 0;
    }
    
    UseRandom = (strcmp(policy, "Random") == 0);

    // Create clients to run the transactions
    for (i = 0; i < NumberOfClients; i++)
    {
	Users[i] = new Client(i, read_write);
	fileName[i] = new char[20];
	::memset(fileName[i], '\0', 20);
	sprintf(fileName[i], "Client_%d_out", i);
	outFiles[i].open(fileName[i], ios::out);
    }

    nodesDataFile.open(nodeFile, ios::out);
    machineStatsFile.open(machineFile, ios::out);

    // Generate the replica consistency protocol managers
    Voting* qc = new Voting(NumberAllowed, NumberAllowed, 0);
    AC* avcp = new AC(1, NumberAllowed, 0);

    // Select a protocol
    if (UseAC)
    {
	// use available copies
	obj = new Object(avcp, NumberAllowed, read_write, UseRandom);
    }
    else
    {
	// use quorum concensus (majority voting)
	obj = new Object(qc, NumberAllowed, read_write, UseRandom);
    }

    // create a placement
    pl = new Placement();

    cout << "Group availability = " << availability << "\n" << flush;

    if (UseRandom)
	pl->RandomPlacementPolicy(Sites, obj, NumberAllowed, availability);
    else
	pl->SimplePlacementPolicy(Sites, obj, availability, read_write, NumberAllowed);
 
    for (int j = 0; j < MAX_MACHINES; j++)
	Sites[j]->Activate();

    pl->Activate();
    for (i = 0; i < NumberOfClients; i++)
	Users[i]->Activate();
    sc->Resume(); // Not a Process, so must call Resume directly

#ifdef FAST
    while (NumberOfOperationsCompleted < 10000 * NumberOfClients)
#else
    while (NumberOfOperationsCompleted < 100000 * NumberOfClients)
#endif
    {
#ifdef FAST
	Hold(1000);
#else
	Hold(10000);
#endif

	cout << "Number of operations performed " << NumberOfOperationsCompleted << endl;
	
#ifdef DEBUG
	for (i = 0; i < NumberOfClients; i++)
	{
	    outFiles[i] << "Operations performed " << Users[i]->OperationsCompleted() << "\n";
	    outFiles[i] << "Number attempted " << Users[i]->OperationsAttempted() << endl;
	}
#endif

#ifdef RUNINFO

	nodesDataFile << " " << Process::CurrentTime() << " " << obj->GetAverageObjects() << endl;

#ifdef DEBUG
	for (i = 0; i < NumberOfClients; i++)
	{
	    outFiles[i] << " Operations performed this period "
			<< Users[i]->OperationsCompleted() - oldOpsPerformed[i] << endl;
	    outFiles[i] << " Operations attempted this period "
			<< Users[i]->OperationsAttempted() - oldOpsAttempted[i] << endl;
	}
#endif

	for (i = 0; i < NumberOfClients; i++)
	{
#ifndef LOAD
#ifdef AVERAGE
            if (Users[i]->OperationsAttempted() && Users[i]->OperationsCompleted())
		outFiles[i] << " " << Process::CurrentTime() << " "
			    << (Users[i]->OperationsCompleted() / Users[i]->OperationsAttempted())*100 << endl;
#else
            if ((Users[i]->OperationsAttempted() - oldOpsAttempted[i]) &&
		(Users[i]->OperationsCompleted() - oldOpsPerformed[i]))
		outFiles[i] << " " << Process::CurrentTime() << " "
			    << ((Users[i]->OperationsCompleted() - oldOpsPerformed[i])/
				(Users[i]->OperationsAttempted() - oldOpsAttempted[i]))*100 << endl;
#endif

	    oldOpsPerformed[i] = Users[i]->OperationsCompleted();
	    oldOpsAttempted[i] = Users[i]->OperationsAttempted();
#else
	    outFiles[i] << " " << Process::CurrentTime() << " " << (Users[i]->GetResponseTime()) << endl;
#endif
	}
#endif
    }

    for (i = 0; i < NumberOfClients; i++)
	outFiles[i] << endl;
    
    pl->Suspend();
    sc->Suspend();
    for (i = 0; i < NumberOfClients; i++)
	Users[i]->Suspend();

#ifndef RUNINFO
    for (i = 0; i < NumberOfClients; i++)
    {
#ifndef RWINFO
	outFiles[i] << "Total operations attempted " << Users[i]->OperationsAttempted() << "\n";
	outFiles[i] << "Total operations completed " << Users[i]->OperationsCompleted() << "\n";
	outFiles[i] << policy << '\t' 
		    << (UseAC?"AC":"QC") << '\t' 
		    << availability << '\t' 
		    << obj->WriteQuorum() << '\t'
		    << (Users[i]->OperationsCompleted() / Users[i]->OperationsAttempted()) << "\t" 
		    << obj->GetReadWriteRatio() << "\n"
		    << flush;
	outFiles[i] << endl;
#else
        outFiles[i] << read_write << " " << (Users[i]->OperationsCompleted() / Users[i]->OperationsAttempted()) << endl;
#endif	
    }
#endif

    _exit(0);
}

void MainBody::Await ()
{
    Resume();
    Thread::Self()->Suspend();
}
