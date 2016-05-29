/*
 * Copyright (C) 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef JOB_H_
#  include "Job.h"
#endif

#ifndef PROCESS_H_
#  include "ClassLib/Process.h"
#endif

Job::Job (long jno, int sno): jobNumber(jno), serverNumber(sno)
{
    arrivalTime = Scheduler::scheduler().CurrentTime();
}

Job::Job (long jno, int sno, double start): jobNumber(jno), 
                serverNumber(sno), arrivalTime(start)
{}

Job::Job (const Job& copy)
{
    jobNumber = copy.jobNumber;
    arrivalTime = copy.arrivalTime;
    serverNumber = copy.serverNumber;
}

Job::~Job ()
{}

double Job::getArrivalTime() const
{
    return arrivalTime;
}

long Job::getJobNumber() const
{
    return jobNumber;
}

Queue* Job::getDestQueue() const
{
    return destinationQueue;
}

Process* Job::getDestProcess() const
{
    return destinationProcess;
}

void Job::setDestQueue(Queue* q)
{
    destinationQueue = q;
}

void Job::setDestProcess(Process* p)
{
    destinationProcess = p;
}

void Job::setServerNumber(int sn)
{
   serverNumber = sn;
}

int Job::getServerNumber() const
{
    return serverNumber;
}

Boolean Job::operator==(const Job& rhs)
{
    return Boolean(rhs.jobNumber==jobNumber && rhs.arrivalTime==arrivalTime);
}

Boolean Job::operator!=(const Job& rhs)
{
    return !(*this==rhs);
}

