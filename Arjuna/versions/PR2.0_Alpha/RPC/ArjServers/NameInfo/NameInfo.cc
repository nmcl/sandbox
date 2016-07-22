/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NameInfo.cc
 */

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef STDLIB_H_
#include <System/stdlib.h>
#endif

#ifndef MALLOC_H_
#include <System/malloc.h>
#endif

#ifndef MEMORY_H_
#include <System/memory.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef DEBUG_H_
#include <Common/Debug.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef BUFFER_H_
#include <Common/Buffer.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#include <RPC/ArjServers/NameInfo.h>

NameData::NameData () { next = 0; }

NameData::~NameData ()
{
    if (next)
	delete next;
}

NameInfo::NameInfo (Boolean& done) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	head = 0;
	replicationLevel = 0;

	if (A.End() == COMMITTED)
	    done = TRUE;
    }
    else
	A.Abort();
}

NameInfo::NameInfo (const Uid& u, Boolean& done) : LockManager(u)
{
    AtomicAction A;

    A.Begin();

    head = 0;
    replicationLevel = 0;

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    done = TRUE;
    }
    else
	A.Abort();
}

NameInfo::~NameInfo ()
{
    terminate();

    if (head)
	delete head;
}

NameData* NameInfo::GetReplicationInfo (Boolean& done, int& level)
{
    AtomicAction C;

    C.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (C.End() == COMMITTED)
	{
	    done = TRUE;
	    level = replicationLevel;
	    return head;
	}
    }
    else
	C.Abort();

    done = FALSE;
    return 0;
}

Boolean NameInfo::SetReplicationInfo (NameData* touse)
{
    AtomicAction C;

    C.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (head)
	    delete head;

	NameData *marker = touse, *ptr;
	ptr = new NameData;
	head = ptr;
	replicationLevel = 0;
	while (marker != 0)
	{
	    replicationLevel++;
	    ptr->hostname = new char[strlen(marker->hostname)+1];
	    memset(ptr->hostname, '\0', strlen(marker->hostname)+1);
	    memcpy(ptr->hostname, marker->hostname, strlen(marker->hostname));
	    ptr->myUID = new Uid(*marker->myUID);
	    if (marker->next != 0)
	    {
		ptr->next = new NameData;
		ptr = ptr->next;
	    }
	    marker = marker->next;
	}

	if (C.End() == COMMITTED)
	    return TRUE;
    }
    else
	C.Abort();

    return FALSE;
}

Boolean NameInfo::save_state (ObjectState& os, ObjectType)
{
    Boolean res;

    res = os.pack(replicationLevel);
    if (!head)
	return res;

    NameData* marker = head;
    while ((marker != 0) && (res))
    {
	res = os.pack(marker->hostname) && marker->myUID->pack(os);
	marker = marker->next;
    }

    return res;
}

Boolean NameInfo::restore_state (ObjectState& os, ObjectType)
{
    Boolean res;

    head = 0;
    res = os.unpack(replicationLevel);
    if (replicationLevel == 0)
	return res;

    if (replicationLevel < 0)
	return FALSE;

    head = new NameData;
    NameData* marker = head;
    
    for (int i = 0; i < replicationLevel; i++)
    {
	marker->myUID = new Uid;
	res = os.unpack(marker->hostname) && marker->myUID->unpack(os);
	if (!res)
	    break;
	else
	    if (i+1 < replicationLevel)
	    {
		marker->next = new NameData;
		marker = marker->next;
	    }
    }

    return res;
}

const TypeName NameInfo::type () const
{
    return "/StateManager/LockManager/NameInfo";
}
