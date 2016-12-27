/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PC_Sem.cc,v 1.1 1998/01/12 13:07:26 nmcl Exp $
 */

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef COMMON_THREAD_PC_THREAD_PC_SEM_H_
#  include <Common/Thread/PC_Thread/PC_Sem.h>
#endif

#ifndef COMMON_THREAD_MUTEXSETUP_H_
#  include <Common/Thread/MutexSetup.h>
#endif

MutexData::MutexData (MutexSetup* setup)
		 : dead(FALSE),
		   reentrant(TRUE),
		   useCount(0)
{
    if (setup && !setup->reentrant)
	reentrant = FALSE;

    pthread_mutexattr_init(&mattr);
    pthread_mutex_init(&mp, &mattr);
}

MutexData::~MutexData ()
{
    pthread_mutexattr_destroy(&mattr);
}

Mutex::Mutex (MutexSetup* setup)
             : _mutexData(new MutexSetup(setup))
{
}

Mutex::~Mutex ()
{
    _mutexData->dead = TRUE;

    pthread_mutex_unlock(&_mutexData->mp);
    pthread_yield(NULL);

    /*
     * This should not happen if everything is reference counted!
     */
    
    while ((pthread_mutex_destroy(&_mutexData->mp) == -1) && (errno == EBUSY))
	pthread_yield(NULL);

    delete _mutexData;
}

Mutex::MutexStatus Mutex::lock ()
{
    Mutex::MutexStatus result = MX_DONE;

    if (_mutexData->reentrant)
    {
	pthread_t id = pthread_self();

	if ((_mutexData->useCount != 0) && (pthread_equal(_mutexData->currentOwner, id)))
	    _mutexData->useCount++;
	else
        {
	    if ((pthread_mutex_lock(&_mutexData->mp) != 0) || (_mutexData->dead))
		result = MX_ERROR;
	    else
	    {
		// if we get here then we are now the owner.

		_mutexData->currentOwner = id;
		_mutexData->useCount = 1;
	    }
	}
    }
    else
    {
	if ((pthread_mutex_lock(&_mutexData->mp) != 0) || (_mutexData->dead))
	    result = MX_ERROR;	
    }

    return result;
}

Mutex::MutexStatus Mutex::unlock ()
{
    Mutex::MutexStatus result = MX_DONE;

    if (_mutexData->reentrant)
    {
	pthread_t id = pthread_self();

	if ((_mutexData->useCount != 0) && (pthread_equal(id, _mutexData->currentOwner)))
	{
	    _mutexData->useCount--;
	    
	    if (_mutexData->useCount == 0)
	    {
		if ((pthread_mutex_unlock(&_mutexData->mp) != 0) || (_mutexData->dead))
		    result = MX_ERROR;
	    }
	}
    }
    else
    {
	if ((pthread_mutex_unlock(&_mutexData->mp) != 0) || (_mutexData->dead))
	    result = MX_ERROR;
    }

    return result;
}

Mutex::MutexStatus Mutex::tryLock ()
{
    Mutex::MutexStatus result = MX_DONE;

    if (_mutexData->reentrant)
    {
	pthread_t id = pthread_self();

	if ((_mutexData->useCount != 0) && (pthread_equal(id, _mutexData->currentOwner)))
	    _mutexData->useCount++;
	else
        {
	    if (pthread_mutex_trylock(&_mutexData->mp) != 0)
		result = MX_WOULD_BLOCK;
	    else
	    {
		// if we get here then we are now the owner.
	    
		_mutexData->currentOwner = id;
		_mutexData->useCount = 1;
	    }
	}
    }
    else
    {
	if (pthread_mutex_trylock(&_mutexData->mp) != 0)
		result = MX_WOULD_BLOCK;
    }

    return result;
}

Semaphore::Semaphore ()
                     : _semaphoreData(0)
{
}

Semaphore::~Semaphore ()
{
}

Semaphore::SemaphoreStatus Semaphore::lock ()
{
    return SEM_ERROR;
}

Semaphore::SemaphoreStatus Semaphore::unlock ()
{
    return SEM_ERROR;
}

Semaphore::SemaphoreStatus Semaphore::tryLock ()
{
    return SEM_ERROR;
}

void Semaphore::initialize ()
{
    InitThreads initThreads;
}

void Mutex::initialize ()
{
    InitThreads initThreads;    
}

Mutex* Mutex::create (MutexSetup* setup)
{
    return new Mutex(setup);
}

void Semaphore::initialize ()
{
    if (!Thread::_initialized)
	Thread::initialize();
}

Semaphore* Semaphore::create (unsigned int, SemaphoreSetup*)
{
    return (Semaphore*) 0;
}
