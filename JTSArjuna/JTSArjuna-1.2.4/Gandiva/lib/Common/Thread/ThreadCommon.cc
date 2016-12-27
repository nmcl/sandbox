/*
 * Copyright (C) 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadCommon.cc,v 1.4 1998/01/16 10:27:55 nmcl Exp $
 */

#include <os/iostream.h>

#ifndef COMMON_THREAD_THREADCOMMON_H_
#  include <Common/Thread/ThreadCommon.h>
#endif

ThreadCommon::ThreadCommon ( Thread::ThreadAttachStatus aStat, Thread *th )
			   : threadStat(Thread::THR_IS_CREATED),
			     threadAstat(aStat),
			     threadId(),
			     action(0),
			     thread(th),
			     mutex(Mutex::create()),
			     arg(0),
			     result(0),
			     threadBody(0)
{
    Resource::ref(mutex);
}

ThreadCommon::~ThreadCommon ()
{
    if (result && (threadAstat == Thread::THR_IS_ATTACHED))
	delete result;

    Resource::unref(mutex);
}

Action *ThreadCommon::currentAction () const
{
    Action *act = 0;

    if (mutex && (mutex->lock() == Mutex::MX_DONE))
    {
	act = action;
	mutex->unlock();
    }
    
    return act;
}

void ThreadCommon::pushAction ( Action *act )
{
    if (mutex && (mutex->lock() == Mutex::MX_DONE))
    {
	action = act;

	mutex->unlock();
    }    
}

void ThreadCommon::popAction ()
{
    if (mutex && (mutex->lock() == Mutex::MX_DONE))
    {
	action = NULL;

	mutex->unlock();
    }
}

void ThreadCommon::purgeAction (Action* act)
{
    if (mutex && (mutex->lock() == Mutex::MX_DONE))
    {
	if (action == act)
	    action = NULL;

	mutex->unlock();
    }
}
