/*
 * Copyright (C) 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NTRWLock.cc,v 1.1 1998/09/16 11:03:30 nmcl Exp $
 */

/*
 * Implementation of R/W locks for NT threads
 */

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef NTRWLOCK_H_
#  include "NTRWLock.h"
#endif

ThreadRWLock *ThreadRWLock::create ()
{
    return new NTRWLock();
}

NTRWLock::NTRWLock ()
		   : readCount(0),
		     waitCount(0)
{
    mutex = CreateMutex(NULL, 0, NULL);
    readers = CreateMutex(NULL, 0, NULL);
    writers = CreateMutex(NULL, 0, NULL);
}

NTRWLock::~NTRWLock ()
{
    CloseHandle(writers);
    CloseHandle(readers);
    CloseHandle(mutex);
}

Boolean NTRWLock::readLock ()
{
    int result = 0;

    if (WaitForSingleObject(mutex, INFINITE) == WAIT_FAILED)
	result = -1;
    else
    {
	while (readCount < 0)
	{
	    if (WaitForSingleObject(readers, INFINITE) == WAIT_FAILED)
	    {
		result = -2; // -2 means that we need to release the mutex.
		break;
	    }
	}
    }
    
    if (result == 0)
	readCount++;

    if (result != -1)
	ReleaseMutex(mutex);
    
    return (result == 0 ? TRUE : FALSE);
}

Boolean NTRWLock::writeLock ()
{
    int result = 0; 

    if (WaitForSingleObject(mutex, INFINITE) == WAIT_FAILED)
	result = -1;
    else
    {
	while (readCount != 0)
	{
	    waitCount++;

	    if (WaitForSingleObject(writers, INFINITE) == WAIT_FAILED)
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
	ReleaseMutex(mutex);

    return (result == 0 ? TRUE : FALSE);
}

Boolean NTRWLock::unlock ()
{
    if (WaitForSingleObject(mutex, INFINITE) == WAIT_FAILED)
	return FALSE;

    if (readCount == -1)		// release write
	readCount = 0;
    else if (readCount > 0)		// release read
	readCount--;
    
    if (readCount == 0)
    {
	// Give writers priority
	
	if (waitCount > 0)
	    ReleaseMutex(writers);
	else
	    ReleaseMutex(readers);
    }

    ReleaseMutex(mutex);

    return TRUE;
}

Boolean PosixRWLock::tryReadLock ()
{
    Boolean result = FALSE;

    if (WaitForSingleObject(mutex, INFINITE) != WAIT_FAILED)
    {
	if ((readCount != -1) && (waitCount== 0))
	{
	    readCount++;
	    result = TRUE;
	}

	ReleaseMutex(mutex);
    }
    
    return result;
}

Boolean PosixRWLock::tryWriteLock ()
{
    Boolean result = FALSE;

    if (WaitForSingleObject(mutex, INFINITE) != WAIT_FAILED)
    {
	if (readCount == 0)
	{
	    readCount = -1;
	    result = TRUE;
	}

	ReleaseMutex(mutex);
    }
    
    return result;
}
