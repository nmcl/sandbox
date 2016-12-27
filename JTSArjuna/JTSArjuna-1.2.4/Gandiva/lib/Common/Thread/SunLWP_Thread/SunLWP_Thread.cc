/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SunLWP_Thread.cc,v 1.6 1998/08/29 10:36:44 nmcl Exp $
 */

#ifndef OS_SYS_TIME_H_
#  include <os/sys/time.h>
#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef BASE_ACTION_H_
#  include <Base/Action.h>
#endif

#ifndef COMMON_THREAD_SUNLWP_THREAD_SUNLWP_THREAD_H_
#  include <Common/Thread/SunLWP_Thread/SunLWP_Thread.h>
#endif

#ifndef CONTROL_THREADCONTROL_H_
#  include <Control/ThreadControl.h>
#endif

#ifndef COMMON_THREAD_THREADSETUP_H_
#  include <Common/Thread/ThreadSetup.h>
#endif

#ifndef TEMPLATE_HASHEDLIST_H_
#  include <Template/HashedList.h>
#endif

#ifndef TEMPLATE_HASHEDITERATOR_H_
#  include <Template/HashedIterator.h>
#endif

static unsigned long minStackSize = MINSTACKSZ*sizeof(stkalign_t);
static unsigned short stackPoolSize = 10;
static unsigned long waitQuantum = 500;

const int Thread::MaxPriority = 127;
const int Thread::MinPriority = MINPRIO;
const int Thread::defaultPriority = Thread::MinPriority+10;
const unsigned long Thread::defaultStackSize = minStackSize*100;

/*
 * Would like to be able to use conditions for waiting as in pthreads,
 * but whenever we call cv_create it causes the program to generate an
 * illegal instruction - even with the examples in the manual!
 * So, we just use a monitor.
 */

ThreadData::ThreadData (Thread::ThreadAttachStatus aStat,
			Thread* th)
		       : commonData(aStat, th),
			 priority(Thread::defaultPriority)
{
    mon_create(&waitLock);
}

ThreadData::~ThreadData ()
{
    mon_exit(waitLock);
    mon_destroy(waitLock);

    // deletion of stack is up to the thread class.
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
    _theThread->sysId = SELF;

    lwp_self(&_theThread->sysId);
    lwp_setpri(_theThread->sysId, Thread::defaultPriority);

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

Thread::Thread (void* (*body)(void*), void* arg, ThreadSetup* setup)
	       : _theThread(NULL),
		 _childList(NULL),
		 _cleanupList(NULL)
{
    Thread::ThreadAttachStatus aStat = THR_IS_ATTACHED;
    int prio = Thread::defaultPriority;
    Thread* currt = Thread::current();
    unsigned long stackSize = 0;

    if (setup)
    {
	aStat = setup->attachStatus;

	if (setup->stackSize > 0)
	    stackSize = setup->stackSize;

	if ((setup->priority >= Thread::MinPriority) &&
	    (setup->priority <= Thread::MaxPriority))
	    prio = setup->priority;
    }

    _theThread = new ThreadData(aStat, this);    

    _theThread->priority = prio;
    _theThread->commonData.threadBody = body;
    _theThread->commonData.arg = arg;

    if (setup && (setup->startState == THR_IS_RUNNING))
	_theThread->commonData.threadStat = THR_IS_RUNNING;
	
    caddr_t p1 = (caddr_t) this;

    if (stackSize > 0)
    {
	_theThread->stack = ::new stkalign_t[stackSize];

	lwp_create(&_theThread->sysId, Thread::execute, _theThread->priority, LWPSUSPEND,
		   STKTOP(&_theThread->stack[stackSize]), 1, p1);
    }
    else
	lwp_create(&_theThread->sysId, Thread::execute, _theThread->priority, LWPSUSPEND,
		   lwp_newstk(), 1, p1);

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

/*
 * Tidy things up before we terminate thread.
 * We need to check for a thread deleting itself, as well
 * as the thread being deleted by some other thread. Because
 * a thread can delete itself we cannot call terminate, since
 * this will prevent us from tidying up.
 */

Thread::~Thread ()
{
    thread_t tid;
    lwp_self(&tid);

    Boolean currentThread = SAMETHREAD(tid, _theThread->sysId);

    if (status() != THR_IS_CREATED)
	Thread::terminate();

    lwp_yield(SELF);

    if (_mutex->lock() == Mutex::MX_DONE)
    {
        list().remove(this, uid());
	_mutex->unlock();
    }

    if (currentThread)
    {
	delete _theThread;
	lwp_destroy(SELF);  // this frees the stack for us
    }
    else
    {
	if (_theThread->stack)
	{
	    ::delete [] _theThread->stack;
	    _theThread->stack = NULL;
	}

	delete _theThread;
    }
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
 * Make sure the thread does not wait for itself.
 * Although we can make use of join since lwp threads allows multiple
 * threads to join (wait) for another thread, we do not. This is
 * to enable the thread pool implementation to reuse threads.
 */

void* Thread::wait (Thread* toWait, unsigned long waittime, Boolean& timeout)
{
    void* result = 0;
    
    if (toWait)
    {
	if ((toWait->_theThread) && !SAMETHREAD(SELF, toWait->_theThread->sysId))
        {
	    Thread* currt = Thread::current();
	    unsigned long timeWaited = 0;
	    
	    timeout = FALSE;
	    
	    currt->_theThread->commonData.threadStat = THR_IS_WAITING;

	    mon_enter(toWait->_theThread->waitLock);

	    while ((toWait->_theThread->commonData.threadStat != THR_IS_TERMINATED) &&
		   (timeWaited < waitTime))
	    {
		mon_exit(toWait->_theThread->waitLock);

		Thread::sleep(waitQuantum);

		timeWaited += waitQuantum;
		
		mon_enter(toWait->_theThread->waitLock);
	    }
	    
	    mon_exit(toWait->_theThread->waitLock);

	    if (timeWaited > waitTime)
		timeout = TRUE;
	    
	    currt->_theThread->commonData.threadStat = THR_IS_RUNNING;
	}

	result = toWait->_theThread->commonData.result;
    }

    return result;
}

void* Thread::wait (Thread* toWait)
{
    void* result = 0;
    
    if (toWait)
    {
	if ((toWait->_theThread) && !SAMETHREAD(SELF, toWait->_theThread->sysId))
        {
	    Thread* currt = Thread::current();

	    currt->_theThread->commonData.threadStat = THR_IS_WAITING;

	    mon_enter(toWait->_theThread->waitLock);

	    while (toWait->_theThread->commonData.threadStat != THR_IS_TERMINATED)
	    {
		mon_exit(toWait->_theThread->waitLock);
		lwp_yield(SELF);
		mon_enter(toWait->_theThread->waitLock);
	    }

	    mon_exit(toWait->_theThread->waitLock);

	    currt->_theThread->commonData.threadStat = THR_IS_RUNNING;
	}

	result = toWait->_theThread->commonData.result;
    }

    return result;
}

Boolean Thread::resume ()
{
    if (_theThread->commonData.threadStat == THR_IS_SUSPENDED)
    {
	_theThread->commonData.threadStat = THR_IS_RUNNING;
	lwp_resume(_theThread->sysId);
    }

    return (_theThread->commonData.threadStat == THR_IS_RUNNING);
}

/*
 * Any thread can be suspended in LWP threads.
 */

Boolean Thread::suspend ()
{
    Boolean result = FALSE;
    
    if (_theThread->commonData.threadStat == THR_IS_RUNNING)
    {
	_theThread->commonData.threadStat = THR_IS_SUSPENDED;
	result = (lwp_suspend(_theThread->sysId) == 0);
	_theThread->commonData.threadStat = THR_IS_RUNNING;
    }
    else
	result = (_theThread->commonData.threadStat == THR_IS_SUSPENDED);

    return result;
}

/*
 * Should do this via signals so the thread is properly woken up
 * prior to being killed.
 */

Boolean Thread::kill ()
{
    if (_theThread->commonData.threadStat == THR_IS_TERMINATED)
	return FALSE;

    killChildren();
    cleanup();

    _theThread->commonData.threadStat = THR_IS_TERMINATED;

    mon_exit(_theThread->waitLock);  // wake up any waiters

    lwp_yield(SELF);

    lwp_destroy(_theThread->sysId);
    
    return TRUE;
}

Boolean Thread::run ()
{
    if (status() != THR_IS_CREATED)
	return FALSE;

    if (lwp_resume(_theThread->sysId) == 0)
    {
        if (status() == THR_IS_CREATED)
	    _theThread->commonData.threadStat = THR_IS_RUNNING;

	return TRUE;
    }
    else
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

/*
 * Because LWP_Thread does not have the ability to store away arbitrary thread
 * specific data, we make current as fast as possible, with the possibility
 * of slower access to the find (const Uid&) method.
 */

Thread* Thread::current ()
{
    if (!_initialized)
	Thread::initialize();

    Thread* t = (Thread*) 0;

    if (_mutex->lock() == Mutex::MX_DONE)
    {
        thread_t tid;

	lwp_self(&tid);

	/*
	 * Not the most efficient way of finding the current
	 * thread, since we may have to iterate through the
	 * entire list.
	 */

	HashedIterator<Thread, Uid> iter(list());

	for (t = iter(); t; t = iter())
        {
	    if (SAMETHREAD(tid, t->_theThread->sysId))
		break;
	}

	_mutex->unlock();
    }

    return t;
}

Thread* Thread::find (const Uid& id)
{
    if (!_initialized)
        Thread::initialize();

    Thread* t = (Thread*) 0;

    if (_mutex->lock() == Mutex::MX_DONE)
    {
        HashedIterator<Thread, Uid> iter(list());

	for (t = iter(); t; t = iter())
	    if (t->uid() == id)
	        break;

	_mutex->unlock();
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
	lwp_destroy(SELF);	
    }
}

Boolean Thread::sleep (unsigned long sleepTime)
{
    Thread* currentThread = Thread::current();

    if (!currentThread)
	return FALSE;
    
    Boolean result = FALSE;
    timeval timeout;

    timeout.tv_sec = sleepTime/1000000;
    timeout.tv_usec = sleepTime%1000000;

    currentThread->_theThread->commonData.threadStat = THR_IS_BLOCKED;

    result = (Boolean) (lwp_sleep(&timeout) == 0);

    currentThread->_theThread->commonData.threadStat = THR_IS_RUNNING;

    return result;
}

Boolean Thread::yield ()
{
    if (!_initialized)
	Thread::initialize();

    // Do this because of bug in lwp_destroy

    Thread* currentThread = Thread::current();

    if (currentThread->status() == THR_IS_TERMINATED)
	lwp_destroy(SELF);
    
    lwp_yield(SELF);

    if (currentThread->status() == THR_IS_TERMINATED)
	lwp_destroy(SELF);
    
    return TRUE;
}

Boolean Thread::terminate ()
{
    if (_theThread->commonData.threadStat == THR_IS_TERMINATED)
	return TRUE;

    thread_t tid;
    lwp_self(&tid);
    
    if (!SAMETHREAD(tid, _theThread->sysId))
    {
	Thread::wait(this);

	return (_theThread->commonData.threadStat == THR_IS_TERMINATED);
    }
    else
    {
	Thread::waitForAllChildren();
	cleanup();

	_theThread->commonData.threadStat = THR_IS_TERMINATED;

	mon_exit(_theThread->waitLock);  // wake up any waiters

	lwp_yield(tid);

	return TRUE;
    }
}

void* Thread::execute (void* startMe)
{
    if (startMe)
    {
	Thread* run = (Thread*) startMe;

	if (run->status() != THR_IS_TERMINATED)
        {
	    /*
	     * Now grab the monitor lock. Can only do this here
	     * when the thread is running.
	     */

	    mon_enter(run->_theThread->waitLock);   // cause any waiters to block.

	    if (run->_theThread->commonData.threadBody)
		run->_theThread->commonData.result = run->_theThread->commonData.threadBody(run->_theThread->commonData.arg);
	}

	run->terminate();

	if (run->_theThread->commonData.threadAstat == THR_IS_DETACHED)
	    Resource::unref(run);
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

    pod_setmaxpri(Thread::MaxPriority);
    lwp_setstkcache((int) minStackSize, stackPoolSize);
    
    _list = new HashedList<Thread, Uid>;
    _mutex = Mutex::create();
    Resource::ref(_mutex);
    
    _initialized = TRUE;

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
    if (status() != THR_IS_TERMINATED)
	return (_theThread->priority);
    else
	return Thread::InvalidPriority;
}

Boolean ThreadControl::setPriority (int prio)
{
    if (_toControl->status() != Thread::THR_IS_TERMINATED)
    {
	return (lwp_setpri(_toControl->_theThread->sysId, prio) == 0);
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
