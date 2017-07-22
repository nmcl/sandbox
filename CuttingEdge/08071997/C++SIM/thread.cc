/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef THREAD_H_
#include "thread.h"
#endif


class Thread *Thread::head = 0; // Initialise head of Thread list


Thread::Thread ()
{
    Thread *marker = head;

    if (head != 0)
    {
	while (marker->next != 0)
	    marker = marker->next;

	marker->next = this;
	marker->next->prev = marker;
	marker = marker->next;
	marker->next = 0;
    }
    else
    {
	this->prev = 0;
	this->next = 0;
	head = this;
    }

    thread_key = 0;
    // Actual thread key value MUST be set in derived class constructor
}

Thread::~Thread ()
{
    if (this->prev != 0)
	this->prev->next = this->next;
    else
	head = this->next;
}

long Thread::Identity () const { return thread_key; }

Thread *Thread::Self ()
{
    if (!head) return 0;

    // Use any thread object (e.g., head) to get access to the current thread
    long my_id = head->Current_Thread();

    for (Thread *marker = head; marker; marker=marker->next)
	if (marker->thread_key == my_id)
	    break;

    return marker;
}
