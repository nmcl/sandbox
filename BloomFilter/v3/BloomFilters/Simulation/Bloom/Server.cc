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

extern double totalJobTime;
extern int processedJobs;
extern int lostJobs;

extern Network* network;
extern Results* results;

extern Queue* serverQueue;
extern Queue* networkQueue;
extern Queue* resultsQueue;

Server::Server(long id, double fhp) : Machine(id), falseHitProb(fhp)
{
    probGenerator = new UniformStream(0, 1);
}

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
	    request = serverQueue->Dequeue();
	    if (!isOperational())
	    {
		Passivate();
	    }
            if (lost)
            {
                delete request;
                lostJobs++;
                lost = FALSE;
            }
            else if ((*probGenerator)() > falseHitProb)
	    {
		resultsQueue->Enqueue(request);
		results->Activate();
	    }
	    else
	    {
#ifdef DEBUG
            cout << "Server: false hit for Job " << request->getJobNumber()
                 << endl;
#endif

		reply = new Job(*request);
		delete request;
		reply->setDestQueue(resultsQueue);
		reply->setDestProcess(results);
		networkQueue->Enqueue(reply);
		network->Activate();
	    }
        }
	Passivate();
    }
}


