/*
 * Copyright (C) 1993-1998,
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Object.cc,v 1.5 1996/03/29 14:10:19 ngdp Exp $
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

    //    useLockDaemon();
    
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

Object::Object(Uid& u, int& res) : LockManager(u)
{
    //    useLockDaemon();
 
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

Boolean Object::prematureCommit ()
{
    AtomicAction *aa = AtomicAction::Current();
    
    if (aa && (aa->End() == COMMITTED))
	return TRUE;
    else
	return FALSE;
}

Boolean Object::prematureAbort ()
{
    AtomicAction *aa = AtomicAction::Current();
	
    if (aa && (aa->Abort() == ABORTED))
	return TRUE;
    else
	return FALSE;
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

