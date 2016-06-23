/*
 * Copyright (C) 1995-1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadCommon.h,v 1.1 1997/09/25 15:28:17 nmcl Exp $
 */

#ifndef THREADCOMMON_H_
#define THREADCOMMON_H_

/*
 *
 * Common structure for various thread systems
 *
 */

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef BASICLIST_H_
#  include <Template/BasicList.h>
#endif

#ifndef THREAD_H_
#  include <Thread/Thread.h>
#endif

#ifndef THREADMUTEX_H_
#  include <Thread/ThreadMutex.h>
#endif

struct ThreadCommon
{
    ThreadCommon (Thread::ThreadAttachStatus, Thread *);
    ~ThreadCommon ();
    
    Thread::ThreadStatus threadStat;
    Thread::ThreadAttachStatus threadAstat;
    Uid threadId;
    Thread *thread;
    ThreadMutex *mutex;
    
    void *arg;
    void *result;
    void * (*threadBody)(void*);
};

#endif
