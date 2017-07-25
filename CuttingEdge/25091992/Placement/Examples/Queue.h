#ifndef QUEUE_
#define QUEUE_

#include "Job.h"
#include <Process.h>

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
