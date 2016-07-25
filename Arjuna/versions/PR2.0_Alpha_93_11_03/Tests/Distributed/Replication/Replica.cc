/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Replica.cc,v 1.1 1993/11/03 14:59:06 nmcl Exp $
 */

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#ifndef REPLICA_H_
#include "Replica.h"
#endif

Replica::Replica(int& res) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;

    A.Begin();

    Replicated = FALSE;
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

Replica::Replica(Uid& u, int& res) : LockManager(u)
{
    Replicated = FALSE;
    res = 0;
}

Replica::~Replica() { terminate(); }

Boolean Replica::change(int new_value)
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

int Replica::value()
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

Boolean Replica::HasRemoteState () const { return Replicated; }

void Replica::makeReplicated (Boolean flag) { Replicated = flag; }

Boolean Replica::save_state(ObjectState& os, ObjectType)
{
    return os.pack(number);
}

Boolean Replica::restore_state(ObjectState& os, ObjectType)
{
    return os.unpack(number);
}

const TypeName Replica::type() const
{
    return "/StateManager/LockManager/Replica";
}

