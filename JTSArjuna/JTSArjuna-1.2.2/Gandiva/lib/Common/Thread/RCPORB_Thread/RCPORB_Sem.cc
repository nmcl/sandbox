/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RCPORB_Sem.cc,v 1.2 1998/01/16 14:32:09 nmcl Exp $
 */

/*
 * Implementation of thread ids for ON threads
 */

#include <os/CORBA.ch>
#include <os/ON_init.h>

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef COMMON_THREAD_INITTHREADS_H_
#  include <Common/Thread/InitThreads.h>
#endif

#ifndef COMMON_THREAD_RCPORB_THREAD_RCPORB_SEM_H_
#  include <Common/Thread/RCPORB_Thread/RCPORB_Sem.h>
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
    
    mutex = new ODS_CORBA::Mutex();
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
    if (_mutexData->valid)
    {
	ON_Thread me = ON_thread_self();
	
	if ((_mutexData->recursive) && (_mutexData->owner == me))
	{
	    _mutexData->count++;
	    return MX_DONE;
	}

	if (_mutexData->mutex->lock() == 0)
	{
	    _mutexData->count = 1;
	    _mutexData->owner = me;
	    return MX_DONE;
	}
	else
	    _mutexData->valid = FALSE;
    }
    
    return MX_ERROR;
}

Mutex::MutexStatus Mutex::unlock ()
{
    if (_mutexData->valid)
    {
	ON_Thread me = ON_thread_self();
	
	if ((_mutexData->recursive) && (_mutexData->owner == me))
	{
	    if (--_mutexData->count > 0)
		return MX_DONE;
	}
	
	_mutexData->owner = 0;
	_mutexData->count = 0;
	
	if (_mutexData->mutex->unlock() == 0)
	    return MX_DONE;
    }
    
    return MX_ERROR;
}

Mutex::MutexStatus Mutex::tryLock ()
{
    if (_mutexData->valid)
    {
	ON_Thread me = ON_thread_self();
	int status = 0;

	if ((_mutexData->recursive) && (_mutexData->owner == me))
	{
	    _mutexData->count++;
	    return MX_DONE;
	}
	
	if ((status = _mutexData->mutex->trylock()) == 0)
	{
	    _mutexData->owner = me;
	    _mutexData->count = 1;
	    return MX_DONE;	
	}
	else
	{
	    if (status != EBUSY)
	    {
		_mutexData->valid = FALSE;
		return MX_ERROR;
	    }
	}
    }

    return MX_WOULD_BLOCK;
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

Semaphore* Semaphore::create (unsigned int, SemaphoreSetup*)
{
    return (Semaphore*) 0;
}

Mutex* Mutex::create (MutexSetup* setup)
{
    return new Mutex(setup);
}
