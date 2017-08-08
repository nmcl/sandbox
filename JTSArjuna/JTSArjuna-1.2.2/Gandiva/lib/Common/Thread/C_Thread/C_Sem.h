/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: C_Sem.h,v 1.1 1998/01/12 13:06:56 nmcl Exp $
 */

#ifndef COMMON_THREAD_C_THREAD_C_SEM_H_
#define COMMON_THREAD_C_THREAD_C_SEM_H_

#ifndef OS_CTHREAD_H_    
#  include <os/cthread.h>
#endif

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

#ifndef COMMON_THREAD_SEMAPHORE_H_
#  include <Common/Thread/Semaphore.h>
#endif

#ifndef TEMPLATE_SEMDATA_H_
#  include <Template/SemData.h>
#endif

class SemaphoreSetup;
class MutexSetup;

/*
 * The current versions of cthreads do not support semaphore, so we
 * provide them using a mutex and a resource counter.
 */

/*
 * C threads is non-preemptive, so we don't have to protect these
 * structures with mutexes (yet!)
 */

class SemaphoreData
{
public:
    SemaphoreData (unsigned int, SemaphoreSetup*);
    ~SemaphoreData ();
    
    mutex_t mp;
    Boolean reentrant;
    int resources;
    SemData<cthread_t> semData;
};

class MutexData
{
public:
    MutexData (MutexSetup*);
    ~MutexData ();

    mutex_t   mp;
    Boolean   dead;
    Boolean   reentrant;
    int       useCount;
    cthread_t currentOwner;
};

#endif
