/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OmniORB_RWLock.cc,v 1.1 1998/01/12 13:07:13 nmcl Exp $
 */

/*
 * Implementation of R/W locks for OmniOrb threads
 */

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef COMMON_THREAD_OMNIORB_THREAD_OMNIORB_RWLOCK_H_
#  include <Common/Thread/OmniORB_Thread/OmniORB_RWLock.h>
#endif

ThreadRWLock* ThreadRWLock::create ()
{
    return new OmniORB_RWLock();
}

OmniORB_RWLock::OmniORB_RWLock ()
			       : readCount(0),
				 waitCount(0)
{
    mutex = new omni_mutex();
    readers = new omni_condition(mutex);
    writers = new omni_condition(mutex);
}

OmniORB_RWLock::~OmniORB_RWLock ()
{
    delete writers;
    delete readers;
    delete mutex;
}

Boolean OmniORB_RWLock::readLock ()
{
    int result = 0;

    try
    {
	mutex->lock();
	
	while (readCount < 0)
	{
	    try
	    {
		readers->wait();
	    }
	    catch (...)
	    {
		result = -2; // -2 means that we need to release the mutex.
		break;
	    }
	}
    }
    catch (...)
    {
	result = -1;
    }
    
    if (result == 0)
	readCount++;

    if (result != -1)
    {
	try
	{
	    mutex->unlock();
	}
	catch (...)
	{
	}
    }
    
    return (result == 0 ? TRUE : FALSE);
}

Boolean OmniORB_RWLock::writeLock ()
{
    int result = 0; 

    try
    {
	mutex->lock();
	
	while (readCount != 0)
	{
	    waitCount++;

	    try
	    {
		writers->wait();
	    }
	    catch (...)
	    {
		result = -2;
		break;
	    }

	    waitCount--;
	}
    }
    catch (...)
    {
	result = -1;
    }
	
    if (result == 0)
	readCount = -1;

    if (result != -1)
    {
	try
	{
	    mutex->unlock();
	}
	catch (...)
	{
	}
    }

    return (result == 0 ? TRUE : FALSE);
}

Boolean OmniORB_RWLock::unlock ()
{
    try
    {
	mutex->lock();
	
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
		readers->broadcast();
	}
    }
    catch (...)
    {
	return FALSE;
    }

    try
    {
	mutex->unlock();
    }
    catch (...)
    {
    }

    return TRUE;
}

Boolean OmniORB_RWLock::tryReadLock ()
{
    Boolean result = FALSE;

    try
    {
	mutex->lock();
	
	if ((readCount != -1) && (waitCount== 0))
	{
	    readCount++;
	    result = TRUE;
	}

	try
	{
	    mutex->unlock();
	}
	catch (...)
	{
	}
    }
    catch (...)
    {
    }
    
    return result;
}

Boolean OmniORB_RWLock::tryWriteLock ()
{
    Boolean result = FALSE;

    try
    {
	mutex->lock();
	
	if (readCount == 0)
	{
	    readCount = -1;
	    result = TRUE;
	}

	try
	{
	    mutex->unlock();
	}
	catch (...)
	{
	}
    }
    catch (...)
    {
    }
    
    return result;
}
