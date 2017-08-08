/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPlus_RWLock.cc,v 1.1 1998/01/12 13:07:19 nmcl Exp $
 */

/*
 * Implementation of R/W locks for MSD threads
 */

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef COMMON_THREAD_ORBPLUS_THREAD_ORBPLUS_RWLOCK_H_
#  include <Common/Thread/OrbPlus_Thread/OrbPlus_RWLock.h>
#endif

ThreadRWLock *ThreadRWLock::create ()
{
    return new OrbPlus_RWLock();
}

OrbPlus_RWLock::OrbPlus_RWLock ()
			       : readCount(0),
				 waitCount(0)
{
    mutex = new MSD_Mutex();
    readers = new MSD_CondVar();
    writers = new MSD_CondVar();
}

OrbPlus_RWLock::~OrbPlus_RWLock ()
{
    delete writers;
    delete readers;
    delete mutex;
}

Boolean OrbPlus_RWLock::readLock ()
{
    int result = 0;

    mutex->lock();

    while (readCount < 0)
    {
	readers->wait(*mutex);
    }
    
    if (result == 0)
	readCount++;

    if (result != -1)
	mutex->unlock();
    
    return (result == 0 ? TRUE : FALSE);
}

Boolean OrbPlus_RWLock::writeLock ()
{
    int result = 0; 

    mutex->lock();

    while (readCount != 0)
    {
	waitCount++;

	writers->wait(*mutex);

	waitCount--;
    }

    if (result == 0)
	readCount = -1;

    if (result != -1)
	mutex->unlock();

    return (result == 0 ? TRUE : FALSE);
}

Boolean OrbPlus_RWLock::unlock ()
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
	    readers->signal();
    }

    mutex->unlock();

    return TRUE;
}

Boolean OrbPlus_RWLock::tryReadLock ()
{
    Boolean result = FALSE;

    mutex->lock();

    if ((readCount != -1) && (waitCount== 0))
    {
	readCount++;
	result = TRUE;
    }

    mutex->unlock();
    
    return result;
}

Boolean OrbPlus_RWLock::tryWriteLock ()
{
    Boolean result = FALSE;

    mutex->lock();

    if (readCount == 0)
    {
	readCount = -1;
	result = TRUE;
    }

    mutex->unlock();
    
    return result;
}
