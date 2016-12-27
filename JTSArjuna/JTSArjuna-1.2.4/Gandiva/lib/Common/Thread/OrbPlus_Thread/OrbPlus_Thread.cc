/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPlus_Thread.cc,v 1.7 1998/08/29 10:36:40 nmcl Exp $
 */

/*
 * Implementation of threads for OrbPlus threads
 */

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

#ifndef TEMPLATE_HASHEDLIST_H_
#  include <Template/HashedList.h>
#endif

#ifndef CONTROL_THREADCONTROL_H_
#  include <Control/ThreadControl.h>
#endif

#ifndef COMMON_THREAD_THREADSETUP_H_
#  include <Common/Thread/ThreadSetup.h>
#endif

#ifndef COMMON_THREAD_ORBPLUS_THREAD_ORBPLUS_THREAD_H_
#  include <Common/Thread/OrbPlus_Thread/OrbPlus_Thread.h>
#endif

#ifndef TEMPLATE_BASICLISTITERATOR_H_
#  include <Template/BasicListIterator.h>
#endif

static MSD_Mutex globalMutex;
static MSD_ThreadSpecificData<Thread>* threadKey = 0;
static BasicList<Thread, Uid>* overflowList = 0;

const int Thread::MaxPriority = 127;
const int Thread::MinPriority = 0;
const int Thread::defaultPriority = Thread::MinPriority+10;
const unsigned long Thread::defaultStackSize = 4096;

ThreadData::ThreadData ( Thread::ThreadAttachStatus aStat,
			 Thread *th )
		       : commonData(aStat, th),
			 onOverflow(FALSE),
			 sysId(0),
			 dead(FALSE),
			 isSuspending(FALSE)
{
    suspendLock = new MSD_Mutex();
    waitLock = new MSD_Mutex();
}

ThreadData::~ThreadData ()
{
    delete suspendLock;
    delete waitLock;

    if (sysId)
	delete sysId;
}

/*
 * Typically used only to create a fake thread for main and for any threads
 * which are created out of our control, e.g., by an Orb.
 *
 * WARNING: we can lazily catch thread creation this way, but there
 * is no way we can catch thread termination.
 */

Thread::Thread (ThreadAttachStatus aStatus)
	       : _theThread(NULL),
		 _childList(NULL),
		 _cleanupList(NULL)    
{
    _theThread = new ThreadData(aStatus, this);
    _theThread->commonData.threadStat = THR_IS_RUNNING;
    _theThread->sysId = new MSD_Thread(MSD_Thread::self());
    (*threadKey) = this;

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
    int detach = 0;
    Thread::ThreadAttachStatus aStat = THR_IS_ATTACHED;
    Thread* currt = Thread::current();
    unsigned long stackSize = 0;
    
    if (setup)
    {
	aStat = setup->attachStatus;

	if (setup->stackSize > 0)
	    stackSize = setup->stackSize;	
    }
    
    _theThread = new ThreadData(aStat, this);

    if (setup && (setup->startState == THR_IS_RUNNING))
	_theThread->commonData.threadStat = THR_IS_RUNNING;
    
    if (aStat == THR_IS_DETACHED)
	detach = 1;
     
    _theThread->commonData.threadBody = body;
    _theThread->commonData.arg = arg;

    _theThread->sysId = new MSD_Thread(execute, (void*) this, detach, stackSize);

    Thread::yield();  // give thread a chance to run
	
    if (_theThread->sysId)
    {
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
	    Resource::ref(this);
	
	/* If thread should be running make it so */

	if (setup && (setup->startState == THR_IS_RUNNING))
	    this->run();
    }
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
	
	if (_theThread->onOverflow)
	    overflowList->remove(this);
	
	_mutex->unlock();
    }

    if (_theThread)
    {
	if (_theThread->commonData.threadAstat == THR_IS_ATTACHED)
	    _theThread->sysId->join(0);
	
	delete _theThread;
    }

    if (currentThread)
	MSD_Thread::exit(0);	
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

/*
 * TODO: Must be redone using conditions, or we cannot use them
 * in a thread pool.
 */

void* Thread::wait (Thread* toWait)
{
    void* result = 0;

    if (toWait)
    {
	if (currt->_theThread->isSuspending)
	    currt->suspend();

	if (currt->_theThread->dead)
	{
	    Thread::exit();
	}

	if (*toWait->_theThread->sysId != MSD_Thread::self())
	{
	    currt->_theThread->commonData.threadStat = THR_IS_WAITING;

	    toWait->_theThread->waitLock->lock();  // why no status return or exception thrown in error?

	    while (toWait->_theThread->commonData.threadStat != THR_IS_TERMINATED)
	    {
		toWait->_theThread->waiters.wait(*toWait->_theThread->waitLock);
	    }

	    toWait->_theThread->waitLock->unlock();

	    currt->_theThread->commonData.threadStat = THR_IS_RUNNING;
	    
	    if (toWait->_theThread->commonData.threadStat != THR_IS_TERMINATED)
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
 * There is no real suspend/resume in MSD. Fake them here using a mutex
 * The mutex is grabbed in execute to ensure that a grab here will block
 */

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
	 * Make sure only the current thread can suspend itself in OrbPlus.
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
    _theThread->dead = TRUE;
    
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

const Uid& Thread::uid () const
{
    return _theThread->commonData.threadId;
}

Thread* Thread::current ()
{
    if (!_initialized)
	Thread::initialize();
    
    Thread* aThread = (*threadKey);

    if (aThread == 0)
    {
	/*
	 * Trouble - we could not locate the current thread - it
	 * may be on the overflow list. Look there
	 */

	if (Thread::_mutex->lock() == Mutex::MX_DONE)
	{	
	    MSD_Thread me = MSD_Thread::self();
	    BasicListIterator<Thread, Uid> iter(*overflowList);

	    while ((aThread = iter()) != 0)
	    {
		if (*aThread->_theThread->sysId == me)
		    break;
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
	MSD_Thread::exit(0);
    }
}

Boolean Thread::sleep ( unsigned long usecs )
{
    Thread* currentThread = Thread::current();

    if (currentThread)
    {
	if (currentThread->_theThread->isSuspending)
	    currentThread->suspend();

	if (currentThread->_theThread->dead)
	    Thread::exit();
    
	currentThread->_theThread->commonData.threadStat = THR_IS_BLOCKED;

	/*
	 * In seconds!
	 */

	MSD_Thread::delay(usecs/1000000);
	
	currentThread->_theThread->commonData.threadStat = THR_IS_RUNNING;
    
	return TRUE;
    }

    return FALSE;
}

Boolean Thread::yield ()
{
    if (!_initialized)
	Thread::initialize();

    Thread* currt = Thread::current();

    if (currt->_theThread->isSuspending)    
	currt->suspend();

    if (currt->_theThread->dead)
	Thread::exit();
    
    MSD_Thread::yield();

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

	/*
	 * Now wake up any threads waiting for this thread
	 * to terminate.
	 */

	_theThread->waitLock->lock();
	_theThread->waiters.broadcast();
	_theThread->waitLock->unlock();
	_theThread->suspendLock->unlock();
	
        return TRUE;
    }
}

/*
 * MSD threads always start running immediately. If the user wanted them
 * created suspended we do it here
 */
 
void* Thread::execute ( void *startMe )
{
    if (startMe)
    {
	Boolean runThread = TRUE;
	Thread *run = (Thread *)startMe;

	(*threadKey) = run;
	
	if (FALSE)
	{
	    /*
	     * Out of thread specific memory - add thread onto overflow list
	     * so current() can find it the hard way
	     */

	    if (Thread::_mutex->lock() == Mutex::MX_DONE)
	    {
		overflowList->insert(run);
		run->_theThread->onOverflow = TRUE;
	    }

	    Thread::_mutex->unlock();	    
	}

	/*
	 * Grab suspend mutex. This ensures that if I grab again (via suspend)
	 * I will block.
	 */

	run->_theThread->suspendLock->lock();

	if (run->_theThread->commonData.threadStat == THR_IS_CREATED)
	{
	    run->_theThread->suspendLock->lock();
	}
	
	run->_theThread->commonData.threadStat = THR_IS_RUNNING;
	
	if (run->_theThread->commonData.threadBody)
	    run->_theThread->commonData.result = run->_theThread->commonData.threadBody(run->_theThread->commonData.arg);

	Boolean detached = (run->_theThread->commonData.threadAstat == THR_IS_DETACHED);
	
	run->terminate();

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

    _list = new HashedList<Thread, Uid>;
    threadKey = new MSD_ThreadSpecificData<Thread>();
    overflowList = new BasicList<Thread, Uid>;
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
    return Thread::defaultPriority;
}

Boolean ThreadControl::setPriority (int prio)
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
