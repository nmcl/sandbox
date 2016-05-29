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

#ifndef SERVER_H_
#  include "Server.h"
#endif

#ifndef NETWORK_H_
#  include "Network.h"
#endif

#ifndef MAINBODY_H_
#  include "MainBody.h"
#endif


extern int noJobs;

extern Queue* serverQueue[];
extern Queue* networkQueue;

extern Server* server[];
extern Network* network;

Client::Client (RandomStream* ad, RandomStream* ws): 
           arrivalDist(ad), whichServer(ws)
{}

Client::~Client ()
{}

void Client::Body ()
{
    double arrTime;
    int serverToReply;
    Job* j;

    for (;;)
    {
	arrTime = (*arrivalDist)();
	Hold(arrTime);
	serverToReply = (int) (*whichServer)();
	for (int i=0; i<MAX_SERVERS; i++)
	{
	    j = new Job(noJobs, serverToReply);
            j->setDestQueue(serverQueue[i]);
	    j->setDestProcess(server[i]);
#ifdef DEBUG
	    cout << "Client sending to server " << i << " Job no " << noJobs
		 << endl;
#endif
	    networkQueue->Enqueue(j);
	    network->Activate();
	}
	noJobs++;
    }
}
