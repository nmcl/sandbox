/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjectData.cc,v 1.3 1998/04/08 10:52:55 nmcl Exp $
 */

#include <System/string.h>

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#include <Thread/Thread.h>

#ifndef OBJECTDATA_H_
#  include "ObjectData.h"
#endif

ObjectData::ObjectData (int& res)
		       : LockManager(ANDPERSISTENT),
			 _data(0),
			 _size(0)
{
    AtomicAction A;

    res = -1;
    
    A.begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (A.commit() == COMMITTED)
	    res = 0;
    }
    else
	A.abort();
}

ObjectData::ObjectData (const Uid& u, int& res)
		       : LockManager(u),
			 _data(0),
			 _size(0)
{
    AtomicAction A;
    
    A.begin();

    res = -1;
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	A.commit();
	
	res = 0;
    }
    else
	A.abort();
}

ObjectData::~ObjectData ()
{
    if (_data)
	::delete _data;
    
    terminate();
}

void* ObjectData::get (int& size)
{
    AtomicAction A;
    void* data = 0;

    A.begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	size = _size;

	data = ::new signed char[_size];
	::memcpy(data, _data, _size);

	if (A.commit() != COMMITTED)
	{
	    ::delete [] data;
	    data = 0;
	    size = 0;
	}
    }
    else
	A.abort();

    return data;
}

Boolean ObjectData::set (void* data, int size)
{
    AtomicAction A;
    Boolean result = FALSE;

    A.begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (_data)
	    ::delete _data;

	_size = size;

	if (_size > 0)
	{
	    _data = ::new signed char[_size];

	    ::memcpy(_data, data, _size);
	}
	else
	    _data = 0;

	if (A.commit() == COMMITTED)
	{
	    result = TRUE;
	}
	else
	{
	    ::delete _data;
	    _data = 0;
	    _size = 0;
	}
    }
    else
	A.abort();

    return result;
}

Boolean ObjectData::save_state (ObjectState& os, ObjectType)
{
    if (os.pack(_size))
    {
	if (_size > 0)
	    return os.packNonMappedBlock(_data, _size);
	else
	    return TRUE;
    }
    else
	return FALSE;
}

Boolean ObjectData::restore_state (ObjectState& os, ObjectType)
{
    _size = 0;

    if (_data)
    {
	::delete _data;
	_data = 0;
    }
    
    if (os.unpack(_size))
    {
	if (_size > 0)
	{
	    size_t dummy = 0;
	
	    return os.unpack(_data, dummy);
	}
	else
	    return TRUE;
    }
    else
	return FALSE;
}

const TypeName ObjectData::type () const
{
    return "/StateManager/LockManager/ObjectData";
}
