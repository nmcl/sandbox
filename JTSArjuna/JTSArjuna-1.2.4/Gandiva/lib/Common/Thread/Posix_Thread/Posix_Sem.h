/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Posix_Sem.h,v 1.1 1998/01/12 13:07:31 nmcl Exp $
 */

#ifndef COMMON_THREAD_POSIX_THREAD_POSIX_SEM_H_
#define COMMON_THREAD_POSIX_THREAD_POSIX_SEM_H_

#if !defined(OS_SEMAPHORE_H_) && defined(PTHREAD_SUN)
#  include <os/semaphore.h>
#endif

#ifndef OS_PTHREAD_PTHREAD_H_    
#  include <os/pthread/pthread.h>
#endif

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

#ifndef COMMON_THREAD_SEMAPHORE_H_
#  include <Common/Thread/Semaphore.h>
#endif

#ifndef TEMPLATE_POSIXSEMDATA_H_
#  include <Template/PosixSemData.h>
#endif

class MutexSetup;
class SemaphoreSetup;

class MutexData
{
public:
    MutexData (MutexSetup*);
    ~MutexData ();

    pthread_mutex_t     mp;
    pthread_mutexattr_t mattr;
    Boolean             dead;
    int                 useCount;
    pthread_t           currentOwner;
};

#ifdef PTHREAD_SUN

class SemaphoreData
{
public:
    SemaphoreData (unsigned int, SemaphoreSetup*);
    ~SemaphoreData ();

    /*
     * These data structures are used to give us reentrant
     * mutexes/sempahores. However, since they are shared
     * between threads we need to protect them with a mutex!
     */
    
    sem_t                   sp;
    PosixSemData<pthread_t> semData;
};

#endif

#endif
