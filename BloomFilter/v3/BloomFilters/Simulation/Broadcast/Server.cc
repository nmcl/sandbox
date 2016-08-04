/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef BOOLEAN_H_
#  include "Common/Boolean.h"
#endif

#ifndef QUEUE_H_
#  include "Queue.h"
#endif

#ifndef SERVER_H_
#  include "Server.h"
#endif

#ifndef RESULTS_H_
#  include "Results.h"
#endif

extern int lostJobs;

extern Results* results;

extern Queue* serverQueue[];
extern Queue* resultsQueue;

Server::Server(long id) : Machine(id), lost(FALSE)
{}

Server::~Server ()
{}

void Server::setLost()
{
    lost = TRUE;
}

void Server::Body ()
{
    Job* request;

    for(;;)
    {
        while (!serverQueue[machineID]->IsEmpty())
        {
	    request = serverQueue[machineID]->Dequeue();
	    if (!isOperational())
	    {
		Passivate();
	    }
#ifdef DEBUG
	    cout << "Server " << machineID << " got job " << request->getJobNumber()
		 << " headed for server " << request->getServerNumber() << endl;
#endif
	    if (machineID == request->getServerNumber())
	    {
	        if (lost)
	        {
		    delete request;
		    lostJobs++;
		    lost = FALSE;
	        }
	        else
	        {
		    resultsQueue->Enqueue(request);
		    results->Activate();
		}
	    }
	    else
	    {
		delete request;
	    }
        }
	Passivate();
    }
}


