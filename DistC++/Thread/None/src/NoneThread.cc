/*
 * Copyright (C) 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NoneThread.cc,v 1.2 1998/11/20 08:28:58 nmcl Exp $
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

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

// Warning: For g++ this MUST be included before Thread.h

#ifndef TSHASHLIST_H_
#  include <Template/TSHashList.h>
#endif

#ifndef THREAD_H_
#  include <Thread/Thread.h>
#endif

#ifndef THREADCOMMON_H_
#  include <Thread/ThreadCommon.h>
#endif

static Thread *mainThread = 0;
static Thread *currentThread = 0;
static Thread *lastThread = 0;

struct ThreadData
{
    ThreadData (Thread::ThreadAttachStatus, Thread *);
    ~ThreadData ();
    
    ThreadCommon commonData;
};

ThreadData::ThreadData ( Thread::ThreadAttachStatus aStat, Thread *th )
		       : commonData(aStat, th)
{
}

ThreadData::~ThreadData ()
{
}

Thread::Thread ( void * (*body)(void *), void *arg, ThreadStatus startState,
		 ThreadAttachStatus aStat, void *, size_t )
{
    theThread = new ThreadData(aStat, this);

    theThread->commonData.threadBody = body;
    theThread->commonData.arg = arg;
    
    allThreads->add(this);

    if (startState == THR_IS_RUNNING)
	resume();
}

Thread::~Thread ()
{
    if (theThread)
    {
	if (theThread->commonData.threadStat != THR_IS_TERMINATED)
	{
	    if (theThread->commonData.threadStat == THR_IS_SUSPENDED)
		resume();

	    join();
	}
    }
    
    allThreads->remove(this);

    if (theThread)
	delete theThread;	
}

Boolean Thread::join ()
{
    /* Cannot wait for oneself */
    
    if (currentThread != this)
    {
	if (theThread && theThread->commonData.threadStat != THR_IS_TERMINATED)
	    resume();
	    
	return TRUE;
    }
    
    return FALSE;  
}

Boolean Thread::resume ()
{
    theThread->commonData.threadStat = THR_IS_RUNNING;
    lastThread = currentThread;
    currentThread = this;

    if (theThread->commonData.threadBody)
    {
	theThread->commonData.result = theThread->commonData.threadBody(theThread->commonData.arg);
    }
    
    theThread->commonData.threadStat = THR_IS_TERMINATED;    
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
    return theThread->commonData.threadStat;
}

const Uid &Thread::get_uid () const
{
    return theThread->commonData.threadId;
}

/*
 * This should not return 0 or their will be trouble
 */

Thread *Thread::current ()
{
    return currentThread;
}

void Thread::current ( Uid& theId )
{
    if (currentThread)
	theId = currentThread->theThread->commonData.threadId;
}

void Thread::exit ()
{
}

Boolean Thread::join ( const Uid& toJoin )
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
	currentThread->theThread->commonData.threadStat = THR_IS_BLOCKED;
    
	usleep(usecs);
    
	currentThread->theThread->commonData.threadStat = THR_IS_RUNNING;
    
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
    theThread->commonData.threadStat = THR_IS_RUNNING;

    allThreads->add(this);
   
}

void *Thread::execute ( void *startMe )
{
    if (startMe)
    {
	Thread *run = (Thread *)startMe;
	
	run->theThread->commonData.threadStat = THR_IS_RUNNING;
	if (run->theThread->commonData.threadBody)
	    run->theThread->commonData.result = run->theThread->commonData.threadBody(run->theThread->commonData.arg);
	run->theThread->commonData.threadStat = THR_IS_TERMINATED;
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
