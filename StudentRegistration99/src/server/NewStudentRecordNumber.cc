/*
 * Copyright (C) 1994, 1995, 1996
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NewStudentRecordNumber.cc,v 1.4 1996/11/21 13:32:05 arjuna Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef REPLICACONFIGURATION_H_
#  include <Replication/ReplicaConfiguration.h>
#endif

#define STATEMANAGER_SERVER
#define LOCKMANAGER_SERVER
#define NEWSTUDENTRECORDNUMBER_SERVER

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef NEWSTUDENTRECORDNUMBER_STUB_H_
#  include "NewStudentRecordNumber_stub.h"
#endif

#include <Replication/RemoteOSTypes.h>

#include <arjuna.h>

static Boolean RPCHandler (RPC_Status, Int32)
{
    cerr << "Rpc_handler called." << endl;
    return FALSE;
}

NewStudentRecordNumber::NewStudentRecordNumber (SOutCome& result, int number) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;
    Boolean res = FALSE;

    result = NOCONFIGURATION;
    
    ReplicaConfiguration conf(ObjectStoreConfiguration, res);

    if (!res)
	return;
    
    setHandler(RPCHandler);

    A.Begin();
    
    _number = number;

    StateManager::setStoreInformation(conf.hosts(), conf.number());
    
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
    Boolean res = FALSE;
    ReplicaConfiguration conf(ObjectStoreConfiguration, res);

    result = NOCONFIGURATION;
    
    if (!res)
	return;
    
    _number = 0;
    setHandler(RPCHandler);
    StateManager::setStoreInformation(conf.hosts(), conf.number());
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

Boolean NewStudentRecordNumber::hasRemoteState () const
{
#ifdef LOCAL_ONLY
    return FALSE;
#else    
    return TRUE;
#endif    
}

Boolean NewStudentRecordNumber::useStoreLocation () const
{
#ifdef LOCAL_ONLY
    return FALSE;
#else    
    return TRUE;
#endif    
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

		
		
	    

	    
	    
	    
	    
	    
		
	    
	    
