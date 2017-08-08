/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SunLWP_Thread.h,v 1.1 1998/01/12 13:07:58 nmcl Exp $
 */

#ifndef COMMON_THREAD_SUNLWP_THREAD_SUNLWP_THREAD_H_
#define COMMON_THREAD_SUNLWP_THREAD_SUNLWP_THREAD_H_

#ifndef OS_LWP_LWP_H_
#  include <os/lwp/lwp.h>
#endif

#ifndef OS_LWP_STACKDEP_H_
#  include <os/lwp/stackdep.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMON_THREAD_THREADCOMMON_H_
#  include <Common/Thread/ThreadCommon.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

class ThreadData
{
public:
    ThreadData (Thread::ThreadAttachStatus, Thread*);
    ~ThreadData ();

    ThreadCommon commonData;

    stkalign_t* stack;
    thread_t sysId;
    mon_t waitLock;			/* used in join */
    int priority;
};

#endif
