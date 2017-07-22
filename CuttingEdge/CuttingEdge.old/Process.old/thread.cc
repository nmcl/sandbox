#include "thread.h"

Thread::Thread()
{
    Thread *marker = head;

    if (head != 0)
    {
	while (marker->next != 0)
	    marker = marker->next;

	marker->next = this;
	marker->next->prev = marker;
	marker = marker->next;
    }
    else
    {
	this->prev = 0;
	this->next = 0;
	head = this;
    }
}

Thread::~Thread()
{
    if (this->prev != 0)
	this->prev->next = this->next;
    else
	head = this->next;
}

Thread *Thread::Self()
{
    Thread *marker = head;
    long my_id = head->Identity();
    boolean found = false;

    while ((marker != 0) && (!found))
    {
	if (marker->thread_key == my_id)
	    found = true;
	else
	    marker = marker->next;
    }

    return marker;
}
