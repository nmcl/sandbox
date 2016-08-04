/*
 * Copyright (C) 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef QUEUE_H_
#  include "Queue.h"
#endif

Queue::Queue ()
	     : head(0),
	       length(0)
{
}

Queue::~Queue ()
{
    List* ptr;
    ptr = head;

    while (head)
    {
	head = ptr->next;
	delete ptr->work;
	delete ptr;
	ptr = head;
    }
}

Boolean Queue::IsEmpty () { return (Boolean) (length == 0); }

long Queue::QueueSize () { return length; }

Job* Queue::Dequeue ()
{
    if (IsEmpty())
	return 0;

    List* ptr = head;
    head = head->next;

    length--;

    Job* toReturn = ptr->work;
    delete ptr;

    return toReturn;
}

void Queue::Enqueue (Job* toadd)
{
    if (!toadd)
	return;

    List* ptr = head;
    
    if (IsEmpty())
    {
	head = new List;
	ptr = head;
    }
    else
    {
	while (ptr->next != 0)
	    ptr = ptr->next;

	ptr->next = new List;
	ptr = ptr->next;
    }

    ptr->next = 0;
    ptr->work = toadd;
    length++;
}

