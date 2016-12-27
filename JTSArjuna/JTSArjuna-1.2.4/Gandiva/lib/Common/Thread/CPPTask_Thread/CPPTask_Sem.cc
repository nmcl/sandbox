/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CPPTask_Sem.cc,v 1.2 1998/01/16 10:28:50 nmcl Exp $
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

#ifndef COMMON_THREAD_CPPTASK_THREAD_CPPTASK_SEM_H_
#  include <Common/Thread/CPPTask_Thread/CPPTask_Sem.h>
#endif

#ifndef COMMON_THREAD_SEMAPHORESETUP_H_
#  include <Common/Thread/SemaphoreSetup.h>
#endif

#ifndef COMMON_THREAD_MUTEXSETUP_H_
#  include <Common/Thread//MutexSetup.h>
#endif

/*
 * Semaphore class definition. Based on the one
 * in the manual.
 */

_semaphore::_semaphore (int number)
                       : sigs(number),
			 dead(FALSE)
{
}

_semaphore::~_semaphore ()
{
    dead = TRUE;
    alert();
}

int _semaphore::pending () { return sigs <= 0; }

void _semaphore::signal ()
{
    if (sigs++ == 0)
    {
        alert();
    }
}

void _semaphore::wait ()
{
    for (;;)
    {
	if (--sigs >= 0)
	{
	    return;
	}

	sigs++;
	
	thistask->sleep(this);

	if (dead)
	    return;
    }
}

SemaphoreData::SemaphoreData (unsigned int number, SemaphoreSetup* setup)
			     : sem(new _semaphore(number)),
			       reentrant(TRUE),
			       semData((array_t) number)			       
{
    if (setup && !setup->reentrant)
	reentrant = FALSE;
}

SemaphoreData::~SemaphoreData ()
{
    delete sem;
}

MutexData::MutexData (MutexSetup* setup)
		     : sem(new _semaphore(1)),
		       dead(FALSE),
		       reentrant(TRUE),
		       useCount(0),
		       currentOwner(0)
{
    if (setup && !setup->reentrant)
	reentrant = FALSE;
}

MutexData::~MutexData ()
{
    delete sem;
}

Semaphore::Semaphore (unsigned int number, SemaphoreSetup* setup)
		     : _semaphoreData(new SemaphoreData(number, setup))
{
}

Semaphore::~Semaphore ()
{
    _semaphoreData->semData.dead(TRUE);

    while (_semaphoreData->sem->pending())
    {
	_semaphoreData->sem->signal();
	object::this_task()->delay(0);
    }
	
    delete _semaphoreData;
}

Semaphore::SemaphoreStatus Semaphore::lock ()
{
    if (_semaphoreData->reentrant)
    {
	task* id = object::this_task();  // assume this doesn't get deleted!

	if (id)
	{
	    array_t index = 0;
	
	    if (!_semaphoreData->semData.isOwner(id, index))
	    {
		_semaphoreData->sem->wait();

		if (_semaphoreData->semData.dead())
		    return SEM_ERROR;
	    
		// if we get here then we are now the owner.

		if (!_semaphoreData->semData.addOwner(id, index))
		{
		    _semaphoreData->sem->signal();
		    return SEM_ERROR;
		}
	    }

	    _semaphoreData->semData.increment(index);

	    return SEM_DONE;
	}

	return SEM_ERROR;
    }
    else
    {
	_semaphoreData->sem->wait();
	
	if (_semaphoreData->semData.dead())
	    return SEM_ERROR;
	else
	    return SEM_DONE;
    }
}

Semaphore::SemaphoreStatus Semaphore::unlock ()
{
    if (_semaphoreData->reentrant)
    {
	task* id = object::this_task();
	array_t index = 0;

	if (id)
	{
	    if (_semaphoreData->semData.isOwner(id, index))
	    {
		_semaphoreData->semData.decrement(index);

		if (_semaphoreData->semData.count(index) > 0)
		    return SEM_DONE;
		else
		{
		    _semaphoreData->semData.reset(index);
		}

		_semaphoreData->sem->signal();
	    
		return SEM_DONE;
	    }
	}
    
	return SEM_ERROR;
    }
    else
    {
	_semaphoreData->sem->signal();
	return SEM_DONE;
    }
}

Semaphore::SemaphoreStatus Semaphore::tryLock ()
{
    if (_semaphoreData->reentrant)
    {
	task* me = object::this_task();
	array_t index = 0;

	if (_semaphoreData->semData.isOwner(me, index))
	{
	    _semaphoreData->semData.increment(index);
	    return SEM_DONE;
	}
    }
    
    if (_semaphoreData->sem->pending())
	return SEM_WOULD_BLOCK;
    else
	return lock();
}

Mutex::Mutex (MutexSetup* setup)
	     : _mutexData(new MutexData(setup))
{
}

Mutex::~Mutex ()
{
    _mutexData->dead = TRUE;
    
    while (_mutexData->sem->pending())
    {
	_mutexData->sem->signal();
	object::this_task()->delay(0);
    }
    
    delete _mutexData;
}

Mutex::MutexStatus Mutex::lock ()
{
    if (_mutexData->reentrant)
    {
	task* me = object::this_task();

	if ((_mutexData->useCount != 0) && (_mutexData->currentOwner == me))
	{
	    _mutexData->useCount++;
	    return MX_DONE;
	}

	_mutexData->sem->wait();

	if (_mutexData->dead)
	    return MX_ERROR;
	    
	// if we get here then we are now the owner.

	_mutexData->currentOwner = me;
	_mutexData->useCount = 1;

	return MX_DONE;
    }
    else
    {
	_mutexData->sem->wait();

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
	task* me = object::this_task();

	if ((_mutexData->useCount != 0) && (_mutexData->currentOwner == me))
	{
	    _mutexData->useCount--;

	    if (_mutexData->useCount == 0)
		_mutexData->sem->signal();

	    return MX_DONE;
	}

	return MX_ERROR;
    }
    else
    {
	if (_mutexData->sem->sigs == 1)
	    return MX_ERROR;
	else
	{
	    _mutexData->sem->signal();
	    return MX_DONE;
	}
    }
}

Mutex::MutexStatus Mutex::tryLock ()
{
    if (_mutexData->sem->sigs == 1)
	return lock();

    if (_mutexData->reentrant)
    {
	task* me = object::this_task();

	if ((_mutexData->useCount != 0) && (_mutexData->currentOwner == me))
        {
	    _mutexData->useCount++;
	    
	    return MX_DONE;
	}
    }

    /*
     * count must be 0, so we would block.
     */

    return MX_WOULD_BLOCK;
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
