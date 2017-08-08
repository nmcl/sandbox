/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NT_Thread.cc,v 1.12 1998/08/29 10:36:39 nmcl Exp $
 */

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef COMMON_THREAD_THREADSETUP_H_
#  include <Common/Thread/ThreadSetup.h>
#endif

#ifndef COMMON_THREAD_NT_THREAD_NT_THREAD_H_
#  include <Common/Thread/NT_Thread/NT_Thread.h>
#endif

#ifndef CONTROL_THREADCONTROL_H_
#  include <Control/ThreadControl.h>
#endif

#ifndef TEMPLATE_HASHEDLIST_H_
#  include <Template/HashedList.h>
#endif

/*
 * error need a mutex for status change to do wait properly.
 */

const int Thread::MaxPriority = THREAD_PRIORITY_ABOVE_NORMAL;
const int Thread::MinPriority = THREAD_PRIORITY_BELOW_NORMAL;
const int Thread::defaultPriority = THREAD_PRIORITY_NORMAL;
const unsigned long Thread::defaultStackSize = 2048;

BasicList<Thread, Uid>* ThreadData::overflowList = 0;

static DWORD globalKey;

ThreadData::ThreadData (Thread::ThreadAttachStatus aStat,
			Thread* th)
		       : commonData(aStat, th),
			 sysId(0),
			 waitLock(0),
			 onOverflow(FALSE)			 
{
    /*
     * Use a mutex to signal thread termination so we can
     * reuse the thread in a thread pool model.
     *
     * DO NOT acquire the lock at this stage, as the
     * owning thread must do this in execute.
     */
     
    waitLock = CreateMutex(NULL, 0, NULL);
}

ThreadData::~ThreadData ()
{
    CloseHandle(waitLock);

    CloseHandle(sysId);
}


Thread::Thread (ThreadAttachStatus aStatus)
	       : _theThread(NULL),
		 _childList(NULL),
		 _cleanupList(NULL)
{
    _theThread = new ThreadData(aStatus, this);
    _theThread->commonData.threadStat = THR_IS_RUNNING;
    _theThread->mid = GetCurrentThreadId();

    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(),
		    GetCurrentProcess(), &_theThread->sysId,
		    0, TRUE, DUPLICATE_SAME_ACCESS);
    
    TlsSetValue(globalKey, this);

    SetThreadPriority(GetCurrentThread(), Thread::defaultPriority);
    
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

/*
 * Does NT have a form of detach for threads?
 */

Thread::Thread (void* (*func)(void*), void* arg, ThreadSetup* setup)
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
    
    _theThread->sysId = CreateThread(NULL, stackSize,
				     (LPTHREAD_START_ROUTINE) Thread::execute,
				     this, CREATE_SUSPENDED, &_theThread->mid);

    SetThreadPriority(_theThread->sysId, prio);

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
    Boolean currentThread = (GetCurrentThreadId() == _theThread->mid);

    if (status() != THR_IS_CREATED)
	Thread::terminate();

    Thread::yield();

    if (_mutex->lock() == Mutex::MX_DONE)
    {
	list().remove(this, uid());

	if (_theThread->onOverflow)
	{
	    ThreadData::overflowList->remove(this);
	}
	else
	{
	    /*
	     * Can only do this if we are the current thread.
	     */
	    
	    if (currentThread)
		TlsSetValue(globalKey, 0);
	}
		
	_mutex->unlock();
    }

    if (_theThread)
    {
	if (_theThread->commonData.threadAstat == THR_IS_ATTACHED)
	    WaitForSingleObject(_theThread->sysId);
	
	delete _theThread;
    }

    if (currentThread)
	ExitThread(0);
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
	if (toWait->_theThread && (toWait->_theThread->mid != GetCurrentThreadId()))
	{
	    /*
	     * Just in case the thread is still in the created state, we must
	     * wait for it to change, which signals it's ok for us to try to
	     * acquire the "synchronization" mutex.
	     */

	    while (toWait->status() == THR_IS_CREATED)
		Thread::yield();

	    timeout = FALSE;
	
	    if (toWait->status() != THR_IS_TERMINATED)
	    {
		Thread* currt = Thread::current();
		DWORD done;

		currt->_theThread->commonData.threadStat = THR_IS_WAITING;

		do
		{
		    done = WaitForSingleObject(toWait->_theThread->waitLock, waittime);
		    
		} while (done != WAIT_OBJECT_0);

		// in case other threads are waiting ...
		
		ReleaseMutex(toWait->_theThread->waitLock);

		currt->_theThread->commonData.threadStat = THR_IS_RUNNING;

		if (toWait->_theThread->commonData.threadStat == THR_IS_TERMINATED)
		    result = toWait->_theThread->commonData.result;	    
	    }
	}
    }

    return result;
}

void* Thread::wait (Thread* toWait)
{
    void* result = 0;
    
    if (toWait)
    {
	if (toWait->_theThread && (toWait->_theThread->mid != GetCurrentThreadId()))
	{
	    /*
	     * Just in case the thread is still in the created state, we must
	     * wait for it to change, which signals it's ok for us to try to
	     * acquire the "synchronization" mutex.
	     */

	    while (toWait->status() == THR_IS_CREATED)
		Thread::yield();
	    
	    if (toWait->status() != THR_IS_TERMINATED)
	    {
		Thread* currt = Thread::current();
		DWORD done;

		currt->_theThread->commonData.threadStat = THR_IS_WAITING;

		do
		{
		    done = WaitForSingleObject(toWait->_theThread->waitLock, INFINITE);
		    
		} while (done != WAIT_OBJECT_0);

		// in case other threads are waiting ...

		ReleaseMutex(toWait->_theThread->waitLock);

		/*
		 * Can set this outside of mutex since must be a different
		 * thread to the one we (and possibly others) are waiting on.
		 */

		currt->_theThread->commonData.threadStat = THR_IS_RUNNING;

		if (toWait->_theThread->commonData.threadStat == THR_IS_TERMINATED)
		    result = toWait->_theThread->commonData.result;
	    }
	}
    }

    return result;
}

Boolean Thread::resume ()
{
    if (_theThread->commonData.threadStat == THR_IS_SUSPENDED)
    {
	_theThread->commonData.threadStat = THR_IS_RUNNING;    

	return (ResumeThread(_theThread->sysId) > 0);
    }
    else
	return (status() == THR_IS_RUNNING);
}

Boolean Thread::suspend ()
{
    if (status() == Thread::THR_IS_RUNNING)
    {
	_theThread->commonData.threadStat = THR_IS_SUSPENDED;
	SuspendThread(_theThread->sysId);
	return TRUE;
    }
    else
	return (status() == THR_IS_SUSPENDED);
}

Boolean Thread::kill ()
{
    if (status() != THR_IS_TERMINATED)
    {
	killChildren();
	cleanup();

	_theThread->commonData.threadStat = THR_IS_TERMINATED;
	
	TerminateThread(_theThread->sysId, 0);

	return TRUE;
    }
    else
	return FALSE;
}
    
Boolean Thread::run ()
{
    if (status() != THR_IS_CREATED)
	return FALSE;

    if (ResumeThread(_theThread->sysId) > 0)
    {
	/*
	 * If non-preemptive scheduling, then we have to change the
	 * status here. Otherwise, it may have already been changed
	 * from THR_IS_CREATED to something else, i.e., we can't just
	 * make it THR_IS_RUNNING in case the thread is actually now
	 * suspended!
	 */
	
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

Thread* Thread::current ()
{
    if (!_initialized)
	Thread::initialize();

    LPVOID p1 = TlsGetValue(globalKey);

    if ((p1) || ((!p1) && (GetLastError() == NO_ERROR)))
	return (Thread*) p1;

    /*
     * On overflow list?
     */
    
    if (_mutex->lock() == Mutex::MX_DONE)
    {
	BasicListIterator<Thread, Uid> iter(*ThreadData::overflowList);
	DWORD tid = GetCurrentThreadId();

	for (Thread* t = iter(); t; t = iter())
	{
	    if (tid == t->_theThread->mid)
	    {
		_mutex->unlock();
		return t;
	    }
	}

	_mutex->unlock();
    }

    return (Thread*) 0;
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
	ExitThread(0);
    }
}

/*
 * Sleep time is in microseconds, but NT sleep function only works in
 * milliseconds. Do some conversion here.
 */

Boolean Thread::sleep (unsigned long sleepTime)
{
    Thread* currentThread = Thread::current();

    if (!currentThread)
	return FALSE;

    currentThread->_theThread->commonData.threadStat = THR_IS_BLOCKED;

    Sleep(sleepTime/1000);

    currentThread->_theThread->commonData.threadStat = THR_IS_RUNNING;    
	
    return TRUE;
}

Boolean Thread::yield ()
{
    if (!_initialized)
	Thread::initialize();
    
    Sleep(0);
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

	ReleaseMutex(_theThread->waitLock);
    
	Thread::yield();
	
	return TRUE;
    }
}

void Thread::execute (void* p1)
{
    if (startMe)
    {
	Thread* run = (Thread*) startMe;
	
	if (! TlsSetValue(globalKey, run))
	{
	    // put on to overflow list

	    if (_mutex->lock() == Mutex::MX_DONE)
	    {
		ThreadData::overflowList->insert(run);
		run->_theThread->onOverflow = TRUE;

		_mutex->unlock();
	    }
	}

	/*
	 * Before setting our status to running, grab the waiters lock
	 * so other threads will block until we terminate, and signal it.
	 */

	WaitForSingleObject(run->_theThread->waitLock, INFINITE);

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

    globalKey = TlsAlloc();
    
    _list = new HashedList<Thread, Uid>;
    ThreadData::overflowList = new BasicList<Thread, Uid>;

    _initialized = TRUE;
    
    _mutex = Mutex::create();
    Resource::ref(_mutex);

    Thread* mainThread = new Thread();
    Resource::ref(mainThread);

    return TRUE;
}

Thread* Thread::create (void* (*body)(void*), void* arg, ThreadSetup* setup)
{
    if (!_initialized)
	Thread::initialize();

    return new Thread(body, arg, setup);
}

Resource* Thread::control (const ClassName&)
{
    return new ThreadControl(this);
}

int Thread::priority () const
{
    if (status() != THR_IS_TERMINATED)
	return GetThreadPriority(_theThread->sysId);
    else
	return Thread::InvalidPriority;
}

Boolean ThreadControl::setPriority (int prio)
{
    if (_toControl->status() != Thread::THR_IS_TERMINATED)
    {
	SetThreadPriority(_toControl->_theThread->sysId, prio);
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
