/*
 * Copyright (C) 1994, 1995, 1996
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LocalStudentRecord.cc,v 1.1 1996/08/01 09:14:42 arjuna Exp $
 */

#include <System/iostream.h>

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef LOCALSTUDENTRECORD_H_
#  include "LocalStudentRecord.h"
#endif


StudentRecord::StudentRecord (SOutCome& result) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;
    
    A.Begin();
    
    studentInformation = (Buffer*) 0;

    if (setlock(new Lock(WRITE), 0) == GRANTED)
	result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
    {
	A.Abort();
	result = LOCKED;
    }
}

StudentRecord::StudentRecord (SOutCome& result, const Uid& myUid) : LockManager(myUid)
{
    studentInformation = (Buffer*) 0;
    result = DONE;
}

StudentRecord::~StudentRecord ()
{
    terminate();

    if (studentInformation)
	delete studentInformation;
}

void StudentRecord::resetRecord (SOutCome& result)
{
	AtomicAction A;

	A.Begin();

	if (setlock(new Lock(WRITE), 0) == GRANTED)
	{
	    if (studentInformation)
	    {
		delete studentInformation;
		studentInformation = (Buffer*) 0;
	    }
	
	    result = DONE;
	}
	else
	    result = LOCKED;

	if (result == DONE)
	{
	    if (A.End() != COMMITTED)
		result = NOTDONE;
	}
	else
	    A.Abort();
}

void StudentRecord::storeRecord (SOutCome& result, Buffer toStore)
{
    AtomicAction A;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (studentInformation == (Buffer*) 0)
	{
	    studentInformation = new Buffer(toStore);
	    result = DONE;
	}
	else
	    result = FULL;
    }
    else
	result = LOCKED;
    
    if (result == DONE)
    {
	if (A.End() != COMMITTED)
	    result = NOTDONE;
    }
    else
	A.Abort();
}

void StudentRecord::fetchRecord (SOutCome& result, Buffer& toRetrieve)
{
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (studentInformation != (Buffer*) 0)
	{
	    toRetrieve = *studentInformation;
	    result = DONE;
	}
	else
	    result = EMPTY;
    }
    else
	result = LOCKED;

    if (result == DONE)
    {
	if (A.End() != COMMITTED)
	    result = NOTDONE;
    }
    else
	A.Abort();
}

void StudentRecord::replaceRecord (SOutCome& result, Buffer replaceWith)
{
    AtomicAction A;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (studentInformation)
	    delete studentInformation;
	
	studentInformation = new Buffer(replaceWith);
	result = DONE;
    }
    else
	result = LOCKED;
    
    if (result == DONE)
    {
	if (A.End() != COMMITTED)
	    result = NOTDONE;
    }
    else
	A.Abort();
}

Boolean StudentRecord::save_state (ObjectState& os, ObjectType)
{
    if (studentInformation == (Buffer*) 0)
	return os.pack(-1);
    else
	return os.pack(1) && studentInformation->packInto(os);
}

Boolean StudentRecord::restore_state (ObjectState& os, ObjectType)
{
    int ptrType = 0;

    if (os.unpack(ptrType))
    {
	if (ptrType == 1)
	{
	    if (studentInformation == (Buffer*) 0)
		studentInformation = new Buffer;

	    return studentInformation->unpackFrom(os);
	}
	else
	{
	    if (studentInformation)
	    {
		delete studentInformation;
		studentInformation = (Buffer*) 0;
	    }
	}
	
	return TRUE;
    }
    
    return FALSE;
}

const TypeName StudentRecord::type () const
{
    return "/StateManager/LockManager/StudentRecord";
}

		
		
	    

	    
	    
	    
	    
	    
		
	    
	    
