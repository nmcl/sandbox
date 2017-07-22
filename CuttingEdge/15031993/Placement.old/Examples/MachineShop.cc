/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef PROCESS_H_
#include <Process.h>
#endif

#ifndef MACHINESHOP_H_
#include "MachineShop.h"
#endif

#ifndef ARRIVALS_H_
#include "Arrivals.h"
#endif

#ifndef MACHINE_H_
#include "Machine.h"
#endif

#ifndef JOB_H_
#include "Job.h"
#endif

#ifndef QUEUE_H_
#include "Queue.h"
#endif

#ifndef BREAKS_H_
#include "Breaks.h"
#endif


Scheduler* sc;
Machine* M;
Queue JobQ;
long TotalJobs = 0;
long ProcessedJobs = 0;
long JobsInQueue = 0;
long CheckFreq = 0;
double TotalResponseTime = 0;
double MachineActiveTime = 0;

MachineShop::MachineShop () {}

MachineShop::~MachineShop () {}

void MachineShop::Body ()
{

    sc = new Scheduler();
    Arrivals* A = new Arrivals(10);
    M = new Machine(8);
    Job* J = new Job;

#ifdef BREAKS
    Breaks*  B = new Breaks;
    B->Activate();
#endif

    A->Activate();
    sc->Resume();

    while (ProcessedJobs < 100000)
	Hold(10000);

    cerr << "Total number of jobs processed " << TotalJobs << "\n";    
    cerr << "Total response time of " << TotalResponseTime << "\n";

    cerr << "Average response time = " << (TotalResponseTime / ProcessedJobs) << "\n";
    cerr << "Probability that machine is busy = " << (MachineActiveTime / CurrentTime()) << "\n";
    cerr << "Average number of jobs present = " << (JobsInQueue / CheckFreq) << "\n";
    sc->Suspend();
    A->Suspend();

#ifdef BREAKS
    B->Suspend();
#endif
}

void MachineShop::Await ()
{
    Resume();
    Thread::Self()->Suspend();
}
