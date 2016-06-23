/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SolarisThread.cc,v 1.1 1997/09/25 15:33:02 nmcl Exp $
 */

/*
 * Implementation of threads for Solaris
 */

#ifndef SOLARISTHREAD_H_
#  include <System/SolarisThread.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef SYS_TIME_H_
#  include <System/sys/time.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

#ifndef THREAD_H_
#  include <Thread/Thread.h>
#endif

#ifndef THREADMUTEX_H_
#  include <Thread/ThreadMutex.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef THREADCOMMON_H_
#  include <Thread/ThreadCommon.h>
#endif

#ifndef TSHASHLIST_H_
#  include <Template/TSHashList.h>
#endif

/* Solaris 2.x (x < 5) has gettimeofday but with only one arg! */

#if defined(_SVID_GETTOD)
extern "C" int gettimeofday (struct timeval *);
#endif

static size_t minStackSize = thr_min_stack()*10;
static Thread *mainThread = 0;
static mutex_t globalMutex = DEFAULTMUTEX;
static thread_key_t threadKey;

static ThreadSafeHashList<Thread> overflowList(11);

struct ThreadData
{
    ThreadData (Thread::ThreadAttachStatus, Thread *);
    ~ThreadData ();

    ThreadCommon commonData;
    
    Boolean onOverflow;
    thread_t sysId;
    mutex_t sleepLock;
    cond_t sleepCondition;
    mutex_t waitLock;			/* used in join */
    cond_t waiters;
};

ThreadData::ThreadData ( Thread::ThreadAttachStatus aStat, Thread *th )
		       : commonData(aStat, th),
			 onOverflow(FALSE),
			 sysId(0)
{
    mutex_init(&sleepLock,0,0);
    cond_init(&sleepCondition,0,0);
    mutex_init(&waitLock,0,0);
    cond_init(&waiters,0,0);
}

ThreadData::~ThreadData ()
{
    mutex_destroy(&sleepLock);
    cond_destroy(&sleepCondition);
    mutex_destroy(&waitLock);
    cond_destroy(&waiters);
}

Thread::Thread ( void * (*body)(void *), void *arg, ThreadStatus startIn,
		 ThreadAttachStatus aStat, void *stack, size_t stackSize)
{
    theThread = new ThreadData(aStat, this);
    theThread->commonData.threadBody = body;
    theThread->commonData.arg = arg;

    if (stack != 0)
    {
	/* Given stack better be big enough */
	
	if (stackSize <= minStackSize)
	{
	    startIn = theThread->commonData.threadStat = THR_IS_TERMINATED;
	}
    }
    else
    {
	if (stackSize < minStackSize)
	    stackSize = minStackSize;
    }

    if (startIn != THR_IS_TERMINATED)
    {
	long flag = THR_SUSPENDED;

	if (aStat == THR_IS_DETACHED)
	    flag |= THR_DETACHED;
	
	if (thr_create(stack, stackSize, execute, this, flag, &(theThread->sysId)) == 0)
	{
	    /* Cannot do before create otherwise id is not set correctly */
    
	    allThreads->add(this);
 
	    /* If thread should be running make it so */
	    
	    if (startIn == THR_IS_RUNNING)
		resume();
	}
	else
	{
	    /* Thread create failed - mark as dead */
	    
	    theThread->commonData.threadStat = THR_IS_TERMINATED;
	}
    }    
}

Thread::~Thread ()
{
    terminate();
    
    if (theThread->onOverflow)
    {
	overflowList.remove(this);
	theThread->onOverflow = FALSE; 
    }

    allThreads->remove(this);

    delete theThread;
}

/*
 * Wait for this thread to terminate.
 * Make sure the thread does not wait for itself
 * Since Solaris threads only allow a single thread to wait for another
 * we do not use thr_join here. Instead we use conditions
 */

Boolean Thread::join ()
{
    if ((theThread) && (theThread->sysId != thr_self()))
    {
	mutex_lock(&theThread->waitLock);

	while (theThread->commonData.threadStat != THR_IS_TERMINATED)
	{
	    if (cond_wait(&theThread->waiters, &theThread->waitLock) != 0)
		break;
	}
	
	mutex_unlock(&theThread->waitLock);

	if (theThread->commonData.threadStat != THR_IS_TERMINATED)
	    return FALSE;
	else
	    return TRUE;
    }
    
    return FALSE;  

}

Boolean Thread::resume ()
{
    if (theThread->commonData.threadStat == THR_IS_SUSPENDED)
    {
	theThread->commonData.threadStat = THR_IS_RUNNING;
	thr_continue(theThread->sysId);
    }

    return (theThread->commonData.threadStat == THR_IS_RUNNING);
}

Thread::ThreadStatus Thread::status () const
{
    return theThread->commonData.threadStat;
}

const Uid &Thread::get_uid () const
{
    return theThread->commonData.threadId;
}

Thread *Thread::current ()
{
    Thread *aThread = 0;
    
    thr_getspecific(threadKey, (void **) &aThread);

    if (aThread == 0)
    {
	/*
	 * Trouble - we could not locate the current thread - it
	 * may be on the overflow list. Look there
	 */

	thread_t me = thr_self();
	ThreadSafeHashListI<Thread> iter(overflowList);
	Thread *t = 0;

	while ((t = iter()) != 0)
	{
	    if (t->theThread->sysId == me)
	    {    
		aThread = t;
		break;
	    }
	}
    }
    
    return aThread;
}

void Thread::current ( Uid& theId )
{
    Thread *me = Thread::current();
    
    theId = me->theThread->commonData.threadId;
}

void Thread::exit ()
{
    Thread *th = Thread::current();

    if (th && th->theThread)
    {
	th->terminate();
	th->theThread->commonData.threadStat = THR_IS_TERMINATED;
    
	thr_exit(0);
    }
}

Boolean Thread::join ( const Uid& toJoin )
{
    Thread *th = allThreads->lookFor(toJoin);

    if (th)
    {
	return th->join();
    }
    
    return FALSE;
}

Boolean Thread::sleep ( unsigned int usecs )
{
    Thread *currentThread = Thread::current();
    
    if (currentThread)
    {
	int err = 0;
	timeval tv;
	
	timestruc_t to;
	mutex_lock(&currentThread->theThread->sleepLock);
	
	currentThread->theThread->commonData.threadStat = THR_IS_BLOCKED;

#ifdef _SVID_GETTOD
        gettimeofday(&tv);
#else
        gettimeofday(&tv, 0);
#endif
	
	to.tv_sec = tv.tv_sec;
	to.tv_nsec = (tv.tv_usec + usecs) * 1000; /* convert to nanoseconds */

	while (to.tv_nsec >= 1000000000)
	{
	    to.tv_sec++;
	    to.tv_nsec -= 1000000000;
	}
	
	while (TRUE)
	{
	    err = cond_timedwait(&currentThread->theThread->sleepCondition,
				 &currentThread->theThread->sleepLock, &to);
	    if (err != EINTR)
		break;
	}
	
	currentThread->theThread->commonData.threadStat = THR_IS_RUNNING;
	mutex_unlock(&currentThread->theThread->sleepLock);
    
	return TRUE;
    }

    return FALSE;
}

/*
 * This is static so only the current thread can suspend itself
 */

Boolean Thread::suspend ()
{
    Boolean result = FALSE;
    
    Thread *th = Thread::current();

    if (th && th->theThread)
    {
	if (th->theThread->commonData.threadStat == THR_IS_RUNNING)
	{
	    th->theThread->commonData.threadStat = THR_IS_SUSPENDED;
	    thr_suspend(th->theThread->sysId);
	    th->theThread->commonData.threadStat = THR_IS_RUNNING;

	    result = TRUE;
	}
	else
	    result = (th->theThread->commonData.threadStat == THR_IS_SUSPENDED);
    }
    
    return result;
}

Boolean Thread::yield ()
{
    thr_yield();
    return TRUE;
}

/*
 * Used ONLY to create a fake thread for main
 */

Thread::Thread ()
{
    theThread = new ThreadData(THR_IS_ATTACHED, this);
    theThread->commonData.threadStat = THR_IS_RUNNING;
    theThread->sysId = thr_self();
    thr_setspecific(threadKey, this);
    
    allThreads->add(this);   
}

/*
 * This operation has to be idempotent
 */

Boolean Thread::terminate ()
{
    if (theThread->commonData.threadStat == THR_IS_TERMINATED)
	return TRUE;
    
    if (this != Thread::current())
    {
	/* terminate not called by current thread force a join */
	
	join();
    }
    else
    {
	theThread->commonData.threadStat = THR_IS_TERMINATED;

	/* Solaris man pages say you must lock the mutex prior to broadcast */
	
	mutex_lock(&theThread->waitLock);
	cond_broadcast(&theThread->waiters);
	mutex_unlock(&theThread->waitLock);

    }
    
    return (theThread->commonData.threadStat == THR_IS_TERMINATED);
}

void *Thread::execute ( void *startMe )
{
    if (startMe)
    {
	Boolean runThread = TRUE;
	Thread *run = (Thread *)startMe;
	
	if (thr_setspecific(threadKey, startMe) == ENOMEM)
	{
	    /*
	     * Out of thread specific memory - add thread onto overflow list
	     * so current() can find it the hard way
	     * Assume overflow list is already thread safe
	     */

	    if (overflowList.insert(run))
		run->theThread->onOverflow = TRUE;
	    else
	    {
		/*
		 * insert failed - Trouble
		 */

		runThread = FALSE;
	    }
	}

	if (runThread)
	{
	    run->theThread->commonData.threadStat = THR_IS_RUNNING;
	    if (run->theThread->commonData.threadBody)
		run->theThread->commonData.result = run->theThread->commonData.threadBody(run->theThread->commonData.arg);
	}
	
	run->terminate();
	
	thr_exit(0);
    }

    return 0;
}

void Thread::init ()
{
    if (mainThread == 0)
    {
	mutex_lock(&globalMutex);

	if (mainThread == 0)
	{
	    /*
	     * This ensures that we get ThreadMutex::create() from the Solaris lib
	     * and not from elsewhere since it creates an unresolved ref.
	     */
	
	    ThreadMutex *dummy = ThreadMutex::create();

	    thr_keycreate(&threadKey,0);
	    
	    mainThread = new Thread();
	    minStackSize = thr_min_stack();
	    
	    delete dummy;
	}
	
	mutex_unlock(&globalMutex);
    }
}

void Thread::shutdown ()
{
    mutex_lock(&globalMutex);

    if (mainThread)
    {
	delete mainThread;
	mainThread = 0;
    }
    
    mutex_unlock(&globalMutex);
}
