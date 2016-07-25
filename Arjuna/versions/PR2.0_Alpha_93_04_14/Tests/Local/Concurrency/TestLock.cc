/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TestLock.cc,v 1.6 1993/03/18 15:07:28 n048z Exp $
 */

#include <Common/Uid.h>

#include "TestLock.h"

TestLock::TestLock () : LockManager(ANDPERSISTENT)
{
    for (int i = 0; i < 10; i++)
    {
	held[i] = 0;
	Owner[i] = FALSE;
    }
}

TestLock::~TestLock () { terminate(); }

LockCode TestLock::acquire (int number, Boolean IsParent)
{
    Lock *new_lock = new Lock(WRITE);
    held[number] = new_lock;

    if (setlock(new_lock, 0) == GRANTED)
    {
	Owner[number] = IsParent;
	return Set;
    }
    else
	return NotSet;
}

Boolean TestLock::release (int number, Boolean IsParent)
{
    Boolean result = FALSE;

    if ((held[number]) && (Owner[number] == IsParent))
    {
	result = releaselock(held[number]->getOwner());
	if (result)
	    held[number] = 0;
    }

    return result;
}

Boolean TestLock::save_state (ObjectState& os, ObjectType)
{
    return TRUE;
}

Boolean TestLock::restore_state (ObjectState& os, ObjectType)
{
    return TRUE;
}

const TypeName TestLock::type () const
{
    return "/StateManager/LockManager/TestLock";
}

