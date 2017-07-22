/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef ARRIVALS_H_
#include "Arrivals.h"
#endif

#ifndef JOB_H_
#include "Job.h"
#endif

Arrivals::Arrivals (double mean) { InterArrivalTime = new ExponentialStream(mean); }

Arrivals::~Arrivals () { delete InterArrivalTime; }

void Arrivals::Body ()
{
    for(;;)
    {
	Hold((*InterArrivalTime)());
	Job* work = new Job();
    }
}
