/*
 * Copyright (C) 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef BREAKS_H_
#  include "Breaks.h"
#endif

#ifndef QUEUE_H_
#  include "Queue.h"
#endif

#ifndef SERVER_H_
#  include "Server.h"
#endif

#include <iostream.h>

extern double serverFailedTime;
extern int lostJobs;

extern Server* server;

extern Queue* serverQueue;

Breaks::Breaks (int id, RandomStream* ot, RandomStream* rt) : 
           breakID(id), operativeTime(ot), repairTime(rt)
{}

Breaks::~Breaks ()
{}

void Breaks::Body ()
{
    Job* j;
    double failedTime;
    double aliveTime;

    for(;;)
    {
        failedTime = (*repairTime)();
	aliveTime = (*operativeTime)();
#ifdef DEBUG
	cout << "Machine " << breakID << " runs for " << aliveTime;
	cout << " time and fails for " << failedTime << "time" << endl;
#endif
	Hold(aliveTime);
	server->breakIt();
	Hold(failedTime);

	while (!serverQueue->IsEmpty())
	{
	    j = serverQueue->Dequeue();
	    delete j;
	    lostJobs++;
	}

	server->setLost();
	serverFailedTime += failedTime;
	server->fixIt();
	server->Activate();
    }
}
