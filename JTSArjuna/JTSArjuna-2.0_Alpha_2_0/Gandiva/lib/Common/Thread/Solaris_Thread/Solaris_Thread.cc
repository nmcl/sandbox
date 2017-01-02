/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Solaris_Thread.cc,v 1.9 1998/08/29 10:36:44 nmcl Exp $
 */

#ifndef OS_SIGNAL_H_
#  include <os/signal.h>
#endif

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef OS_UNISTD_H_
#  include <os/unistd.h>
#endif

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef COMMON_THREAD_SOLARIS_THREAD_SOLARIS_THREAD_H_
#  include <Common/Thread/Solaris_Thread/Solaris_Thread.h>
#endif

#ifndef CONTROL_THREADCONTROL_H_
#  include <Control/ThreadControl.h>
#endif

#ifndef COMMON_THREAD_THREADSETUP_H_
#  include <Common/Thread/ThreadSetup.h>
#endif

#ifndef TEMPLATE_BASICLISTITERATOR_H_
#  include <Template/BasicListIterator.h>
#endif

#ifndef TEMPLATE_HASHEDLIST_H_
#  include <Template/HashedList.h>
#endif

static size_t minStackSize = thr_min_stack()*5;

const int Thread::MaxPriority = 127;
const int Thread::MinPriority = 0;
const int Thread::defaultPriority = Thread::MinPriority+10;
const unsigned long Thread::defaultStackSize = 102400;

BasicList<Thread, Uid>* ThreadData::overflowList = 0;
thread_key_t ThreadData::threadKey = 0;

ThreadData::ThreadData ( Thread::ThreadAttachStatus aStat, Thread *th )
		       : commonData(aStat, th),
			 onOverflow(FALSE),
			 sysId(0)
{
    mutex_init(&sleepLock, USYNC_THREAD, 0);
    cond_init(&sleepCondition, USYNC_THREAD, 0);
    mutex_init(&waitLock, 0, 0);
    cond_init(&waiters, 0, 0);
}

ThreadData::~ThreadData ()
{
    mutex_destroy(&sleepLock);
    cond_destroy(&sleepCondition);
    mutex_destroy(&waitLock);
    cond_destroy(&waiters);
}

#ifdef HAVE_INTSIG
int ThreadData::interrupt (int dummy DOTDOTDOT)
#else
void ThreadData::interrupt (int dummy DOTDOTDOT)
#endif    
{
    thr_setspecific(ThreadData::threadKey, NULL); // can only do this if we are current thread
    thr_exit(0);
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
    _theThread->sysId = thr_self();
    thr_setspecific(ThreadData::threadKey, this);

    thr_setprio(_theThread->sysId, Thread::defaultPriority);

    if (_mutex->lock() == Mutex::MX_DONE)
    {
        list().insert(this, uid());
	_mutex->unlock();
    }
}

Thread::Thread (void * (*body)(void *), void *arg, ThreadSetup* setup)
	       : _theThread(NULL),
		 _childList(NULL),
		 _cleanupList(NULL)    
{
    Thread::ThreadAttachStatus aStat = THR_IS_ATTACHED;
    int prio = Thread::defaultPriority;
    Thread* currt = Thread::current();
    size_t stackSize = defaultStackSize;
    
    if (setup)
    {
	aStat = setup->attachStatus;
	
	if ((setup->priority >= Thread::MinPriority) &&
	    (setup->priority <= Thread::MaxPriority))
	    prio = setup->priority;
	
	stackSize = setup->stackSize;
	
	if (stackSize < minStackSize)
	    stackSize = defaultStackSize;
    }
    
    _theThread = new ThreadData(aStat, this);
 
    _theThread->commonData.threadBody = body;
    _theThread->commonData.arg = arg;

    long flag = THR_SUSPENDED;

    if (aStat == THR_IS_DETACHED)
	flag |= THR_DETACHED;
	
    if (thr_create(NULL, stackSize, execute, this, flag, &(_theThread->sysId)) == 0)
    {
	thr_setprio(_theThread->sysId, prio);
	
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
}

Thread::~Thread ()
{
    Boolean currentThread = (Thread::current() == this);

    if (status() != THR_IS_CREATED)
	Thread::terminate();

    thr_yield();

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
	    thr_join(_theThread->sysId, 0, 0);
	
	delete _theThread;
    }
    
    if (currentThread)
	thr_exit(0);
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
 * we do not use thr_join here. Instead we use conditions
 */

void* Thread::wait (Thread* toWait, unsigned long waittime, Boolean& timeout)
{
    void* result = 0;
    
    if (toWait)
    {
	if (toWait->_theThread && (toWait->_theThread->sysId != thr_self()))
	{
	    int err = 0;
	    timeval tv;
	    timestruc_t to;

#ifndef HAS_BSD_GETTIMEOFDAY
	    gettimeofday(&tv);
#else
	    gettimeofday(&tv, NULL);
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
	    
	    Thread* currt = Thread::current();

	    currt->_theThread->commonData.threadStat = THR_IS_WAITING;
	    
	    mutex_lock(&toWait->_theThread->waitLock);
	    
	    /*
	     * Just because a condition is signalled does not mean that
	     * the 'condition state' has been set correctly. We may have
	     * to loop.
	     */

	    while (toWait->_theThread->commonData.threadStat != THR_IS_TERMINATED)
	    {
		timeout = (cond_timedwait(&currentThread->_theThread->waiters,
					  &currentThread->_theThread->waitLock, &to) == ETIMEOUT);

		if (timeout)
		    break;
	    }

	    mutex_unlock(&toWait->_theThread->waitLock);

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
	if (toWait->_theThread && (toWait->_theThread->sysId != thr_self()))
	{
	    Thread* currt = Thread::current();

	    currt->_theThread->commonData.threadStat = THR_IS_WAITING;
	    
	    mutex_lock(&toWait->_theThread->waitLock);
	    
	    /*
	     * Just because a condition is signalled does not mean that
	     * the 'condition state' has been set correctly. We may have
	     * to loop.
	     */

	    while (toWait->_theThread->commonData.threadStat != THR_IS_TERMINATED)
	    {
		if (cond_wait(&toWait->_theThread->waiters, &toWait->_theThread->waitLock) != 0)
		    break;
	    }

	    mutex_unlock(&toWait->_theThread->waitLock);

	    currt->_theThread->commonData.threadStat = THR_IS_RUNNING;

	    if (toWait->_theThread->commonData.threadStat == THR_IS_TERMINATED)
		result = toWait->_theThread->commonData.result;
	}
    }

    return result;
}

Boolean Thread::resume ()
{
    if (_theThread->commonData.threadStat == THR_IS_SUSPENDED)
    {
	_theThread->commonData.threadStat = THR_IS_RUNNING;
	thr_continue(_theThread->sysId);
    }

    return (_theThread->commonData.threadStat == THR_IS_RUNNING);
}

/*
 * Any thread can be suspended in Solaris threads.
 */

Boolean Thread::suspend ()
{
    Boolean result = FALSE;

    if (_theThread->commonData.threadStat == THR_IS_RUNNING)
    {
	_theThread->commonData.threadStat = THR_IS_SUSPENDED;
	thr_suspend(_theThread->sysId);	
	_theThread->commonData.threadStat = THR_IS_RUNNING;
	
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

    mutex_lock(&_theThread->waitLock);
    cond_broadcast(&_theThread->waiters);
    mutex_unlock(&_theThread->waitLock);

    return (Boolean) (thr_kill(_theThread->sysId, SIGTERM) == 0);    
}

Boolean Thread::run ()
{
    if (_theThread->commonData.threadStat != THR_IS_CREATED)
	return FALSE;

    if (thr_continue(_theThread->sysId) == 0)
    {
	if (status() == THR_IS_CREATED)
	    _theThread->commonData.threadStat = THR_IS_RUNNING;

	return TRUE;
    }

    return FALSE;
}

Thread::ThreadStatus Thread::status () const
{
    return _theThread->commonData.threadStat;
}

const Uid& Thread::uid () const
{
    return _theThread->commonData.threadId;
}

Boolean Thread::yield ()
{
    if (!_initialized)
	Thread::initialize();
    
    thr_yield();
    
    return TRUE;
}

/*
 * This operation has to be idempotent
 */

Boolean Thread::terminate ()
{
    if (_theThread->commonData.threadStat == THR_IS_TERMINATED)
	return TRUE;

    if (_theThread->sysId != thr_self())
    {
	/* terminate not called by current thread force a join */

	Thread::wait(this);
    }
    else
    {
	Thread::waitForAllChildren();
	cleanup();

	_theThread->commonData.threadStat = THR_IS_TERMINATED;

	/* Solaris man pages say you must lock the mutex prior to broadcast */

	mutex_lock(&_theThread->waitLock);
	cond_broadcast(&_theThread->waiters);
	mutex_unlock(&_theThread->waitLock);

	thr_yield();
    }
    
    return (_theThread->commonData.threadStat == THR_IS_TERMINATED);
}

void *Thread::execute ( void *startMe )
{
    if (startMe)
    {
	Thread *run = (Thread *)startMe;
	Boolean runThread = TRUE;

	if (thr_setspecific(ThreadData::threadKey, startMe) == ENOMEM)
	{
	    /*
	     * Out of thread specific memory - add thread onto overflow list
	     * so current() can find it the hard way
	     * Assume overflow list is already thread safe
	     */

	    if (_mutex->lock() == Mutex::MX_DONE)
	    {
		ThreadData::overflowList->insert(run);
		run->_theThread->onOverflow = TRUE;

		_mutex->unlock();
	    }
	    else
	    {
		/*
		 * insert failed - Trouble
		 */

		runThread = FALSE;
	    }
	}

	if (run->status() == THR_IS_TERMINATED)
	    runThread = FALSE;

	if (runThread)
	{
	    run->_theThread->commonData.threadStat = THR_IS_RUNNING;
	    if (run->_theThread->commonData.threadBody)
		run->_theThread->commonData.result = run->_theThread->commonData.threadBody(run->_theThread->commonData.arg);
	}

	Boolean detached = (run->_theThread->commonData.threadAstat == THR_IS_DETACHED);
	
	run->terminate();

	(void) thr_setspecific(ThreadData::threadKey, NULL);

	thr_yield();
	
	if (detached)
	    Resource::unref(run);
    }

    return 0;
}

Thread *Thread::current ()
{
    if (!_initialized)
	Thread::initialize();
    
    Thread *aThread = 0;
    
    thr_getspecific(ThreadData::threadKey, (void **) &aThread);

    if (aThread == 0)
    {
	/*
	 * Trouble - we could not locate the current thread - it
	 * may be on the overflow list. Look there.
	 */

	thread_t me = thr_self();

	if (_mutex->lock() == Mutex::MX_DONE)
	{
	    BasicListIterator<Thread, Uid> iter(*ThreadData::overflowList);
	    Thread *t = 0;

	    while ((t = iter()) != 0)
	    {
		if (t->_theThread->sysId == me)
		{
		    aThread = t;
		    break;
		}
	    }

	    _mutex->unlock();
	}
    }

    return aThread;
}

Thread* Thread::find (const Uid& id)
{
    if (!_initialized)
        Thread::initialize();

    Thread* t = (Thread*) 0;

    if (_mutex->lock() == Mutex::MX_DONE)
    {
        const BasicList<Thread, Uid>* theList = list().find(id);

	if (theList)
	    t = theList->find(id);
	
	_mutex->unlock();
    }

    return t;
}

void Thread::exit ()
{
    Thread *th = Thread::current();

    if (th && th->_theThread)
    {
	th->terminate();
	th->_theThread->commonData.threadStat = THR_IS_TERMINATED;
	thr_exit(0);
    }
}

Boolean Thread::sleep ( unsigned long usecs )
{
    Thread *currentThread = Thread::current();
    
    if (currentThread)
    {
	int err = 0;
	timeval tv;
	
	timestruc_t to;
	mutex_lock(&currentThread->_theThread->sleepLock);
	
	currentThread->_theThread->commonData.threadStat = THR_IS_BLOCKED;

#ifndef HAS_BSD_GETTIMEOFDAY
        gettimeofday(&tv);
#else
        gettimeofday(&tv, NULL);
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
	    err = cond_timedwait(&currentThread->_theThread->sleepCondition,
				 &currentThread->_theThread->sleepLock, &to);
	    if (err != EINTR)
		break;
	}
	
	currentThread->_theThread->commonData.threadStat = THR_IS_RUNNING;
	mutex_unlock(&currentThread->_theThread->sleepLock);
    
	return TRUE;
    }

    return FALSE;
}

/*
 * Implementation specific Thread methods.
 */

Boolean Thread::initialize ()
{
    if (_initialized)
        return TRUE;

    _list = new HashedList<Thread, Uid>;
    _mutex = Mutex::create();
    Resource::ref(_mutex);
    
    _initialized = TRUE;
    
    thr_keycreate(&ThreadData::threadKey, 0);
    ThreadData::overflowList = new BasicList<Thread, Uid>;

    Thread* mainThread = new Thread();
    Resource::ref(mainThread);

    minStackSize = thr_min_stack();

    /*
     * Now set up the signal handler in case a thread is ever killed.
     * Signal handlers are inherited by spawned threads, so we only need
     * to do this once.
     */
    
    sigset_t s;
    sigemptyset(&s);
    sigaddset(&s, SIGTERM);
    struct sigaction act;

    act.sa_handler = ThreadData::interrupt;
    sigaction(SIGTERM, &act, NULL);
    
    return TRUE;
}

// check that initialize has been called.

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
    if (status() != THR_IS_TERMINATED)
    {
	int prio = -1;
    
	(void) thr_getprio(_theThread->sysId, &prio);

	return prio;
    }
    else
	return Thread::InvalidPriority;
}

Boolean ThreadControl::setPriority (int prio)
{
    if (_toControl->status() != Thread::THR_IS_TERMINATED)
    {
	thr_setprio(_toControl->_theThread->sysId, prio);
	return TRUE;
    }
    else
	return FALSE;
}

ostream& Thread::print (ostream& strm) const
{
    strm << "[ Thread " << _theThread->commonData.threadId << " <Status: ";
    strm << _theThread->commonData.threadStat << "> <Priority: ";
    strm << priority() << "> ]";

    return strm;
}
