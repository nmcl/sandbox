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

#ifndef SERVER_H_
#  include "Server.h"
#endif

#ifndef NETWORK_H_
#  include "Network.h"
#endif

#ifndef RECEIVER_H_
#  include "Receiver.h"
#endif

extern int lostJobs;

extern Receiver* receiver;
extern Network* network;

extern Queue* serverQueue;
extern Queue* receiverQueue;
extern Queue* networkQueue;

Server::Server(long id, RandomStream* sd) : Machine(id), lost(FALSE), serverDist(sd)
{}

Server::~Server ()
{}

void Server::setLost()
{
    lost = TRUE;
}

void Server::Body ()
{
    double serTime;
    double jobTime;
    Job* request;
    Job* reply;

    for(;;)
    {
        while (!serverQueue->IsEmpty())
        {
	    if (!isOperational())
            {
		Passivate();
	    }
	    request = serverQueue->Dequeue();
	    serTime = (*serverDist)();
#ifdef DEBUG
	    cout << "Server delay for Job " << request->getJobNumber() << " = ";
            cout << serTime << endl;
#endif
	    startWorking();
	    Hold(serTime);
            stopWorking();
	    if (lost)
	    {
		delete request;
		lostJobs++;
		lost = FALSE;
	    }
	    else
	    {
	        jobTime=CurrentTime()-request->getArrivalTime();
                reply = new Job(*request);
                reply->setDestQueue(receiverQueue);
                reply->setDestProcess(receiver);
            
                delete request;

                networkQueue->Enqueue(reply);
	        network->Activate();
	    }
        }
	Passivate();
    }
}


