/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Object.cc,v 1.1 1993/11/03 14:58:14 nmcl Exp $
 */

#define STATEMANAGER_SERVER
#define LOCKMANAGER_SERVER
#define OBJECT_SERVER

#include "Object_stub.h"

#include <iostream.h>
#include <Arjuna/AtomicA.h>

Object::Object(int& res) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;

    A.Begin();
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	number = 0;
	A.End();
	res = 0;
    }
    else
    {
	A.Abort();
	res = -1;
    }
}

Object::Object(Uid& u, int& res) : LockManager(u)
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

