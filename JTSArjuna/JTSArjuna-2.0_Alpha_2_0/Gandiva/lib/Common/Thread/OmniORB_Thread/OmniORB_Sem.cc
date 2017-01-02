/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OmniORB_Sem.cc,v 1.2 1998/01/13 13:48:45 nmcl Exp $
 */

/*
 * Implementation of thread ids for OmniOrb threads
 */

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef COMMON_THREAD_OMNIORB_THREAD_OMNIORB_SEM_H_
#  include <Common/Thread/OmniORB_Thread/OmniORB_Sem.h>
#endif

#ifndef COMMON_THREAD_INITTHREADS_H_
#  include <Common/Thread/InitThreads.h>
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
    
    mutex = new omni_mutex();
    valid = TRUE;
}

MutexData::~MutexData ()
{
    if (valid)
    {
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
	omni_thread* me = omni_thread::self();

	if ((_mutexData->recursive) && (_mutexData->owner == me))
	{
	    _mutexData->count++;
	    result = MX_DONE;
	}
	else
	{
	    try
	    {
		_mutexData->mutex->lock();
		
		_mutexData->count = 1;
		_mutexData->owner = me;
		result = MX_DONE;
	    }
	    catch (...)
	    {
	    }
	}
    }
    
    return result;
}

Mutex::MutexStatus Mutex::unlock ()
{
    Mutex::MutexStatus result = MX_ERROR;

    if (_mutexData->valid)
    {
	omni_thread* me = omni_thread::self();

	if ((_mutexData->recursive) && (_mutexData->owner == me))
	{
	    result = MX_DONE;
		
	    if (--_mutexData->count == 0)
	    {
		_mutexData->owner = 0;
		_mutexData->count = 0;

		try
		{
		    _mutexData->mutex->unlock();
		}
		catch (...)
		{
		    result = MX_ERROR;
		}
	    }
	}
    }
    
    return result;
}

/*
 * For some bizarre reason OmniOrb doesn't support trywait!!
 */

Mutex::MutexStatus Mutex::tryLock ()
{
    Mutex::MutexStatus result = MX_ERROR;

    if (_mutexData->valid)
    {
	omni_thread* me = omni_thread::self();
	int status = 0;

	if ((_mutexData->recursive) && (_mutexData->owner == me))
	{
	    _mutexData->count++;
	    result = MX_DONE;
	}
	else
	{
	    if (_mutexData->owner == NULL)
		return lock();
	    else
		result = MX_WOULD_BLOCK;
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
