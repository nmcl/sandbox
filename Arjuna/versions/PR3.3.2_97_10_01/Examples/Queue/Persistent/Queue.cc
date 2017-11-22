/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Queue.cc,v 1.3 1994/09/02 11:32:25 ngdp Exp $
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


Queue::Queue (const Uid& u, Boolean& res) : StateManager(u)
{
    headOfList = 0;
    tailOfList = 0;
    numberOfElements = 0;

    if (!StateManager::activate())
	res = FALSE;
    else
	res = TRUE;
}

Queue::Queue (Boolean& res) : StateManager(ANDPERSISTENT)
{
    headOfList = 0;
    tailOfList = 0;
    numberOfElements = 0;
    
    StateManager::modified();
    
    res = TRUE;
}

Queue::~Queue ()
{
    StateManager::deactivate();
    StateManager::terminate();
    
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
    QueueStatus res = NOT_DONE;
    QueueLinks* tmp = 0;
    
    StateManager::modified();
    
    if (numberOfElements <= QUEUE_SIZE)
    {
	tmp = new QueueLinks;
	tmp->data = new QueueElement(v);
	tmp->prev = tailOfList;
	if (tailOfList)
	    tailOfList->next = tmp;
	tailOfList = tmp;

	if (headOfList == 0)
	    headOfList = tailOfList;

	numberOfElements++;
	res = DONE;
    }
    else
	res = OVER_FLOW;

    return res;
}

QueueStatus Queue::dequeue (int& v)
{
    if (numberOfElements > 0)
    {
	StateManager::modified();
	
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
	StateManager::modified();
	
	QueueLinks* tmp = headOfList;
	
	for (int i = 0; i < index; i++)
	    tmp = tmp->next;
	
	tmp->data->value = v;
	return TRUE;
    }
    
    return FALSE;
}

Boolean Queue::save_state (ObjectState& os, ObjectType)
{
    QueueLinks* tmp = headOfList;
    Boolean     res = TRUE;
    
    res = os.pack(numberOfElements);
    
    for (int i = 0; (i < numberOfElements) && (res); i++)
    {
	res = os.pack(tmp->data->value);
	tmp = tmp->next;
    }
    
    return res;
}

Boolean Queue::restore_state (ObjectState& os, ObjectType)
{
    QueueLinks *tmp = headOfList;
    Boolean     res = TRUE;
    int    i, value = 0;
    
    for (i = 0; i < numberOfElements; i++)
    {
	tmp = tmp->next;
	delete headOfList;
	headOfList = tmp;
    }
    
    numberOfElements = 0;
    headOfList = 0;
    tailOfList = 0;
    
    res = os.unpack(numberOfElements);
    for (i = 0; (i < numberOfElements) && (res); i++)
    {
	res = os.unpack(value);
	tmp = new QueueLinks;
	tmp->data = new QueueElement(value);
	tmp->prev = tailOfList;
	if (tailOfList)
	    tailOfList->next = tmp;
	tailOfList = tmp;

	if (headOfList == 0)
	    headOfList = tailOfList;
    }
    
    return res;
}

const TypeName Queue::type () const
{
    return "/StateManager/Queue";
}

	
