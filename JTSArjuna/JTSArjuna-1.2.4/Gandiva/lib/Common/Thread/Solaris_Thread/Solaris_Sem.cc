/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Solaris_Sem.cc,v 1.1 1998/01/12 13:07:51 nmcl Exp $
 */

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef COMMON_THREAD_SOLARIS_THREAD_SOLARIS_SEM_H_
#  include <Common/Thread/Solaris_Thread/Solaris_Sem.h>
#endif

SemaphoreData::SemaphoreData (unsigned int number, SemaphoreSetup* setup)
			     : reentrant(TRUE),
			       semData((array_t) number)
{
    int type = USYNC_THREAD; // default behavior;

    if (setup)
    {
	if (setup->processBased)
	    type = USYNC_PROCESS;

	reentrant = setup->reentrant;
    }
    
    sema_init(&sp, number, type, NULL);
}

SemaphoreData::~SemaphoreData ()
{
    sema_destroy(&sp);
    thr_yield();
}

MutexData::MutexData (MutexSetup* setup)
		     : dead(FALSE),
		       reentrant(TRUE),
		       useCount(0),
		       currentOwner(0)    
{
    int type = USYNC_THREAD; // default behavior;

    if (setup)
    {
	if (setup->processBased)
	    type = USYNC_PROCESS;

	reentrant = setup->reentrant;
    }

    mutex_init(&mp, type, NULL);
}

MutexData::~MutexData ()
{
    mutex_destroy(&mp);
    thr_yield();
}

Semaphore::Semaphore (unsigned int number, SemaphoreSetup* setup)
		     : _semaphoreData(new SemaphoreData(number, setup))
{
}

Semaphore::~Semaphore ()
{
    _semaphoreData->semData.dead(TRUE);

    while (sema_post(&_semaphoreData->sp) != 0)
      thr_yield();

    delete _semaphoreData;
}

Semaphore::SemaphoreStatus Semaphore::lock ()
{
    if (!_semaphoreData->reentrant)
    {
	if ((sema_wait(&_semaphoreData->sp) != 0) || (_semaphoreData->semData.dead()))
	    return SEM_ERROR;
	else
	    return SEM_DONE;
    }
    else
    {
	array_t index = 0;
	thread_t id = thr_self();
	Semaphore::SemaphoreStatus result = SEM_DONE;

	if (!_semaphoreData->semData.isOwner(id, index))
	{
	    if ((sema_wait(&_semaphoreData->sp) != 0) || (_semaphoreData->semData.dead()))
		result = SEM_ERROR;
	    else
	    {
		// if we get here then we are now the owner.

		if (!_semaphoreData->semData.addOwner(id, index))
		{
		    sema_post(&_semaphoreData->sp);
		    result = SEM_ERROR;
		}
	    }
	}

	if (result == SEM_DONE)
	    _semaphoreData->semData.increment(index);
	
	return result;
    }
}

Semaphore::SemaphoreStatus Semaphore::unlock ()
{
    if (!_semaphoreData->reentrant)
    {
	if ((sema_post(&_semaphoreData->sp) != 0) || (_semaphoreData->semData.dead()))
	    return SEM_ERROR;
	else
	    return SEM_DONE;
    }
    else
    {
	thread_t id = thr_self();
	array_t index = 0;
	Semaphore::SemaphoreStatus result = SEM_DONE;

	if (_semaphoreData->semData.isOwner(id, index))
	{
	    _semaphoreData->semData.decrement(index);
	    
	    if (_semaphoreData->semData.count(index) == 0)
	    {
		_semaphoreData->semData.reset(index);

		if ((sema_post(&_semaphoreData->sp) != 0) || (_semaphoreData->semData.dead()))
		    result = SEM_ERROR;
	    }
	}
	else
	    result = SEM_ERROR;

	return result;
    }
}

Semaphore::SemaphoreStatus Semaphore::tryLock ()
{
    /*
     * sem_trywait returns WOULD_BLOCK even if we currently
     * own the semaphore, so we need to check this condition
     * before we get there.
     */

    if (!_semaphoreData->reentrant)
    {
	if (sema_trywait(&_semaphoreData->sp) != 0)
	    return SEM_WOULD_BLOCK;
	else
	    return SEM_DONE;
    }
    else
    {
	thread_t id = thr_self();
	array_t index = 0;
	Semaphore::SemaphoreStatus result = SEM_DONE;

	if (!_semaphoreData->semData.isOwner(id, index))
	{
	    if (sema_trywait(&_semaphoreData->sp) != 0)
		result = SEM_WOULD_BLOCK;
	    else
	    {
		// if we get here then we are now the owner.

		if (!_semaphoreData->semData.addOwner(id, index))
		{
		    sema_post(&_semaphoreData->sp);
		    result = SEM_ERROR;
		}
	    }
	}

	if (result == SEM_DONE)
	   _semaphoreData->semData.increment(index);

	return result;
    }
}


Mutex::Mutex (MutexSetup* setup)
	     : _mutexData(new MutexData(setup))
{
}

Mutex::~Mutex ()
{
    _mutexData->dead = TRUE;
    
    while (mutex_unlock(&_mutexData->mp) != 0)
	thr_yield();

    delete _mutexData;
}

Mutex::MutexStatus Mutex::lock ()
{
    if (!_mutexData->reentrant)
    {
	if ((mutex_lock(&_mutexData->mp) != 0) || (_mutexData->dead))
	    return MX_ERROR;
	else
	    return MX_DONE;
    }
    else
    {
	thread_t me = thr_self();
	Mutex::MutexStatus result = MX_DONE;

	if ((_mutexData->useCount != 0) && (_mutexData->currentOwner == me))
	    _mutexData->useCount++;
	else
	{
	    if ((mutex_lock(&_mutexData->mp) != 0) || (_mutexData->dead))
		result = MX_ERROR;
	    else
	    {
		_mutexData->currentOwner = me;
		_mutexData->useCount = 1;
	    }
	}

	return result;
    }
}

Mutex::MutexStatus Mutex::unlock ()
{
    if (!_mutexData->reentrant)
    {
	if ((mutex_unlock(&_mutexData->mp) != 0) || (_mutexData->dead))
	    return MX_ERROR;
	else
	    return MX_DONE;
    }
    else
    {
	thread_t me = thr_self();
	Mutex::MutexStatus result = MX_DONE;

	if ((_mutexData->useCount != 0) && (_mutexData->currentOwner == me))
	{
	    if (--_mutexData->useCount == 0)
	    {
		_mutexData->currentOwner = 0;
	    
		if ((mutex_unlock(&_mutexData->mp) != 0) || (_mutexData->dead))
		    result = Mutex::MX_ERROR;
	    }
	}
	else
	    result = Mutex::MX_ERROR;

	return result;
    }
}

Mutex::MutexStatus Mutex::tryLock ()
{
    if (!_mutexData->reentrant)
    {
	if (mutex_trylock(&_mutexData->mp) != 0)
	    return MX_WOULD_BLOCK;
	else
	    return MX_DONE;
    }
    else
    {
	thread_t me = thr_self();
	Mutex::MutexStatus result = MX_DONE;

	if ((_mutexData->useCount != 0) && (_mutexData->currentOwner == me))
	    _mutexData->useCount++;
	else
	{
	    if (mutex_trylock(&_mutexData->mp) != 0)
		result = Mutex::MX_WOULD_BLOCK;
	    else
	    {
		_mutexData->currentOwner = me;
		_mutexData->useCount = 1;
	    }
	}

	return result;
    }
}

void Semaphore::initialize ()
{
    InitThreads initThreads;
}

Semaphore* Semaphore::create (unsigned int number,
			      SemaphoreSetup* setup)
{
    return new Semaphore(number, setup);
}

void Mutex::initialize ()
{
    InitThreads initThreads;    
}

Mutex* Mutex::create (MutexSetup* setup)
{
    return new Mutex(setup);
}
