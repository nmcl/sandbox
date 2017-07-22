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

#ifndef JOB_H_
#include "Job.h"
#endif

#ifndef MACHINE_H_
#include "Machine.h"
#endif

#ifndef QUEUE_H_
#include "Queue.h"
#endif


extern Machine* M;
extern Queue JobQ;
extern long TotalJobs;
extern double TotalResponseTime;
extern SimulatedTime;
extern Scheduler *sc;

Job::Job ()
{
    boolean empty;

    ResponseTime = 0;
    ArrivalTime = sc->CurrentTime();
    empty = JobQ.IsEmpty();
    JobQ.Enqueue(this);
    TotalJobs++;

    if (empty && !M->Processing() && M->IsOperational())
	M->Activate();
}

Job::~Job ()
{
    ResponseTime = sc->CurrentTime() - ArrivalTime;

#ifdef DEBUG
    cerr << "ArrivalTime is " << ArrivalTime << "\n";
    cerr << "Time now is " << sc->CurrentTime() << "\n";
    cerr << "ResponseTime is " << ResponseTime << "\n";
#endif

    TotalResponseTime += ResponseTime;
}
