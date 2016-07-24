/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 */

/*
 * $Id: OperActions.cc,v 1.1 1993/11/03 14:58:27 nmcl Exp $
 */

#include <System/iostream.h>

#include <Arjuna/Lock.h>
#include <Arjuna/AtomicA.h>

#define STATEMANAGER_SERVER
#define LOCKMANAGER_SERVER
#define OPERACTIONS_SERVER

#include "OperActions_stub.h"

OperActions::OperActions(size_t s, ObjectType ot) : LockManager(ot)
{
    AtomicAction A;

    A.Begin();

        size = s;
        value = new char[size];

        if (setlock(new Lock(WRITE), 0) != GRANTED)
	    cerr << "OperActions::OperActions( . . . ): Unable to set lock\n" << flush;

    if (A.End() != COMMITTED)
	cerr << "OperActions::OperActions( . . . ): Unable to commit\n" << flush;
}

OperActions::~OperActions()
{
    terminate();

    if (value != NULL)
	delete value;
}

void OperActions::NOP()
{
}

void OperActions::BRE()
{
    AtomicAction A;

    A.Begin();

        if (setlock(new Lock(READ), 0) != GRANTED)
	    cerr << "OperActions::BRE( . . . ): Unable to set lock\n" << flush;

    if (A.End() != COMMITTED)
	cerr << "OperActions::BRE( . . . ): Unable to commit\n" << flush;
}

void OperActions::BWE()
{
    AtomicAction A;

    A.Begin();

        if (setlock(new Lock(WRITE), 0) != GRANTED)
	    cerr << "OperActions::BWE( . . . ): Unable to set lock\n" << flush;

    if (A.End() != COMMITTED)
	cerr << "OperActions::BWE( . . . ): Unable to commit\n" << flush;
}

void OperActions::BRA()
{
    AtomicAction A;

    A.Begin();

        if (setlock(new Lock(READ), 0) != GRANTED)
	    cerr << "OperActions::BRA( . . . ): Unable to set lock\n" << flush;

    if (A.Abort() != ABORTED)
	cerr << "OperActions::BRA( . . . ): Unable to commit\n" << flush;
}

void OperActions::BWA()
{
    AtomicAction A;

    A.Begin();

        if (setlock(new Lock(WRITE), 0) != GRANTED)
	    cerr << "OperActions::BWA( . . . ): Unable to set lock\n" << flush;

    if (A.Abort() != ABORTED)
	cerr << "OperActions::BWA( . . . ): Unable to commit\n" << flush;
}

Boolean OperActions::save_state(ObjectState& os, ObjectType)
{
    return os.pack(value, size);
}

Boolean OperActions::restore_state(ObjectState& os, ObjectType)
{
    return os.unpack(value, size);
}

const TypeName OperActions::type() const
{
    return "/StateManager/LockManager/OperActions";
}
