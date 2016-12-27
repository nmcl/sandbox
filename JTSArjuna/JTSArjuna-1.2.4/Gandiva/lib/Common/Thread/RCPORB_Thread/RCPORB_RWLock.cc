/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RCPORB_RWLock.cc,v 1.1 1998/01/12 13:07:35 nmcl Exp $
 */

/*
 * Implementation of R/W locks for ON threads
 */

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef COMMON_THREAD_RCPORB_THREAD_RCPORB_RWLOCK_H_
#  include <Common/Thread/RCPORB_Thread/RCPORB_RWLock.h>
#endif

ThreadRWLock *ThreadRWLock::create ()
{
    return new RCPORB_RWLock();
}

RCPORB_RWLock::RCPORB_RWLock ()
		       : readCount(0),
			 waitCount(0)
{
    mutex = new ODS_CORBA::Mutex();
    readers = new ODS_CORBA::CondMutex();
    writers = new ODS_CORBA::CondMutex();
}

RCPORB_RWLock::~RCPORB_RWLock ()
{
    delete writers;
    delete readers;
    delete mutex;
}

Boolean RCPORB_RWLock::readLock ()
{
    int result = 0;
    
    if (mutex->lock() == -1)
	result = -1;
    else
    {
	while (readCount < 0)
	{
	    if (readers->wait(mutex) == -1)
	    {
		result = -2; // -2 means that we need to release the mutex.
		break;
	    }
	}
    }
    
    if (result == 0)
	readCount++;

    if (result != -1)
	mutex->unlock();
    
    return (result == 0 ? TRUE : FALSE);
}

Boolean RCPORB_RWLock::writeLock ()
{
    int result = 0; 

    if (mutex->lock() == -1)
	result = -1;
    else
    {
	while (readCount != 0)
	{
	    waitCount++;
	    
	    if (writers->wait(mutex) == -1)
	    {
		result = -2;
		break;
	    }

	    waitCount--;
	}
    }

    if (result == 0)
	readCount = -1;

    if (result != -1)
	mutex->unlock();

    return (result == 0 ? TRUE : FALSE);
}

Boolean RCPORB_RWLock::unlock ()
{
    if (mutex->lock() == -1)
	return FALSE;

    if (readCount == -1)		// release write
	readCount = 0;
    else if (readCount > 0)		// release read
	readCount--;
    
    if (readCount == 0)
    {
	// Give writers priority
	
	if (waitCount > 0)
	    writers->signal();
	else
	    readers->signal();
    }

    mutex->unlock();

    return TRUE;
}

Boolean RCPORB_RWLock::tryReadLock ()
{
    Boolean result = FALSE;

    if (mutex->lock() == 0)
    {
	if ((readCount != -1) && (waitCount== 0))
	{
	    readCount++;
	    result = TRUE;
	}

	mutex->unlock();
    }
    
    return result;
}

Boolean RCPORB_RWLock::tryWriteLock ()
{
    Boolean result = FALSE;

    if (mutex->lock() == 0)
    {
	if (readCount == 0)
	{
	    readCount = -1;
	    result = TRUE;
	}

	mutex->unlock();
    }
    
    return result;
}
