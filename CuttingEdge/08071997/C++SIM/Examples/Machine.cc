/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef MACHINE_H_
#include "Machine.h"
#endif

#ifndef JOB_H_
#include "Job.h"
#endif

#ifndef QUEUE_H_
#include "Queue.h"
#endif


extern long ProcessedJobs;
extern long JobsInQueue;
extern long CheckFreq;
extern double MachineActiveTime;
extern Queue JobQ;

Machine::Machine (double mean)
{
    STime = new ExponentialStream(mean);
    operational = true;
    working = false;
}

Machine::~Machine () { delete STime; }

void Machine::Body ()
{
    double ActiveStart, ActiveEnd;

    for (;;)
    {
	working = true;

	while (!JobQ.IsEmpty())
	{
	    ActiveStart = CurrentTime();
	    CheckFreq++;

	    JobsInQueue += JobQ.QueueSize();
	    Job* J = JobQ.Dequeue();
	    Hold((*STime)());

	    ActiveEnd = CurrentTime();
	    MachineActiveTime += ActiveEnd - ActiveStart;
	    ProcessedJobs++;
	    delete J;
	}

	working = false;
	Cancel();
	set_evtime(CurrentTime());
    }
}

boolean Machine::Processing () { return working; }

void Machine::Broken () { operational = false; }

void Machine::Fixed ()
{
    operational = true;
    set_evtime(CurrentTime());
}

boolean Machine::IsOperational () { return operational; }

double Machine::ServiceTime () { return (*STime)(); }
