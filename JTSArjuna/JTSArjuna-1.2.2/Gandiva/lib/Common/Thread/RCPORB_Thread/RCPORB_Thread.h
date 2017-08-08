/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RCPORB_Thread.h,v 1.2 1998/06/01 14:21:57 nmcl Exp $
 */

/*
 * Implementation of threads for ON threads
 */

#ifndef COMMON_THREAD_RCPORB_THREAD_RCPORB_THREAD_H_
#define COMMON_THREAD_RCPORB_THREAD_RCPORB_THREAD_H_

#include <os/CORBA.ch>
#include <os/ON_init.h>
#include <os/ON_sync.h>

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
    
    Boolean onOverflow;			/* on overflow list */
    ODS_CORBA::Thread* sysId;           /* system internal id */
    ODS_CORBA::Mutex* suspendLock;	/* used for suspend/resume */
    ODS_CORBA::Mutex* waitLock;		/* used in join */
    ODS_CORBA::CondMutex* waiters;
    Boolean isSuspending;
};

#endif
