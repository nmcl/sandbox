/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RexLWP_Thread.cc,v 1.8 1998/08/29 10:36:43 nmcl Exp $
 */

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef OS_SYS_TYPES_H_
#  include <os/sys/types.h>
#endif

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef BASE_ACTION_H_
#  include <Base/Action.h>
#endif

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

#ifndef COMMON_THREAD_REXLWP_THREAD_REXLWP_THREAD_H_
#  include <Common/Thread/RexLWP_Thread/RexLWP_Thread.h>
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

extern struct pcb *currp;

static unsigned long waitQuantum = 500;

const int Thread::MaxPriority = MAXTPRI;
const int Thread::MinPriority = 0;
const int Thread::defaultPriority = Thread::MinPriority+2;
const unsigned long Thread::defaultStackSize = 2048;

ThreadData::ThreadData (Thread::ThreadAttachStatus aStat,
			Thread* th)
		       : commonData(aStat, th),
			 sysId(0),
			 isSuspending(FALSE)
{
    suspendLock = creats(1);
    waitLock = creats(0);    // want to force blocking immediately
}

ThreadData::~ThreadData ()
{
    delete suspendLock;
    delete waitLock;

    if (sysId)
	destroyp(sysId);
}

void ThreadData::execute (int, char**, Thread* startMe)
{
    Thread::execute(startMe);
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
    _theThread->sysId = currp;
    
    setenvp(NULL, (caddr_t) this);
    
    prisetp(Thread::defaultPriority);

    waits(_theThread->suspendLock);
    
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
    unsigned long stackSize = Thread::defaultStackSize;

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

    if (setup && (setup->startState == THR_IS_RUNNING))
	_theThread->commonData.threadStat = THR_IS_RUNNING;

    _theThread->commonData.threadBody = body;
    _theThread->commonData.arg = arg;
    
    _theThread->sysId = creatp(prio, ThreadData::execute, stackSize, 0, 0, (caddr_t) this);

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
    if (status() != THR_IS_CREATED)
	Thread::terminate();

    yieldp();
    
    if (_mutex->lock() == Mutex::MX_DONE)
    {
        list().remove(this, uid());
	_mutex->unlock();
    }

    delete _theThread;
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
 */

void* Thread::wait (Thread* toWait, unsigned long waittime, Boolean& timeout)
{
    void* result = 0;
    
    if (toWait)
    {
	Thread* currt = Thread::current();

	if (currt->_theThread->isSuspending)
	    currt->suspend();
	
	if (currt != toWait)
	{
	    unsigned long timeWaited = 0;

	    timeout = FALSE;
	    currt->_theThread->commonData.threadStat = THR_IS_WAITING;

	    while ((toWait->status() != THR_IS_TERMINATED) &&
		   (timeWaited < waittime))
	    {
		Thread::sleep(waitQuantum);

		timeWaited += waitQuantum;
	    }

	    if (timeWaited > waittime)
		timeout = TRUE;
	    
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
	
	if (currt != toWait)
	{
	    if (toWait->status() != THR_IS_TERMINATED)
	    {
		currt->_theThread->commonData.threadStat = THR_IS_WAITING;

		waits(toWait->_theThread->waitLock);

		currt->_theThread->commonData.threadStat = THR_IS_RUNNING;
	    }
	    
	    if (toWait->_theThread->commonData.threadStat == THR_IS_TERMINATED)
		result = toWait->_theThread->commonData.result;
	}
    }

    return result;
}

/*
 * There is no real suspend/resume in Rex. Fake them here using a mutex
 * The mutex is grabbed in execute to ensure that a grab here will block
 */

Boolean Thread::resume ()
{
    if (_theThread->commonData.threadStat == THR_IS_SUSPENDED)
    {
	_theThread->commonData.threadStat = THR_IS_RUNNING;

	if (!_theThread->isSuspending)
	    signals(_theThread->suspendLock);
	else
	    _theThread->isSuspending = FALSE;
    }

    return (_theThread->commonData.threadStat == THR_IS_RUNNING);
}

Boolean Thread::suspend ()
{
    Thread* currt = Thread::current();

    /*
     * Make sure only the current thread can suspend itself in Rex.
     * If we aren't the current thread then set its status so it will
     * suspend itself later.
     */
    
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
	waits(_theThread->suspendLock);
	_theThread->commonData.threadStat = THR_IS_RUNNING;

	result = TRUE;
    }
    else
	result = (Boolean) (status() == Thread::THR_IS_SUSPENDED);

    return result;
}

Boolean Thread::kill ()
{
    if (status() == THR_IS_TERMINATED)
	return FALSE;

    killChildren();
    cleanup();
    
     _theThread->commonData.threadStat = THR_IS_TERMINATED;

     while (_theThread->waitLock->count != 0)
     {
	 signals(_theThread->waitLock);
	 yieldp();
     }
     
    if (_theThread->sysId)
    {
	destroyp(_theThread->sysId);
	_theThread->sysId = NULL;
    }

    return TRUE;
}

Boolean Thread::run ()
{
    if (status() != Thread::THR_IS_CREATED)
	return FALSE;
    
    signals(_theThread->suspendLock);

    if (status() == Thread::THR_IS_RUNNING)
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

    return (Thread*) getenvp(NULL);
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
	suicidep();
	th->_theThread->sysId = NULL;
    }
}

Boolean Thread::sleep (unsigned long sleepTime)
{
    Thread* currentThread = Thread::current();

    if (currentThread)
    {
	if (currentThread->_theThread->isSuspending)
	    currentThread->suspend();
	
	currentThread->_theThread->commonData.threadStat = THR_IS_BLOCKED;
	delayp((long) sleepTime);
	currentThread->_theThread->commonData.threadStat = THR_IS_RUNNING;

	return TRUE;
    }
    else
	return FALSE;
}

Boolean Thread::yield ()
{
    if (!_initialized)
        Thread::initialize();

    Thread* currt = Thread::current();

    if (currt->_theThread->isSuspending)
	currt->suspend();
    
    yieldp();
    
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
	 * Now wake up all waiters. Keep calling signal until the
	 * semaphore count is back to 0, at which point all waiters
	 * should be active.
	 */
	
	while (_theThread->waitLock->count != 0)
	{
	    signals(_theThread->waitLock);
	    yieldp();
	}

	yieldp();
	
	return TRUE;
    }
}

void* Thread::execute (void* startMe)
{
    if (startMe)
    {
	Thread* run = (Thread*) startMe;

	setenvp(NULL, (caddr_t) startMe);
	
	waits(run->_theThread->suspendLock);

	if (run->_theThread->commonData.threadStat == THR_IS_CREATED)
	{
	    waits(run->_theThread->suspendLock);
	}
	    
	run->_theThread->commonData.threadStat = THR_IS_RUNNING;
	
	if (run->_theThread->commonData.threadBody)
	    run->_theThread->commonData.result = run->_theThread->commonData.threadBody(run->_theThread->commonData.arg);

	Boolean detached = (run->_theThread->commonData.threadAstat == THR_IS_DETACHED);
	
	run->terminate();

	/*
	 * We are about to exit, so prevent the system from later attempting
	 * to call destroyp/suicidep on this thread. It will cause the system
	 * to crash.
	 */
	
	run->_theThread->sysId = NULL;

	if (detached)
	    Resource::unref(run);	
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

    initlp(Thread::defaultPriority);
    
    _list = new HashedList<Thread, Uid>;

    _initialized = TRUE;
    
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
    return new ThreadControl(this);
}

int Thread::priority () const
{
    if (status() != Thread::THR_IS_TERMINATED)
    {
	if (_theThread->sysId)
	    return _theThread->sysId->pri;
    }

    return Thread::InvalidPriority;
}

Boolean ThreadControl::setPriority (int prio)
{
    if (_toControl->status() != Thread::THR_IS_TERMINATED)
    {
	_toControl->_theThread->sysId->pri = prio;
	reschedp();

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
