/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RexLWP_Sem.h,v 1.1 1998/01/12 13:07:42 nmcl Exp $
 */

#ifndef COMMON_THREAD_REXLWP_THREAD_REXLWP_SEM_H_
#define COMMON_THREAD_REXLWP_THREAD_REXLWP_SEM_H_

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

struct sem;

class SemaphoreData
{
public:
    SemaphoreData (unsigned int, SemaphoreSetup*);
    ~SemaphoreData ();

    sem*    to_wait;
    Boolean reentrant;
    SemData<void*> semData;
};

class MutexData
{
public:
    MutexData (MutexSetup*);
    ~MutexData ();

    sem*    to_wait;
    Boolean dead;
    Boolean reentrant;
    int     useCount;
    void*   currentOwner;
};

#endif
