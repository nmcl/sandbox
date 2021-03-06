/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PC_Thread.h,v 1.2 1998/06/01 14:16:11 nmcl Exp $
 */

#ifndef COMMON_THREAD_PC_THREAD_PC_THREAD_H_
#define COMMON_THREAD_PC_THREAD_PC_THREAD_H_

#ifndef OS_PTHREAD_PTHREAD_H_    
#  include <os/pthread/pthread.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BASE_THREAD_H_
#  include <Base/Thread.h>
#endif

#ifndef BASE_MUTEX_H_
#  include <Base/Mutex.h>
#endif

class ThreadData
{
public:
    ThreadData (Thread::ThreadAttachStatus, Thread*);
    ~ThreadData ();

    ThreadCommon commonData;

    Boolean onOverflow;		        /* on overflow list */
    pthread_t sysId;		        /* system internal id */
    pthread_attr_t  attribute;
    pthread_mutex_t suspendLock;	/* used for suspend/resume */
    pthread_mutex_t sleepLock;
    pthread_cond_t sleepCondition;    
    pthread_mutex_t waitLock;		/* used in join */
    pthread_cond_t  waiters;
    Boolean isSuspending;
    
    static void interrupt (...);        /* for issuing kill signal to thread */

    static pthread_key_t threadKey;
    static BasicList<Thread, Uid>* overflowList;
};

#endif
