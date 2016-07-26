/*
 * Copyright (C) 1994
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StudentRecord.cc,v 1.7 1996/11/27 11:51:31 snowman Exp $
 */

#include <System/iostream.h>

#define STATEMANAGER_SERVER
#define LOCKMANAGER_SERVER
#define STUDENTRECORD_SERVER

#include <Common/Error.h>

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef STUDENTRECORD_H_
#  include <StudentRecord_stub.h>
#endif

#include <Replication/ReplicaConfiguration.h>
#include <Replication/RemoteOSTypes.h>

#include <arjuna.h>
#include <local.h>

Boolean (*OldHandler) (RPC_Status, Int32);

static Boolean RPCHandler (RPC_Status, Int32)
{
    cerr << "Rpc_handler called." << endl;
    return FALSE;
}

StudentRecord::StudentRecord (SOutCome& result) : LockManager(ANDPERSISTENT)
{
    AtomicAction A;

#ifndef LOCAL_ONLY
    Boolean res = FALSE;

    ReplicaConfiguration conf(ObjectStoreConfiguration, res);

    if (!res)
    {
	result = NOCONFIGURATION;
	return;
    }
#endif

    A.Begin();
    
    studentInformation = (Buffer*) 0;

#ifndef LOCAL_ONLY
    OldHandler = setHandler(RPCHandler);
    StateManager::setStoreInformation(conf.hosts(), conf.number());
#endif
    
    if (setlock(new Lock(WRITE), 10, 250000) == GRANTED)
	result = ((A.End() == COMMITTED) ? DONE : NOTDONE);
    else
    {
	A.Abort();
	result = LOCKED;
    }
}

StudentRecord::StudentRecord (SOutCome& result, const Uid& myUid) : LockManager(myUid)
{
#ifndef LOCAL_ONLY
    Boolean res = FALSE;

    ReplicaConfiguration conf(ObjectStoreConfiguration, res);

    if (!res)
    {
	result = NOCONFIGURATION;
	return;
    }
    
    studentInformation = (Buffer*) 0;

    OldHandler = setHandler(RPCHandler);
    StateManager::setStoreInformation(conf.hosts(), conf.number());

    result = DONE;
#else
    studentInformation = (Buffer*) 0;
    result = DONE;
#endif
}

StudentRecord::~StudentRecord ()
{
    if (studentInformation)
	delete studentInformation;

    terminate();
}

void StudentRecord::resetRecord (SOutCome& result)
{
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(WRITE), 10, 250000) == GRANTED)
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
    
    if (setlock(new Lock(WRITE), 10, 250000) == GRANTED)
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
    
    if (setlock(new Lock(READ), 10, 250000) == GRANTED)
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

void StudentRecord::fetchExclusiveRecord (SOutCome& result, Buffer& toRetrieve)
{
    AtomicAction A;
    
    A.Begin();

    if (setlock(new Lock(WRITE), 10, 250000) == GRANTED)
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
    {
	result = LOCKED;
    }

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
    
    if (setlock(new Lock(WRITE), 10, 250000) == GRANTED)
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

Boolean StudentRecord::hasRemoteState () const
{
#ifdef LOCAL_ONLY
    return FALSE;
#else    
    return TRUE;
#endif    
}

Boolean StudentRecord::useStoreLocation () const
{
#ifdef LOCAL_ONLY
    return FALSE;
#else    
    return TRUE;
#endif    
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

		
		
	    

	    
	    
	    
	    
	    
		
	    
	    
