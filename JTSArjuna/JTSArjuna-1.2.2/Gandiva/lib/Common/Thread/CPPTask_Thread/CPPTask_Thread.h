/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CPPTask_Thread.h,v 1.4 1998/06/01 13:42:04 nmcl Exp $
 */

#ifndef COMMON_THREAD_CPPTASK_THREAD_CPPTASK_THREAD_H_
#define COMMON_THREAD_CPPTASK_THREAD_CPPTASK_THREAD_H_

#ifndef OS_SYS_TYPES_H_
#  include <os/sys/types.h>
#endif

#ifndef OS_TASK_H_
#  include <os/task.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef COMMON_THREAD_CPPTASK_THREAD_CPPTASK_SEM_H_
#  include <Common/Thread/CPPTask_Thread/CPPTask_Sem.h>
#endif

#ifndef COMMON_THREAD_THREADCOMMON_H_
#  include <Common/Thread/ThreadCommon.h>
#endif

/*
 * We do this because multiple (single) inheritence doesn't work
 * (setjmp/longjmp limitations).
 */

class ThreadData : public task
{
public:
    ThreadData (Thread::ThreadAttachStatus, Thread*, void* (*body)(void*), void* arg, int stackSize);
    ~ThreadData ();

    int pending () const;
    
    ThreadCommon commonData;

    task* sysId;
    _semaphore suspendLock;
    _semaphore waitLock;
    int priority;
    int isPending;
    Boolean isSuspending;

    static task    *mainSysId;
    static Thread  *mainTask;
};

#endif
