/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Queue.cc,v 1.2 1994/07/08 15:27:37 nmcl Exp $
 */


#ifndef QUEUE_H_
#  include "Queue.h"
#endif


QueueLinks::QueueLinks ()
		       : data(0),
			 next(0),
			 prev(0)
{
}

QueueLinks::~QueueLinks ()
{
    if (data)
	delete data;
}


Queue::Queue ()
              : headOfList(0),
		tailOfList(0),
		numberOfElements(0)
{
}

Queue::~Queue ()
{
    // delete list

    while (headOfList)
    {
	tailOfList = headOfList;
	headOfList = headOfList->next;
	delete tailOfList;
    }
}

QueueStatus Queue::enqueue (int v)
{
    if (numberOfElements <= QUEUE_SIZE)
    {
	QueueLinks* tmp = new QueueLinks;
	
	tmp->data = new QueueElement(v);
	tmp->prev = tailOfList;
	if (tailOfList)
	    tailOfList->next = tmp;
	tailOfList = tmp;
	
	if (headOfList == 0)
	    headOfList = tailOfList;
	
	numberOfElements++;
	
	return DONE;
    }
    
    return OVER_FLOW;
}

QueueStatus Queue::dequeue (int& v)
{
    if (numberOfElements > 0)
    {
	QueueLinks* tmp = headOfList;
	
	v = tmp->data->value;
	headOfList = headOfList->next;
	if (headOfList)
	    headOfList->prev = 0;

	delete tmp;
	numberOfElements--;

	if (numberOfElements == 0)
	    tailOfList = 0;
	
	return DONE;
    }
    
    return UNDER_FLOW;
}

unsigned int Queue::queueSize ()
{
    return numberOfElements;
}

Boolean Queue::inspectValue (unsigned int index, int& v)
{
    if (numberOfElements >= index)
    {
	QueueLinks* tmp = headOfList;
	
	for (int i = 0; i < index; i++)
	    tmp = tmp->next;
	
	v = tmp->data->value;
	return TRUE;
    }
    
    return FALSE;
}

Boolean Queue::setValue (unsigned int index, int v)
{
    if (numberOfElements >= index)
    {
	QueueLinks* tmp = headOfList;
	
	for (int i = 0; i < index; i++)
	    tmp = tmp->next;
	
	tmp->data->value = v;
	return TRUE;
    }
    
    return FALSE;
}


	
	    
