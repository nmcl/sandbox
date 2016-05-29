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

#ifndef RECEIVER_H_
#  include "Receiver.h"
#endif

#ifndef RESULTS_H_
#  include "Results.h"
#endif

#ifndef BREAKS_H_
#  include "Breaks.h"
#endif

extern "C"
{
    int strcmp(const char*, const char*);
    int atoi(const char*);
    void _exit(int);
}

const int SIMULATION_TIME = 10000;
const int DEBUG_SIMULATION_TIME = SIMULATION_TIME;
const int MAX_JOBS = 10000;
const int FAST_MAX_JOBS = MAX_JOBS;

int noJobs = 0;
int processedJobs = 0;
double totalJobTime = 0.0;
int lostJobs = 0;
double serverFailedTime = 0.0;

int simTime;

Queue* networkQueue;
Queue* resultsQueue;
Queue* serverQueue;
Queue* receiverQueue;

Network* network;
Client* client;
Server* server;
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

MainBody::~MainBody () {}

void MainBody::Body ()
{
    // Create a client to run the transactions
    int randSeed = 0;

    serverQueue = new Queue;
    networkQueue = new Queue;
    receiverQueue = new Queue;
    resultsQueue = new Queue;

    ExponentialStream* networkDelay = new ExponentialStream(networkMean);
    ExponentialStream* clientDelay = new ExponentialStream(arrMean);
    ExponentialStream* serverDelay = new ExponentialStream(serviceMean);
    ExponentialStream* receiverDelay = new ExponentialStream(receiverMean);

    UniformStream* serverOperativeTime = new UniformStream(serverOpLower, 
                                                           serverOpUpper);
    UniformStream* serverRepairTime = new UniformStream(serverRepLower,
                                                        serverRepUpper);
 
    client = new Client(clientDelay);
    network = new Network(networkDelay, LAN);
    server = new Server(0, serverDelay);
    receiver = new Receiver(receiverDelay);
    results = new Results(0.95);

    cout << "Starting simulation" << endl;
    client->Activate();

    Breaks* b = (Breaks*) 0;
    if (useBreaks)
    {
	b = new Breaks(0, serverOperativeTime, serverRepairTime);
	b->Activate();
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

    network->Suspend();
    results->Suspend();
    receiver->Suspend();
    server->Suspend();

    delete client;
    delete network;
    delete server;
    delete receiver;
    delete results;

    delete networkDelay;
    delete clientDelay;
    delete serverDelay;
    delete receiverDelay;
    delete serverOperativeTime;
    delete serverRepairTime;

    delete serverQueue;
    delete networkQueue;
    delete receiverQueue;
    delete resultsQueue;
    if (b)
    {
	delete b;
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
