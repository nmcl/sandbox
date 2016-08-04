/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef MAINBODY_H_
#  include "MainBody.h"
#endif

#ifndef NETWORK_H_
#  include "Network.h"
#endif

#ifndef QUEUE_H_
#  include "Queue.h"
#endif

#ifndef CLIENT_H_
#  include "Client.h"
#endif

#ifndef SERVER_H_
#  include "Server.h"
#endif

#ifndef RESULTS_H_
#  include "Results.h"
#endif

#ifndef RECEIVER_H_
#  include "Receiver.h"
#endif

#ifndef BREAKS_H_
#  include "Breaks.h"
#endif

extern "C"
{
    int strcmp(const char*, const char*);
    int strncmp(const char*, const char*, int);
    unsigned int strlen(const char*);
    char* strcpy(char*, const char*);
    int atoi(const char*);
    void _exit(int);
}


const int SIMULATION_TIME = 10000;
const int DEBUG_SIMULATION_TIME = 10000;
const int MAX_JOBS = 10000;

int noJobs = 0;
int processedJobs = 0;
double totalJobTime = 0.0;
int lostJobs = 0;
double serverFailedTime = 0.0;

int simTime;

Queue* networkQueue;
Queue* receiverQueue;
Queue* resultsQueue;
Queue* serverQueue[MAX_SERVERS];

Network* network;
Client* client;
Server* server[MAX_SERVERS];
Receiver* receiver;
Results* results;

MainBody::MainBody (int argc, char** argv)
{
    if (strcmp(argv[1], "-b") == 0)
    {
        useBreaks = TRUE;
        arrMean = atof(argv[2]);
        serviceMean = atof(argv[3]);
        networkMean = atof(argv[4]);
        receiverMean = atof(argv[5]);
        serverOpLower = atof(argv[6]);
        serverOpUpper = atof(argv[7]);
        serverRepLower = atof(argv[8]);
        serverRepUpper = atof(argv[9]);
    }
    else
    {
        useBreaks = FALSE;
        arrMean = atof(argv[1]);
        serviceMean = atof(argv[2]);
        networkMean = atof(argv[3]);
        receiverMean = atof(argv[4]);
        serverOpLower = atof(argv[5]);
        serverOpUpper = atof(argv[6]);
        serverRepLower = atof(argv[7]);
        serverRepUpper = atof(argv[8]);
    }
}

MainBody::~MainBody () 
{}

void MainBody::Body ()
{
    // Create a client to run the transactions

    int i;

    for (i=0; i<MAX_SERVERS; i++)
    {
	serverQueue[i] = new Queue;
    }
    networkQueue = new Queue;
    resultsQueue = new Queue;
    receiverQueue = new Queue;

    
    ExponentialStream* networkDelay = new ExponentialStream(networkMean);
    ExponentialStream* clientDelay = new ExponentialStream(arrMean);
    ExponentialStream* receiverDelay = new ExponentialStream(receiverMean);

    UniformStream* serverOperativeTime[MAX_SERVERS];
    UniformStream* serverRepairTime[MAX_SERVERS];
    ExponentialStream* serverDelay[MAX_SERVERS];

    for (i=0; i<MAX_SERVERS; i++)
    {
        serverOperativeTime[i] = new UniformStream(serverOpLower, 
                                                   serverOpUpper);
        serverRepairTime[i] = new UniformStream(serverRepLower,
                                                serverRepUpper);
	serverDelay[i] = new ExponentialStream(serviceMean);
    }
    
    network = new Network(networkDelay, LAN);
    client = new Client(clientDelay);
    for (i=0; i<MAX_SERVERS; i++)
    {
        server[i] = new Server(i, serverDelay[i]);
    }
    receiver = new Receiver(receiverDelay);
    results = new Results(0.95);

    cout << "Starting simulation" << endl;
    client->Activate();

    Breaks* b[MAX_SERVERS];
    for (i=0; i<MAX_SERVERS; i++)
    {
        if (useBreaks)
	{
	    b[i] = new Breaks(i, serverOperativeTime[i], serverRepairTime[i]);
	    b[i]->Activate();
	}
	else
	{
	    b[i] = 0;
	}
    }

    Scheduler::scheduler().Resume(); 

#ifndef DEBUG
    Hold(SIMULATION_TIME);
    simTime = SIMULATION_TIME;
#else
    Hold(DEBUG_SIMULATION_TIME);
    simTime = DEBUG_SIMULATION_TIME;
#endif
    cout << "Jobs started =  " << noJobs << endl;
    cout << "Jobs processed =  " << processedJobs << endl;
    cout << "Total processing Time = " << totalJobTime << endl;
    cout << "Throughput = " << double(processedJobs)/simTime << endl;
    cout << "Average Response Time = " << totalJobTime/processedJobs << endl;
    cout << "Jobs Lost = " << lostJobs << endl;
    cout << "Server Failed Time = " << serverFailedTime << endl;

    results->getStats().print(cout);
    client->Suspend();
    network->Suspend();
    receiver->Suspend();
    results->Suspend();
    for (i=0; i<MAX_SERVERS; i++)
    {
        server[i]->Suspend();
    }

    delete client;

    for (i=0; i<MAX_SERVERS; i++)
    {
        delete server[i];
    }
    delete network;
    delete results;
    delete receiver;

    delete networkDelay;
    delete clientDelay;
    delete receiverDelay;

    for (i=0; i<MAX_SERVERS; i++)
    {
        delete serverQueue[i];
        delete serverDelay[i];
        delete serverOperativeTime[i];
        delete serverRepairTime[i];
    }
    delete networkQueue;
    delete resultsQueue;
    delete receiverQueue;

    for (i=0; i<MAX_SERVERS; i++)
    {
	if (b[i])
	{
	     delete b[i];
	}
    }
    Scheduler::scheduler().reset();
    Scheduler::scheduler().Suspend();
    Thread::mainResume();
}

void MainBody::Await ()
{
    Resume();
    Thread::Self()->Suspend();
}

void MainBody::Exit()
{
    Thread::Exit();
}

