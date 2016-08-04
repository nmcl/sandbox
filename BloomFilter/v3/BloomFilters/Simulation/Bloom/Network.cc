/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef NETWORK_H_
#  include "Network.h"
#endif

#ifndef BOOLEAN_H_
#  include "Common/Boolean.h"
#endif

#ifndef QUEUE_H_
#  include "Queue.h"
#endif

extern Queue* networkQueue;

Network::Network (RandomStream* nd, NetworkType wn)
          : whichNetwork(wn), networkDist(nd)
{}

Network::~Network ()
{
}

void Network::Body ()
{
    double waitTime;
    Job* j;

    for (;;)
    {
        while(!networkQueue->IsEmpty())
        {
	    j = networkQueue->Dequeue();
	    waitTime = ((*networkDist)());
	    Hold(waitTime);

#ifdef DEBUG
	    cout << "Network waitTime for Job " << j->getJobNumber()
                 << "  = " << waitTime << endl;
#endif
	    j->getDestQueue()->Enqueue(j);
	    j->getDestProcess()->Activate();
	}
	Passivate();
    }

}
