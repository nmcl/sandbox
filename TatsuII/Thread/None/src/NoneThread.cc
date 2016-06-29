/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NoneThread.cc,v 1.2 1997/06/09 19:53:36 nmcl Exp $
 */

/*
 * Implementation of thread ids for None threads
 */

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

#ifndef THREAD_H_
#  include <Thread/Thread.h>
#endif

#ifndef THREADID_H_
#  include <Thread/ThreadID.h>
#endif

#ifndef THREADLIST_H_
#  include <Thread/ThreadList.h>
#endif

static Thread *mainThread = 0;
static Thread *currentThread = 0;
static Thread *lastThread = 0;

struct ThreadData
{
    ThreadData (Thread::ThreadAttachStatus, Thread *);
    ~ThreadData ();
    
    ThreadList children;		/* child threads */
    
    Thread::ThreadStatus threadStat;	/* current status */
    Thread::ThreadAttachStatus threadAstat;
    ThreadID threadId;
    AtomicAction *action;
    Thread *thread;
    ThreadData *parentData;
    void *arg;
    void *result;
    void * (*threadBody)(void*);
};

ThreadData::ThreadData ( Thread::ThreadAttachStatus aStat, Thread *th )
		       : children(),
			 threadStat(Thread::THR_IS_SUSPENDED),
			 threadAstat(aStat),
			 threadId(),
			 action(0),
			 thread(th),
			 parentData(0),
			 arg(0),
			 result(0),
			 threadBody(0)
{
}

ThreadData::~ThreadData ()
{
}

Thread::Thread ( void * (*body)(void *), void *arg, ThreadStatus startState,
		 ThreadAttachStatus aStat, void *, size_t )
{
    theThread = new ThreadData(aStat, this);

    if (currentThread)
	theThread->action = currentThread->theThread->action;

    theThread->threadBody = body;
    theThread->arg = arg;
    
    allThreads->add(this);

    if (theThread->parentData)
	theThread->parentData->children.add(this);
	 
    if (startState == THR_IS_RUNNING)
    {
	theThread->threadStat = THR_IS_RUNNING;
	
	if (theThread->threadBody)
	    theThread->result = theThread->threadBody(theThread->arg);
	theThread->threadStat = THR_IS_TERMINATED;
    }
}

Thread::~Thread ()
{
    if (theThread)
    {
	if (theThread->threadStat != THR_IS_TERMINATED)
	{
	    if (theThread->threadStat == THR_IS_SUSPENDED)
		resume();

	    join();
	}
    }
    
    allThreads->remove(this);

    if (theThread)
	delete theThread;	
}

AtomicAction *Thread::currentAction () const
{
    if (theThread)
	return theThread->action;
    
    return 0;
}

void Thread::currentAction ( AtomicAction *act )
{
    if (theThread)
	theThread->action = act;
}

Boolean Thread::join ()
{
    /* Cannot wait for oneself */
    
    if (currentThread != this)
    {
	if (theThread && theThread->threadStat != THR_IS_TERMINATED)
	    resume();
	    
	return TRUE;
    }
    
    return FALSE;  
}

Boolean Thread::resume ()
{
    theThread->threadStat = THR_IS_RUNNING;
    lastThread = currentThread;
    currentThread = this;

    if (theThread->threadBody)
    {
	theThread->result = theThread->threadBody(theThread->arg);
    }
    
    theThread->threadStat = THR_IS_TERMINATED;    
    currentThread = lastThread;
    lastThread = 0;
    return TRUE;
}

/*
 * Cannot suspend in a non-threaded environment
 */

Boolean Thread::suspend ()
{
    return FALSE;
}

Thread::ThreadStatus Thread::status () const
{
    return theThread->threadStat;
}

const ThreadID &Thread::threadId () const
{
    return theThread->threadId;
}

/*
 * This should not return 0 or their will be trouble
 */

Thread *Thread::current ()
{
    return currentThread;
}

void Thread::current ( ThreadID& theId )
{
    if (currentThread)
	theId = currentThread->theThread->threadId;
}

void Thread::exit ()
{
}

Boolean Thread::join ( const ThreadID& toJoin )
{
    Thread *th = allThreads->lookFor(toJoin);

    if (th)
	return th->join();
    
    return FALSE;
}

Boolean Thread::sleep ( unsigned int usecs )
{
    if (currentThread)
    {
	currentThread->theThread->threadStat = THR_IS_BLOCKED;
    
	usleep(usecs);
    
	currentThread->theThread->threadStat = THR_IS_RUNNING;
    
	return TRUE;
    }

    return FALSE;
}

Boolean Thread::yield ()
{
    return FALSE;
}

Thread::Thread ()
{
    theThread = new ThreadData(THR_IS_ATTACHED, this);
    theThread->threadStat = THR_IS_RUNNING;

    allThreads->add(this);
   
}

void *Thread::execute ( void *startMe )
{
    if (startMe)
    {
	Thread *run = (Thread *)startMe;
	
	run->theThread->threadStat = THR_IS_RUNNING;
	if (run->theThread->threadBody)
	    run->theThread->result = run->theThread->threadBody(run->theThread->arg);
	run->theThread->threadStat = THR_IS_TERMINATED;
    }

    return 0;
}

void Thread::init ()
{
    if (mainThread == 0)
	currentThread = mainThread = new Thread();
}

void Thread::shutdown ()
{
    if (mainThread != 0)
	delete mainThread;

    mainThread = 0;
    
}
