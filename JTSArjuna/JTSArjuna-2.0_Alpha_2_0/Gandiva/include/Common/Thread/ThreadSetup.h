/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadSetup.h,v 1.1 1998/01/12 12:56:13 nmcl Exp $
 */

#ifndef COMMON_THREAD_THREADSETUP_H_
#define COMMON_THREAD_THREADSETUP_H_

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

/*
 * Some default values to note are:
 *
 * priority: -1, a default priority will be chosen by the thread package.
 * stackSize: 0, a default size will be chosen by the thread package.
 *
 * These values may be different from the static values in the Thread
 * base class (Thread::defaultPriority, and Thread::defaultStackSize), which
 * are also specified on a per thread package basis, but which we have chosen
 * as a "useful" default value for most threads. Relying upon the thread
 * package to choose default values may result in smaller, faster programs,
 * but could also result in unforseen errors. Be warned.
 */

class ThreadSetup
{
public:
    ThreadSetup ();
    ThreadSetup (const ThreadSetup&);
    ~ThreadSetup ();

    ThreadSetup& operator= (const ThreadSetup&);
    
    Thread::ThreadStatus       startState;
    Thread::ThreadAttachStatus attachStatus;
    int                        priority;
    unsigned long              stackSize;
    Uid                        threadId;  // so we can recreate a thread with known id.
};

#endif
