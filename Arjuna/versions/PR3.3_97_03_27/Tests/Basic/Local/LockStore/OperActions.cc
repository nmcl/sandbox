/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: OperActions.cc,v 1.1 1994/10/12 14:55:18 ngdp Exp $
 */

#include <System/iostream.h>
#include <System/memory.h>
#include <Arjuna/Lock.h>
#include <Arjuna/AtomicA.h>

#include "OperActions.h"

OperActions::OperActions()
			: LockManager(ANDPERSISTENT),
			  value(1)
{
    AtomicAction A;
    
    A.Begin();
    if (setlock(new Lock(WRITE), 0) != GRANTED)
    {
	A.Abort();
	cerr << "OperActions::OperActions( . . . ): Unable to set lock\n" << flush;
    }
    else
	if (A.End() != COMMITTED)
	    cerr << "OperActions::OperActions( . . . ): Unable to commit\n" << flush;
}

OperActions::OperActions ( const Uid& u ) 
                         : LockManager(u), value(0)
{
}

OperActions::~OperActions()
{
    terminate();

}

void OperActions::anop()
{
}

Boolean OperActions::save_state(ObjectState& os, ObjectType)
{
    return os.pack(value);
}

Boolean OperActions::restore_state(ObjectState& os, ObjectType)
{
    return os.unpack(value);
}

const TypeName OperActions::type() const
{
    return "/StateManager/LockManager/OperActions";
}
