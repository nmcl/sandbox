/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ServerList.cc
 */

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif


#include "ServerList.h"

ServerData::ServerData () {}

ServerData::~ServerData () {}

ServerList::ServerList (int& res) : LockManager(ANDPERSISTENT)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerList::ServerList (int& res)" << endl;
#endif
    
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	self = 0;
	state_set = FALSE;
	v_number = 0;

	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

ServerList::ServerList (const Uid& u_id, int& res) : LockManager(u_id)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerList::ServerList (const Uid& u_id, int& res)" << endl;
#endif

    AtomicAction A;

    A.Begin();

    self = 0;
    state_set = FALSE;
    v_number = 0;

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
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerList::~ServerList ()" << endl;
#endif

    terminate();

    if (self)
	delete self;
}

unsigned long ServerList::GetVersionNumber () { return v_number; }

Boolean ServerList::StoreName (Buffer key_buff, Buffer data_buff, unsigned long& version)
{
    AtomicAction A;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean ServerList::StoreName (Buffer key_buff, Buffer data_buff)\n" << flush;
#endif

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	self = new ServerData;
	self->key = key_buff;
	self->data = data_buff;
	state_set = TRUE;
	v_number++;
	version = v_number;
	
	if (A.End() == COMMITTED)
	    return TRUE;
	else
	    return FALSE;
    }
    else
	return FALSE;
}

Boolean ServerList::FetchName (Buffer& data_buff, unsigned long& version)
{
    AtomicAction A;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean ServerList::FetchName (Buffer& data_buff)\n" << flush;
#endif

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	version = v_number;

	if (!state_set)
	{
	    A.Abort();
	    return FALSE;
	}

	data_buff = self->data;

	if (A.End() == COMMITTED)
	    return TRUE;
	else
	    return FALSE;
    }
    else
    {
	A.Abort();
	return FALSE;
    }
}

Boolean ServerList::ReplaceName (Buffer data_buff, unsigned long& version)
{
    AtomicAction B;

    B.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	v_number++;
	version = v_number;

	if (!state_set)
	{
	    B.Abort();
	    return FALSE;
	}

	self->data = data_buff;

	if (B.End() == COMMITTED)
	    return TRUE;
	else
	    return FALSE;
    }
    else
    {
	B.Abort();
	return FALSE;
    }
}

Boolean ServerList::CompareBuffers (Buffer key_buff)
{
    AtomicAction B;
    Boolean same = FALSE;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean ServerList::CompareBuffers (Buffer key_buff)\n" << flush;
#endif

    B.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (!state_set)
	{
	    B.Abort();
	    return FALSE;
	}

	same = (Boolean) (self->key == key_buff);

	if (B.End() == COMMITTED)
	    return same;
	else
	    return FALSE;
    }
    else
    {
	B.Abort();
	return FALSE;
    }
}

Boolean ServerList::GetKey (Buffer& key_buff)
{
    AtomicAction B;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerList::GetKey (Buffer& key_buff)\n" << flush;
#endif

    B.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (!state_set)
	{
	    B.Abort();
	    return FALSE;
	}

	key_buff = self->key;

	if (B.End() == COMMITTED)
	    return TRUE;
	else
	    return FALSE;
    }
    else
    {
	B.Abort();
	return FALSE;
    }
}

Boolean ServerList::save_state (ObjectState& os, ObjectType)
{
    Boolean res;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "ServerList::save_state\n" << flush;
#endif

    res = os.pack(state_set) && os.pack(v_number);

    if (state_set)
	res = self->key.packInto(os) && self->data.packInto(os);

    return res;
}

Boolean ServerList::restore_state (ObjectState&os, ObjectType)
{
    Boolean res;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean ServerList::restore_state (ObjectState& os, ObjectType)\n" << flush;
#endif

    if (self)
	delete self;

    res = os.unpack(state_set) && os.unpack(v_number);

    if (state_set)
    {
	self = new ServerData;
	res = self->key.unpackFrom(os) && self->data.unpackFrom(os);
    }
    else
	self = 0;

    return res;
}

const TypeName ServerList::type () const
{
    return "/StateManager/LockManager/ServerList";
}
