/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Single_Thread.h,v 1.2 1998/06/01 12:56:15 nmcl Exp $
 */

#ifndef COMMON_THREAD_SINGLE_THREAD_SINGLE_THREAD_H_
#define COMMON_THREAD_SINGLE_THREAD_SINGLE_THREAD_H_

#ifndef COMMON_THREAD_THREADCOMMON_H_
#  include <Common/Thread/ThreadCommon.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

class ThreadData
{
public:
    ThreadData (Thread::ThreadAttachStatus, Thread *);
    ~ThreadData ();
    
    ThreadCommon commonData;
    Thread* lastThread;
};

#endif
