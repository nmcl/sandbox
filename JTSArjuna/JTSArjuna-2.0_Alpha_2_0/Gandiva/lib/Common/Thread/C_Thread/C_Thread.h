/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: C_Thread.h,v 1.2 1998/06/01 13:53:25 nmcl Exp $
 */

#ifndef COMMON_THREAD_C_THREAD_C_THREAD_H_
#define COMMON_THREAD_C_THREAD_C_THREAD_H_

#ifndef OS_CTHREAD_H_
#  include <os/cthread.h>
#endif

#ifndef OS_SETJMP_H_
#  include <os/setjmp.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

#ifndef COMMON_THREAD_THREADCOMMON_H_
#  include <Common/Thread/ThreadCommon.h>
#endif

class ThreadData
{
public:
    ThreadData (Thread::ThreadAttachStatus, Thread *);
    ~ThreadData ();

    ThreadCommon commonData;

    cthread_t   sysId;
    mutex_t     suspendLock;
    mutex_t     waitLock;
    condition_t waiters;
    int         priority;
    Boolean     dead;
    Boolean isSuspending;
    
    static jmp_buf env;    
};

#endif
