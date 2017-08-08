/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NT_Thread.h,v 1.1 1998/01/12 13:07:11 nmcl Exp $
 */

#ifndef COMMON_THREAD_NT_THREAD_NT_THREAD_H_
#define COMMON_THREAD_NT_THREAD_NT_THREAD_H_

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef OS_SYS_TYPES_H_
#  include <os/sys/types.h>
#endif

#ifndef OS_WINDOWS_H_
#  include <os/windows.h>
#endif

#ifndef OS_PROCESS_H_
#  include <os/process.h>
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

class ostream;

#ifndef COMMON_THREAD_THREADCOMMON_H_
#  include <Common/Thread/ThreadCommon.h>
#endif

class ThreadData
{
public:
    ThreadData (Thread::ThreadAttachStatus, Thread*);
    ~ThreadData ();

    ThreadCommon commonData;

    HANDLE sysId;
    HANDLE waitLock;
    DWORD mid;
    Boolean onOverflow;

    static BasicList<Thread, Uid> *overflowList;
};

#endif
