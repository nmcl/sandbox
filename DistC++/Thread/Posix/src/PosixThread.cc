/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PosixThread.cc,v 1.1 1997/09/25 15:32:56 nmcl Exp $
 */

/*
 * Implementation of threads for Posix threads
 */

#ifndef POSIXTHREAD_H_
#  include <System/PosixThread.h>
#endif

/*
 * So we can yield on Solaris!
 */

#ifdef HAVE_SOLARIS_THREADS

#ifndef SOLARISTHREAD_H_
#  include <System/SolarisThread.h>
#endif

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

#ifndef THREADCOMMON_H_
#  include <Thread/ThreadCommon.h>
#endif

#ifndef TSHASHLIST_H_
#  include <Template/TSHashList.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef PTHREAD_STACK_MIN
#  define PTHREAD_STACK_MIN 4096
#endif

#if defined(_SVID_GETTOD)
extern "C" int gettimeofday (struct timeval *);
#endif

static size_t minStackSize = 0;
static Thread *mainThread = 0;
static pthread_mutex_t globalMutex = {0};
static pthread_key_t threadKey;

static ThreadSafeHashList<Thread> overflowList(11);

struct ThreadData
{
    ThreadData (Thread::ThreadAttachStatus, Thread *);
    ~ThreadData ();
    
    ThreadCommon commonData;
    
    Boolean onOverflow;			/* on overflow list */
    pthread_t sysId;			/* system internal id */
    pthread_attr_t attribute;
    pthread_mutex_t suspendLock;	/* used for suspend/resume */
    pthread_mutex_t sleepLock;
    pthread_cond_t sleepCondition;
    pthread_mutex_t waitLock;		/* used in join */
    pthread_cond_t waiters;
};

ThreadData::ThreadData ( Thread::ThreadAttachStatus aStat,
			 Thread *th )
		       : commonData(aStat, th),
			 onOverflow(FALSE)
{
#ifndef HAVE_HPUX_PTHREADS
    sysId = 0;
    pthread_attr_init(&attribute);
    pthread_mutex_init(&suspendLock,0);
    pthread_mutex_init(&sleepLock,0);
    pthread_cond_init(&sleepCondition,0);
    pthread_mutex_init(&waitLock,0);
    pthread_cond_init(&waiters,0);    
#else
    pthread_attr_create(&attribute);
    pthread_mutex_init(&suspendLock,pthread_mutexattr_default);
    pthread_mutex_init(&sleepLock,pthread_mutexattr_default);
    pthread_cond_init(&sleepCondition,pthread_mutexcond_default);
    pthread_mutex_init(&waitLock,pthread_mutexattr_default);
    pthread_cond_init(&waiters,pthread_mutexcond_default);
#endif
}

ThreadData::~ThreadData ()
{
    pthread_mutex_destroy(&suspendLock);
    pthread_mutex_destroy(&sleepLock);
    pthread_cond_destroy(&sleepCondition);
    pthread_mutex_destroy(&waitLock);
    pthread_cond_destroy(&waiters);

#ifndef HAVE_HPUX_PTHREADS    
    pthread_attr_destroy(&attribute);
#else
    pthread_attr_delete(&attribute);
#endif    
}

/*
 * Create a thread.
 * Note we ALWAYS create the thread SUSPENDED - this ensures we can add it to
 * various lists before it gets to run
 * WARNING
 * Do not add the thread to any lists until AFTER the _create call is made
 */

#ifndef PTHREAD_CREATE_DETACHED
#  define PTHREAD_CREATE_DETACHED 1
#endif

Thread::Thread ( void * (*body)(void *), void *arg, ThreadStatus startIn,
		 ThreadAttachStatus aStat, void *stack, size_t stackSize)
{
    theThread = new ThreadData(aStat, this);

    if (aStat == THR_IS_DETACHED)
    {
#ifndef HAVE_HPUX_PTHREADS
	int dstate = PTHREAD_CREATE_DETACHED;
	pthread_attr_setdetachstate(&theThread->attribute, dstate);
#endif	
    }
     
    theThread->commonData.threadBody = body;
    theThread->commonData.arg = arg;

    if (stack != 0)
    {
	/* Given stack better be big enough */
	
	if (stackSize > minStackSize)
	{
#ifndef HAVE_HPUX_PTHREADS	    
#ifndef LINUX_THREADS
	    pthread_attr_setstackaddr(&theThread->attribute, stack);
	    pthread_attr_setstacksize(&theThread->attribute, stackSize);
#endif
#else
	    // ignore stack variable! (Should delete?)

	    pthread_attr_setstacksize(&theThread->attribute, stackSize);
#endif	    
	}
	else
	    startIn = theThread->commonData.threadStat = THR_IS_TERMINATED;
    }
    else
	if (stackSize != 0)
	{
	    if (stackSize < minStackSize)
		stackSize = minStackSize;

#ifndef HAVE_HPUX_PTHREADS	    
#ifndef LINUX_THREADS	    
	    pthread_attr_setstacksize(&theThread->attribute, stackSize);
#endif
#else
	    pthread_attr_setstacksize(&theThread->attribute, stackSize);
#endif	    
	}

    if (startIn != THR_IS_TERMINATED)
    {
#ifndef HAVE_HPUX_PTHREADS
	if (pthread_create(&(theThread->sysId), &theThread->attribute, execute, this) == 0)
#else
	if (pthread_create(&(theThread->sysId), theThread->attribute, execute, this) == 0)
#endif	    
	{
	    Thread::yield();  // give thread a chance to run
	    
#ifdef HAVE_HPUX_PTHREADS
	    /*
	     * Do the detach now.
	     */

	    (void) pthread_detach(&theThread->sysId);
#endif	    
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
	overflowList.remove(this);

    allThreads->remove(this);    
    delete theThread;
}

/*
 * Wait for thread to terminate.
 * Make sure the thread does not wait for itself
 * Since Posix threads only allow a single thread to wait for another
 * we do not use pthread_join here. Instead we use conditions
 */

Boolean Thread::join ()
{
    if ((theThread) && (!pthread_equal(theThread->sysId, pthread_self())))
    {
	pthread_mutex_lock(&theThread->waitLock);

	while (theThread->commonData.threadStat != THR_IS_TERMINATED)
	{
	    if (pthread_cond_wait(&theThread->waiters, &theThread->waitLock) != 0)
		break;
	}
	
	pthread_mutex_unlock(&theThread->waitLock);

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
	pthread_mutex_unlock(&theThread->suspendLock);
    }

    return (theThread->commonData.threadStat == THR_IS_RUNNING);
}

/*
 * There is no real suspend/resume in POSIX. Fake them here using a mutex
 * The mutex is grabbed in execute to ensure that a grab here will block
 */

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
	    pthread_mutex_lock(&th->theThread->suspendLock);
	    result = TRUE;
	}
	else
	    result = (th->theThread->commonData.threadStat == THR_IS_SUSPENDED);
    }
    
    return result;
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

#ifndef HAVE_HPUX_THREADS
    aThread = (Thread*) pthread_getspecific(threadKey);
#else
    void* tmpPtr = 0;
    
    if (pthread_getspecific(threadKey, &tmpPtr) != 0)
	return 0;

    aThread = (Thread*) tmpPtr;
#endif    
    
    if (aThread == 0)
    {
	/*
	 * Trouble - we could not locate the current thread - it
	 * may be on the overflow list. Look there
	 */

	pthread_t me = pthread_self();
	ThreadSafeHashListI<Thread> iter(overflowList);
	Thread *t = 0;

	while ((t = iter()) != 0)
	{
	    if (pthread_equal(t->theThread->sysId, me))
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
	pthread_exit(0);
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
	
	timespec to;
	pthread_mutex_lock(&currentThread->theThread->sleepLock);
	
	currentThread->theThread->commonData.threadStat = THR_IS_BLOCKED;

#ifdef _SVID_GETTOD
        gettimeofday(&tv);
#else
        gettimeofday(&tv, 0);
#endif

	unsigned int seconds = usecs/1000000;
	unsigned int msecs = usecs%1000000;
	
	to.tv_sec = tv.tv_sec + seconds;
	to.tv_nsec = (tv.tv_usec + msecs) * 1000; /* convert to nanoseconds */

	while (to.tv_nsec >= 1000000000)
	{
	    to.tv_sec++;
	    to.tv_nsec -= 1000000000;
	}

	while (TRUE)
	{
	    err = pthread_cond_timedwait(&currentThread->theThread->sleepCondition,
					 &currentThread->theThread->sleepLock, &to);
	    if (err != ETIMEDOUT)
		break;
	}
	
	currentThread->theThread->commonData.threadStat = THR_IS_RUNNING;
	pthread_mutex_unlock(&currentThread->theThread->sleepLock);
    
	return TRUE;
    }

    return FALSE;
}

/*
 * Posix threads no longer have a yield operation. Standard says to use
 * sched_yield, but that is not currently implemented in Solaris (it just
 * returns immediately). So, since pthreads on Solaris is a wrapper for
 * Solaris threads anyway, we use their yield for now!
 */

Boolean Thread::yield ()
{
#ifdef HAVE_SOLARIS_THREADS    
    (void) sched_yield(); // doesn't do anything yet, but when it does, remove next line
    thr_yield();
#else
#ifndef LINUX_THREADS    
    pthread_yield();
#endif    
#endif

    return TRUE;
}

/*
 * Used ONLY to create a fake thread for main
 */

Thread::Thread ()
{
    theThread = new ThreadData(THR_IS_ATTACHED, this);
    theThread->commonData.threadStat = THR_IS_RUNNING;
    theThread->sysId = pthread_self();
    pthread_setspecific(threadKey, this);
    
    allThreads->add(this);
}

Boolean Thread::terminate ()
{
    if (theThread->commonData.threadStat == THR_IS_TERMINATED)
	return TRUE;
    
    if (this != Thread::current())
    {
	join();

        return (theThread->commonData.threadStat == THR_IS_TERMINATED);
    }
    else
    {    
	theThread->commonData.threadStat = THR_IS_TERMINATED;
	pthread_mutex_lock(&theThread->waitLock);
	pthread_cond_broadcast(&theThread->waiters);
	pthread_mutex_unlock(&theThread->waitLock);

        return TRUE;
    }
}

/*
 * Pthreads always start running immediately. If the user wanted them created
 * suspended we do it here
 */
 
void *Thread::execute ( void *startMe )
{
    if (startMe)
    {
	Boolean runThread = TRUE;
	Thread *run = (Thread *)startMe;
	
	if (pthread_setspecific(threadKey, startMe) == ENOMEM)
	{
	    /*
	     * Out of thread specific memory - add thread onto overflow list
	     * so current() can find it the hard way
	     */

	    if (overflowList.insert(run))
		run->theThread->onOverflow = TRUE;
	}

	/*
	 * Grab suspend mutex. This ensures that if I grab again (via suspend)
	 * I will block.
	 */
	
	pthread_mutex_lock(&run->theThread->suspendLock);

	if (run->theThread->commonData.threadStat == THR_IS_SUSPENDED)
	    pthread_mutex_lock(&run->theThread->suspendLock);
	
	run->theThread->commonData.threadStat = THR_IS_RUNNING;
	if (run->theThread->commonData.threadBody)
	    run->theThread->commonData.result = run->theThread->commonData.threadBody(run->theThread->commonData.arg);
	
	run->terminate();

	pthread_mutex_unlock(&run->theThread->suspendLock);
    }

    return 0;
}

/*
 * This MUST get run while the thread system is inactive since we cannot use
 * ANY threading stuff until init is called
 */

void Thread::init ()
{
    if (mainThread == 0)
    {
	/*
	 * This ensures that we get ThreadMutex::create() from the Posix lib
	 * and not from elsewhere since it creates an unresolved ref.
	 */
	
	ThreadMutex *dummy = ThreadMutex::create();

#ifndef HAVE_HPUX_PTHREADS	
	pthread_key_create(&threadKey, 0);
#else
	pthread_keycreate(&threadKey,0);
#endif	
	
	mainThread = new Thread();
	minStackSize = PTHREAD_STACK_MIN;
	
	delete dummy;
    }
}

/*
 * Hopefully there is only the main thread left by now. But lock the semaphore
 * just in case
 */

void Thread::shutdown ()
{
    pthread_mutex_lock(&globalMutex);

    if (mainThread)
    {
	delete mainThread;
	mainThread = NULL;
    }

    pthread_mutex_unlock(&globalMutex);
}
