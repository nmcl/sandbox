/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HPPosix_Thread.cc,v 1.8 1998/08/29 10:36:38 nmcl Exp $
 */

#include <os/dce/cma_px.h>

#ifndef OS_SIGNAL_H_
#  include <os/signal.h>
#endif

#ifndef OS_SYS_TIME_H_
#  include <os/sys/time.h>
#endif

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef COMMON_THREAD_HPPOSIX_THREAD_HPPOSIX_THREAD_H_
#  include <Common/Thread/HPPosix_Thread/HPPosix_Thread.h>
#endif

#ifndef COMMON_THREAD_THREADSETUP_H_
#  include <Common/Thread/ThreadSetup.h>
#endif

#ifndef CONTROL_THREADCONTROL_H_
#  include <Control/ThreadControl.h>
#endif

#ifndef TEMPLATE_HASHEDLIST_H_
#  include <Template/HashedList.h>
#endif

#ifndef TEMPLATE_BASICLISTITERATOR_H_
#  include <Template/BasicListIterator.h>
#endif

#ifndef PTHREAD_STACK_MIN
#  define PTHREAD_STACK_MIN 2048
#endif

const int Thread::MaxPriority = 127;
const int Thread::MinPriority = 0;
const int Thread::defaultPriority = Thread::MinPriority+10;
const unsigned long Thread::defaultStackSize = PTHREAD_STACK_MIN*10;

BasicList<Thread, Uid>* ThreadData::overflowList = 0;
pthread_key_t ThreadData::threadKey = 0;

ThreadData::ThreadData (Thread::ThreadAttachStatus aStat,
			Thread* th)
		       : commonData(aStat, th),
			 onOverflow(FALSE),
			 isSuspending(FALSE)
{
    pthread_mutex_init(&suspendLock, pthread_mutexattr_default);
    pthread_mutex_init(&sleepLock, pthread_mutexattr_default);
    pthread_cond_init(&sleepCondition, pthread_condattr_default);
    pthread_mutex_init(&waitLock, pthread_mutexattr_default);
    pthread_cond_init(&waiters, pthread_condattr_default);
}

ThreadData::~ThreadData ()
{
    pthread_mutex_destroy(&suspendLock);
    pthread_mutex_destroy(&sleepLock);
    pthread_cond_destroy(&sleepCondition);
    pthread_mutex_destroy(&waitLock);
    pthread_cond_destroy(&waiters);
}

/*
 * Typically used only to create a fake thread for main.
 */

Thread::Thread (ThreadAttachStatus aStatus)
	       : _theThread(NULL),
		 _childList(NULL),
		 _cleanupList(NULL)		 
{
    _theThread = new ThreadData(aStatus, this);
    _theThread->commonData.threadStat = THR_IS_RUNNING;
    _theThread->sysId = pthread_self();
    pthread_setspecific(ThreadData::threadKey, this);

    pthread_setprio(pthread_self(), Thread::defaultPriority);
    pthread_setcancel(CANCEL_ON);
    
    pthread_mutex_lock(&_theThread->suspendLock);
    
    if (_mutex->lock() == Mutex::MX_DONE)
    {
        list().insert(this, uid());
	_mutex->unlock();
    }
}

/*
 * Create a thread.
 * If the thread is detached then we do not inherit the action status
 * nor do we mark the thread as being a child of the creating thread
 * Note we ALWAYS create the thread SUSPENDED - this ensures we can add it to
 * various lists before it gets to run
 * WARNING
 * Do not add the thread to any lists until AFTER the _create call is made
 */

#ifndef PTHREAD_CREATE_DETACHED
#  define PTHREAD_CREATE_DETACHED 1
#endif

Thread::Thread (void* (*body)(void*), void* arg, ThreadSetup* setup)
	       : _theThread(NULL),
		 _childList(NULL),
		 _cleanupList(NULL)		 
{
    Thread::ThreadAttachStatus aStat = THR_IS_ATTACHED;
    int prio = Thread::defaultPriority;
    unsigned long stackSize = Thread::defaultStackSize;
    Thread* currt = Thread::current();

    if (setup)
    {
	aStat = setup->attachStatus;
	
	if ((setup->priority >= Thread::MinPriority) &&
	    (setup->priority <= Thread::MaxPriority))
	    prio = setup->priority;

	if (setup->stackSize > 0)
	    stackSize = setup->stackSize;
    }
    
    _theThread = new ThreadData(aStat, this);
    _theThread->commonData.threadBody = body;
    _theThread->commonData.arg = arg;

    if (setup && (setup->startState == THR_IS_RUNNING))
	_theThread->commonData.threadStat = THR_IS_RUNNING;

    pthread_attr_t attribute;

    pthread_attr_create(&attribute);

    pthread_attr_setstacksize(&attribute, (unsigned int) stackSize);
    
    if (pthread_create(&_theThread->sysId, attribute, Thread::execute, this) == 0)
    {
	pthread_setprio(_theThread->sysId, prio);
	
	if (aStat == THR_IS_DETACHED)
	{
	    pthread_detach(&_theThread->sysId);
	}
	
	Thread::yield();  // give thread a chance to run
	
	/* Cannot do before create otherwise id is not set correctly */
	
	if (_mutex->lock() == Mutex::MX_DONE)
	{
	    list().insert(this, uid());
	    _mutex->unlock();
	}

	if (currt)
	    this->currentAction(currt->currentAction());

	if (aStat != THR_IS_DETACHED)
	{
	    this->addChild();
	}
	else
	{
	    /*
	     * Reference count this thread here to avoid the race
	     * condition of a creator running and then unreferencing
	     * a thread before it gets a chance to reference itself.
	     */
	    
	    Resource::ref(this);
	}
	
	/* If thread should be running make it so */

	if (setup && (setup->startState == THR_IS_RUNNING))
	    this->run();
    }
    else
    {
	/* Thread create failed - mark as dead */
	    
	_theThread->commonData.threadStat = THR_IS_TERMINATED;
    }

    pthread_attr_delete(&attribute);
}

/*
 * Tidy things up before we terminate thread. Because threads
 * are reference counted it should not be possible to delete
 * a thread object which represents a running thread.
 */

Thread::~Thread ()
{
    Boolean currentThread = (Thread::current() == this);

    if ((this->status() != THR_IS_CREATED) &&
	(this->status() != THR_IS_TERMINATED))
	Thread::terminate();

    if (_mutex->lock() == Mutex::MX_DONE)
    {
        list().remove(this, uid());
	
	if (_theThread->onOverflow)
	    ThreadData::overflowList->remove(this);
	
	_mutex->unlock();
    }

    if (_theThread)
    {
	if (_theThread->commonData.threadAstat == THR_IS_ATTACHED)
	{
	    pthread_join(_theThread->sysId, 0);
	}

    	delete _theThread;
    }

    if (currentThread)
	pthread_exit(0);
}

/*
 * Action specific methods.
 */

Action* Thread::currentAction ()
{
    if (_theThread)
	return _theThread->commonData.currentAction();

    return 0;
}

void Thread::currentAction (Action* act)
{
    if (_theThread)
    {
	_theThread->commonData.pushAction(act);
    }
}

/*
 * Wait for thread to terminate.
 * Make sure the thread does not wait for itself
 * Since Posix threads only allow a single thread to wait for another
 * we do not use pthread_join here. Instead we use conditions
 */

void* Thread::wait (Thread* toWait, long waittime, Boolean& timeout)
{
    void* result = NULL;

    timeout = FALSE;
    
    if (toWait)
    {
	Thread* currt = Thread::current();

	if (currt->_theThread->isSuspending)
	    currt->suspend();

	if (toWait->_theThread && !pthread_equal(toWait->_theThread->sysId, pthread_self()))
	{
	    int err = 0;
	    timeval tv;
	    timespec to;
	    
#ifdef HAS_BSD_GETTIMEOFDAY
	    ::gettimeofday(&tv, NULL);
#else
	    ::gettimeofday(&tv);
#endif	

	    unsigned int seconds = waittime/1000000;
	    unsigned int usecs = waittime%1000000;
	
	    to.tv_sec = tv.tv_sec + seconds;
	    to.tv_nsec = (tv.tv_usec + usecs) * 1000; /* convert to nanoseconds */

	    while (to.tv_nsec >= 1000000000)
	    {
		to.tv_sec++;
		to.tv_nsec -= 1000000000;
	    }
	    
	    currt->_theThread->commonData.threadStat = THR_IS_WAITING;
	    
	    pthread_mutex_lock(&toWait->_theThread->waitLock);
	    
	    /*
	     * Just because a condition is signalled does not mean that
	     * the 'condition state' has been set correctly. We may have
	     * to loop.
	     */

	    while (toWait->_theThread->commonData.threadStat != THR_IS_TERMINATED)
	    {
		timeout = (pthread_cond_timedwait(&toWait->_theThread->waiters,
						  &toWait->_theThread->waitLock, &to) == ETIMEDOUT);

		if (timeout)
		    break;
	    }

	    pthread_mutex_unlock(&toWait->_theThread->waitLock);

	    currt->_theThread->commonData.threadStat = THR_IS_RUNNING;

	    if (toWait->_theThread->commonData.threadStat == THR_IS_TERMINATED)
		result = toWait->_theThread->commonData.result;
	}
    }

    return result;
}

void* Thread::wait (Thread* toWait)
{
    void* result = 0;
    
    if (toWait)
    {
	Thread* currt = Thread::current();

	if (currt->_theThread->isSuspending)
	    currt->suspend();

	if (toWait->_theThread && !pthread_equal(toWait->_theThread->sysId, pthread_self()))
	{
	    currt->_theThread->commonData.threadStat = THR_IS_WAITING;
	    
	    pthread_mutex_lock(&toWait->_theThread->waitLock);
	    
	    /*
	     * Just because a condition is signalled does not mean that
	     * the 'condition state' has been set correctly. We may have
	     * to loop.
	     */

	    while (toWait->_theThread->commonData.threadStat != THR_IS_TERMINATED)
	    {
		if (pthread_cond_wait(&toWait->_theThread->waiters, &toWait->_theThread->waitLock) != 0)
		    break;
	    }

	    pthread_mutex_unlock(&toWait->_theThread->waitLock);

	    currt->_theThread->commonData.threadStat = THR_IS_RUNNING;

	    if (toWait->_theThread->commonData.threadStat == THR_IS_TERMINATED)
		result = toWait->_theThread->commonData.result;
	}
    }

    return result;
}

/*
 * There is no real suspend/resume in POSIX. Fake them here using a mutex
 * The mutex is grabbed in execute to ensure that a grab here will block
 */

Boolean Thread::resume ()
{
    if (_theThread->commonData.threadStat == THR_IS_SUSPENDED)
    {
	_theThread->commonData.threadStat = THR_IS_RUNNING;	    

	if (!_theThread->isSuspending)
	{
	    pthread_mutex_unlock(&_theThread->suspendLock);
	}
	else
	    _theThread->isSuspending = FALSE;
    }

    return (_theThread->commonData.threadStat == THR_IS_RUNNING);
}

Boolean Thread::suspend ()
{
    /*
     * Make sure only the current thread can suspend itself in Posix.
     * If we aren't the current thread then set its status so it will
     * suspend itself later.
     */

    if (!pthread_equal(_theThread->sysId, pthread_self()))
    {
	Thread* currt = Thread::current();

	currt->_theThread->isSuspending = TRUE;
	currt->_theThread->commonData.threadStat = THR_IS_SUSPENDED;

	return TRUE;
    }

    Boolean result = FALSE;

    if (_theThread->commonData.threadStat == THR_IS_RUNNING)
    {
	_theThread->commonData.threadStat = THR_IS_SUSPENDED;
	pthread_mutex_lock(&_theThread->suspendLock);
        result = TRUE;
    }
    else
	result = (_theThread->commonData.threadStat == THR_IS_SUSPENDED);

    return result;
}

Boolean Thread::kill ()
{
    if (_theThread->commonData.threadStat == THR_IS_TERMINATED)
	return FALSE;

    killChildren();

    /*
     * Cannot call terminate, as that will force
     * us to wait for this thread to die!
     */

    cleanup();

    _theThread->commonData.threadStat = THR_IS_TERMINATED;

    /*
     * Now wake up any threads waiting for this thread
     * to terminate.
     */

    pthread_mutex_lock(&_theThread->waitLock);
    pthread_cond_broadcast(&_theThread->waiters);
    pthread_mutex_unlock(&_theThread->waitLock);
    pthread_mutex_unlock(&_theThread->suspendLock);

    /*
     * HPPosix threads do not support pthread_kill. So, we send a cancel
     * message to the thread. The thread should die eventually, unless
     * the programmer has disabled cancellations.
     */

    pthread_cancel(_theThread->sysId);

    return TRUE;
}

Boolean Thread::run ()
{
    if (_theThread->commonData.threadStat != THR_IS_CREATED)
	return FALSE;

    /*
     * There should be no need to change the thread's status
     * as it will be done automatically when it runs.
     */

    pthread_mutex_unlock(&_theThread->suspendLock);

    /*
     * If non-preemptive scheduling, then we have to change the
     * status here. Otherwise, it may have already been changed
     * from THR_IS_CREATED to something else, i.e., we can't just
     * make it THR_IS_RUNNING in case the thread is actually now
     * suspended!
     */

    if (_theThread->commonData.threadStat == THR_IS_CREATED)
	_theThread->commonData.threadStat = THR_IS_RUNNING;
    
    return TRUE;
}

Thread::ThreadStatus Thread::status () const
{
    return _theThread->commonData.threadStat;
}

const Uid& Thread::uid () const
{
    return _theThread->commonData.threadId;
}

Thread* Thread::current ()
{
    if (!_initialized)
	Thread::initialize();

    any_t arg = (any_t) 0;
    
    if (pthread_getspecific(ThreadData::threadKey, &arg) != 0)  // error!!
	return NULL;

    Thread* aThread = (Thread*) arg;
    
    if (aThread == 0)
    {
	/*
	 * Trouble - we could not locate the current thread - it
	 * may be on the overflow list. Look there
	 */

	if (Thread::_mutex->lock() == Mutex::MX_DONE)
	{
	    pthread_t me = pthread_self();	    
	    BasicListIterator<Thread, Uid> iter(*ThreadData::overflowList);
	    Thread* t = (Thread*) 0;

	    while ((t = iter()) != 0)
	    {
		if (pthread_equal(t->_theThread->sysId, me))
		{
		    aThread = t;
		    break;
		}
	    }

	    Thread::_mutex->unlock();
	}
    }
    
    return aThread;
}

Thread* Thread::find (const Uid& id)
{
    if (!_initialized)
        Thread::initialize();

    Thread* t = (Thread*) 0;

    if (Thread::_mutex->lock() == Mutex::MX_DONE)
    {
        const BasicList<Thread, Uid>* theList = list().find(id);

	if (theList)
	    t = theList->find(id);

	Thread::_mutex->unlock();
    }

    return t;
}

void Thread::exit ()
{
    Thread* th = Thread::current();

    if (th && th->_theThread)
    {
	th->terminate();
	th->_theThread->commonData.threadStat = THR_IS_TERMINATED;
	pthread_exit(0);
    }
}

Boolean Thread::sleep (unsigned long sleepTime)
{
    Thread* currentThread = Thread::current();
    Boolean result = FALSE;

    if (currentThread)
    {
	if (currentThread->_theThread->isSuspending)
	    currentThread->suspend();
	
	int err = 0;
	timeval tv;	
	timespec to;

	pthread_mutex_lock(&currentThread->_theThread->sleepLock);

	currentThread->_theThread->commonData.threadStat = THR_IS_BLOCKED;

#ifndef SOLARIS2    
	::gettimeofday(&tv, NULL);
#else
	::gettimeofday(&tv);
#endif    

	unsigned int seconds = (unsigned int) sleepTime/1000000;
	unsigned int usecs = (unsigned int) sleepTime%1000000;
	
	to.tv_sec = tv.tv_sec + seconds;
	to.tv_nsec = (tv.tv_usec + usecs) * 1000; /* convert to nanoseconds */

	while (to.tv_nsec >= 1000000000)
	{
	    to.tv_sec++;
	    to.tv_nsec -= 1000000000;
	}

	while (TRUE)
	{
	    err = pthread_cond_timedwait(&currentThread->_theThread->sleepCondition,
					 &currentThread->_theThread->sleepLock, &to);

	    /*
	     * If we are killed while "asleep" we need to signal this as a bad
	     * return value.
	     */
	    
	    if (err == ETIMEDOUT)
		result = TRUE;
	    
	    break;
	}

	currentThread->_theThread->commonData.threadStat = THR_IS_RUNNING;
	pthread_mutex_unlock(&currentThread->_theThread->sleepLock);
    }

    return result;
}

Boolean Thread::yield ()
{
    if (!_initialized)
	Thread::initialize();

    Thread* currt = Thread::current();

    if (currt->_theThread->isSuspending)
	currt->suspend();
    
    pthread_yield();
    
    return TRUE;
}

Boolean Thread::terminate ()
{
    if (_theThread->commonData.threadStat == THR_IS_TERMINATED)
	return TRUE;

    if (Thread::current() != this)
    {
	Thread::wait(this);

	return (_theThread->commonData.threadStat == THR_IS_TERMINATED);
    }
    else
    {
	Thread::waitForAllChildren();
	cleanup();
	
	_theThread->commonData.threadStat = THR_IS_TERMINATED;

	/*
	 * Now wake up any threads waiting for this thread
	 * to terminate.
	 */

	pthread_mutex_lock(&_theThread->waitLock);
	pthread_cond_broadcast(&_theThread->waiters);
	pthread_mutex_unlock(&_theThread->waitLock);
	pthread_mutex_unlock(&_theThread->suspendLock);

	pthread_yield();	

	return TRUE;
    }
}

/*
 * Pthreads always start running immediately. If the user wanted them created
 * suspended we do it here
 */

void* Thread::execute (void* startMe)
{
    if (startMe)
    {
	Thread* run = (Thread*) startMe;

	/*
	 * Make sure cancellations are allowed, so we can try to
	 * kill the thread if required.
	 */
	
	pthread_setcancel(CANCEL_ON);
	
	/*
	 * We're assuming no other error type is returned here!
	 */
	
	if (pthread_setspecific(ThreadData::threadKey, startMe) == ENOMEM)
	{
	    /*
	     * Out of thread specific memory - add thread onto overflow list
	     * so current() can find it the hard way
	     */

	    if (Thread::_mutex->lock() == Mutex::MX_DONE)
	    {
		ThreadData::overflowList->insert(run);
		run->_theThread->onOverflow = TRUE;
	    }

	    Thread::_mutex->unlock();
	}
	
	/*
	 * Grab suspend mutex. This ensures that if I grab again (via suspend)
	 * I will block.
	 */

	pthread_mutex_lock(&run->_theThread->suspendLock);

	if (run->_theThread->commonData.threadStat == THR_IS_CREATED)
	{
	    pthread_mutex_lock(&run->_theThread->suspendLock);
	}
	
	run->_theThread->commonData.threadStat = THR_IS_RUNNING;
	
	if (run->_theThread->commonData.threadBody)
	    run->_theThread->commonData.result = run->_theThread->commonData.threadBody(run->_theThread->commonData.arg);

	Boolean detached = (run->_theThread->commonData.threadAstat == THR_IS_DETACHED);

	run->terminate();

	pthread_yield();

	if (detached)
	{
	    Resource::unref(run);
	}
    }

    return 0;
}

/*
 * This MUST get run while the thread system is inactive since we cannot use
 * ANY threading stuff until init is called
 */

Boolean Thread::initialize ()
{
    if (_initialized)
        return TRUE;

    _list = new HashedList<Thread, Uid>;
    _mutex = Mutex::create();
    Resource::ref(_mutex);
    
    _initialized = TRUE;

    pthread_keycreate(&ThreadData::threadKey, 0);
    ThreadData::overflowList = new BasicList<Thread, Uid>;

    Thread* mainThread = new Thread();
    Resource::ref(mainThread);
    
    return TRUE;
}

Thread* Thread::create (void* (*body)(void*), void* param, ThreadSetup* setup)
{
    if (!_initialized)
        Thread::initialize();

    return new Thread(body, param, setup);
}

Resource* Thread::control (const ClassName&)
{
    return new ThreadControl(this);
}

int Thread::priority () const
{
    if (_theThread->commonData.threadStat != Thread::THR_IS_TERMINATED)
    {
	return pthread_getprio(_theThread->sysId);
    }
    else
	return Thread::InvalidPriority;
}

Boolean ThreadControl::setPriority (int prio)
{
    if (_toControl->status() != Thread::THR_IS_TERMINATED)
    {
	if (pthread_setprio(_toControl->_theThread->sysId, prio) == 0)
	    return TRUE;
    }

    return FALSE;
}

ostream& Thread::print (ostream& strm) const
{
    strm << "[ Thread " << _theThread->commonData.threadId << " <Status: ";
    strm << _theThread->commonData.threadStat << "> <Priority: ";
    strm << priority() << "> ]";

    return strm;
}
