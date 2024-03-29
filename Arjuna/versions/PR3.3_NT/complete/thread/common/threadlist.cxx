/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadList.cc,v 1.2 1995/07/03 14:30:24 ngdp Exp $
 */


#ifndef THREAD_H_
#  include <Thread/Thread.h>
#endif

#ifndef THREADLIST_H_
#  include <Thread/ThreadList.h>
#endif

#ifndef THREADID_H_
#  include <Thread/ThreadID.h>
#endif

#ifndef THREADMUTEX_H_
#  include <Thread/ThreadMutex.h>
#endif

ThreadListEntry::ThreadListEntry ( ThreadListEntry *old, Thread *th )
				 : next(old),
				   theThread(th)
{
}

ThreadListEntry::~ThreadListEntry ()
{
}

ThreadList::ThreadList () : lock(ThreadMutex::create(TRUE)), entryCount(0)
{
    for (int i = 0; i < LISTMAX; i++)
	head[i] = 0;
}

/*
 * Note we do not actually delete the thread itelf. Furthermore since pop()
 * deletes the list entry we do not have much to do here.
 */

ThreadList::~ThreadList ()
{
    Thread *te;

    if (lock)
	lock->lock();
    
    while ((te = orderedPop()))
    {
    }

    if (lock)
    {
	lock->unlock();
	delete lock;
    }
}

Boolean ThreadList::add ( Thread *th )
{
    if (th && lock)
    {
	if (lock->lock())
	{
	    unsigned int entry = th->threadId().hash() % LISTMAX;
	    ThreadListEntry *te = new ThreadListEntry(head[entry], th);
	    
	    head[entry] = te;
	    entryCount++;
	    
	    lock->unlock();
	    return TRUE;
	}
    }
    
    return FALSE;
}

/*
 * Insert is like add execpt that it only adds once. i.e. if the thread
 * already exists it returns FALSE;
 */

Boolean ThreadList::insert ( Thread *th )
{
    if (th && lock)
    {
	if (lock->lock())
	{
	    unsigned int entry = th->threadId().hash() % LISTMAX;
	    ThreadListEntry *te = head[entry];
	
	    while (te)
	    {
		/* exists already ? */
		
		if (te->theThread == th)
		{
		    lock->unlock();
		    
		    return FALSE;
		}
		
		te = te->next;
	    }

	    /* insert if not found */
	    
	    te = new ThreadListEntry(head[entry], th);
	    
	    head[entry] = te;
	    entryCount++;
	    
	    lock->unlock();
	    return TRUE;
	}
    }
    
    return FALSE;
}


Boolean ThreadList::remove ( Thread *findMe )
{
    if (findMe && lock)
    {
	if (lock->lock())
	{
	    unsigned int entry = findMe->threadId().hash() % LISTMAX;
	    ThreadListEntry *te = head[entry];
	    ThreadListEntry *old = 0;
	
	    while (te)
	    {
		if (te->theThread == findMe)
		{
		    if (old == 0)
			head[entry] = te->next;
		    else
			old->next = te->next;
		    
		    delete te;
		    entryCount--;
		    
		    lock->unlock();
		    
		    return TRUE;
		}
	    
		old = te;
		te = te->next;
	    }

	    lock->unlock();
	}
    }
    
    return FALSE;
}

Thread *ThreadList::lookFor ( const ThreadID& findMe )
{
    if (lock)
    {
	if (lock->lock())
	{
	    unsigned int entry = findMe.hash() % LISTMAX;
	    ThreadListEntry *te = head[entry];
	    Thread *th = 0;
	    
	    while (te)
	    {
		th = te->theThread;
		if (th && th->threadId() == findMe)
		{
		    lock->unlock();
		    return th;
		}
	    
		te = te->next;
	    }
	    
	    lock->unlock();
	}
    }
    
    return 0;
}

Thread *ThreadList::orderedPop ()
{
    Thread *th = 0;
	    
    if (lock)
    {
	if (lock->lock())
	{
	    for (int i = 0; i < LISTMAX; i++)
	    {
		if (head[i])
		{
		    ThreadListEntry *te = head[i];
		    
		    head[i] = te->next;
		    th = te->theThread;
		    entryCount--;
		    
		    delete te;
		}
	    }

	    lock->unlock();
	}
    }
    
    return th;
}
