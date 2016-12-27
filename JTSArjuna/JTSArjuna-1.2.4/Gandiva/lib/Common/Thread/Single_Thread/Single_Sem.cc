/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Single_Sem.cc,v 1.1 1998/01/12 13:07:46 nmcl Exp $
 */

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef COMMON_THREAD_SINGLE_THREAD_SINGLE_SEM_H_
#  include <Common/Thread/Single_Thread/Single_Sem.h>
#endif

SemaphoreData::SemaphoreData (unsigned int, SemaphoreSetup*)
			     : useCount(0)
{
}

SemaphoreData::~SemaphoreData ()
{
    if (useCount > 0)
        cerr << "SingleSemaphore destructor error - there are still "
	     << useCount << " users" << endl;
}

MutexData::MutexData (MutexSetup*)
		     : useCount(0)
{
}

MutexData::~MutexData ()
{
    if (useCount > 0)
        cerr << "SingleMutex destructor error - there are still "
	     << useCount << " users" << endl;
}

Semaphore::Semaphore (unsigned int number, SemaphoreSetup* setup)
		     : _semaphoreData(new SemaphoreData(number, setup))
{
}

Semaphore::~Semaphore ()
{
    delete _semaphoreData;
}

Semaphore::SemaphoreStatus Semaphore::lock ()
{
    _semaphoreData->useCount++;
    return SEM_DONE;
}

Semaphore::SemaphoreStatus Semaphore::unlock ()
{
    _semaphoreData->useCount--;
    return SEM_DONE;
}

Semaphore::SemaphoreStatus Semaphore::tryLock ()
{
    return lock();
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
    _mutexData->useCount++;

    return MX_DONE;
}

Mutex::MutexStatus Mutex::unlock ()
{
    _mutexData->useCount--;

    return MX_DONE;
}

Mutex::MutexStatus Mutex::tryLock ()
{
    return lock();
}

void Semaphore::initialize ()
{
}

Semaphore* Semaphore::create (unsigned int number, SemaphoreSetup* setup)
{
    return new Semaphore(number, setup);
}

void Mutex::initialize ()
{
}

Mutex* Mutex::create (MutexSetup* setup)
{
    return new Mutex(setup);
}
