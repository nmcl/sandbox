/*
 * Copyright (C) 1994, 1995, 1996
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LocalNewStudentRecordNumber.cc,v 1.1 1996/08/01 09:14:41 arjuna Exp $
 */

#include <System/iostream.h>

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#include "LocalNewStudentRecordNumber.h"


NewStudentRecordNumber::NewStudentRecordNumber (SOutCome& result, int number) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;

    A.Begin();
    
    _number = number;

    if (setlock(new Lock(WRITE), 10, 250000) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    result = DONE;
        else
            result = NOTDONE;
    }
    else
    {
	A.Abort();
	result = LOCKED;
    }
}

NewStudentRecordNumber::NewStudentRecordNumber (SOutCome& result, const Uid& myUid) : LockManager(myUid)
{
    _number = 0;
    result = DONE;
}

NewStudentRecordNumber::~NewStudentRecordNumber ()
{
    terminate();
}

void NewStudentRecordNumber::setNumber(SOutCome& result, int number)
{
    AtomicAction A;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 10, 250000) == GRANTED)
    {
        _number = number;
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

void NewStudentRecordNumber::getNumber (SOutCome& result, int &number)
{
    AtomicAction A;
    
    A.Begin();
    
    number = -1;
    if (setlock(new Lock(READ), 10, 250000) == GRANTED)
    {
        number = _number;
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

void NewStudentRecordNumber::getThenIncreaseNumber (SOutCome& result, int &number)
{
    AtomicAction A;
    
    A.Begin();
    
    number = -1;
    if (setlock(new Lock(WRITE), 10, 250000) == GRANTED)
    {
        number = _number;
        _number++;
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

Boolean NewStudentRecordNumber::save_state (ObjectState& os, ObjectType)
{
    return os.pack(_number);
}

Boolean NewStudentRecordNumber::restore_state (ObjectState& os, ObjectType)
{
    return os.unpack(_number);
}

const TypeName NewStudentRecordNumber::type () const
{
    return "/StateManager/LockManager/NewStudentRecordNumber";
}

		
		
	    

	    
	    
	    
	    
	    
		
	    
	    
