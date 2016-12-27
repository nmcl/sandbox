/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OmniORB_Thread.h,v 1.5 1998/07/21 12:41:01 nmcl Exp $
 */

/*
 * Implementation of threads for OmniOrb threads
 */

#ifndef OS_OMNITHREAD_H_
#  include <os/omnithread.h>
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

/*
 * So we can delete the data structures for attached threads
 * without having to do a join! Aaagghhh!!!!
 */

class OmniORB_Thread : public omni_thread
{
public:
    OmniORB_Thread (void* (*fn)(void*), void* arg = NULL,
		    priority_t pri = PRIORITY_NORMAL)
		   : omni_thread(fn, arg, pri)
    {
    }

    virtual ~OmniORB_Thread ()
    {
    }
};

class ThreadData
{
public:
    ThreadData (Thread::ThreadAttachStatus, Thread *);
    ~ThreadData ();

    static void* execute (void* arg);
    
    ThreadCommon commonData;
    
    Boolean onOverflow;			/* on overflow list */
    omni_thread* sysId;			/* system internal id */
    omni_mutex* suspendLock;    	/* used for suspend/resume */
    omni_mutex* waitLock;               /* used in join */
    omni_condition* waiters;
    Boolean isSuspending;
};
