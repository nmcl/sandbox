/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Single_Sem.h,v 1.1 1998/01/12 13:07:47 nmcl Exp $
 */

#ifndef COMMON_THREAD_SINGLE_THREAD_SINGLE_SEM_H_
#define COMMON_THREAD_SINGLE_THREAD_SINGLE_SEM_H_

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

#ifndef COMMON_THREAD_SEMAPHORE_H_
#  include <Common/Thread/Semaphore.h>
#endif

class SemaphoreData
{
public:
    SemaphoreData (unsigned int, SemaphoreSetup*);
    ~SemaphoreData ();
    
    int useCount;
};

class MutexData
{
public:
    MutexData (MutexSetup*);
    ~MutexData ();
    
    int useCount;
};

#endif
