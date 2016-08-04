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
#include "Results.h"
#endif

#ifndef BOOLEAN_H_
#  include "Common/Boolean.h"
#endif

#ifndef QUEUE_H_
#  include "Queue.h"
#endif

#ifndef RECEIVER_H_
#  include "Receiver.h"
#endif

#ifndef NETWORK_H_
#  include "Network.h"
#endif

extern Results* results;
extern Network* network;

extern Queue* receiverQueue;
extern Queue* resultsQueue;
extern Queue* networkQueue;

Receiver::Receiver(RandomStream* rd) : receiverDist(rd)
{}

Receiver::~Receiver ()
{}

void Receiver::Body ()
{
    double receiverDelay;
    Job* request;
    Job* reply;

    for(;;)
    {
        while (!receiverQueue->IsEmpty())
        {
	    request = receiverQueue->Dequeue();
	    receiverDelay = (*receiverDist)();
#ifdef DEBUG
	    cout << "Receiver delay for Job " << request->getJobNumber()
	         << " = " << receiverDelay << endl;
#endif
	    Hold(receiverDelay);

            reply = new Job(*request);
            reply->setDestQueue(resultsQueue);
            reply->setDestProcess(results);
            
            delete request;

            networkQueue->Enqueue(reply);
	    network->Activate();
        }
	Passivate();
    }
}


