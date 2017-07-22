#include <iostream.h>

#include "MainBody.h"
#include "Client.h"
#include "Object.h"
#include "AC.h"
#include "Voting.h"
#include "Placement.h"

#define INIT_
#include "Machine.h"

extern "C"
{
    int strcmp(char*, char*);
    int atoi(char*);
    double atof(char*);
    void _exit(int);
}

Machine* Sites[MAX_MACHINES];
Placement* pl = 0;
Object* obj = 0;
Client* cl = 0;

int NumberAllowed = 0;
double availability = 0.0;
double NumberOfOperations = 0.0;
double OperationsPerformed = 0.0;

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
	NumberAllowed = atoi(argv[3]);
	if (NumberAllowed > MAX_OBJECTS)
	    NumberAllowed = MAX_OBJECTS;
	mean = (double) atoi(argv[4]);
	sd = (double) atoi(argv[5]);
	availability = atof(argv[6]);
	read_write = atof(argv[7]);
	rate = atof(argv[8]);
	if (rate > mean*0.0008)
	    rate = mean*0.0008;
    }
    else
    {
	NumberAllowed = MAX_OBJECTS;
	mean = 2000.0;
	sd = 600.0;
//	availability = 0.99;
	availability = 1.0;
	read_write = 10.0; // expected read/write ratio
	rate = 4;
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
    boolean UseRandom;

#ifdef RUNINFO
    double oldOpsPerformed = 0.0, oldOpsAttempted = 0.0;
    unsigned int timeT = 0;
#endif    

    // Generate machines with randomly selected MTTF and MTTR stats
    // Note we assume MTTR is approx 1% of MTTF
    for (i = 0; i < MAX_MACHINES; i++)
    {
	Sites[i] = new Machine(i, (*NS)(), (*NS)() * 0.01);
    }

    UseRandom = (strcmp(policy, "Random") == 0);

    // Create a client to run the transactions
    cl = new Client(1, rate);

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
	pl->SimplePlacementPolicy(Sites, obj, NumberAllowed, availability, read_write);
 
    for (int j = 0; j < MAX_MACHINES; j++)
	Sites[j]->Activate();

    pl->Activate();
    cl->Activate();
    sc->Resume(); // Not a Process, so must call Resume directly

#ifdef FAST
    while (OperationsPerformed < 10000)
#else
    while (OperationsPerformed < 100000)
#endif
    {
#ifdef RUNINFO
	Hold(1000);
#else
	Hold(100000);
#endif

#ifndef RUNINFO
	cerr << "Operations performed " << OperationsPerformed << "\n";
#endif

#ifdef RUNINFO
	oldOpsPerformed = OperationsPerformed - oldOpsPerformed;
	oldOpsAttempted = NumberOfOperations - oldOpsAttempted;

#ifndef AVERAGE
	cout << timeT << " " << (OperationsPerformed / NumberOfOperations) << endl;
	timeT++;
#else
	cout << timeT << " " << (oldOpsPerformed / oldOpsAttempted) << endl;
	timeT++;
#endif
#endif
    }

    pl->Suspend();
    sc->Suspend();
    cl->Suspend();

#ifndef RUNINFO
    cerr << "Total operations attempted " << NumberOfOperations << "\n";
    cerr << "Total operations completed " << OperationsPerformed << "\n";
    cout << policy << '\t' 
	 << (UseAC?"AC":"QC") << '\t' 
	 << availability << '\t' 
	 << obj->WriteQuorum() << '\t'
	 << (OperationsPerformed / NumberOfOperations) << "\t" 
	 << obj->GetReadWriteRatio() << "\n"
	 << flush;
#endif

    _exit(0);
}

void MainBody::Await ()
{
    Resume();
    Thread::Self()->Suspend();
}
