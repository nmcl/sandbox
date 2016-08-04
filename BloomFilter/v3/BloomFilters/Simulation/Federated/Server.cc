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

#ifndef MAINBODY_H_
#  include "MainBody.h"
#endif

extern double totalJobTime;
extern int processedJobs;
extern int lostJobs;

extern Queue* serverQueue[];
extern Queue* receiverQueue;
extern Queue* networkQueue;

extern Receiver* receiver;
extern Network* network;
extern Server* server[];

Server::Server(long id, RandomStream* sd) : Machine(id), lost(FALSE),
                                            serverDist(sd)
{
    probGenerator = new UniformStream(0, 1);
    findProbability = 1.0/(MAX_SERVERS - id);
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
    Boolean foundName;

    for(;;)
    {
        while (!serverQueue[machineID]->IsEmpty())
        {
	    if (!isOperational())
            {
		Passivate();
	    }
	    request = serverQueue[machineID]->Dequeue();
	    serTime = (*serverDist)();
#ifdef DEBUG
	    cout << "Service Time for Job " << request->getJobNumber()
                 << " = " << serTime << endl;
#endif
	    foundName = FALSE;
	    if ((machineID == (MAX_SERVERS -1)) || 
                (*probGenerator)()<findProbability)
	    {
		foundName = TRUE;
	    }

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
                delete request;

		if (foundName)
		{
                    reply->setDestQueue(receiverQueue);
                    reply->setDestProcess(receiver);
                    networkQueue->Enqueue(reply);
	            network->Activate();
		}
		else if (machineID < MAX_SERVERS - 1)
		{
		    reply->setDestQueue(serverQueue[machineID+1]);
		    reply->setDestProcess(server[machineID+1]);
                    networkQueue->Enqueue(reply);
	            network->Activate();
		}
		else
		{
		    cerr << "Error - didn't find name" << endl;
		    lostJobs++;
		    delete reply;
		}
	    }
        }
	Passivate();
    }
}


