#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef ATOMICINT_H_
#  include "AtomInt.h"
#endif

#include <iostream.h>

AtomicInt::AtomicInt (int& res) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;

    A.Begin();

    value = 0;

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

AtomicInt::AtomicInt (int& res, const Uid& u) : LockManager(u)
{
    AtomicAction A;

    A.Begin();

    value = 0;

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
};

AtomicInt::~AtomicInt ()
{
    LockManager::terminate();
}

Boolean AtomicInt::set (int v)
{
    AtomicAction A;
    Boolean result = FALSE;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	value = v;
	result = TRUE;
    }

    if (result)
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
	A.Abort();

    return result;
}

Boolean AtomicInt::get (int& v)
{
    AtomicAction A;
    Boolean result = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	v = value;
	result = TRUE;
    }

    if (result)
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
	A.Abort();

    return result;
}

Boolean AtomicInt::save_state (ObjectState& os, ObjectType)
{
    return os.pack(value);
}

Boolean AtomicInt::restore_state (ObjectState& os, ObjectType)
{
    return os.unpack(value);
}

const TypeName AtomicInt::type () const
{
#ifndef MSDOS
    return ("/StateManager/LockManager/AtomicInt");
#else
    return ("/StateMan/LockMan/AtomicInt");
#endif
}
