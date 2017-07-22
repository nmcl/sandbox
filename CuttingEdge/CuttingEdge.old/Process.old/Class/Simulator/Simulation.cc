// Simulating transactions on replicated objects in a network of
// unreliable nodes

#include <iostream.h>

#include "Common.h"
#include <Process.h>
#include "MachineControl.h"
#include "ReplicatedObject.h"
#include "Display.h"
#include "Random.h"

// Simulation variables
boolean Debug; // True for debug print messages
int ObjectID; // UID generator for transactions
int MachineID; // UID generator for machines

// Loop control variables for main program
int StartedTransactions, CompletedTransactions;

// Global data structures for basic machine and object configuration
Machine *Nodes[TotalMachines];
ReplicatedObject *Objects[TotalObjects];

// Random number objects used by the simulation
UniformStream *Rand1 = new UniformStream(1, TotalMachines);
UniformStream *Rand2 = new UniformStream(0, TotalMachines-1);
UniformStream *Rand3 = new UniformStream(1, TotalObjects);
UniformStream *Rand4 = new UniformStream(0, TotalObjects-1);
UniformStream *Rand5 = new UniformStream(MinTransactionTime, MaxTransactionTime);

main()
{
    LWP_Thread::Initialize();
    Scheduler *sc = new Scheduler(); // create process scheduler

    Debug = true;

    // Basic Algorithm
    // Main program configures and launches the simulation by:
    //  1) Creating a set of machines (the "network" )
    //  2) Allocating objects to machines (the "placement")
    //  3) Generating a transaction stream
    //  4) Reporting results

    sc->Resume();
    // Creating the network of machines
    for (MachineID = 0; MachineID < TotalMachines; MachineID++)
    {
	Stats *s = new Stats;
	MachineControl* m;

	s->ID = MachineID;
	s->MTTFMean = 3600.0*24.0;
	s->MTTFStdDev = 3600.0;
	s->MTTRMean = 3600.0*2.0;
	s->MTTRStdDev = 1200.0;
	s->Speed = 10;
	Nodes[MachineID] = new Machine(s);
	m = new MachineControl(Nodes[MachineID]);
	m->Activate();
    }

    // Create Objects and Allocate to machines (initial placement)
    for (ObjectID = 0; ObjectID < TotalObjects; ObjectID++)
	Objects[ObjectID] = new ReplicatedObject(ObjectID, 0.98);

    // Generate transaction stream
    Transaction* t = new Transaction();
    t->Activate();

    Thread::Self()->Suspend();
    if (Debug)
    {
	Display::DumpMachines();
	Display::DumpObjects();
    }

    Display::DumpMachines();

    // Report simulation results
    cout << "The number of completed transactions is " << CompletedTransactions << "\n";

    return 0;
}
