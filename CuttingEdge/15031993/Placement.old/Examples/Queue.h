/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#ifndef PROCESS_H_
#include <Process.h>
#endif

#ifndef JOB_H_
#include "Job.h"
#endif


/* This is the queue on which Jobs are placed before they are used. */

struct List
{
    Job* work;
    List* next;
};

class Queue
{
public:
    Queue ();
    ~Queue ();

    boolean IsEmpty ();
    long QueueSize ();
    Job *Dequeue ();
    void Enqueue (Job*);

private:
    List* head;
    long length;
};

#endif
