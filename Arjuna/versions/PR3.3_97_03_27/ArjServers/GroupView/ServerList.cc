/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ServerList.cc,v 1.5 1995/07/11 14:04:08 nmcl Exp $
 */

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef SERVERLIST_H_
#  include "ServerList.h"
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif


ServerList* ServerList::headOfList = (ServerList*) 0;


ServerList::ServerList (int& res) : LockManager(ANDPERSISTENT),
                                    linkCount(0),
                                    next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PROTECTED;
    debug_stream << "ServerList::ServerList (int& res)" << endl;
#endif
    
    AtomicAction A;

    A.Begin();

    self = (ServerData*) 0;
    v_number = 1;

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

ServerList::ServerList (const Uid& u_id, int& res) : LockManager(u_id),
                                                     linkCount(0),
                                                     next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PROTECTED;
    debug_stream << "ServerList::ServerList (const Uid& u_id, int& res)" << endl;
#endif

    AtomicAction A;

    A.Begin();

    self = (ServerData*) 0;
    v_number = 1;

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

ServerList::~ServerList ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PROTECTED;
    debug_stream << "ServerList::~ServerList ()" << endl;
#endif

    terminate();

    if (self)
	delete self;
}

ServerList* ServerList::create (int& res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ServerList* ServerList::create (int& res)" << endl;
#endif

    ServerList* slPtr = headOfList;
    
    if (headOfList == (ServerList*) 0)
    {
        headOfList = new ServerList(res);
	slPtr = headOfList;
    }
    else
    {
        while (slPtr->next)
	    slPtr = slPtr->next;
	slPtr->next = new ServerList(res);
	slPtr = slPtr->next;
    }

    slPtr->linkCount++;
    return slPtr;
}

ServerList* ServerList::create (const Uid& u, int& res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ServerList* ServerList::create (const Uid& u, int& res)" << endl;
#endif

    ServerList* slPtr = headOfList;
    Boolean found = FALSE;

    if (headOfList == (ServerList*) 0)
    {
        headOfList = new ServerList(u, res);
	slPtr = headOfList;
    }
    else
    {
        while ((slPtr->next) && (!found))
	{
	    if (slPtr->get_uid() == u)
	        found = TRUE;
	    else
	        slPtr = slPtr->next;
	 }

	if (!found)
	{
	    slPtr->next = new ServerList(u, res);
	    slPtr = slPtr->next;
	}
    }

    slPtr->linkCount++;
    return slPtr;
}

void ServerList::remove (ServerList*& slPtr, Boolean canDelete)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ServerList::~ServerList ()" << endl;
#endif

    ServerList *tempPtr = headOfList, *indx = (ServerList*) 0;
    Boolean found = FALSE;

    while ((tempPtr) && (!found))
    {
        if (tempPtr == slPtr)
	    found = TRUE;
	else
        {
	    indx = tempPtr;
	    tempPtr = tempPtr->next;
	}
    }

    if (found)
    {
        tempPtr->linkCount--;
	if (tempPtr->linkCount == 0)
	{
	    if (canDelete)
	    {
		if (tempPtr == headOfList)
		    headOfList = tempPtr->next;
		else
		    indx->next = tempPtr->next;

		delete tempPtr;
	    }
	    else
		tempPtr->linkCount++;
	}
    }
    else
        delete slPtr;

    slPtr = (ServerList*) 0;
}

Boolean ServerList::imposeState (ObjectState& os)
{
#ifdef DEBUG
     debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
     debug_stream << "Boolean ServerList::imposeState (ObjectState& os)" << endl;
#endif

     AtomicAction A;
     Boolean result = FALSE;

     A.Begin();

     if (setlock(new Lock(WRITE), 0) == GRANTED)
	 result = restore_state(os, ANDPERSISTENT);

     if (result)
	 result = (Boolean) (A.End() == COMMITTED);
     else
	 A.Abort();

     return result;
}

Boolean ServerList::storeName (Buffer key_buff, Buffer data_buff, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ServerList::storeName (Buffer key_buff, Buffer data_buff)\n" << flush;
#endif

    AtomicAction A;
    Boolean result = FALSE;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	self = new ServerData;
	self->key = key_buff;
	self->data = data_buff;
	v_number++;
	version = v_number;
	result = TRUE;
    }

    if (result)
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
	A.Abort();
    
    return result;
}

Boolean ServerList::fetchName (Buffer& data_buff, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ServerList::fetchName (Buffer& data_buff)\n" << flush;
#endif

    AtomicAction A;
    Boolean result = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	version = v_number;
	data_buff = self->data;
	result = TRUE;
    }

    if (result)
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
	A.Abort();
    
    return result;
}

Boolean ServerList::replaceName (Buffer data_buff, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ServerList::replaceName (Buffer data_buff, unsigned long& version)" << endl;
#endif    

    AtomicAction B;
    Boolean result = FALSE;

    B.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	v_number++;
	version = v_number;
	self->data = data_buff;
	result = TRUE;
    }

    if (result)
    {
	if (B.End() != COMMITTED)
	    result = FALSE;
    }
    else
	B.Abort();
    
    return result;
}

Boolean ServerList::compareBuffers (Buffer key_buff, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ServerList::compareBuffers (Buffer key_buff)\n" << flush;
#endif

    AtomicAction B;
    Boolean result = FALSE;

    B.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        version = v_number;
        result = (Boolean) (self->key == key_buff);
    }

    if (result)
    {
	if (B.End() != COMMITTED)
	    result = FALSE;
    }
    else
	B.Abort();

    return result;
}

Boolean ServerList::getKey (Buffer& key_buff, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ServerList::getKey (Buffer& key_buff)\n" << flush;
#endif

    AtomicAction B;
    Boolean result = FALSE;

    B.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        version = v_number;
        key_buff = self->key;
	result = TRUE;
    }

    if (result)
    {
	if (B.End() != COMMITTED)
	    result = FALSE;
    }
    else
	B.Abort();
    
    return result;
}

Boolean ServerList::save_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ServerList::save_state\n" << flush;
#endif

    Boolean res;

    res = os.pack(v_number);
    if (self == (ServerData*) 0)
	res = res && os.pack(-1);
    else
	res = res && os.pack(1) && self->key.packInto(os) && self->data.packInto(os);

    return res;
}

Boolean ServerList::restore_state (ObjectState&os, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean ServerList::restore_state (ObjectState& os, ObjectType)\n" << flush;
#endif

    Boolean res;
    int ptrType = 0;

    if (self)
	delete self;

    res = os.unpack(v_number);

    if (res)
    {
	res = os.unpack(ptrType);
	if (ptrType == 1)
	{
	    self = new ServerData;
	    res = self->key.unpackFrom(os) && self->data.unpackFrom(os);
	}
	else
	    self = (ServerData*) 0;
    }

    return res;
}

const TypeName ServerList::type () const
{
    return "/StateManager/LockManager/ServerList";
}
