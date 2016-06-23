/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PosixRWLock.cc,v 1.1 1997/09/25 15:32:55 nmcl Exp $
 */

/*
 * Implementation of R/W locks for Posix threads
 */

#ifndef POSIXTHREAD_H_
#  include <System/PosixThread.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef POSIXRWLOCK_H_
#  include "PosixRWLock.h"
#endif

ThreadRWLock *ThreadRWLock::create ()
{
    return new PosixRWLock();
}

PosixRWLock::PosixRWLock ()
			 : readCount(0),
			   waitCount(0)
{
#ifndef HAVE_HPUX_PTHREADS    
    pthread_mutex_init(&mutex, 0);
    pthread_cond_init(&readers,0);
    pthread_cond_init(&writers,0);
#else
    pthread_mutex_init(&mutex, pthread_mutexattr_default);
    pthread_cond_init(&readers, pthread_mutexattr_default);
    pthread_cond_init(&writers, pthread_mutexattr_default);    
#endif
}

PosixRWLock::~PosixRWLock ()
{
    pthread_cond_destroy(&writers);
    pthread_cond_destroy(&readers);
    pthread_mutex_destroy(&mutex);
}

Boolean PosixRWLock::readLock ()
{
    int result = 0;
    
    if (pthread_mutex_lock(&mutex) == -1)
	result = -1;
    else
    {
	while (readCount < 0)
	{
	    if (pthread_cond_wait(&readers, &mutex) == -1)
	    {
		result = -2; // -2 means that we need to release the mutex.
		break;
	    }
	}
    }
    
    if (result == 0)
	readCount++;

    if (result != -1)
	pthread_mutex_unlock(&mutex);
    
    return (result == 0 ? TRUE : FALSE);
}

Boolean PosixRWLock::writeLock ()
{
    int result = 0; 

    if (pthread_mutex_lock(&mutex) == -1)
	result = -1;
    else
    {
	while (readCount != 0)
	{
	    waitCount++;
	    
	    if (pthread_cond_wait(&writers, &mutex) == -1)
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
	pthread_mutex_unlock (&mutex);

    return (result == 0 ? TRUE : FALSE);
}

Boolean PosixRWLock::unlock ()
{
    if (pthread_mutex_lock(&mutex) == -1)
	return FALSE;

    if (readCount == -1)		// release write
	readCount = 0;
    else if (readCount > 0)		// release read
	readCount--;
    
    if (readCount == 0)
    {
	// Give writers priority
	
	if (waitCount > 0)
	    pthread_cond_signal(&writers);
	else
	    pthread_cond_broadcast(&readers);
    }

    pthread_mutex_unlock(&mutex);

    return TRUE;
}

Boolean PosixRWLock::tryReadLock ()
{
    Boolean result = FALSE;

    if (pthread_mutex_lock(&mutex) == 0)
    {
	if ((readCount != -1) && (waitCount== 0))
	{
	    readCount++;
	    result = TRUE;
	}
	
	pthread_mutex_unlock (&mutex);
    }
    
    return result;
}

Boolean PosixRWLock::tryWriteLock ()
{
    Boolean result = FALSE;

    if (pthread_mutex_lock (&mutex) == 0)
    {
	if (readCount == 0)
	{
	    readCount = -1;
	    result = TRUE;
	}
	
	pthread_mutex_unlock (&mutex);
    }
    
    return result;
}
