/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Object.cc,v 1.3 1994/09/28 09:32:39 nmcl Exp $
 */

#include "Object.h"

#include <System/iostream.h>

#include <Arjuna/AtomicA.h>
#include <Common/Debug.h>

Object::Object(int& res) : LockManager(ANDPERSISTENT), number(0)
{
    AtomicAction A;

    A.Begin();

    number = 0;

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	A.End();
	res = 0;
    }
    else
    {
	A.Abort();
	res = -1;
    }
}

Object::Object(Uid& u, int& res) : LockManager(u), number(0)
{
    res = 1;
}

Object::~Object() { terminate(); }

Boolean Object::change(int new_value)
{
    AtomicAction B;

    B.Begin();
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	number = new_value;
	B.End();
	return TRUE;
    }
    else
    {
	B.Abort();
	return FALSE;
    }
}

int Object::value()
{
    AtomicAction C;

    C.Begin();
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	C.End();
	return number;
    }
    else
    {
	C.Abort();
	return -1;
    }
}

Boolean Object::save_state(ObjectState& os, ObjectType)
{
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_STATE_MAN << VIS_PUBLIC;
	debug_stream << "Object::save_state ()" << endl;
#endif

    return os.pack(number);
}

Boolean Object::restore_state(ObjectState& os, ObjectType)
{
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_STATE_MAN << VIS_PUBLIC;
	debug_stream << "Object::restore_state ()" << endl;
#endif
    return os.unpack(number);
}

const TypeName Object::type() const
{
    return "/StateManager/LockManager/Object";
}


Object *Object::getLink ()
{
    return nextObject;
}

void Object::setLink ( Object *setTo )
{    
    nextObject = setTo;
}


