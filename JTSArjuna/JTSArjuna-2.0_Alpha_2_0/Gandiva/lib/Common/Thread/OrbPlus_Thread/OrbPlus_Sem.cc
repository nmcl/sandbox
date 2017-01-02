/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPlus_Sem.cc,v 1.2 1998/01/19 10:35:10 nmcl Exp $
 */

/*
 * Implementation of thread ids for MSD threads
 */

#include <os/iostream.h>

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef COMMON_THREAD_INITTHREADS_H_
#  include <Common/Thread/InitThreads.h>
#endif

#ifndef COMMON_THREAD_ORBPLUS_THREAD_ORBPLUS_SEM_H_
#  include <Common/Thread/OrbPlus_Thread/OrbPlus_Sem.h>
#endif

#ifndef COMMON_THREAD_MUTEXSETUP_H_
#  include <Common/Thread/MutexSetup.h>
#endif

MutexData::MutexData (MutexSetup* setup)
		     : mutex(0),
		       owner(0),
		       count(0),
		       recursive(TRUE),
		       valid(FALSE)
{
    if (setup && !setup->reentrant)
	recursive = FALSE;

    mutex = new MSD_Mutex();
    valid = TRUE;
}

MutexData::~MutexData ()
{
    if (valid)
    {
	mutex->unlock();
	delete mutex;
    }
}

Mutex::Mutex (MutexSetup* setup)
	     : _mutexData(new MutexData(setup))
{
}

Mutex::~Mutex ()
{
    delete _mutexData;
}

Mutex::MutexStatus Mutex::lock ()
{
    Mutex::MutexStatus result = MX_ERROR;
    
    if (_mutexData->valid)
    {
	MSD_Thread me = MSD_Thread::self();
	
	if ((_mutexData->recursive) && (_mutexData->owner == me.number()))
	{
	    _mutexData->count++;
	    result = MX_DONE;
	}
	else
	{
	    _mutexData->mutex->lock();
	    
	    _mutexData->count = 1;
	    _mutexData->owner = me.number();
	    result = MX_DONE;
	}
    }
    
    return result;
}

Mutex::MutexStatus Mutex::unlock ()
{
    Mutex::MutexStatus result = MX_ERROR;

    if (_mutexData->valid)
    {
	MSD_Thread me = MSD_Thread::self();

	if (_mutexData->recursive)
	{
	    if (_mutexData->owner == me.number())
	    {
		result = MX_DONE;
		
		if (--_mutexData->count == 0)
		{
		    _mutexData->owner = 0;
		    _mutexData->count = 0;

		    _mutexData->mutex->unlock();
		}
	    }
	}
	else
	{
	    _mutexData->mutex->unlock();
	    result = MX_DONE;
	}
    }
    
    return result;
}

Mutex::MutexStatus Mutex::tryLock ()
{
    Mutex::MutexStatus result = MX_ERROR;

    if (_mutexData->valid)
    {
	MSD_Thread me = MSD_Thread::self();

	if ((_mutexData->recursive) && (_mutexData->owner == me.number()))
	{
	    _mutexData->count++;
	    result = MX_DONE;
	}
	else
	{
	    if (_mutexData->mutex->trylock() == 0)
	    {
		result = MX_WOULD_BLOCK;
	    }
	    else
	    {
		_mutexData->count = 1;
		_mutexData->owner = me.number();		
		result = MX_DONE;
	    }
	}
    }

    return result;
}

Semaphore::Semaphore (unsigned int, SemaphoreSetup*)
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

Semaphore* Semaphore::create (unsigned int, SemaphoreSetup*)
{
    return (Semaphore*) 0;
}
