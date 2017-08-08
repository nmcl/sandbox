/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RexLWP_Sem.cc,v 1.1 1998/01/12 13:07:40 nmcl Exp $
 */

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef OS_LWP_H_
#  include <os/lwp.h>
#endif

#ifndef COMMON_THREAD_REXLWP_THREAD_REXLWP_THREAD_H_
#  include <Common/Thread/RexLWP_Thread/RexLWP_Thread.h>
#endif

#ifndef COMMON_THREAD_REXLWP_THREAD_REXLWP_SEM_H_
#  include <Common/Thread/RexLWP_Thread/RexLWP_Sem.h>
#endif

#ifndef COMMON_THREAD_SEMAPHORESETUP_H_
#  include <Common/Thread/SemaphoreSetup.h>
#endif

#ifndef COMMON_THREAD_MUTEXSETUP_H_
#  include <Common/Thread/MutexSetup.h>
#endif

SemaphoreData::SemaphoreData (unsigned int number, SemaphoreSetup* setup)
                           : to_wait(0),
			     reentrant(FALSE),
			     semData((array_t) number)
{
    to_wait = creats((int) number);

    if (setup && !setup->reentrant)
	reentrant = FALSE;
}

SemaphoreData::~SemaphoreData ()
{
    delete to_wait;
}

Semaphore::Semaphore (unsigned int number, SemaphoreSetup* setup)
                     : _semaphoreData(new SemaphoreData(number, setup))
{
}

Semaphore::~Semaphore ()
{
    _semaphoreData->semData.dead(TRUE);
    
    if (_semaphoreData->to_wait)
    {
        while (_semaphoreData->to_wait->count < 0)
	{
	    signals(_semaphoreData->to_wait);
	    yieldp();
	}
    }

    delete _semaphoreData;
}

Semaphore::SemaphoreStatus Semaphore::lock ()
{
    if (_semaphoreData->reentrant)
    {
	void* id = getenvp(NULL);

	if (id)
        {
	    array_t index = 0;

	    if (!_semaphoreData->semData.isOwner(id, index))
	    {
		waits(_semaphoreData->to_wait);

		if (_semaphoreData->semData.dead())
		    return SEM_ERROR;
	    
		// if we get here then we are now the owner.

		if (!_semaphoreData->semData.addOwner(id, index))
		{
		    signals(_semaphoreData->to_wait);
		    return SEM_ERROR;
		}
	    }

	    _semaphoreData->semData.increment(index);

	    return SEM_DONE;
	}
	else
	{
	    return SEM_ERROR;
	}
    }
    else
    {
	waits(_semaphoreData->to_wait);

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
	void* id = getenvp(NULL);
	array_t index = 0;

	if ((id) && (_semaphoreData->semData.isOwner(id, index)))
        {
	    _semaphoreData->semData.decrement(index);

	    if (_semaphoreData->semData.count(index) > 0)
		return SEM_DONE;
	    else
	    {
		_semaphoreData->semData.reset(index);
	    }

	    signals(_semaphoreData->to_wait);

	    return SEM_DONE;
	}

	return SEM_ERROR;
    }
    else
    {
	signals(_semaphoreData->to_wait);

	return SEM_DONE;
    }
}

Semaphore::SemaphoreStatus Semaphore::tryLock ()
{
    if (_semaphoreData->reentrant)
    {
	void* me = getenvp(NULL);
	array_t index = 0;

	if (_semaphoreData->semData.isOwner(me, index))
        {
	    _semaphoreData->semData.increment(index);
	    return SEM_DONE;
	}
    }

    if (_semaphoreData->to_wait->count <= 0)
	return SEM_WOULD_BLOCK;
    else
        return lock();
}

MutexData::MutexData (MutexSetup* setup)
                     : to_wait(0),
		       dead(FALSE),
		       reentrant(TRUE),
		       useCount(0),
		       currentOwner(NULL)
{
    if (setup && !setup->reentrant)
	reentrant = FALSE;
    
    to_wait = creats(1);
}

MutexData::~MutexData ()
{
    delete to_wait;
}

Mutex::Mutex (MutexSetup* setup)
             : _mutexData(new MutexData(setup))
{
}

Mutex::~Mutex ()
{
    _mutexData->dead = TRUE;

    if (_mutexData->to_wait)
    {
        while (_mutexData->to_wait->count < 0)
	{
	    signals(_mutexData->to_wait);
	    yieldp();
	}
    }

    delete _mutexData;
}

Mutex::MutexStatus Mutex::lock ()
{
    if (_mutexData->reentrant)
    {
	void* me = getenvp(NULL);

	if (me)
        {
	    if ((_mutexData->useCount != 0) && (_mutexData->currentOwner == me))
	    {
		_mutexData->useCount++;
		return MX_DONE;
	    }

	    waits(_mutexData->to_wait);

	    if (_mutexData->dead)
		return MX_ERROR;
	    
	    // if we get here then we are now the owner.

	    _mutexData->currentOwner = me;
	    _mutexData->useCount = 1;

	    return MX_DONE;
	}

	return MX_ERROR;
    }
    else
    {
	waits(_mutexData->to_wait);

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
	void* me = getenvp(NULL);

	if (me)
        {
	    if ((_mutexData->useCount != 0) && (_mutexData->currentOwner == me))
	    {
		_mutexData->useCount--;

		if (_mutexData->useCount == 0)
		    signals(_mutexData->to_wait);

		return MX_DONE;
	    }
	}

	return MX_ERROR;
    }
    else
    {
	if (_mutexData->to_wait->count == 1)
	    return MX_ERROR;
	else
        {
	    signals(_mutexData->to_wait);
	    return MX_DONE;
	}
    }
}

Mutex::MutexStatus Mutex::tryLock ()
{
    if (_mutexData->to_wait->count == 1)
	return lock();

    if (_mutexData->reentrant)
    {
	void* me = getenvp(NULL);

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

void Semaphore::initialize ()
{
    InitThreads initThreads;
}

Semaphore* Semaphore::create (unsigned int number, SemaphoreSetup* setup)
{
    Semaphore::initialize();
    
    return new Semaphore(number, setup);
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
