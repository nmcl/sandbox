/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Solaris_Sem.h,v 1.1 1998/01/12 13:07:52 nmcl Exp $
 */

#ifndef COMMON_THREAD_SOLARIS_THREAD_SOLARIS_SEM_H_
#define COMMON_THREAD_SOLARIS_THREAD_SOLARIS_SEM_H_

#ifndef OS_THREAD_H_
#  include <os/thread.h>
#endif

#ifndef OS_SYNCH_H_
#  include <os/synch.h>
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
 * Classic Dijkstra (sp?) semaphores, which are not reentrant
 * but can be incremented above an initial value.
 */

class SemaphoreData
{
public:
    SemaphoreData (unsigned int, SemaphoreSetup*);
    ~SemaphoreData ();

    sema_t  sp;
    Boolean reentrant;
    SemData<thread_t> semData;
};

class MutexData
{
public:
    MutexData (MutexSetup*);
    ~MutexData ();

    mutex_t  mp;
    Boolean  dead;
    Boolean  reentrant;
    int      useCount;
    thread_t currentOwner;
};

#endif
