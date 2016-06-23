/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadCommon.cc,v 1.1 1997/09/25 15:32:27 nmcl Exp $
 */

#ifndef THREADCOMMON_H_
#  include <Thread/ThreadCommon.h>
#endif

ThreadCommon::ThreadCommon ( Thread::ThreadAttachStatus aStat, Thread *th )
			   : threadStat(Thread::THR_IS_SUSPENDED),
			     threadAstat(aStat),
			     threadId(),
			     thread(th),
			     mutex(ThreadMutex::create()),
			     arg(0),
			     result(0),
			     threadBody(0)
{
}

ThreadCommon::~ThreadCommon ()
{
    if (result)
	delete result;
    
    if (mutex)
	delete mutex;
}
