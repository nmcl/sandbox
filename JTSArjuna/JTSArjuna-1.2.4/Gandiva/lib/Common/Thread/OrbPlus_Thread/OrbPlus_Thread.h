/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPlus_Thread.h,v 1.4 1998/06/01 14:13:34 nmcl Exp $
 */

#ifndef COMMON_ORBPLUS_THREAD_ORBPLUS_THREAD_H_
#define COMMON_ORBPLUS_THREAD_ORBPLUS_THREAD_H_

#include <os/msd/thread.hh>
#include <os/msd/tsd.hh>
#include <os/msd/time.hh>
#include <os/msd/sighandler.hh>

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef COMMON_THREAD_THREADCOMMON_H_
#  include <Common/Thread/ThreadCommon.h>
#endif

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

class ThreadData
{
public:
    ThreadData (Thread::ThreadAttachStatus, Thread *);
    ~ThreadData ();
    
    ThreadCommon commonData;
    
    Boolean onOverflow;			/* on overflow list */
    MSD_Thread* sysId;			/* system internal id */
    MSD_Mutex* suspendLock;	        /* used for suspend/resume */
    MSD_Mutex* waitLock;		/* used in join */
    MSD_CondVar waiters;
    Boolean dead;
    Boolean isSuspending;
};

#endif
