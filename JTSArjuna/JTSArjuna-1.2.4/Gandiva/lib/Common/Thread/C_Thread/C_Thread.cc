/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: C_Thread.cc,v 1.8 1998/08/29 10:36:37 nmcl Exp $
 */

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef COMMON_THREAD_C_THREAD_C_THREAD_H_
#  include <Common/Thread/C_Thread/C_Thread.h>
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

const int Thread::MaxPriority = 127;
const int Thread::MinPriority = 0;
const int Thread::defaultPriority = Thread::MinPriority+10;
const unsigned long Thread::defaultStackSize = CTHREAD_STACK;

jmp_buf ThreadData::env;

ThreadData::ThreadData (Thread::ThreadAttachStatus aStat,
			Thread* th)
		       : commonData(aStat, th),
			 priority(Thread::defaultPriority),
			 dead(FALSE),
			 isSuspending(FALSE)
{
    sysId = 0;
    
    mutex_alloc(&suspendLock, N_CURRENT);
    mutex_alloc(&waitLock, N_CURRENT);
    condition_alloc(&waiters, N_CURRENT);
    
    mutex_init(suspendLock);
    mutex_init(waitLock);
    condition_init(waiters);
}

ThreadData::~ThreadData ()
{
    mutex_free(suspendLock);
    mutex_free(waitLock);
    condition_free(waiters);
}


Thread::Thread (ThreadAttachStatus aStatus)
	       : _theThread(NULL),
		 _childList(NULL),
		 _cleanupList(NULL)
{
    _theThread = new ThreadData(aStatus, this);
    _theThread->commonData.threadStat = THR_IS_RUNNING;
    _theThread->sysId = cthread_self();

    cthread_set_data(cthread_self(), (any_t) this);
    
    mutex_lock(_theThread->suspendLock);

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
    Thread::ThreadAttachStatus aStat = ((setup) ? setup->attachStatus : THR_IS_ATTACHED);
    Thread* currt = Thread::current();

    _theThread = new ThreadData(aStat, this);
    
    if (setup && (setup->startState == THR_IS_RUNNING))
	_theThread->commonData.threadStat = THR_IS_RUNNING;
    
    _theThread->commonData.threadBody = body;
    _theThread->commonData.arg = arg;
	    
    _theThread->sysId = cthread_fork(Thread::execute, (any_t) this, N_CURRENT);

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
	Resource::ref(this);

    /*
     * Must always detach so system can clean up the thread upon
     * termination.
     */
    
    cthread_detach(_theThread->sysId);

    /* If thread should be running make it so */

    if (setup && (setup->startState == THR_IS_RUNNING))
	this->run();
}

/*
 * Tidy things up before we terminate thread. Because threads
 * are reference counted it should not be possible to delete
 * a thread object which represents a running thread.
 */

Thread::~Thread ()
{
    Boolean currentThread = (Thread::current() == this);

    if (status() != THR_IS_CREATED)
	Thread::terminate();
    
    Thread::yield();
    
    if (_mutex->lock() == Mutex::MX_DONE)
    {
        list().remove(this, uid());
	_mutex->unlock();

        cthread_set_data(_theThread->sysId, NULL);
    }

    if (_theThread)
    {
	void** p = 0;
	
	if (_theThread->commonData.threadAstat == THR_IS_ATTACHED)
	    _theThread->sysId->join(p);
	
	delete _theThread;
    }

    if (currentThread)
	cthread_exit(0);
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
 * Since C threads only allow a single thread to wait for another
 * we do not use join here. Instead we use conditions.
 */

void* Thread::wait (Thread* toWait)
{
    void* result = 0;
    
    if (toWait)
    {
	Thread* currt = Thread::current();

	if (currt->_theThread->isSuspending)
	    currt->suspend();
	
	if (toWait->_theThread && (toWait->_theThread->sysId != cthread_self()))
	{
	    if (currt->_theThread->dead)
	    {
		Thread::exit();
	    }
	    
	    currt->_theThread->commonData.threadStat = THR_IS_WAITING;
	    
	    mutex_lock(toWait->_theThread->waitLock);
	    
	    /*
	     * Just because a condition is signalled does not mean that
	     * the 'condition state' has been set correctly. We may have
	     * to loop.
	     */

	    while (toWait->_theThread->commonData.threadStat != THR_IS_TERMINATED)
	    {
		condition_wait(toWait->_theThread->waiters, toWait->_theThread->waitLock);
	    }

	    mutex_unlock(toWait->_theThread->waitLock);

	    currt->_theThread->commonData.threadStat = THR_IS_RUNNING;

	    if (toWait->_theThread->commonData.threadStat == THR_IS_TERMINATED)
		result = toWait->_theThread->commonData.result;
	}
    }

    return result;
}

void* Thread::wait (Thread* toWait, unsigned long waittime, Boolean& timeout)
{
    error_stream << WARNING << "Sorry, timed Thread::wait() not implememted." << endl;

    return Thread::wait(toWait);
}

/*
 * There is no real suspend/resume in C threads. Fake them here using a mutex
 * The mutex is grabbed in execute to ensure that a grab here will block
 */

Boolean Thread::resume ()
{
    if (_theThread->dead)
    {
	Thread::exit();
    }
	    
    if (_theThread->commonData.threadStat == THR_IS_SUSPENDED)
    {
	_theThread->commonData.threadStat = THR_IS_RUNNING;

	if (!_theThread->isSuspending)
	    mutex_unlock(_theThread->suspendLock);
	else
	    _theThread->isSuspending = FALSE;
    }

    return (_theThread->commonData.threadStat == THR_IS_RUNNING);
}

Boolean Thread::suspend ()
{
    if (_theThread->dead)
    {
	Thread::exit();
    }
    
    /*
     * Make sure only the current thread can suspend itself in C threads.
     * If we are not the current thread, then set it to suspend itself
     * later.
     */

    if (_theThread->sysId != cthread_self())
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
	mutex_lock(_theThread->suspendLock);
	_theThread->commonData.threadStat = THR_IS_RUNNING;
	
        result = TRUE;
    }
    else
	result = (_theThread->commonData.threadStat == THR_IS_SUSPENDED);

    return result;
}

/*
 * We cannot kill a thread in C threads. Set its 'dead' flag, and hope
 * it eventually realizes, and kills itself.
 */

Boolean Thread::kill ()
{
    if (status() == THR_IS_TERMINATED)
	return FALSE;
    else
    {
	_theThread->dead = TRUE;
	cthread_yield();
	
	return TRUE;
    }
}

Boolean Thread::run ()
{
    if (_theThread->commonData.threadStat != THR_IS_CREATED)
	return FALSE;

    /*
     * There should be no need to change the thread's status
     * as it will be done automatically when it runs.
     */

    mutex_unlock(_theThread->suspendLock);
    
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

    any_t p1 = cthread_data(cthread_self());

    return (Thread*) p1;
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
    Thread* th = Thread::current();

    if (th && th->_theThread)
    {
	th->terminate();
	th->_theThread->commonData.threadStat = THR_IS_TERMINATED;
	cthread_exit(0);
    }
}

Boolean Thread::sleep (unsigned long)
{
    return FALSE;
}

Boolean Thread::yield ()
{
    if (!_initialized)
	Thread::initialize();

    cthread_yield();

    Thread* currentThread = Thread::current();

    if (currentThread->_theThread->isSuspending)
	currentThread->suspend();
    
    if (currentThread->_theThread->dead)
    {
	Thread::exit();
    }
    
    return TRUE;
}

Boolean Thread::terminate ()
{
    if (_theThread->commonData.threadStat == THR_IS_TERMINATED)
	return TRUE;

    if (_theThread->sysId != cthread_self())
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

	/*
	 * Use condition_free rather than condition_broadcast as we
	 * want to unallocate the space for the condition now as
	 * well as signal any waiting threads.
	 */

	condition_broadcast(_theThread->waiters);
	
	cthread_yield();
	
	return TRUE;
    }
}
    
/*
 * C threads always start running immediately. If the user wanted them created
 * suspended we do it here
 */

void* Thread::execute (void* startMe)
{
    if (startMe)
    {
	Thread* run = (Thread*) startMe;

	/*
	 * We're assuming no other error type is returned here!
	 */

	cthread_set_data(run->_theThread->sysId, run);
	
	/*
	 * Grab suspend mutex. This ensures that if I grab again (via suspend)
	 * I will block.
	 */

	mutex_lock(run->_theThread->suspendLock);
	
	if (run->_theThread->commonData.threadStat == THR_IS_CREATED)
	{
	    mutex_lock(run->_theThread->suspendLock);
	}
	
	run->_theThread->commonData.threadStat = THR_IS_RUNNING;
	
	if (run->_theThread->commonData.threadBody)
	    run->_theThread->commonData.result = run->_theThread->commonData.threadBody(run->_theThread->commonData.arg);

	Boolean detached = (run->_theThread->commonData.threadAstat == THR_IS_DETACHED);

	run->terminate();

	cthread_yield();

	if (detached)
        {
	    Resource::unref(run);
	}
    }

    return 0;
}

/*
 * We have to do this because the cthreads approach to making "main"
 * a thread is to force it (the first thread) into a separate procedure
 * for it to run. When it returns from this procedure the application ends.
 * This is different to all of the other threads packages, which simply
 * treat the main thread as automatically running within "main". Rather
 * than make all of the other packages conform to cthreads, we can force
 * cthreads to work in the manner of the other scheme by making the main
 * thread jump back to the calling point of cthread_init using setjmp/longjmp.
 * This is messy, but it allows us to still write applications which do not
 * have to be specific to a given threads package.
 *
 * Don't try this at home!
 */

static void dummyMain ()
{
    longjmp(ThreadData::env, 1);
}

Boolean Thread::initialize ()
{
    if (_initialized)
        return TRUE;
    
    configuration conf;

    conf.memory_exponent = DEFAULT_MEM;
    conf.threads_per_proc = CTHREAD_THREADS;
    conf.stack_size = CTHREAD_STACK;
    
    cthread_configure(&conf, PUT_CONFIG);

    _list = new HashedList<Thread, Uid>;
    _initialized = TRUE;

    if (setjmp(ThreadData::env) == 0)
    	cthread_init(1, ::dummyMain);

    /*
     * Only create mutex now since thread package is
     * initialized.
     */
    
    _mutex = Mutex::create();
    Resource::ref(_mutex);
    
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
    return (Resource*) 0;
}

int Thread::priority () const
{
    if (status() != THR_IS_TERMINATED)
	return this->_theThread->priority;
    else
	return Thread::InvalidPriority;
}

Boolean ThreadControl::setPriority (int)
{
    return FALSE;
}

ostream& Thread::print (ostream& strm) const
{
    strm << "[ Thread " << _theThread->commonData.threadId << " <Status: ";
    strm << _theThread->commonData.threadStat << "> <Priority: ";
    strm << priority() << "> ]";

    return strm;
}
