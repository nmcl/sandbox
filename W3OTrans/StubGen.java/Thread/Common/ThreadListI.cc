/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadListI.cc,v 1.1 1995/06/26 10:45:44 ngdp Exp $
 */


#ifndef THREADMUTEX_H_
#  include <Thread/ThreadMutex.h>
#endif

#ifndef THREADLIST_H_
#  include <Thread/ThreadList.h>
#endif

#ifndef THREADLISTI_H_
#  include <Thread/ThreadListI.h>
#endif

ThreadListI::ThreadListI ( ThreadList *list )
			 : nextEnt(0),
			   theList(list),
			   lastBucket(0)
{
}

ThreadListI::~ThreadListI ()
{
}

Thread *ThreadListI::operator() ()
{
    Thread *th = 0;

    if (theList && theList->lock)
    {
	if (theList->lock->lock())
	{
	    if (nextEnt == 0)
	    {
		while ((lastBucket < LISTMAX) &&
		       ((nextEnt = theList->head[lastBucket]) == 0))
		    lastBucket++;
	    }
	    
	    if (nextEnt != 0)
	    {
		th = nextEnt->theThread;
		nextEnt = nextEnt->next;
	    }

	    theList->lock->unlock();
	}
    }
    
    return th;
}
