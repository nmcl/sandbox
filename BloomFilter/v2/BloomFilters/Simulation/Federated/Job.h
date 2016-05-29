/*
 * Copyright (C) 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef JOB_H_
#define JOB_H_

#ifndef BOOLEAN_H_
#  include "Common/Boolean.h"
#endif

#ifndef PROCESS_H_
#  include "ClassLib/Process.h"
#endif

#ifndef QUEUE_H_
#  include "Queue.h"
#endif

class Queue;

class Job
{
public:
    Job (long jno);
    Job (const Job& copy);

    ~Job ();

    double getArrivalTime() const; 
    long getJobNumber() const;

    Queue* getDestQueue() const;
    Process* getDestProcess() const;

    void setDestQueue(Queue* q);
    void setDestProcess(Process* p);

    Boolean operator==(const Job& rhs);
    Boolean operator!=(const Job& rhs);
private:
    double arrivalTime;
    long jobNumber;
    Queue* destinationQueue;
    Process* destinationProcess;
};

#endif
