/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NameInfo.cc,v 1.13 1994/09/02 10:26:58 ngdp Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef MALLOC_H_
#  include <System/malloc.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef NAMEINFO_H_
#  include <ArjServers/NameInfo.h>
#endif


NameData::NameData ()
                    : hostname(0),
		      myUID(0),
		      next(0)
{
}

NameData::NameData (const Uid& u, char* node)
                                            : hostname(0),
					      next(0)
{
    myUID = new Uid(u);
    if (node)
    {
	hostname = ::new char[::strlen(node) +1];
	::memset(hostname, '\0', ::strlen(node)+1);
	::memcpy(hostname, node, ::strlen(node));
    }
}

NameData::~NameData ()
{
    if (hostname)
#ifndef GCC_ARRAY_DELETE_BUG
	::delete [] hostname;
#else
        ::delete hostname;
#endif  

    if (myUID)
	delete myUID;
    if (next)
	delete next;
}

NameData& NameData::operator= (const NameData& nd)
{
    if (&nd == this)
	return *this;

    if (nd.hostname)
    {
	if (hostname)
#ifndef GCC_ARRAY_DELETE_BUG
	    ::delete [] hostname;
#else
            ::delete hostname;
#endif
    
	hostname = ::new char[::strlen(nd.hostname)+1];
	::memset(hostname, '\0', ::strlen(nd.hostname)+1);
	::memcpy(hostname, nd.hostname, ::strlen(nd.hostname));
    }
    
    if (nd.myUID)
    {
	if (myUID)
	    delete myUID;
	myUID = new Uid(*nd.myUID);
    }
    
    return *this;
}

Boolean NameData::operator== (const NameData& nd)
{
    if (nd.myUID && nd.hostname && myUID && hostname)
	return (Boolean) ((::strcmp(nd.hostname, hostname) == 0) &&
			  (*nd.myUID == *myUID));
    else
	return FALSE;
}

NameInfo::NameInfo (Boolean& done) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;

    A.Begin();

    head = (NameData*) 0;
    replicationLevel = 0;

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
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

    head = (NameData*) 0;
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

NameData* NameInfo::getReplicationInfo (Boolean& done, int& level)
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
    return (NameData*) 0;
}

Boolean NameInfo::setReplicationInfo (NameData* touse)
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
	while (marker != (NameData*) 0)
	{
	    replicationLevel++;
	    ptr->hostname = new char[strlen(marker->hostname)+1];
	    memset(ptr->hostname, '\0', strlen(marker->hostname)+1);
	    memcpy(ptr->hostname, marker->hostname, strlen(marker->hostname));
	    ptr->myUID = new Uid(*marker->myUID);
	    if (marker->next != (NameData*) 0)
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

Boolean NameInfo::add (NameData* toAdd)
{
    Boolean res = FALSE;
    AtomicAction A;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	NameData* newEntry = new NameData;
	*newEntry = *toAdd;
	newEntry->next = head;
	head = newEntry;
	replicationLevel++;
	res = TRUE;
    }
    
    if (res)
    {
	if (A.End() != COMMITTED)
	    res = FALSE;
    }
    else
	A.Abort();

    return res;
}

Boolean NameInfo::remove (NameData* toRemove)
{
    Boolean res = FALSE;
    AtomicAction A;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	NameData *tmp = head, *trail = (NameData*) 0;
	Boolean found = FALSE;
	
	while ((tmp) && (!found))
	{
	    if (*tmp == *toRemove)
		found = TRUE;
	    else
	    {
		trail = tmp;
		tmp = tmp->next;
	    }
	}
	
	if (found)
	{
	    if (trail == (NameData*) 0)  // must be head
		head = tmp->next;
	    else
		trail->next = tmp->next;
	    tmp->next = (NameData*) 0;
	    delete tmp;
	    replicationLevel--;
	    res = TRUE;
	}
    }

    if (res)
    {
	if (A.End() != COMMITTED)
	    res = FALSE;
    }
    else
	A.Abort();
    
    return res;
}

Boolean NameInfo::reset ()
{
    AtomicAction A;
    Boolean res = FALSE;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (head)
	{
	    delete head;
	    head = (NameData*) 0;
	}

	replicationLevel = 0;
	res = TRUE;
    }

    if (res)
    {
	if (A.End() != COMMITTED)
	    res = FALSE;
    }
    else
	A.Abort();

    return res;
}

Boolean NameInfo::save_state (ObjectState& os, ObjectType)
{
    Boolean res;

    res = os.pack(replicationLevel);
    if (replicationLevel == 0)
	return res;

    NameData* marker = head;
    for (int i = 0; (i < replicationLevel) && res; i++)
    {
	res = os.packMappedString(marker->hostname) && marker->myUID->pack(os);
	marker = marker->next;
    }

    return res;
}

Boolean NameInfo::restore_state (ObjectState& os, ObjectType)
{
    Boolean res;

    if (head)
    {
	delete head;
	head = (NameData*) 0;
    }

    replicationLevel = 0;
    res = os.unpack(replicationLevel);

    if (replicationLevel < 0)
	return res;

    for (int i = 0; (i < replicationLevel) && res; i++)
    {
	NameData* marker = new NameData;
	marker->myUID = new Uid;
	res = os.unpack(marker->hostname) && marker->myUID->unpack(os);
	if (!res)
	    break;
	else
	{
	    marker->next = head;
	    head = marker;
	}
    }

    return res;
}

const TypeName NameInfo::type () const
{
    return "/StateManager/LockManager/NameInfo";
}
