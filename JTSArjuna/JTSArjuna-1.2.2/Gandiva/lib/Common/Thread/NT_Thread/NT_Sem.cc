/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NT_Sem.cc,v 1.2 1998/06/01 11:23:21 nmcl Exp $
 */

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef COMMON_THREAD_NT_THREAD_NT_SEM_H_
#  include <Common/Thread/NT_Thread/NT_Sem.h>
#endif

#ifndef COMMON_THREAD_SEMAPHORESETUP_H_
#  include <Common/Thread/SemaphoreSetup.h>
#endif

#ifndef COMMON_THREAD_MUTEXSETUP_H_
#  include <Common/Thread/MutexSetup.h>
#endif

#ifndef COMMON_THREAD_INITTHREADS_H_
#  include <Common/Thread/InitThreads.h>
#endif

SemaphoreData::SemaphoreData (unsigned int number, SemaphoreSetup* setup)
			     : sem(0),
			       dead(FALSE)
{
    if (setup && !setup->reentrant)
	cerr << "NT reentrant semaphores not yet supported" << endl;
    
    sem = CreateSemaphore(NULL, number, number, NULL);
}

SemaphoreData::~SemaphoreData ()
{
    CloseHandle(sem);
}

MutexData::MutexData (MutexSetup* setup)
		     : mutex(0),
		       dead(FALSE)
{
    BOOL type = 0;  // default

    if (setup)
    {
	if (setup->initialOwner)
	    type = 1;
	
	if (!setup->reentrant)
	    cerr << "NT reentrant mutexes not yet supported" << endl;
    }

    mutex = CreateMutex(NULL, type, NULL);    
}

MutexData::~MutexData ()
{
    CloseHandle(mutex);
}


Semaphore::Semaphore (unsigned int number, SemaphoreSetup* setup)
		     : _semaphoreData(new SemaphoreData(number, setup))
{
}

Semaphore::~Semaphore ()
{
    _semaphoreData->dead = TRUE;
    CloseHandle(_semaphoreData->sem);
    
    Sleep(0);
    
    delete _semaphoreData;
}

Semaphore::SemaphoreStatus Semaphore::lock ()
{
    if (WaitForSingleObject(_semaphoreData->sem, INFINITE) != WAIT_FAILED)
	return ((_semaphoreData->dead) ? SEM_ERROR : SEM_DONE);
    else
	return SEM_ERROR;
}

Semaphore::SemaphoreStatus Semaphore::unlock ()
{
    if (ReleaseSemaphore(_semaphoreData->sem, 1, NULL) != 0)
	return ((_semaphoreData->dead) ? SEM_ERROR : SEM_DONE);
    else
	return SEM_ERROR;  // can't say what happened
}

Semaphore::SemaphoreStatus Semaphore::tryLock ()
{
    DWORD res = WaitForSingleObject(_semaphoreData->sem, 0);

    if (res == WAIT_TIMEOUT)
	return SEM_WOULD_BLOCK;
    else
    {
	if (res == WAIT_FAILED)
	    return SEM_ERROR;
	else
	    return ((_semaphoreData->dead) ? SEM_ERROR : SEM_DONE);
    }
}

Mutex::Mutex (MutexSetup* setup)
	     : _mutexData(new MutexData(setup))
{
}

Mutex::~Mutex ()
{
    _mutexData->dead = TRUE;
    CloseHandle(_mutexData->mutex);
    Sleep(0);
}

Mutex::MutexStatus Mutex::lock ()
{
    if (WaitForSingleObject(_mutexData->mutex, INFINITE) != WAIT_FAILED)
	return ((_mutexData->dead) ? MX_ERROR : MX_DONE);
    else
	return MX_ERROR;
}

Mutex::MutexStatus Mutex::unlock ()
{
    if (ReleaseMutex(_mutexData->mutex) != 0)
	return ((_mutexData->dead) ? MX_ERROR : MX_DONE);
    else
	return MX_ERROR;
}

Mutex::MutexStatus Mutex::tryLock ()
{
    DWORD res = WaitForSingleObject(_mutexData->mutex, 0);

    if (res == WAIT_TIMEOUT)
        return MX_WOULD_BLOCK;
    else
    {
	if (res == WAIT_FAILED)
	    return MX_ERROR;
	else
	    return ((_mutexData->dead) ? MX_ERROR : MX_DONE);
    }
}

void Mutex::initialize ()
{
    InitThreads threadInit;
}

Mutex* Mutex::create (MutexSetup* setup)
{
    return new Mutex(setup);
}

void Semaphore::initialize ()
{
    InitThreads threadInit;
}

Semaphore* Semaphore::create (unsigned int number, SemaphoreSetup* setup)
{
    return new Semaphore(number, setup);
}
