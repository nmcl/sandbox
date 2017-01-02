/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RCPORB_Thread.cc,v 1.8 1998/08/29 10:36:42 nmcl Exp $
 */

/*
 * Implementation of threads for ON threads
 */

#include <os/CORBA.ch>
#include <os/ON_init.h>
#include <os/ON_sync.h>

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

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

#ifndef COMMON_THREAD_THREADCOMMON_H_
#  include <Common/Thread/ThreadCommon.h>
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

#ifndef COMMON_THREAD_RCPORB_THREAD_RCPORB_THREAD_H_
#  include <Common/Thread/RCPORB_Thread/RCPORB_Thread.h>
#endif

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

#ifndef ON_STACK_MIN
#  define ON_STACK_MIN 102400
#endif

const int Thread::MaxPriority = 127;
const int Thread::MinPriority = 0;
const int Thread::defaultPriority = ON_thread_getprio(ON_thread_self());
const unsigned long Thread::defaultStackSize = ON_STACK_MIN;

static Thread* mainThread = 0;
static ODS_CORBA::Mutex* globalMutex = NULL;
static ON_ThreadKey threadKey;
static BasicList<Thread, Uid> overflowList;

ThreadData::ThreadData ( Thread::ThreadAttachStatus aStat,
			 Thread *th )
		       : commonData(aStat, th),
			 onOverflow(FALSE),
			 sysId(NULL),
			 isSuspending(FALSE)
{
    suspendLock = new ODS_CORBA::Mutex();
    waitLock = new ODS_CORBA::Mutex();
    waiters = new ODS_CORBA::CondMutex(*waitLock);

    /*
     * Do this here rather than in execute as normal because
     * ON threads gives undefined behaviour if the same thread
     * tries to lock a mutex multiple times (despite the fact that
     * the underlying thread package may allow this.)
     */
    
    suspendLock->lock();
}

ThreadData::~ThreadData ()
{
    delete suspendLock;
    delete waiters;
    delete waitLock;
    
    if (sysId)
    {
	delete sysId;
    }
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
    _theThread->sysId = new ODS_CORBA::Thread(ON_thread_self());
    ON_thread_setspecific(threadKey, this);
    
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
 *
 * WARNING:
 * Do not add the thread to any lists until AFTER the _create call is made
 */

Thread::Thread ( void * (*body)(void *), void *arg, ThreadSetup* setup)
	       : _theThread(NULL),
		 _childList(NULL),
		 _cleanupList(NULL)		 
{
    ON_Boolean detachState = ON_FALSE;
    Thread::ThreadAttachStatus aStat = THR_IS_ATTACHED;
    int prio = Thread::defaultPriority;
    Thread* currt = Thread::current();
    unsigned long stackSize = 0;
    
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

    if (aStat != THR_IS_DETACHED)
    {
	_theThread->commonData.action = Thread::current()->_theThread->commonData.action;
    }
    else
	detachState = ON_TRUE;
     
    _theThread->commonData.threadBody = body;
    _theThread->commonData.arg = arg;

    _theThread->sysId = new ODS_CORBA::Thread(execute, (void*) this,
					      (ON_Priority) prio, detachState, ON_FALSE, 0, stackSize);

    if (_theThread->sysId)
    {
	Thread::yield();  // give thread a chance to run

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
	    
	if (setup && (setup->startState == THR_IS_RUNNING))
	    this->run();
    }
    else
    {
	/* Thread create failed - mark as dead */
	    
	_theThread->commonData.threadStat = THR_IS_TERMINATED;
    }
}

Thread::~Thread ()
{
    Boolean currentThread = (Thread::current() == this);

    if (status() != THR_IS_CREATED)    
	Thread::terminate();

    Thread::yield();
    
    if (_mutex->lock() == Mutex::MX_DONE)
    {
        list().remove(this, uid());
	
	if (_theThread->onOverflow)
	    overflowList.remove(this);
	
	_mutex->unlock();
    }
    
    if (_theThread)
    {
	void** ptr = 0;
	
	if (_theThread->commonData.threadAstat == THR_IS_ATTACHED)
	    _theThread->sysId->join(ptr);
	
	delete _theThread;
    }

    if (currentThread)
	ODS_CORBA::Thread::exit(0);	
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
 * Since ON threads only allow a single thread to wait for another
 * we do not use ON_join here. Instead we use conditions
 */

void* Thread::wait (Thread* toWait)
{
    void* result = 0;
    
    if (toWait)
    {
	Thread* currt = Thread::current();

	if (currt->_theThread->isSuspending)
	    currt->suspend();
	
	if (toWait->_theThread && (toWait->_theThread->sysId->get_thread() != ON_thread_self()))
	{
	    currt->_theThread->commonData.threadStat = THR_IS_WAITING;
	    
	    toWait->_theThread->waitLock->lock();
	
	    while (toWait->_theThread->commonData.threadStat != THR_IS_TERMINATED)
	    {
		if (toWait->_theThread->waiters->wait() != ON_SUCCESS)
		{
		    break;
		}
	    }

	    toWait->_theThread->waitLock->unlock();

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
 * There is no real suspend/resume in POSIX. Fake them here using a mutex
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
    }

    return (_theThread->commonData.threadStat == THR_IS_RUNNING);
}

/*
 * This is static so only the current thread can suspend itself
 */

Boolean Thread::suspend ()
{
    Boolean result = FALSE;
    
    Thread *th = Thread::current();

    if (th && th->_theThread)
    {
	/*
	 * Make sure only the current thread can suspend itself in RCP.
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

    _theThread->waitLock->lock();
    _theThread->waiters->broadcast();
    _theThread->waitLock->unlock();

    /*
     * No easy way to kill the thread like pthread_kill(_theThread->sysId, SIGTERM)
     * so fill in the blanks later!
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
    if (!_initialized)
        Thread::initialize();

    void* tmpPtr = 0;
    Thread* aThread = 0;

    if (ON_thread_getspecific(threadKey, &tmpPtr) != 0)
    {
	/*
	 * Trouble - we could not locate the current thread - it
	 * may be on the overflow list. Look there
	 */
	
	if (_mutex->lock() == Mutex::MX_DONE)
	{
	    ON_Thread me = ON_thread_self();
	    BasicListIterator<Thread, Uid> iter(overflowList);
	    Thread *t = 0;

	    while ((t = iter()) != 0)
	    {
		if (t->_theThread->sysId->get_thread() == me)
		{
		    aThread = t;
		    break;
		}
	    }

	    _mutex->unlock();
	}
    }
    else
    {
	aThread = (Thread*) tmpPtr;
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
	ODS_CORBA::Thread::exit(0);
    }
}

Boolean Thread::sleep ( unsigned long usecs )
{
    Thread *currentThread = Thread::current();
    
    if (currentThread != NULL)
    {
	if (currentThread->_theThread->isSuspending)
	    currentThread->suspend();
	
	ON_TimeSpec to;
	ON_TimeSpec remain;
	
	currentThread->_theThread->commonData.threadStat = THR_IS_BLOCKED;

	to.tv_sec = usecs/1000000;
	to.tv_nsec = (usecs%1000000) * 1000; /* convert to nanoseconds */

	while (to.tv_nsec >= 1000000000)
	{
	    to.tv_sec++;
	    to.tv_nsec -= 1000000000;
	}

	ODS_CORBA::Thread::nanosleep(&to, &remain);
	
	currentThread->_theThread->commonData.threadStat = THR_IS_RUNNING;
    
	return TRUE;
    }

    return FALSE;
}

Boolean Thread::yield ()
{
    Thread* currt = Thread::current();

    if (currt->_theThread->isSuspending)
	currt->suspend();
    
    ODS_CORBA::Thread::yield();

    return TRUE;
}

Boolean Thread::terminate ()
{
    if (_theThread->commonData.threadStat == THR_IS_TERMINATED)
	return TRUE;
    
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
	
	_theThread->waitLock->lock();
	_theThread->waiters->broadcast();
	_theThread->waitLock->unlock();

	Thread::yield();
    }
    
    return (_theThread->commonData.threadStat == THR_IS_TERMINATED);
}

void *Thread::execute ( void *startMe )
{
    if (startMe)
    {
	Boolean runThread = TRUE;
	Thread *run = (Thread *)startMe;

	if (ON_thread_setspecific(threadKey, startMe) == ENOMEM)
	{
	    /*
	     * Out of thread specific memory - add thread onto overflow list
	     * so current() can find it the hard way
	     */

	    overflowList.insert(run);
	    run->_theThread->onOverflow = TRUE;
	}

	/*
	 * Grab suspend mutex. This ensures that if I grab again (via suspend)
	 * I will block.
	 *
	 * We already acquired the lock in the ThreadData constructor.
	 */

	if (run->_theThread->commonData.threadStat == THR_IS_CREATED)
	    run->_theThread->suspendLock->lock();
	
	run->_theThread->commonData.threadStat = THR_IS_RUNNING;
	if (run->_theThread->commonData.threadBody)
	    run->_theThread->commonData.result = run->_theThread->commonData.threadBody(run->_theThread->commonData.arg);

	Boolean detached = (run->_theThread->commonData.threadAstat == THR_IS_DETACHED);
	
	run->terminate();

	ODS_CORBA::Thread::yield();
	
	run->_theThread->suspendLock->unlock();

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
    
    if (mainThread == 0)
    {
	ON_nucleus_initialise(0);

	globalMutex = new ODS_CORBA::Mutex();
	
	ON_task_pool_set_hwm(ON_task_pool_getgeneral(), 100);
	
	/*
	 * This ensures that we get ThreadMutex::create() from the ON lib
	 * and not from elsewhere since it creates an unresolved ref.
	 */

	_list = new HashedList<Thread, Uid>;
	_initialized = TRUE;

	_mutex = Mutex::create();
	Resource::ref(_mutex);

	ON_thread_key_create(&threadKey, 0);
	
	mainThread = new Thread();
	Resource::ref(mainThread);
    }
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
	return (int) _theThread->sysId->getprio();
    }
    else
	return Thread::InvalidPriority;
}

Boolean ThreadControl::setPriority (int prio)
{
    if (_toControl->status() != Thread::THR_IS_TERMINATED)
    {
	if (_toControl->_theThread->sysId->setprio((ON_Priority) prio) == ON_SUCCESS)
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
