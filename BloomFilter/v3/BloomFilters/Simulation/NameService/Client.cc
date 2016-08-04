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

extern int noJobs;

extern Server* server;
extern Network* network;

extern Queue* serverQueue;
extern Queue* networkQueue;

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
	j = new Job(noJobs);
        j->setDestQueue(serverQueue);
	j->setDestProcess(server);
	noJobs++;

	networkQueue->Enqueue(j);
	network->Activate();

    }
}
