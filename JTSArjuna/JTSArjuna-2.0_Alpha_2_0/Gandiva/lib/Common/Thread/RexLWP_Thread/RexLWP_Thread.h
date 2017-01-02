/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RexLWP_Thread.h,v 1.4 1998/06/01 14:24:05 nmcl Exp $
 */

#ifndef COMMON_THREAD_REXLWP_THREAD_REXLWP_THREAD_H_
#define COMMON_THREAD_REXLWP_THREAD_REXLWP_THREAD_H_

#ifndef OS_LWP_H_
#  include <os/lwp.h>
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

struct pcb;
struct sem;

class ThreadData
{
public:
    ThreadData (Thread::ThreadAttachStatus, Thread*);
    ~ThreadData ();

    ThreadCommon commonData;

    static void execute (int, char**, Thread*);
    
    pcb* sysId;
    sem* suspendLock;
    sem* waitLock;
    Boolean isSuspending;
};

#endif
