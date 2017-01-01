/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CPPTask_Thread.cc,v 1.9 1998/08/29 10:36:37 nmcl Exp $
 */

#ifndef OS_UNISTD_H_
#  include <os/unistd.h>
#endif

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef COMMON_CLASSNAME_H_
#  include <Common/ClassName.h>
#endif

#ifndef COMMON_THREAD_THREADSETUP_H_
#  include <Common/Thread/ThreadSetup.h>
#endif

#ifndef COMMON_THREAD_CPPTASK_THREAD_CPPTASK_THREAD_H_
#  include <Common/Thread/CPPTask_Thread/CPPTask_Thread.h>
#endif

#ifndef CONTROL_THREADCONTROL_H_
#  include <Control/ThreadControl.h>
#endif

#ifndef TEMPLATE_HASHEDLIST_H_
#  include <Template/HashedList.h>
#endif

const int Thread::MaxPriority = 127;
const int Thread::MinPriority = 0;
const int Thread::defaultPriority = Thread::MinPriority+10;
const unsigned long Thread::defaultStackSize = SIZE;

Thread* ThreadData::mainTask = 0;
task* ThreadData::mainSysId = 0;

static _semaphore mainSem(0);


ThreadData::ThreadData (Thread::ThreadAttachStatus aStat, Thread* th,
			void* (*body)(void*), void* arg, int stackSize)
		       : task(NULL, DEFAULT_MODE, ((stackSize > 0) ? stackSize : SIZE)),
			 commonData(aStat, th),
			 sysId(object::this_task()),
			 suspendLock(1),
			 waitLock(0),
			 priority(Thread::defaultPriority),
			 isPending(1),
			 isSuspending(FALSE)
{
    /*
     * Do this here rather than in constructor of thread as is usual because we
     * need it setup up in the constructor, since the thread will run
     * immediately.
     */
     
    commonData.threadBody = body;
    commonData.arg = arg;
    
    Thread::execute(taskObject);
}

ThreadData::~ThreadData ()
{
    if (isPending)
    {
	isPending = 0;

	resultis(0);
    }

    if (sysId && (sysId != ThreadData::mainSysId))
	delete sysId;
}

int ThreadData::pending () const
{
    return isPending;
}

/*
 * Typically used only to create a fake thread for main
 */

Thread::Thread (ThreadAttachStatus aStatus)
	       : _theThread(NULL),
		 _childList(NULL),
		 _cleanupList(NULL)
{
   _theThread = new ThreadData(aStatus, this);
   _theThread->commonData.threadStat = THR_IS_RUNNING;

   suspendLock.wait();

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
    Thread* currt = Thread::current();
    int stackSize = (int) Thread::defaultStackSize;
    
    if (setup)
    {
	aStat = setup->attachStatus;
	
	if (setup->stackSize > 0)
	    stackSize = setup->stackSize;
    }
    
    _theThread = new ThreadData(aStat, this, body, arg, stackSize);
    
    if (setup && (setup->startState == THR_IS_RUNNING))
	_theThread->commonData.threadStat = THR_IS_RUNNING;

    if (!mainSysId)
	mainSysId = object::this_task();
    
    if (_mutex->lock() == Mutex::MX_DONE)
    {
        list().insert(this, uid());
	_mutex->unlock();
    }

    Thread* currentThread = Thread::current();

    if (currentThread)
	this->currentAction(currentThread->currentAction());

    if (aStat != THR_IS_DETACHED)
    {
	this->addChild();
    }
    else
	Resource::ref(this);
	
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
    if ((status() != THR_IS_CREATED) &&
	(status() != THR_IS_TERMINATED))
	Thread::terminate();

    Thread::yield();

    if (_mutex->lock() == Mutex::MX_DONE)
    {
        list().remove(this, uid());
	_mutex->unlock();
    }

    if (ThreadData::mainTask == this)
        ThreadData::mainTask = NULL;

    if (_theThread)
    {
	if (_theThread->commonData.threadAstat == THR_IS_ATTACHED)
	    _theThread->sysId->result();

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
 * Since C++ tasks only allow a single thread to wait for another
 * we do not use join here. Instead we use mutexes.
 */

void* Thread::wait (Thread* toWait)
{
    void* result = NULL;

    if (toWait)
    {
	if (toWait->_theThread && (toWait->_theThread->sysId != object::this_task()))
	{
	    if (toWait->status() != THR_IS_TERMINATED)
	    {
		Thread* currt = Thread::current();

		if (currt->_theThread->isSuspending)
		    currt->suspend();
		
		currt->_theThread->commonData.threadStat = THR_IS_WAITING;
		
		toWait->_theThread->waitLock.wait();

		currt->_theThread->commonData.threadStat = THR_IS_RUNNING;
	    }

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
 * There is no real suspend/resume in C++ Tasks. Fake them here using a mutex
 * The mutex is grabbed in execute to ensure that a grab here will block
 */

Boolean Thread::resume ()
{
    if (_theThread->commonData.threadStat == THR_IS_SUSPENDED)
    {
	_theThread->commonData.threadStat = THR_IS_RUNNING;

	if (!_theThread->isSuspending)
	{
	    _theThread->suspendLock.signal();
	}
	ekse
	    _theThread->isSuspending = FALSE;
    }

    return (_theThread->commonData.threadStat == THR_IS_RUNNING);
}

Boolean Thread::suspend ()
{
    Thread* currt = Thread::current();
    
    if (currt != this)
    {
	currt->_theThread->isSuspending = TRUE;
	currt->_theThread->commonData.threadStat = THR_IS_SUSPENDED;
	return TRUE;
    }
    
    Boolean result = FALSE;
    
    if (_theThread->commonData.threadStat == THR_IS_RUNNING)
    {
	_theThread->commonData.threadStat = THR_IS_SUSPENDED;
	_theThread->suspendLock.wait();
	_theThread->commonData.threadStat = THR_IS_RUNNING;
	
        result = TRUE;
    }
    else
	result = (_theThread->commonData.threadStat == THR_IS_SUSPENDED);

    return result;
}

Boolean Thread::kill ()
{
    if (status() == THR_IS_TERMINATED)
	return FALSE;
    
    killChildren();

    cleanup();
    
    _theThread->commonData.threadStat = THR_IS_TERMINATED;

    _theThread->isPending = 0;

    _theThread->sysId->resultis(0);

    return TRUE;
}

Boolean Thread::run ()
{
    if (status() != THR_IS_CREATED)
	return FALSE;
    else
    {
	_theThread->suspendLock.signal();

	if (status() == THR_IS_CREATED)
	    _theThread->commonData.threadStat = THR_IS_RUNNING;

	return TRUE;
    }
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

    task* currTask = object::this_task();

    if (currTask == ThreadData::mainSysId)
        return ThreadData::mainTask;
    else
	return ((ThreadData*) currTask)->thread;
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
	th->_theThread->sysId->resultis(0);
    }
}

Boolean Thread::sleep (unsigned long sleepTime)
{
    Boolean result = FALSE;
    Thread* currt = Thread::current();
    
    if ((currt->status() == THR_IS_RUNNING) && (sleepTime > 0))
    {
	currt->_theThread->commonData.threadStat = THR_IS_BLOCKED;

	currt->_theThread->sysId->delay((long) sleepTime);

	currt->_theThread->commonData.threadStat = THR_IS_RUNNING;
	
	result = TRUE;
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
		
    object::this_task()->delay(0);

    return TRUE;
}

Boolean Thread::terminate ()
{
    if (_theThread->commonData.threadStat == THR_IS_TERMINATED)
	return TRUE;

    if ((Thread::current() != this) && (_theThread->commonData.threadStat != THR_IS_CREATED))
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

	_theThread->isPending = 0;

	while (_theThread->waitLock.pending())
	{
	    _theThread->waitLock.signal();
	    Thread::yield();
	}

	Thread::yield();
	
	return TRUE;
    }
}

void Thread::execute (void* startMe)
{
    if (startMe)
    {
	Thread* run = (Thread*) startMe;
	
	/*
	 * Grab suspend mutex. This ensures that if I grab again (via suspend)
	 * I will block.
	 */
	
	run->_theThread->suspendLock.wait();

	if (run->_theThread->commonData.threadStat == THR_IS_CREATED)
	{
	    run->_theThread->suspendLock.wait();
	}

	run->_theThread->commonData.threadStat = THR_IS_RUNNING;
	
	if (run->_theThread->commonData.threadBody)
	    run->_theThread->commonData.result = run->_theThread->commonData.threadBody(run->_theThread->commonData.arg);

	Boolean detached = (run->_theThread->commonData.threadAstat == THR_IS_DETACHED);

	run->terminate();
	
	if (detached)
	{
	    Resource::unref(run);
	}
    }

    run->_theThread->isPending = 0;
    
    object::this_task()->resultis(0);
    
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
	return _theThread->priority;
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
