/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Single_Thread.cc,v 1.4 1998/08/29 10:36:43 nmcl Exp $
 */

#ifndef OS_UNISTD_H_
#  include <os/unistd.h>
#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef OS_SYS_TYPES_H_
#  include <os/sys/types.h>
#endif

#ifndef OS_STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef COMMON_THREAD_SINGLE_THREAD_SINGLE_THREAD_H_
#  include <Common/Thread/Single_Thread/Single_Thread.h>
#endif

#ifndef COMMON_THREAD_CLEANUPLIST_H_
#  include <Common/Thread/CleanupList.h>
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
const unsigned long Thread::defaultStackSize = 0;

static Thread *mainThread = 0;
static Thread *currentThread = 0;

/*
 * Define those routines which are specific to the given thread
 * implementation selected.
 */

ThreadData::ThreadData ( Thread::ThreadAttachStatus aStat, Thread *th )
		       : commonData(aStat, th),
			 lastThread(0)
{
}

ThreadData::~ThreadData ()
{
}

Thread::Thread (ThreadAttachStatus aStatus)
	       : _theThread(new ThreadData(aStatus, this)),
		 _childList(NULL),
		 _cleanupList(NULL)
    
{
    _theThread->commonData.threadStat = THR_IS_RUNNING;
    list().insert(this, uid());
}

Thread::Thread (void* (*body)(void*), void* arg, ThreadSetup* setup)
	       : _theThread(NULL),
		 _childList(NULL),
		 _cleanupList(NULL)
{
    Thread::ThreadAttachStatus aStat = Thread::THR_IS_ATTACHED;

    if (setup)
	aStat = setup->attachStatus;

    _theThread = new ThreadData(aStat, this);

    if (currentThread)
	this->currentAction(currentThread->currentAction());
    
    _theThread->commonData.threadBody = body;
    _theThread->commonData.arg = arg;

    list().insert(this, uid());
    
    if (_theThread->commonData.threadAstat == THR_IS_ATTACHED)
        this->addChild();

    if (setup && setup->startState == THR_IS_RUNNING)
        this->run();
}

Thread::~Thread ()
{
    if (_theThread)
    {
	if (_theThread->commonData.threadStat != THR_IS_TERMINATED)
	{
	    terminate();
	}
    }

    /*
     * Note: we may be in trouble here if the static list
     * no longer exists when we come to remove ourselves.
     * Can we rely upon the pointer being NULL?
     */

    list().remove(this, uid());

    if (_theThread)
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

void* Thread::wait (Thread* toWait, long waittime, Boolean& timeout)
{
    return wait(toWait);
}

void* Thread::wait (Thread* toWait)
{
    void* result = 0;
    
    if (toWait)
    {
	/* Cannot wait for oneself */

	if (currentThread != toWait)
	{
	    if (toWait->_theThread && toWait->_theThread->commonData.threadStat != THR_IS_TERMINATED)
	    {
		toWait->_theThread->commonData.threadStat = Thread::THR_IS_WAITING;
		toWait->resume();
		toWait->_theThread->commonData.threadStat = Thread::THR_IS_RUNNING;
	    }

	    if (toWait->_theThread->commonData.threadStat == THR_IS_TERMINATED)
		result = toWait->_theThread->commonData.result;
	}
    }

    return result;
}

Boolean Thread::resume ()
{
    if (_theThread)
    {
	if (_theThread->commonData.threadStat == THR_IS_SUSPENDED)
	{
	    _theThread->commonData.threadStat = THR_IS_RUNNING;
	    _theThread->lastThread = currentThread;
	    currentThread = this;

	    if (_theThread->commonData.threadBody)
	    {
		_theThread->commonData.result = _theThread->commonData.threadBody(_theThread->commonData.arg);
	    }
	    
	    terminate();
	    
	    currentThread = _theThread->lastThread;
	    _theThread->lastThread = 0;
	    
	    return TRUE;
	}
    }

    return FALSE;
}

/*
 * Cannot suspend in a non-threaded environment
 */

Boolean Thread::suspend ()
{
    return FALSE;
}

Boolean Thread::run ()
{
    if (_theThread && _theThread->commonData.threadStat == THR_IS_CREATED)
    {
	_theThread->commonData.threadStat = THR_IS_SUSPENDED;
	
	return resume();
    }
    else
	return FALSE;
}

Boolean Thread::kill ()
{
    if (status() != THR_IS_TERMINATED)
	return FALSE;
    else
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

    return currentThread;
}

Thread* Thread::find (const Uid& id)
{
    if (!_initialized)
        Thread::initialize();

    const BasicList<Thread, Uid>* theList = list().find(id);

    if (theList)
        return theList->find(id);
    else
        return (Thread*) 0;
}

void Thread::exit ()
{
}

Boolean Thread::sleep (unsigned long sleepTime)
{
    if (currentThread)
    {
	currentThread->_theThread->commonData.threadStat = Thread::THR_IS_BLOCKED;
	usleep((unsigned int) sleepTime);
	currentThread->_theThread->commonData.threadStat = Thread::THR_IS_RUNNING;
	
	return TRUE;
    }

    return FALSE;
}

Boolean Thread::yield ()
{
    return FALSE;
}

Boolean Thread::terminate ()
{
    if (status() != Thread::THR_IS_TERMINATED)
    {
	Thread::waitForAllChildren();
	cleanup();
	
	_theThread->commonData.threadStat = Thread::THR_IS_TERMINATED;
    }

    return TRUE;
}

Thread* Thread::create (void* (*body)(void*), void* arg, ThreadSetup* setup)
{
    if (!_initialized)
	Thread::initialize();
    
    return new Thread(body, arg, setup);
}

void *Thread::execute ( void *startMe )
{
    if (startMe)
    {
	Thread *run = (Thread *)startMe;
	
	if (run->_theThread->commonData.threadAstat == THR_IS_DETACHED)
	    Resource::ref(run);

	run->_theThread->commonData.threadStat = THR_IS_RUNNING;
	if (run->_theThread->commonData.threadBody)
	    run->_theThread->commonData.result = run->_theThread->commonData.threadBody(run->_theThread->commonData.arg);

	run->_theThread->commonData.threadStat = THR_IS_TERMINATED;

	if (run->_theThread->commonData.threadAstat == THR_IS_DETACHED)
	    Resource::unref(run);
    }

    return 0;
}

Boolean Thread::initialize ()
{
    if (_initialized)
        return TRUE;

    _list = new HashedList<Thread, Uid>;  // set list before we use it.
    _mutex = Mutex::create();
    Resource::ref(_mutex);
    
    _initialized = TRUE;

    currentThread = mainThread = new Thread();
    Resource::ref(currentThread);

    return TRUE;
}

Resource* Thread::control (const ClassName&)
{
    return (Resource*) 0;
}

int Thread::priority () const
{
    if (status() != Thread::THR_IS_TERMINATED)
	return Thread::defaultPriority;
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
