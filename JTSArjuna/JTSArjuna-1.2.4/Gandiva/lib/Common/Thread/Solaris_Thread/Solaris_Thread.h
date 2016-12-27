/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Solaris_Thread.h,v 1.1 1998/01/12 13:07:54 nmcl Exp $
 */

#ifndef COMMON_THREAD_SOLARIS_THREAD_SOLARIS_THREAD_H_
#define COMMON_THREAD_SOLARIS_THREAD_SOLARIS_THREAD_H_

#ifndef OS_THREAD_H_
#  include <os/thread.h>
#endif

#ifndef OS_SYNCH_H_
#  include <os/synch.h>
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

#ifndef TEMPLATE_BASICLIST_H_
#  include <Template/BasicList.h>
#endif

class ThreadData
{
public:
    ThreadData (Thread::ThreadAttachStatus, Thread *);
    ~ThreadData ();

    ThreadCommon commonData;
    
    Boolean onOverflow;
    thread_t sysId;
    mutex_t sleepLock;
    cond_t sleepCondition;
    mutex_t waitLock;			/* used in join */
    cond_t waiters;

#ifdef SOLARIS2
    static void interrupt (int);        /* for issuing kill signal to thread */
#else    
    static void interrupt (...);
#endif

    static thread_key_t threadKey;
    static BasicList<Thread, Uid>* overflowList;
};    

#endif
