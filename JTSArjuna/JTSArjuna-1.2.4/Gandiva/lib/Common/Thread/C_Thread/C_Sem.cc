/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: C_Sem.cc,v 1.1 1998/01/12 13:06:55 nmcl Exp $
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

#ifndef COMMON_THREAD_C_THREAD_C_SEM_H_
#  include <Common/Thread/C_Thread/C_Sem.h>
#endif

#ifndef COMMON_THREAD_C_THREAD_C_THREAD_H_
#  include <Common/Thread/C_Thread/C_Thread.h>
#endif

SemaphoreData::SemaphoreData (unsigned int number, SemaphoreSetup* setup)
			     : reentrant(TRUE),
			       resources(number),
			       semData((array_t) number)
{
    if (setup && !setup->reentrant)
	reentrant = FALSE;
    
    mutex_alloc(&mp, N_CURRENT);
    mutex_init(mp);
}

SemaphoreData::~SemaphoreData ()
{
    mutex_free(mp);
}

/*
 * Because cthreads does not support re-entrant mutexes
 * we have to do the checking at this level.
 */

MutexData::MutexData (MutexSetup* setup)
		     : dead(FALSE),
		       reentrant(TRUE),
		       useCount(0)
{
    mutex_alloc(&mp, N_CURRENT);
    mutex_init(mp);
}

MutexData::~MutexData ()
{
    mutex_free(mp);    
}


Semaphore::Semaphore (unsigned int number, SemaphoreSetup* setup)
		     : _semaphoreData(new SemaphoreData(number, setup))
{
}

Semaphore::~Semaphore ()
{
    _semaphoreData->semData.dead(TRUE);

    mutex_clear(_semaphoreData->mp);
    cthread_yield();  // give threads a chance to run
    
    delete _semaphoreData;
}

Semaphore::SemaphoreStatus Semaphore::lock ()
{
    if (_semaphoreData->reentrant)
    {
	cthread_t id = cthread_self();
	array_t index = 0;
    
	if (!_semaphoreData->semData.isOwner(id, index))
	{
	    if (_semaphoreData->semData.full())
	    {
		mutex_lock(_semaphoreData->mp);
		cthread_yield();

		if (_semaphoreData->semData.dead())
		    return SEM_ERROR;
	    }
	    
	    // if we get here then we are now the owner.

	    if (!_semaphoreData->semData.addOwner(id, index))
	    {
		mutex_unlock(_semaphoreData->mp);
		cthread_yield();

		return SEM_ERROR;
	    }

	    cthread_yield();
	}

	_semaphoreData->semData.increment(index);

	return SEM_DONE;
    }
    else
    {
	if (_semaphoreData->resources == 0)
	{
	    mutex_lock(_semaphoreData->mp);
	    cthread_yield();

	    if (_semaphoreData->semData.dead())
		return SEM_ERROR;
	}

	_semaphoreData->resources--;

	return SEM_DONE;
    }
}

Semaphore::SemaphoreStatus Semaphore::unlock ()
{
    if (_semaphoreData->reentrant)
    {
	cthread_t id = cthread_self();
	array_t index = 0;

	if (_semaphoreData->semData.isOwner(id, index))
	{
	    _semaphoreData->semData.decrement(index);

	    if (_semaphoreData->semData.count(index) > 0)
		return SEM_DONE;
	    else
	    {
		_semaphoreData->semData.reset(index);
	    }

	    if (_semaphoreData->semData.full())
	    {
		mutex_unlock(_semaphoreData->mp);
	    }
	}
    
	cthread_yield();

	return SEM_DONE;
    }
    else
    {
	if (_semaphoreData->resources == 0)
	    mutex_unlock(_semaphoreData->mp);

	_semaphoreData->resources++;

	cthread_yield();
    }
}

Semaphore::SemaphoreStatus Semaphore::tryLock ()
{
    if (!_semaphoreData->reentrant)
    {
	if (_semaphoreData->resources == 0)
	    return SEM_WOULD_BLOCK;
	else
	    return lock();
    }
    else
    {
	cthread_t id = cthread_self();
	array_t index = 0;
    
	if (_semaphoreData->semData.isOwner(id, index) ||
	    !_semaphoreData->semData.full())
	    return lock();
	else
	    return SEM_WOULD_BLOCK;
    }
}


Mutex::Mutex (MutexSetup* setup)
	     : _mutexData(new MutexData(setup))
{
}

Mutex::~Mutex ()
{
    _mutexData->dead = TRUE;
    
    mutex_clear(_mutexData->mp);
    cthread_yield();  // give threads a chance to run

    delete _mutexData;
}

Mutex::MutexStatus Mutex::lock ()
{
    if (_mutexData->reentrant)
    {
	cthread_t id = cthread_self();

	if ((_mutexData->useCount != 0) && (_mutexData->currentOwner == id))
	{
	    _mutexData->useCount++;
	    return MX_DONE;
	}

	mutex_lock(_mutexData->mp);
	cthread_yield();

	if (_mutexData->dead)
	    return MX_ERROR;

	// if we get here then we are now the owner.

	_mutexData->currentOwner = id;
	_mutexData->useCount = 1;

	return MX_DONE;
    }
    else
    {
	mutex_lock(_mutexData->mp);

	_mutexData->useCount = 1;
	
	if (_mutexData->dead)
	    return MX_ERROR;
	else
	    return MX_DONE;
    }
}

Mutex::MutexStatus Mutex::unlock ()
{
    if (_mutexData->reentrant)
    {
	cthread_t id = cthread_self();

	if ((_mutexData->useCount != 0) && (id == _mutexData->currentOwner))
	{
	    _mutexData->useCount--;
	    
	    if (_mutexData->useCount == 0)
	    {
		mutex_unlock(_mutexData->mp);
		cthread_yield();
	    }

	    return MX_DONE;
	}

	return MX_ERROR;
    }
    else
    {
	_mutexData->useCount = 0;
	
	mutex_unlock(_mutexData->mp);
	cthread_yield();

	return MX_DONE;
    }
}

Mutex::MutexStatus Mutex::tryLock ()
{
    if (_mutexData->reentrant)
    {
	cthread_t id = cthread_self();

	if ((_mutexData->useCount != 0) && (id == _mutexData->currentOwner))
	{
	    _mutexData->useCount++;
	    return MX_DONE;
	}

	if (_mutexData->useCount != 0)    
	    return MX_WOULD_BLOCK;
	else
	    return lock();
    }
    else
    {
	if (_mutexData->useCount != 0)
	    return MX_WOULD_BLOCK;
	else
	    return lock();
    }
}

void Mutex::initialize ()
{
    InitThreads initThreads;
}

Mutex* Mutex::create (MutexSetup* setup)
{
    Mutex::initialize();
    
    return new Mutex(setup);
}

void Semaphore::initialize ()
{
    InitThreads initThreads;    
}

Semaphore* Semaphore::create (unsigned int number, SemaphoreSetup* setup)
{
    Semaphore::initialize();
    
    return new Semaphore(number, setup);
}
