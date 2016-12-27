/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OmniORB_Sem.h,v 1.2 1998/01/13 13:48:46 nmcl Exp $
 */

#ifndef COMMON_THREAD_OMNIORB_THREAD_OMNIORB_SEM_H_
#define COMMON_THREAD_OMNIORB_THREAD_OMNIORB_SEM_H_

#include <os/omnithread.h>

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
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
 *
 * OmniOrb thread system simple mutex.
 *
 */

class MutexData
{
public:
    MutexData (MutexSetup* setup);
    ~MutexData ();

    omni_mutex* mutex;
    omni_thread* owner;
    int count;
    Boolean recursive;
    Boolean valid;
};

#endif
