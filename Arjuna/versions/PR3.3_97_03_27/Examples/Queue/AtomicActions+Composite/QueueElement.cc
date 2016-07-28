/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: QueueElement.cc,v 1.3 1994/09/28 09:28:14 nmcl Exp $
 */


#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef QUEUEELEMENT_H_
#  include "QueueElement.h"
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

Boolean QueueElement::getValue (int& v)
{
    AtomicAction A;
    Boolean res = FALSE;
    
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

Boolean QueueElement::setValue (int v)
{
    AtomicAction A;
    Boolean res = FALSE;
    
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
