/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: TowerPersis.cc,v 1.1 1993/11/03 12:25:28 nmcl Exp $
 */

#define STATEMANAGER_SERVER
#define LOCKMANAGER_SERVER
#define TOWERPERSIS_SERVER

#include "TowerPersis_stub.h"

#include <Common/Boolean.h>
#include <Common/Error.h>
#include <Arjuna/AtomicA.h>
#include <Replication/RemoteOSTypes.h>

TowerPersis::TowerPersis(int& res) : LockManager(ANDPERSISTENT)
{
    for (int i = 0; i < maxDisk; i++)
	disks[i] = 0;
    
    AtomicAction A;
    
    A.Begin();
    
    remoteState = FALSE;
    res = 0;
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	diskNumber  = 0;
	res = 1;
    }
    
    if (res != 0)
    {
	if (A.End() != COMMITTED)
	    res = 0;
    }
    else
        A.Abort();
}

TowerPersis::TowerPersis(int& res, Uid& U, Boolean isReplicated) : LockManager(U)
{
    remoteState = isReplicated;
    for (int i = 0; i < maxDisk; i++)
	disks[i] = 0;
    res = 1;
}

TowerPersis::~TowerPersis()
{
    terminate();
}

Boolean TowerPersis::HasRemoteState () const { return remoteState; }

void TowerPersis::PushDisk(int size, TowerState& state, int& res)
{
    AtomicAction A;
    
    A.Begin();

    res = 0;
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (diskNumber < maxDisk)
	{
	    disks[diskNumber] = size;
	    diskNumber++;
	    for (int i = 0; i < diskNumber; i++)
		state.state[i] = disks[i];
	    state.num = diskNumber;
	    res = 1;
	}
	else
	{
	    error_stream << "TowerPersis::PushDisk Tower Overflow\n";
	    res = 0;
	}
    }

    if (res != 0)
    {
	if (A.End() != COMMITTED)
	    res = 0;
    }
    else
        A.Abort();
}

void TowerPersis::PopDisk(int& size, TowerState& state, int& res)
{
    AtomicAction A;
    
    A.Begin();

    res = 0;
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (diskNumber > 0)
	{
	    diskNumber--;
	    size  = disks[diskNumber];
	    disks[diskNumber] = 0;
	    for (int i = 0; i < diskNumber; i++)
		state.state[i] = disks[i];
	    state.num = diskNumber;
	    res = 1;
	}
	else
	{
	    error_stream << "TowerPersis::PopDisk Tower Underflow\n";
	    res = 0;
	}
    }

    if (res != 0)
    {
	if (A.End() != COMMITTED)
	    res = 0;
    }
    else
        A.Abort();
}

void TowerPersis::GetState(TowerState& state, int& res)
{
    AtomicAction A;
    
    A.Begin();

    res = 0;
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	for (int i = 0; i < diskNumber; i++)
	    state.state[i] = disks[i];
	state.num = diskNumber;
	res = 1;
    }

    if (res != 0)
    {
        if (A.End() != COMMITTED)
            res = 0;
    }
    else
    {
        res = 0;
        A.Abort();
    }
}

Boolean TowerPersis::save_state(ObjectState& os, ObjectType t)
{
    Boolean res = FALSE;

    res = os.pack(diskNumber);
    for (int i = 0; (i < maxDisk) && res; i++)
	res = os.pack(disks[i]);	

    return res;
}

Boolean TowerPersis::restore_state(ObjectState& os, ObjectType t)
{
    Boolean res = FALSE;

    res = os.unpack(diskNumber);
    for (int i = 0; (i < maxDisk) && res; i++)
	res = os.unpack(disks[i]);

    return res;
}

const TypeName TowerPersis::type() const
{
    return "/StateManager/LockManager/TowerPersis";
}
