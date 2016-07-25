/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: WorkStack.cc,v 1.3 1993/08/06 10:48:21 nmcl Exp $
 */

#include <System/iostream.h>

#define STATEMANAGER_SERVER
#define LOCKMANAGER_SERVER
#define WORKSTACK_SERVER

#include "WorkStack_stub.h"

#include <Arjuna/AtomicA.h>
#include <RPC/ArjServers/RemoteOSTypes.h>

WorkStack::WorkStack(int sz, int& res) : LockManager(ANDPERSISTENT)
{
    works       = 0;
    AtomicAction A;
    remoteState = FALSE;

    A.Begin();
    
    res = 0;
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	size        = sz;
	workNumber  = 0;
	works       = new Work[size];
	res         = 1;
    }
    
    if (res == 1)
    {
	if (A.End() != COMMITTED)
	    res = 0;
    }
    else
        A.Abort();
}

WorkStack::WorkStack(int& res, Uid& U, Boolean isReplicated) : LockManager(U)
{
    size        = 0;
    workNumber  = 0;
    works       = 0;
    res         = 1;
    remoteState = isReplicated;
}

WorkStack::~WorkStack()
{
    terminate();
}

Boolean WorkStack::HasRemoteState () const { return remoteState; }

void WorkStack::PushWork(struct Work work, int& res)
{
    AtomicAction A;

    A.Begin();

    res = 0;
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (workNumber < size)
	{
	    works[workNumber] = work;
	    workNumber++;
	    res = 1;
	}
	else
	{
	    cerr << "WorkStack::PopWork Stack Overflow\n";
	    res = 0;
	}
    }

    if (res == 1)
    {
	if (A.End() != COMMITTED)
	    res = 0;
    }
    else
        A.Abort();
}

void WorkStack::PopWork(struct Work& work, int& res)
{
    AtomicAction A;
    
    A.Begin();

    res = 0;
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (workNumber > 0)
	{
	    workNumber--;
	    work = works[workNumber];
	    res = 1;
	}
	else
	{
	    cerr << "WorkStack::PopWork Stack Underflow\n";
	    res = 0;
	}
    }

    if (res == 1)
    {
	if (A.End() != COMMITTED)
	    res = 0;
    }
    else
        A.Abort();
}

Boolean WorkStack::save_state(ObjectState& os, ObjectType t)
{
    Boolean res = FALSE;

    res = os.pack(size);

    res = res && os.pack(workNumber);

    for (int i = 0; (i < size) && res; i++)
        res = os.pack(works[i].start) && os.pack(works[i].pinTo) &&
	      os.pack(works[i].pinFrom) && os.pack(works[i].pinTmp) &&
	      os.pack(works[i].number);

    return res;
}

Boolean WorkStack::restore_state(ObjectState& os, ObjectType t)
{
    Boolean res = FALSE;

    res = os.unpack(size);
    if (res && (works == 0))
	works = new Work[size];

    res = res && os.unpack(workNumber);

    for (int i = 0; (i < size) && res; i++)
        res = os.unpack(works[i].start) && os.unpack(works[i].pinTo) &&
	      os.unpack(works[i].pinFrom) && os.unpack(works[i].pinTmp) &&
	      os.unpack(works[i].number);

    return res;
}

const TypeName WorkStack::type() const
{
    return "/StateManager/LockManager/WorkStack";
}
