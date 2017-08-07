/*
 * Copyright (C) 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadCommon.h,v 1.3 1998/01/15 12:28:50 nmcl Exp $
 */

#ifndef COMMON_THREAD_THREADCOMMON_H_
#define COMMON_THREAD_THREADCOMMON_H_

/*
 *
 * Common structure for various thread systems
 *
 */

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

#ifndef BASE_ACTION_H_
#  include <Base/Action.h>
#endif

class ThreadCommon
{
public:
    ThreadCommon (Thread::ThreadAttachStatus, Thread *);
    ~ThreadCommon ();

    Action *currentAction () const;
    void pushAction (Action *);
    void popAction ();
    void purgeAction (Action *);
    
    Thread::ThreadStatus threadStat;
    Thread::ThreadAttachStatus threadAstat;
    Uid threadId;
    Action *action;
    BasicList<Action, Uid> pastActions;	    
    Thread *thread;
    Mutex *mutex;
    
    void *arg;
    void *result;
    void * (*threadBody)(void*);
};

#endif
