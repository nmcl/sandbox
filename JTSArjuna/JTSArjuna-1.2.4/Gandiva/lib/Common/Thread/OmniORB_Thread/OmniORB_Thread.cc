/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OmniORB_Thread.cc,v 1.12 1998/11/19 15:42:05 nmcl Exp $
 */

/*
 * Implementation of threads for OmniOrb threads.
 */

#ifndef OS_OMNITHREAD_H_
#  include <os/omnithread.h>
#endif

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef OS_SYS_TIME_H_
#  include <os/sys/time.h>
#endif

#ifndef COMMON_THREAD_THREADSETUP_H_
#  include <Common/Thread/ThreadSetup.h>
#endif

#ifndef COMMON_THREAD_OMNIORB_THREAD_OMNIORB_THREAD_H_
#  include <Common/Thread/OmniORB_Thread/OmniORB_Thread.h>
#endif

#ifndef TEMPLATE_BASICLISTITERATOR_H_
#  include <Template/BasicListIterator.h>
#endif

#ifndef CONTROL_THREADCONTROL_H_
#  include <Control/ThreadControl.h>
#endif

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

const int Thread::MaxPriority = omni_thread::PRIORITY_HIGH;
const int Thread::MinPriority = omni_thread::PRIORITY_LOW;
const int Thread::defaultPriority = omni_thread::PRIORITY_NORMAL;
const unsigned long Thread::defaultStackSize = 0;

static size_t minStackSize = 0;  // have no control over this anyway!
static BasicList<Thread, Uid>* overflowList = 0;
static Thread* mainThread = (Thread*) 0;
static omni_mutex* globalMutex = (omni_mutex*) 0;

ThreadData::ThreadData ( Thread::ThreadAttachStatus aStat,
			 Thread *th )
		       : commonData(aStat, th),
			 onOverflow(TRUE),
			 sysId(0),
			 isSuspending(FALSE)
{
    suspendLock = new omni_mutex();
    waitLock = new omni_mutex();
    waiters = new omni_condition(waitLock);
}

ThreadData::~ThreadData ()
{
    if (suspendLock)
    {
	suspendLock->unlock();
	delete suspendLock;
    }
    if (waiters)
	delete waiters;
    if (waitLock)
	delete waitLock;
}

void* ThreadData::execute (void* arg)
{
    return Thread::execute(arg);
}

/*
 * Typically used only to create a fake thread for main.
 */

Thread::Thread (ThreadAttachStatus aStatus)
	       : _theThread(0),
		 _childList(0),
		 _cleanupList(0)    
{
    _theThread = new ThreadData(aStatus, this);
    _theThread->commonData.threadStat = THR_IS_RUNNING;
    _theThread->sysId = omni_thread::self();

    /*
     * To have thread specific data in omnithread we need to derive a
     * class from omnithread. Currently we don't do that, so we put
     * everything onto the overflow list.
     */
    
    if (_mutex->lock() == Mutex::MX_DONE)
    {
        list().insert(this, uid());

	overflowList->insert(this);
	_theThread->onOverflow = TRUE;
	
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

Thread::Thread ( void * (*body)(void *), void *arg, ThreadSetup* setup)
	       : _theThread(0),
		 _childList(0),
		 _cleanupList(0)    
{
    Thread::ThreadAttachStatus aStat = THR_IS_ATTACHED;
    int prio = Thread::defaultPriority;
    Thread* currt = Thread::current();
    unsigned long stackSize = 0;
    
    if (setup)
    {
	aStat = setup->attachStatus;
	
	if (setup->priority != Thread::InvalidPriority)
	    prio = setup->priority;

	if (setup->stackSize > 0)
	    stackSize = setup->stackSize;
    }
    
    _theThread = new ThreadData(aStat, this);
 
    if (aStat == THR_IS_DETACHED)
    {
	// do nothing currently!
    }
     
    _theThread->commonData.threadBody = body;
    _theThread->commonData.arg = arg;
    _theThread->commonData.threadAstat = aStat;

    if (setup && (setup->startState == THR_IS_RUNNING))
	_theThread->commonData.threadStat = THR_IS_RUNNING;

    _theThread->sysId = new OmniORB_Thread(ThreadData::execute, (void*) this);

    _theThread->sysId->start();
    
    Thread::yield();
	
    if (_theThread->sysId)
    {
	/* Cannot do before create otherwise id is not set correctly */

	if (_mutex->lock() == Mutex::MX_DONE)
	{
	    list().insert(this, uid());
	    _mutex->unlock();
	}

	if (aStat != THR_IS_DETACHED)
	{
	    /* Add new child to action */
	    
	    this->addChild();
	}
	else
	    Resource::ref(this);
	
	/* If thread should be running make it so */

	/* If thread should be running make it so */

	if (setup && (setup->startState == THR_IS_RUNNING))	
	    resume();	    
    }
    else
    {
	/* Thread create failed - mark as dead */
	    
	_theThread->commonData.threadStat = THR_IS_TERMINATED;
    }
}

Thread::~Thread ()
{
    if (status() != THR_IS_CREATED)
	Thread::terminate();

    Thread::yield();

    if (_mutex->lock() == Mutex::MX_DONE)
    {
        list().remove(this, uid());
	
	if (_theThread->onOverflow)
	    overflowList->remove(this);
	
	_mutex->unlock();
    }

    if (_theThread->commonData.threadAstat != THR_IS_DETACHED)
    {
	void** p = 0;
	Boolean error = FALSE;
	
	try
	{
	    _theThread->sysId->join(p);
	}
	catch (...)
	{
	    error = TRUE;
	}

	/*
	 * Join should free sysId for us.
	 */

	if (error)
	{
	    /*
	     * Something went wrong, so don't do anything. It will cause a
	     * memory leak, but at least the program should continue to
	     * run.
	     */
	    
	    cerr << "OmniThread::join for " << uid() << " failed." << endl;
	}

	delete _theThread;
    }
    else
    {
	if (this == Thread::current())
	{
	    if (_theThread->commonData.threadAstat == THR_IS_DETACHED)
	    {
		delete _theThread;
		_theThread = 0;
		
		omni_thread::exit(0);
	    }
	}

	if (_theThread)
	    delete _theThread;	
    }
}

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

void* Thread::wait (Thread* toWait)
{
    void* result = 0;
    
    if (toWait)
    {
	Thread* currt = Thread::current();

	if (currt->_theThread->isSuspending)
	    currt->suspend();

	if (toWait->_theThread && (toWait->_theThread->sysId != omni_thread::self()))
	{
	    currt->_theThread->commonData.threadStat = THR_IS_WAITING;
	    
	    try
	    {
		toWait->_theThread->waitLock->lock();
	    }
	    catch (...)
	    {
		return NULL;
	    }

	    while (toWait->_theThread->commonData.threadStat != THR_IS_TERMINATED)
	    {
		try
		{
		    toWait->_theThread->waiters->wait();
		}
		catch (...)
		{
		    break;
		}
	    }

	    try
	    {
		toWait->_theThread->waitLock->unlock();
	    }
	    catch (...)
	    {
	    }

	    currt->_theThread->commonData.threadStat = THR_IS_RUNNING;
	    
	    if (currt->_theThread->commonData.threadStat == THR_IS_TERMINATED)
		result = toWait->_theThread->commonData.result;
	}
    }
    
    return result;
}

void* Thread::wait (Thread* toWait, unsigned long waittime, Boolean& timeout)
{
    cerr << "Sorry, timed Thread::wait() not implememted." << endl;

    return Thread::wait(toWait);
}

/*
 * There is no real suspend/resume in omnithread. Fake them here using a mutex
 * The mutex is grabbed in execute to ensure that a grab here will block
 */

Boolean Thread::resume ()
{
    if (_theThread->commonData.threadStat == THR_IS_SUSPENDED)
    {
	_theThread->commonData.threadStat = THR_IS_RUNNING;

	if (!_theThread->isSuspending)
	    _theThread->suspendLock->unlock();
	else
	    _theThread->isSuspending = FALSE;

	return TRUE;
    }
    else
	return (_theThread->commonData.threadStat == THR_IS_RUNNING);
}

Boolean Thread::suspend ()
{
    Boolean result = FALSE;
    
    Thread *th = Thread::current();

    if (th && th->_theThread)
    {
	/*
	 * Make sure only the current thread can suspend itself in omniORB.
	 * If we aren't the current thread then set its status so it will
	 * suspend itself later.
	 */
	
	if (th != this)
	{
	    th->_theThread->isSuspending = TRUE;
	    th->_theThread->commonData.threadStat = THR_IS_SUSPENDED;

	    return TRUE;
	}
	
	if (th->_theThread->commonData.threadStat == THR_IS_RUNNING)
	{
	    th->_theThread->commonData.threadStat = THR_IS_SUSPENDED;
	    th->_theThread->suspendLock->lock();
	    result = TRUE;
	}
	else
	    result = (th->_theThread->commonData.threadStat == THR_IS_SUSPENDED);
    }
    
    return result;
}

Boolean Thread::kill ()
{
    if (_theThread->commonData.threadStat == THR_IS_TERMINATED)
	return FALSE;

    killChildren();

    cleanup();

    _theThread->commonData.threadStat = THR_IS_TERMINATED;

    /*
     * Now wake up any threads waiting for this thread
     * to terminate.
     */

    try
    {
	_theThread->waitLock->lock();
	_theThread->waiters->broadcast();
	_theThread->waitLock->unlock();
    }
    catch (...)
    {
    }

    /*
     * To be completed ...
     *
     * pthread_kill(_theThread->sysId, SIGTERM);
     */

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

    _theThread->suspendLock->unlock();    

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

const Uid &Thread::uid () const
{
    return _theThread->commonData.threadId;
}

Thread* Thread::current ()
{
    Thread* aThread = 0;

    // would like to check thread specific data here, but ...
    
    if (aThread == 0)
    {
	/*
	 * Trouble - we could not locate the current thread - it
	 * may be on the overflow list. Look there
	 */

	if (_mutex->lock() == Mutex::MX_DONE)
	{
	    omni_thread* me = omni_thread::self();	    
	    BasicListIterator<Thread, Uid> iter(*overflowList);
	    Thread *t = iter();

	    while (t != (Thread*) 0)
	    {
		if (t->_theThread->sysId == me)
		{
		    aThread = t;
		    break;
		}
		else
		{
		    t = iter();
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
    Thread *th = Thread::current();

    if (th && th->_theThread)
    {
	th->terminate();
	th->_theThread->commonData.threadStat = THR_IS_TERMINATED;
	omni_thread::exit(NULL);
    }
}

Boolean Thread::sleep ( unsigned long usecs )
{
    Thread* currentThread = Thread::current();

    if (currentThread->_theThread->isSuspending)
	currentThread->suspend();    
    
    try
    {
	currentThread->_theThread->commonData.threadStat = THR_IS_BLOCKED;
	omni_thread::sleep(usecs/1000000, usecs%1000000);
	currentThread->_theThread->commonData.threadStat = THR_IS_RUNNING;
	return TRUE;
    }
    catch (...)
    {
	currentThread->_theThread->commonData.threadStat = THR_IS_RUNNING;
	return FALSE;
    }
}

Boolean Thread::yield ()
{
    Thread* currt = Thread::current();

    if (currt->_theThread->isSuspending)
	currt->suspend();
    
    omni_thread::yield();
    
    return TRUE;
}

Boolean Thread::terminate ()
{
    if (_theThread->commonData.threadStat == THR_IS_TERMINATED)
	return TRUE;

    omni_thread::yield();
    
    if (this != Thread::current())
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

	try
	{
	    _theThread->waitLock->lock();
	    _theThread->waiters->broadcast();
	    _theThread->waitLock->unlock();
	}
	catch (...)
	{
	}

        return TRUE;
    }
}

/*
 * Start running threads immediately. If the user wanted them created
 * suspended we do it here
 */
 
void* Thread::execute ( void *startMe )
{
    if (startMe)
    {
	Boolean runThread = TRUE;
	Thread *run = (Thread*) startMe;
	
	/*
	 * To have thread specific data in omnithread we need to derive a
	 * class from omnithread. Currently we don't do that, so we put
	 * everything onto the overflow list.
	 */

	overflowList->insert(run);
	run->_theThread->onOverflow = TRUE;

	/*
	 * Grab suspend mutex. This ensures that if I grab again (via suspend)
	 * I will block.
	 */

	run->_theThread->suspendLock->lock();

	if (run->_theThread->commonData.threadStat == THR_IS_CREATED)
	    run->_theThread->suspendLock->lock();	    
	
	run->_theThread->commonData.threadStat = THR_IS_RUNNING;
	if (run->_theThread->commonData.threadBody)
	    run->_theThread->commonData.result = run->_theThread->commonData.threadBody(run->_theThread->commonData.arg);
	
	Boolean detached = (run->_theThread->commonData.threadAstat == THR_IS_DETACHED);

	run->terminate();

	omni_thread::yield();

	if (detached)
	{
	    Resource::unref(run);
	}
    }

    return NULL;
}

/*
 * This MUST get run while the thread system is inactive since we cannot use
 * ANY threading stuff until init is called
 */

Boolean Thread::initialize ()
{
    if (_initialized)
	return TRUE;
    
    if (mainThread == 0)
    {
	/*
	 * This ensures that we get ThreadMutex::create() from the OmniThread
	 * lib and not from elsewhere since it creates an unresolved ref.
	 */

	_list = new HashedList<Thread, Uid>;
	overflowList = new BasicList<Thread, Uid>();
	globalMutex = new omni_mutex();
	
	_initialized = TRUE;

	_mutex = Mutex::create();
	Resource::ref(_mutex);

	mainThread = new Thread();
	Resource::ref(mainThread);

	mainThread->_theThread->sysId->set_priority((omni_thread::priority_t) Thread::defaultPriority);
    }

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
    {
	try
	{
	    return (int) _theThread->sysId->priority();
	}
	catch (...)
	{
	}
    }
    
    return Thread::InvalidPriority;
}

Boolean ThreadControl::setPriority (int prio)
{
    if (_toControl->status() != Thread::THR_IS_TERMINATED)
    {
	try
	{
	    _toControl->_theThread->sysId->set_priority((omni_thread::priority_t) prio);

	    return TRUE;
	}
	catch (...)
	{
	}
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
