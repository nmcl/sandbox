/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Queue.cc,v 1.4 1994/09/02 11:32:20 ngdp Exp $
 */


#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif


#define STATEMANAGER_SERVER
#define LOCKMANAGER_SERVER
#define QUEUE_SERVER

#ifndef QUEUE_H_
#  include "Queue_stub.h"
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


Queue::Queue (const Uid& u, Boolean& res) : LockManager(u)
{
    headOfList = 0;
    tailOfList = 0;
    numberOfElements = 0;
    res = TRUE;
}

Queue::Queue (Boolean& res) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;

    headOfList = 0;
    tailOfList = 0;
    numberOfElements = 0;

    res = FALSE;
    
    if (A.Begin() == RUNNING)
    {
	if (setlock(new Lock(WRITE), 0) == GRANTED)
	{
	    if (A.End() == COMMITTED)
		res = TRUE;
	}
	else
	    A.Abort();
    }
}

Queue::~Queue ()
{
    LockManager::terminate();
    
    // delete list

    while (headOfList)
    {
	tailOfList = headOfList;
	headOfList = headOfList->next;
	delete tailOfList;
    }
}

/*
 * ret is a parameter used to indicate whether the rpc worked.
 */

QueueStatus Queue::enqueue (int v, int& ret)
{
    QueueStatus  res = NOT_DONE;
    QueueLinks*  tmp = 0;
    AtomicAction A;

    ret = 0;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
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
    }
    
    if (res == DONE)
    {
	if (A.End() != COMMITTED)
	    res = NOT_DONE;
    }
    else
	A.Abort();

    return res;
}

QueueStatus Queue::dequeue (int& v, int& ret)
{
    AtomicAction A;
    QueueStatus res = NOT_DONE;
    QueueLinks* tmp = 0;

    ret = 0;

    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (numberOfElements > 0)
	{
	    tmp = headOfList;
	    v = tmp->data->value;
	    headOfList = headOfList->next;
	    if (headOfList)
		headOfList->prev = 0;

	    delete tmp;
	    numberOfElements--;

	    if (numberOfElements == 0)
		tailOfList = 0;
	    
	    res = DONE;
	}
	else
	    res = UNDER_FLOW;
    }
    
    if (res == DONE)
    {
	if (A.End() != COMMITTED)
	    res = NOT_DONE;
    }
    else
	A.Abort();
    
    return res;
}

/*
 * We change this method slightly now because we want to signify that
 * a problem occurred within the action by returning a negative value.
 */

int Queue::queueSize (int& ret)
{
    AtomicAction A;
    int value = -1;

    ret = 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
	value = (int) numberOfElements;

    if (value > 0)
    {
	if (A.End() != COMMITTED)
	    value = -1;
    }
    else
	A.Abort();
    
    return value;
}

Boolean Queue::inspectValue (unsigned int index, int& v, int& ret)
{
    AtomicAction A;
    Boolean result = FALSE;

    ret = 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (numberOfElements >= index)
	{
	    QueueLinks* tmp = headOfList;
	
	    for (int i = 0; i < index; i++)
		tmp = tmp->next;
	    
	    v = tmp->data->value;
	    result = TRUE;
	}
    }
    
    if (result == TRUE)
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
	A.Abort();
    
    return result;
}

Boolean Queue::setValue (unsigned int index, int v, int& ret)
{
    Boolean result = FALSE;
    AtomicAction A;

    ret = 0;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (numberOfElements >= index)
	{
	    QueueLinks* tmp = headOfList;
	
	    for (int i = 0; i < index; i++)
		tmp = tmp->next;
	
	    tmp->data->value = v;
	    result = TRUE;
	}
    }
    
    if (result == TRUE)
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
	A.Abort();
    
    return result;
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
    return "/StateManager/LockManager/Queue";
}

	
