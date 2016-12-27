/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Linux_Sem.h,v 1.1 1998/01/12 13:07:05 nmcl Exp $
 */

#ifndef COMMON_THREAD_LINUX_THREAD_LINUX_SEM_H_
#define COMMON_THREAD_LINUX_THREAD_LINUX_SEM_H_

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
    pthread_mutexattr_t mattr;
    Boolean             dead;
};

#endif
