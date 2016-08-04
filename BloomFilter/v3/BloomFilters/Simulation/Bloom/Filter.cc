/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef FILTER_H_
#  include "Filter.h"
#endif

#ifndef BOOLEAN_H_
#  include "Common/Boolean.h"
#endif

#ifndef QUEUE_H_
#  include "Queue.h"
#endif

#ifndef SERVER_H_
#  include "Server.h"
#endif

#ifndef NETWORK_H_
#  include "Network.h"
#endif

extern Network* network;
extern Server* server;

extern Queue* networkQueue;
extern Queue* serverQueue;
extern Queue* filterQueue;

Filter::Filter(RandomStream* fd) : filterDist(fd)
{}

Filter::~Filter()
{
}

void Filter::Body ()
{
    double waitTime;
    Job* j;

    for (;;)
    {
        while(!filterQueue->IsEmpty())
        {
	    j = filterQueue->Dequeue();
	    waitTime = ((*filterDist)());
	    Hold(waitTime);
#ifdef DEBUG
	    cout << "Filter waitTime for Job " << j->getJobNumber()
                 << "  = " << waitTime << endl;
#endif
	    j->setDestQueue(serverQueue);
	    j->setDestProcess(server);
	    networkQueue->Enqueue(j);
	    network->Activate();
	}
	Passivate();
    }
}
