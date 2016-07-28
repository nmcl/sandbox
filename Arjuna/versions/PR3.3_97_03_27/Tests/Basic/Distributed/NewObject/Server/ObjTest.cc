/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjTest.cc,v 1.3 1994/09/28 09:32:12 nmcl Exp $
 */

#include <System/iostream.h>

#include <Arjuna/AtomicA.h>

#define STATEMANAGER_SERVER
#define LOCKMANAGER_SERVER
#define OBJTEST_SERVER

#include "ObjTest_stub.h"

ObjTest::ObjTest(int& res, Boolean do_abort) : LockManager(ANDPERSISTENT)
{
    cout << "New persistent object\n" << flush;
    AtomicAction B;

    B.Begin();
    cout << "B.Begin() : Started\n" << flush;

    state_number = 1;

    cout << "Setting write lock. Should call save_state\n" << flush;
    if (setlock(new Lock(WRITE), 0) != GRANTED)
    {
	cout << "Error: Lock refused\n" << flush;
	res = -1;
    }
    else
    {
	res = 1;
	cout << "state_number is " << state_number << "\n" << flush;
	if (do_abort)
	{
	    B.Abort();
	    cout << "B.Abort : Done\n" << flush;
	}
	else
	{
	    if (B.End() != COMMITTED)
	    {
		cout << "B.End() : Not Done\n" << flush;
		res = 0;
	    }
	    else
		cout << "B.End() : Done\n" << flush;
	}
    }

    cout << "state_number is " << state_number << "\n" << flush;
}

ObjTest::~ObjTest() { terminate(); }

Boolean ObjTest::save_state(ObjectState& os, ObjectType)
{
    cout << "Saving state\n" << flush;
    return (Boolean) os.pack(state_number);
}

Boolean ObjTest::restore_state(ObjectState& os, ObjectType)
{
    cout << "Restoring state\n" << flush;
    return (Boolean) os.unpack(state_number);
}

const TypeName ObjTest::type() const
{
    return "/StateManager/LockManager/ObjTest";
}
