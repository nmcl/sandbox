/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: VersionPersis.cc,v 1.1 1993/10/07 15:08:23 nsmw Exp $
 */

#include <Arjuna/AtomicA.h>

#define STATEMANAGER_SERVER
#define LOCKMANAGER_SERVER
#define SEATPERSIS_SERVER

#include "VersionPersis_stub.h"

VersionPersis *VersionPersis::listHead = NULL;

VersionPersis *VersionPersis::Create(int& res)
{
    VersionPersis *tempPtr = new VersionPersis(res);

    tempPtr->nextElem = listHead;
    listHead          = tempPtr;

    return tempPtr;
}

VersionPersis *VersionPersis::Create(int& res, const Uid& uid)
{
    Boolean done = FALSE;

    VersionPersis *tempPtr = listHead;
    while ((! done) && (tempPtr != NULL))
    {
        if (uid == tempPtr->get_uid())
            done = TRUE;
        else
            tempPtr = tempPtr->nextElem;
    }

    if (tempPtr == NULL)
    {
        tempPtr           = new VersionPersis(res, uid);
        tempPtr->nextElem = listHead;
        listHead          = tempPtr;
    }
    else
	res = 1;	

    return tempPtr;
}

void VersionPersis::Destroy()
{
    VersionPersis *tempPtr = listHead;

    while (tempPtr != NULL)
    {
        tempPtr = listHead->nextElem;
        delete listHead;
        listHead = tempPtr;
    }
}

VersionPersis::VersionPersis(int& res) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;

    A.Begin();

    res = 0;
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
        _version = 0;
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

VersionPersis::VersionPersis(int& res, const Uid& U) : LockManager(U)
{
    res = 1;
}

VersionPersis::~VersionPersis()
{
    terminate();
}

Boolean VersionPersis::HasRemoteState () const { return TRUE; }

void VersionPersis::increase(int& res)
{
    TopLevelAction A;

    A.Begin();

    res = 0;
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	res = 1;
        _version++;
    }

    if (res != 0)
    {
        if (A.End() != COMMITTED)
            res = 0;
    }
    else
        A.Abort();
}

void VersionPersis::get(int& res, int& version)
{
    TopLevelAction A;

    A.Begin();

    res = 0;
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        version = _version;
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

Boolean VersionPersis::save_state(ObjectState& os, ObjectType)
{
    return os.pack(_version);
}

Boolean VersionPersis::restore_state(ObjectState& os, ObjectType)
{
    return os.unpack(_version);
}

const TypeName VersionPersis::type() const
{
    return "/StateManager/LockManager/VersionPersis";
}
