#include "Queue.h"

Queue::Queue ()
{
    head = 0;
    length = 0;
}

Queue::~Queue () { delete head; }

boolean Queue::IsEmpty ()
{
    if (head == 0)
	return true;
    else
	return false;
}

long Queue::QueueSize () { return length; }

Job* Queue::Dequeue ()
{
    if (IsEmpty())
	return 0;

    List* ptr = head;
    head = head->next;

    length--;
    return ptr->work;
}

void Queue::Enqueue (Job* toadd)
{
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

    
