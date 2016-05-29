/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>
#include <fstream.h>

#ifndef VARIANCE_H_
#  include <Stat/Variance.h>
#endif

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

#ifndef FILTER_H_
#  include "Filter.h"
#endif

#ifndef RESULTS_H_
#  include "Results.h"
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
const int DEBUG_SIMULATION_TIME = SIMULATION_TIME;
const int MAX_JOBS = 100000;

int noJobs = 0;
int processedJobs = 0;
double totalJobTime = 0.0;
int lostJobs = 0;
double serverFailedTime = 0.0;

int simTime;

Queue* networkQueue;
Queue* resultsQueue;
Queue* serverQueue;
Queue* filterQueue;

Network* network;
Client* client;
Results* results;
Server* server;
Filter* filter;

MainBody::MainBody (int argc, char** argv)
{
    if (strcmp(argv[1], "-b") == 0)
    {
        useBreaks = TRUE;
        arrMean = atof(argv[2]);
        filterMean = atof(argv[3]);
        networkMean = atof(argv[4]);
        falseHitProbability = atof(argv[5]);
	quantile = atof(argv[6]);
    }
    else
    {
        useBreaks = FALSE;
        arrMean = atof(argv[1]);
        filterMean = atof(argv[2]);
        networkMean = atof(argv[3]);
        falseHitProbability = atof(argv[4]);
	quantile = atof(argv[5]);
    }
}

MainBody::~MainBody () 
{}

void MainBody::Body ()
{
    // Create a client to run the transactions

    int i;

    serverQueue = new Queue;
    filterQueue = new Queue;
    networkQueue = new Queue;
    resultsQueue = new Queue;
    
    ExponentialStream* networkDelay = new ExponentialStream(networkMean);
    ExponentialStream* clientDelay = new ExponentialStream(arrMean);
    ExponentialStream* filterDelay = new ExponentialStream(filterMean);

    network = new Network(networkDelay, LAN);
    client = new Client(clientDelay);
    server = new Server(0, falseHitProbability);

    results = new Results(quantile);
    filter = new Filter(filterDelay);

    cout << "Starting simulation" << endl;
    client->Activate();

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
    server->Suspend();
    results->Suspend();

    delete client;
    delete network;
    delete results;
    delete server;

    delete networkDelay;
    delete clientDelay;

    delete networkQueue;
    delete resultsQueue;
    delete serverQueue;

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

