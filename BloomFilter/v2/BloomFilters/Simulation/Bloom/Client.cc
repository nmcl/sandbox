/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef CLIENT_H_
#  include "Client.h"
#endif

#ifndef BOOLEAN_H_
#  include "Common/Boolean.h"
#endif

#ifndef QUEUE_H_
#  include "Queue.h"
#endif

#ifndef FILTER_H_
#  include "Filter.h"
#endif

extern int noJobs;

extern Filter* filter;

extern Queue* filterQueue;

Client::Client (RandomStream* ad): arrivalDist(ad)
{}

Client::~Client ()
{}

void Client::Body ()
{
    double arrTime;
    Job* j;

    for (;;)
    {
	arrTime = (*arrivalDist)();
	Hold(arrTime);
	j = new Job(noJobs, -1); // 2nd field is there in case there is a
				 // false hit and broadcasting is required.
#ifdef DEBUG
	cout << "Job " << j->getJobNumber() << " generated." << endl;
#endif
	noJobs++;
	filterQueue->Enqueue(j);
	filter->Activate();
    }
}
