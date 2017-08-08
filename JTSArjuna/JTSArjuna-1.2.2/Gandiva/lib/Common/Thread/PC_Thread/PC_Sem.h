/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PC_Sem.h,v 1.1 1998/01/12 13:07:26 nmcl Exp $
 */

#ifndef COMMON_THREAD_PC_THREAD_PC_SEM_H_
#define COMMON_THREAD_PC_THREAD_PC_SEM_H_

#ifndef OS_PTHREAD_PTHREAD_H_
#  include <os/pthread/pthread.h>
#endif

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

#ifndef COMMON_THREAD_SEMAPHORE_H_
#  include <Common/Thread/Semaphore.h>
#endif

class SemaphoreSetup;
class MutexSetup;

class MutexData
{
public:
    MutexData (MutexSetup*);
    ~MutexData ();

    pthread_mutex_t mp;
    Boolean         dead;
    Boolean         reentrant;
    int             useCount;
    pthread_t       currentOwner;
};

#endif
