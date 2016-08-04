/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef RESULTS_H_
#  include "Results.h"
#endif

#ifndef BOOLEAN_H_
#  include "Common/Boolean.h"
#endif

#ifndef QUEUE_H_
#  include "Queue.h"
#endif

extern int processedJobs;
extern double totalJobTime;

extern Queue* resultsQueue;

Results::Results(double q) : stats(q)
{}

Results::~Results ()
{
}

Quantile Results::getStats() const
{
    return stats;
}

void Results::Body ()
{
    Job* j;
    double jobTime;

    for (;;)
    {
	while (!resultsQueue->IsEmpty())
	{
	    j = resultsQueue->Dequeue();
	    jobTime = CurrentTime()-j->getArrivalTime();
            stats += jobTime;
	    processedJobs++;
	    totalJobTime+=jobTime;

	    delete j;
	}
	Passivate();
    }
}
