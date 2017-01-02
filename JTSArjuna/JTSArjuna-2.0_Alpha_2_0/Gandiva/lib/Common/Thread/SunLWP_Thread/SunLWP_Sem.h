/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SunLWP_Sem.h,v 1.1 1998/01/12 13:07:56 nmcl Exp $
 */

#ifndef COMMON_THREAD_SUNLWP_THREAD_SUNLWP_SEM_H_
#define COMMON_THREAD_SUNLWP_THREAD_SUNLWP_SEM_H_

#ifndef OS_LWP_LWP_H_
#  include <os/lwp/lwp.h>
#endif

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

#ifndef COMMON_THREAD_SEMAPHORE_H_
#  include <Common/Thread/Semaphore.h>
#endif

class MutexSetup;
class SemaphoreSetup;

/*
 * No semaphores in SunLWP. Could implement them using
 * monitors.
 *
 * Mutexes are always reentrant.
 */

class MutexData
{
public:
    MutexData (MutexSetup*);
    ~MutexData ();

    mon_t mid;
    Boolean dead;
};

#endif
