/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * Stack implementation. (AtomicAction)
 */

#include <Arjuna/Lock.h>
#include <Arjuna/AtomicA.h>
#include <Arjuna/StateMan.h>

#include "Stack.h"

Stack::Stack(enum stackOutcome& outcome) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;
    A.Begin();

    outcome = unknown;
    top = 0;
    if (setlock(new Lock(WRITE), 0) == GRANTED)
	outcome = done;

    if (outcome == done)
    {
	if (A.End() != COMMITTED)
	    outcome = not_done;
    }
    else
    {
	A.Abort();
	outcome = not_done;
    }
}

Stack::Stack(Uid uid, enum stackOutcome& outcome) : LockManager(uid)
{
    outcome = done;
}

Stack::~Stack()
{
    terminate();
}

void Stack::Push(enum stackOutcome& outcome, int  value)
{
    AtomicAction A;
    A.Begin();

    outcome = unknown;
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (top < stackSize)
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		values[top] = value;
		top ++;
		outcome = done;
	    }
	}
	else
	    outcome = stack_full;
    }

    if ((outcome == done) || (outcome == stack_full))
    {
	if (A.End() != COMMITTED)
	    outcome = not_done;
    }
    else
    {
	A.Abort();
	outcome = not_done;
    }
}

void Stack::Pop (enum stackOutcome& outcome, int& value)
{
    AtomicAction A;
    A.Begin();

    outcome = unknown;
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (top > 0)
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		top --;
		value = values[top];
		outcome = done;
	    }
	}
	else
	    outcome = stack_empty;
    }

    if ((outcome == done) || (outcome == stack_empty))
    {
	if (A.End() != COMMITTED)
	    outcome = not_done;
    }
    else
    {
	A.Abort();
	outcome = not_done;
    }
}

Boolean Stack::save_state(ObjectState& os, ObjectType)
{
    Boolean res;

    res = os.pack(top);
    for (int i = 0; res && (i < top); i++)
	res = os.pack(values[i]);

    return res;
}

Boolean Stack::restore_state(ObjectState& os, ObjectType)
{
    Boolean res;

    res = os.unpack(top);
    for (int i = 0; res && (i < top); i++)
	res = os.unpack(values[i]);

    return res;
}

const TypeName Stack::type() const
{
    return "/StateManager/LockManager/Stack";
}
