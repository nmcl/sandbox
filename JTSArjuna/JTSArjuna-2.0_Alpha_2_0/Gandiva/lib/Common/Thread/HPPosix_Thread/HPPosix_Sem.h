/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HPPosix_Sem.h,v 1.2 1998/01/16 10:29:20 nmcl Exp $
 */

#ifndef COMMON_HPPOSIX_THREAD_HPPOSIX_SEM_H_
#define COMMON_HPPOSIX_THREAD_HPPOSIX_SEM_H_

#ifndef OS_PTHREAD_PTHREAD_H_    
#  include <os/pthread/pthread.h>
#endif

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

#ifndef COMMON_THREAD_SEMAPHORE_H_
#  include <Common/Thread/Semaphore.h>
#endif

class MutexSetup;

class MutexData
{
public:
    MutexData (MutexSetup*);
    ~MutexData ();

    pthread_mutex_t     mp;
    Boolean             dead;
};

#endif
