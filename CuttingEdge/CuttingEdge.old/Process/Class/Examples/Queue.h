#ifndef QUEUE_
#define QUEUE_

#include "Job.h"
#include <Process.h>

struct List
{
    Job* work;
    List* next;
};

class Queue
{
private:
    List* head;
    long length;
public:
    Queue();
    ~Queue();

    boolean IsEmpty();
    long QueueSize();
    Job *Dequeue();
    void Enqueue(Job*);
};

#endif
