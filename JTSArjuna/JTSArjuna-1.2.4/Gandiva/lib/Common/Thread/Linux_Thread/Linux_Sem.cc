/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Linux_Sem.cc,v 1.2 1998/01/16 10:29:46 nmcl Exp $
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

#ifndef COMMON_THREAD_LINUX_THREAD_LINUX_THREAD_H_
#  include <Common/Thread/Linux_Thread/Linux_Thread.h>
#endif

#ifndef COMMON_THREAD_LINUX_THREAD_LINUX_SEM_H_
#  include <Common/Thread/Linux_Thread/Linux_Sem.h>
#endif

// currently not implemented, but we can still pretend

#ifndef PTHREAD_PROCESS_SHARED
#define PTHREAD_PROCESS_SHARED 0
#define PTHREAD_PROCESS_PRIVATE 0
#endif

MutexData::MutexData (MutexSetup* setup)
		     : dead(FALSE)
{
    pthread_mutexattr_init(&mattr);

    /*
     * Have recursive mutexes! (Non-portable implementation though!)
     */
    
    if (!setup || (setup && setup->reentrant))
	pthread_mutexattr_setkind_np(&mattr, PTHREAD_MUTEX_RECURSIVE_NP);
    
    pthread_mutex_init(&mp, &mattr);
}

MutexData::~MutexData ()
{
    pthread_mutexattr_destroy(&mattr);    
}

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

    while ((pthread_mutex_destroy(&_mutexData->mp) != 0) && (errno == EBUSY))
    {
	pthread_mutex_unlock(&_mutexData->mp);
	Thread::yield();
    }

    delete _mutexData;
}

Mutex::MutexStatus Mutex::lock ()
{
    if (pthread_mutex_lock(&_mutexData->mp) == 0)
    {
	if (_mutexData->dead)
	    return Mutex::MX_ERROR;
	else
	    return Mutex::MX_DONE;
    }
    else
        return Mutex::MX_ERROR;
}

Mutex::MutexStatus Mutex::unlock ()
{
    if (pthread_mutex_unlock(&_mutexData->mp) == 0)
        return Mutex::MX_DONE;
    else
        return Mutex::MX_ERROR;
}

Mutex::MutexStatus Mutex::tryLock ()
{
    if (pthread_mutex_trylock(&_mutexData->mp) == 0)
        return Mutex::MX_DONE;
    else
        return Mutex::MX_WOULD_BLOCK;
}

Semaphore::Semaphore (unsigned int, SemaphoreSetup*)
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
    cerr << "Error - Linux threads do no support semaphores." << endl;
    
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
