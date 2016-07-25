/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Object.cc,v 1.5 1995/03/09 12:35:16 ngdp Exp $
 */

#include "Object.h"

#include <System/iostream.h>

#include <Arjuna/AtomicA.h>

Object::Object(int& res) : LockManager(ANDPERSISTENT,FALSE,FALSE), number(0)
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

Object::Object(Uid& u, int& res) : LockManager(u,ANDPERSISTENT,FALSE)
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

Boolean Object::value(int& v)
{
    AtomicAction C;

    C.Begin();
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
		C.End();
		v = number;
		return TRUE;
    }
    else
    {
		C.Abort();
		return FALSE;
    }
}

Boolean Object::save_state(ObjectState& os, ObjectType)
{
    return os.pack(number);
}

Boolean Object::restore_state(ObjectState& os, ObjectType)
{
    return os.unpack(number);
}

const TypeName Object::type() const
{
    return "/StateManager/LockManager/Object";
}

