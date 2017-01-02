/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Posix_Sem.cc,v 1.3 1998/07/21 11:08:05 nmcl Exp $
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

#ifndef COMMON_THREAD_POSIX_THREAD_POSIX_SEM_H_
#  include <Common/Thread/Posix_Thread/Posix_Sem.h>
#endif

#if defined(PTHREAD_SUN) && !defined(OS_THREAD_H_)
#  include <os/thread.h>
#endif

#ifdef PTHREAD_SUN

SemaphoreData::SemaphoreData (unsigned int number, SemaphoreSetup* setup)
			     : semData((array_t) number)
{
    int pshared = 0; // default behavior;

    if ((setup) && (setup->processBased))
        pshared = 1;

    (void) sem_init(&sp, pshared, number);
}

SemaphoreData::~SemaphoreData ()
{
}

/*
 * Because draft 6 of pthreads does not support re-entrant mutexes
 * we have to do the checking at this level. Draft 8 (currently) supports
 * re-entrance, but not inter-process locking!
 */

MutexData::MutexData (MutexSetup* setup)
		     : dead(FALSE),
		       useCount(0)
{
    pthread_mutexattr_init(&mattr);
    
#if defined(PTHREAD_DRAFT_6) || defined(PTHREAD_SUN)
    int pshared = PTHREAD_PROCESS_PRIVATE; // default behavior;

    if ((setup) && (setup->processBased))
        pshared = PTHREAD_PROCESS_SHARED;

    pthread_mutexattr_setpshared(&mattr, pshared);
#else
#ifdef HAVE_RECURSIVE_MUTEX
    pthread_mutexattr_settype(&mattr, PTHREAD_MUTEXTYPE_RECURSIVE);
#endif    
#endif    

    pthread_mutex_init(&mp, &mattr);
}

MutexData::~MutexData ()
{
    pthread_mutexattr_destroy(&mattr);
}

Semaphore::Semaphore (unsigned int number, SemaphoreSetup* setup)
		     : _semaphoreData(new SemaphoreData(number, setup))
{
}

Semaphore::~Semaphore ()
{
    _semaphoreData->semData.dead(TRUE);

    while ((sem_destroy(&_semaphoreData->sp) != 0) && (errno == EBUSY))
    {
	sem_post(&_semaphoreData->sp);
	Thread::yield();
    }

    delete _semaphoreData;
}

Semaphore::SemaphoreStatus Semaphore::lock ()
{
    pthread_t id = pthread_self();
    array_t index = 0;
    Semaphore::SemaphoreStatus result = SEM_DONE;

    if (!_semaphoreData->semData.dead() && !_semaphoreData->semData.isOwner(id, index))
    {
	if ((sem_wait(&_semaphoreData->sp) != 0) || (_semaphoreData->semData.dead()))
	    result = SEM_ERROR;
	else
	{
	    // if we get here then we are now the owner.

	    if (!_semaphoreData->semData.addOwner(id, index))
	    {
		sem_post(&_semaphoreData->sp);
		result = SEM_ERROR;
	    }
	}
    }

    if (result == SEM_DONE)
	_semaphoreData->semData.increment(index);

    return result;
}

Semaphore::SemaphoreStatus Semaphore::unlock ()
{
    pthread_t id = pthread_self();
    array_t index = 0;
    Semaphore::SemaphoreStatus result = SEM_DONE;

    if (!_semaphoreData->semData.dead() && _semaphoreData->semData.isOwner(id, index))
    {
	_semaphoreData->semData.decrement(index);
	    
	if (_semaphoreData->semData.count(index) == 0)
	{
	    _semaphoreData->semData.reset(index);

	    if ((sem_post(&_semaphoreData->sp) != 0) || (_semaphoreData->semData.dead()))
		result = SEM_ERROR;
	}
    }
    else
	result = SEM_ERROR;

    return result;
}

Semaphore::SemaphoreStatus Semaphore::tryLock ()
{
    /*
     * sem_trywait returns WOULD_BLOCK even if we currently
     * own the semaphore, so we need to check this condition
     * before we get there.
     */

    pthread_t id = pthread_self();
    array_t index = 0;
    Semaphore::SemaphoreStatus result = SEM_DONE;

    if (!_semaphoreData->semData.dead() && !_semaphoreData->semData.isOwner(id, index))
    {
	if (sem_trywait(&_semaphoreData->sp) != 0)
	    result = SEM_WOULD_BLOCK;
	else
	{
	    // if we get here then we are now the owner.

	    if (!_semaphoreData->semData.addOwner(id, index))
	    {
		sem_post(&_semaphoreData->sp);
		result = SEM_ERROR;
	    }
	}
    }

    if (result == SEM_DONE)
	_semaphoreData->semData.increment(index);

    return result;
}

#else

Semaphore::Semaphore (SemaphoreSetup*)
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

#endif

// currently not implemented, but we can still pretend

#ifndef PTHREAD_PROCESS_SHARED
#define PTHREAD_PROCESS_SHARED 0
#define PTHREAD_PROCESS_PRIVATE 0
#endif

Mutex::Mutex (MutexSetup* setup)
	     : _mutexData(new MutexData(setup))
{
}

Mutex::~Mutex ()
{
    _mutexData->dead = TRUE;

    /*
     * This should not happen if everything is reference counted!
     */
	
    while ((pthread_mutex_destroy(&_mutexData->mp) == -1) && (errno == EBUSY))
    {
	pthread_mutex_unlock(&_mutexData->mp);
	Thread::yield();
    }

    delete _mutexData;
}

Mutex::MutexStatus Mutex::lock ()
{
#ifndef HAVE_RECURSIVE_MUTEX
    pthread_t id = pthread_self();
    Mutex::MutexStatus result = MX_DONE;

    if (!_mutexData->dead && (_mutexData->useCount != 0) && (pthread_equal(_mutexData->currentOwner, id)))
	_mutexData->useCount++;
    else
    {
	if ((pthread_mutex_lock(&_mutexData->mp) != 0) || (_mutexData->dead))
	    result = Mutex::MX_ERROR;
	else
	{
	    // if we get here then we are now the owner.

	    _mutexData->currentOwner = id;
	    _mutexData->useCount = 1;
	}
    }

    return result;
#else
    if (!_mutexData->dead && pthread_mutex_lock(&_mutexData->mp) == 0)
    {
	if (_mutexData->dead)
	    return Mutex::MX_ERROR;
	else
	    return Mutex::MX_DONE;
    }
    else
        return Mutex::MX_ERROR;
#endif
}

Mutex::MutexStatus Mutex::unlock ()
{
#ifndef HAVE_RECURSIVE_MUTEX
    pthread_t id = pthread_self();
    Mutex::MutexStatus result = MX_DONE;

    if (!_mutexData->dead && (_mutexData->useCount != 0) && (pthread_equal(id, _mutexData->currentOwner)))
    {
	_mutexData->useCount--;
	    
	if (_mutexData->useCount == 0)
	{
	    if ((pthread_mutex_unlock(&_mutexData->mp) != 0) || (_mutexData->dead))
		result = MX_ERROR;
	}
    }
    else
	result = MX_ERROR;

    return result;
#else
    if (!_mutexData->dead && pthread_mutex_unlock(&_mutexData->mp) == 0)
        return Mutex::MX_DONE;
    else
        return Mutex::MX_ERROR;
#endif
}

Mutex::MutexStatus Mutex::tryLock ()
{
#ifndef HAVE_RECURSIVE_MUTEX
    pthread_t id = pthread_self();
    Mutex::MutexStatus result = MX_DONE;

    if (!_mutexData->dead && (_mutexData->useCount != 0) && (pthread_equal(id, _mutexData->currentOwner)))
	_mutexData->useCount++;
    else
    {
	if (pthread_mutex_trylock(&_mutexData->mp) != 0)
	    result = Mutex::MX_WOULD_BLOCK;
	else
	{
	    // if we get here then we are now the owner.

	    _mutexData->currentOwner = id;
	    _mutexData->useCount = 1;
	}
    }

    return result;
#else
    if (!_mutexData->dead && pthread_mutex_trylock(&_mutexData->mp) == 0)
        return Mutex::MX_DONE;
    else
        return Mutex::MX_ERROR;
#endif
}

void Semaphore::initialize ()
{
    InitThreads initThreads;
}

Semaphore* Semaphore::create (unsigned int number,
			      SemaphoreSetup* setup)
{
#ifdef PTHREAD_SUN
    return new Semaphore(number, setup);
#else
    return (Semaphore*) 0;
#endif  
}

void Mutex::initialize ()
{
    InitThreads initThreads;
}

Mutex* Mutex::create (MutexSetup* setup)
{
    return new Mutex(setup);
}
