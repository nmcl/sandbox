/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: QueueElement.cc,v 1.2 1994/01/22 12:17:55 nmcl Exp $
 */


#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif


#define STATEMANAGER_SERVER
#define LOCKMANAGER_SERVER
#define QUEUEELEMENT_SERVER

#ifndef QUEUEELEMENT_H_
#  include "QueueElement_stub.h"
#endif


QueueElement::QueueElement (const Uid& u, Boolean& res) : LockManager(u)
{
    value = 0;
    res = TRUE;
}

QueueElement::QueueElement (int v, Boolean& res) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;
    
    value = 0;
    res = FALSE;
    
    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	value = v;

	if (A.End() == COMMITTED)
	    res = TRUE;
    }
    else
	A.Abort();
}

QueueElement::~QueueElement ()
{
    LockManager::terminate();
}

Boolean QueueElement::getValue (int& v, int& ret)
{
    AtomicAction A;
    Boolean res = FALSE;
    
    ret = 0;

    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	v = value;
	if (A.End() == COMMITTED)
	    res = TRUE;
    }
    else
	A.Abort();
    
    return res;
}

Boolean QueueElement::setValue (int v, int& ret)
{
    AtomicAction A;
    Boolean res = FALSE;

    ret = 0;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	value = v;
	if (A.End() == COMMITTED)
	    res = TRUE;
    }
    else
	A.Abort();
    
    return res;
}

Boolean QueueElement::save_state (ObjectState& os, ObjectType)
{
    return os.pack(value);
}

Boolean QueueElement::restore_state (ObjectState& os, ObjectType)
{
    return os.unpack(value);
}

const TypeName QueueElement::type () const
{
    return "/StateManager/LockManager/QueueElement";
}
