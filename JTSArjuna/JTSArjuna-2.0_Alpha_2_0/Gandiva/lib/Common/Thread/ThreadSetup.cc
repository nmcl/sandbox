/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadSetup.cc,v 1.2 1998/07/21 11:07:59 nmcl Exp $
 */

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef COMMON_THREAD_THREADSETUP_H_
#  include <Common/Thread/ThreadSetup.h>
#endif

ThreadSetup::ThreadSetup ()
			 : startState(Thread::THR_IS_SUSPENDED),
			   attachStatus(Thread::THR_IS_ATTACHED),
			   priority(Thread::InvalidPriority),
			   stackSize(0),
			   threadId(Uid::invalid())
{
}

ThreadSetup::ThreadSetup (const ThreadSetup& s)
			 : startState(s.startState),
			   attachStatus(s.attachStatus),
			   priority(s.priority),
			   stackSize(s.stackSize),
			   threadId(s.threadId)
{
}

ThreadSetup::~ThreadSetup ()
{
}

ThreadSetup& ThreadSetup::operator= (const ThreadSetup& s)
{
    if (&s == this)
	return *this;

    startState = s.startState;
    attachStatus = s.attachStatus;
    priority = s.priority;
    stackSize = s.stackSize;
    threadId = s.threadId;

    return *this;
}
