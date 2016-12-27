/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CPPTask_Sem.h,v 1.2 1998/01/16 10:28:51 nmcl Exp $
 */

#ifndef COMMON_THREAD_CPPTASK_THREAD_CPPTASK_SEM_H_
#define COMMON_THREAD_CPPTASK_THREAD_CPPTASK_SEM_H_

#ifndef OS_TASK_H_
#  include <os/task.h>
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

class MutexSetup;
class SemaphoreSetup;

/*
 * A simple semaphore class based on the one in the Cfront 2.1 release notes.
 */

class _semaphore : public object
{
public:
    _semaphore (int number = 0);
    ~_semaphore ();

    int pending ();
    void wait ();
    void signal ();

    int     sigs;	// the number of excess signals
    Boolean dead;
};


class SemaphoreData
{
public:
    SemaphoreData (unsigned int, SemaphoreSetup*);
    ~SemaphoreData ();
    
    _semaphore* sem;
    Boolean reentrant;
    SemData<task*> semData;
};

class MutexData
{
public:
    MutexData (MutexSetup*);
    ~MutexData ();

    _semaphore* sem;
    Boolean dead;
    Boolean reentrant;
    int useCount;
    task* currentOwner;
};

#endif
