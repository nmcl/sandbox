/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SunLWP_Sem.cc,v 1.1 1998/01/12 13:07:56 nmcl Exp $
 */

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef OS_LWP_LWPERROR_H_
#  include <os/lwp/lwperror.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef COMMON_THREAD_SUNLWP_THREAD_SUNLWP_SEM_H_
#  include <Common/Thread/SunLWP_Thread/SunLWP_Sem.h>
#endif

MutexData::MutexData (MutexSetup*)
                     : dead(FALSE)
{
    mon_create(&mid);
}

MutexData::~MutexData ()
{
    mon_destroy(mid);
}

Mutex::Mutex (MutexSetup* setup)
             : _mutexData(new MutexData(setup))
{
}

Mutex::~Mutex ()
{
    mon_exit(_mutexData->mid);
    _mutexData->dead = TRUE;
    
    /*
     * If there are any threads waiting on this mutex
     * then we must wake them up.
     */

    while (mon_break(_mutexData->mid) != -1)
	lwp_yield(SELF);
    
    delete _mutexData;
}

Mutex::MutexStatus Mutex::lock ()
{
    if ((mon_enter(_mutexData->mid) != 0) || (_mutexData->dead))
	return MX_ERROR;
    else
        return MX_DONE;
}

Mutex::MutexStatus Mutex::unlock ()
{
    if ((_mutexData->dead) || (mon_exit(_mutexData->mid) == 0))
	return MX_DONE;
    else
	return MX_ERROR;
}

Mutex::MutexStatus Mutex::tryLock ()
{
    if (mon_cond_enter(_mutexData->mid) < 0) 
        return MX_WOULD_BLOCK;
    else
	return MX_DONE;
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

Semaphore* Semaphore::create (unsigned int, SemaphoreSetup*)
{
    return (Semaphore*) 0;
}

void Mutex::initialize ()
{
    InitThreads initThreads;    
}

Mutex* Mutex::create (MutexSetup* setup)
{
    return new Mutex(setup);
}
