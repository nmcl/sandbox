/*
 * Copyright (C) 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef PROCESS_H_
#  include <ClassLib/Process.h>
#endif

#ifndef JOB_H_
#  include "Job.h"
#endif

class Job;

/* This is the queue on which Jobs are placed before they are used. */

class List
{
    friend class Queue;
private:
    Job* work;
    List* next;
};

class Queue
{
    friend class List;
public:
    Queue ();
    ~Queue ();

    Boolean IsEmpty ();
    long QueueSize ();
    Job *Dequeue ();
    void Enqueue (Job* toadd);

private:
    List* head;
    long length;
};

#endif
