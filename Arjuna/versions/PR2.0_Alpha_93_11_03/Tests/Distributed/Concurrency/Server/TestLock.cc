/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TestLock.cc,v 1.1 1993/11/03 14:53:47 nmcl Exp $
 */

#define STATEMANAGER_SERVER
#define LOCKMANAGER_SERVER
#define TESTLOCK_SERVER

#include "TestLock_stub.h"

#include <Common/Uid.h>
#include <iostream.h>
#include <stdio.h>

TestLock::TestLock(int& res) : LockManager(ANDPERSISTENT)
{
    for (int i = 0; i < 10; i++)
    {
	held[i] = 0;
	Owner[i] = FALSE;
    }
    
    number = 0;

    res = 0;
}

TestLock::~TestLock() { terminate(); }

Boolean TestLock::acquire(int number, Boolean IsParent)
{
    held[number] = new Lock(WRITE);

    if (setlock(held[number], 0) == GRANTED)
    {
	Owner[number] = IsParent;
	return TRUE;
    }
    else
	return FALSE;
}

Boolean TestLock::release(int number, Boolean IsParent)
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

Boolean TestLock::save_state(ObjectState& os, ObjectType)
{
    return os.pack(number);
}

Boolean TestLock::restore_state(ObjectState& os, ObjectType)
{
    return os.unpack(number);
}

const TypeName TestLock::type() const
{
    return "/StateManager/LockManager/TestLock";
}

