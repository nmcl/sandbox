/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RCPORB_Sem.h,v 1.2 1998/01/16 14:32:10 nmcl Exp $
 */

#ifndef COMMON_THREAD_RCPORB_THREAD_RCPORB_SEM_H_
#define COMMON_THREAD_RCPORB_THREAD_RCPORB_SEM_H_

#include <os/CORBA.ch>
#include <os/ON_init.h>
#include <os/ON_sync.h>

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMON_THREAD_SEMAPHORE_H_
#  include <Common/Thread/Semaphore.h>
#endif

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

class SemaphoreSetup;
class MutexSetup;

/*
 *
 * ON thread system simple mutex
 *
 */

class MutexData
{
public:
    MutexData (MutexSetup*);
    ~MutexData ();

    ODS_CORBA::Mutex* mutex;
    ON_Thread owner;
    int count;
    Boolean recursive;
    Boolean valid;
};

#endif
