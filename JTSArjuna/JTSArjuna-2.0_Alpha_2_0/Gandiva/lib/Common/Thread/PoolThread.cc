/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PoolThread.cc,v 1.3 1998/01/19 13:04:05 nmcl Exp $
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

#ifndef COMMON_THREAD_THREADSETUP_H_
#  include <Common/Thread/ThreadSetup.h>
#endif

#ifndef COMMON_THREAD_THREADPOOL_H_
#  include <Common/Thread/ThreadPool.h>
#endif

#ifndef COMMON_THREAD_POOLTHREAD_H_
#  include <Common/Thread/PoolThread.h>
#endif

PoolThread::PoolThread (ThreadSetup* setup)
		       : Thread(poolBody, this, setup),
			 _passivated(TRUE),
			 _destroyed(FALSE),
			 _attached(FALSE),
			 _running(FALSE),
			 _arg(NULL),
			 _result(NULL),
			 _threadBody(NULL)
{
    if (setup)
    {
	if (setup->attachStatus != Thread::THR_IS_DETACHED)
	    _attached = TRUE;

	if (setup->startState != Thread::THR_IS_SUSPENDED)
	    _running = TRUE;
    }
}

PoolThread::~PoolThread ()
{
}

void PoolThread::setDestroyed ()
{
    _destroyed = TRUE;
	
    if (status() == Thread::THR_IS_SUSPENDED)
	this->resume();
    else
    {
	if (status() == Thread::THR_IS_CREATED)
	    this->run();
	else
	    this->Thread::kill();
    }
}

void* PoolThread::poolBody (void* arg)
{
    PoolThread* t = (PoolThread*) arg;
    
    if (t)
    {
	for (;;)
	{
	    if (t->getDestroyed())
		break;

	    t->doWork();

	    /*
	     * Now call terminate so that other threads can still
	     * wait on this thread to "die". The pool thread
	     * fakes it's death, though.
	     */

	    t->terminate();

	    Thread::yield();
	    
	    /*
	     * Because of thread scheduling, it is possible that between
	     * the call to terminate and here, we have been re-used, and
	     * another thread wants us to run. So, we only suspend if
	     * we are still marked as passivated.
	     */

	    if (t->passivated())
	    {
		t->waitForWork();
	    }
	}
    }

    return 0;
}

/*
 * Fake terminate so that this pool thread can run again with
 * another body.
 */

Boolean PoolThread::terminate ()
{
    _passivated = TRUE;
    
    return Thread::terminate();
}

/*
 * Do the necessary action setup for this "new" thread.
 */

void PoolThread::setup ()
{
    Thread* currentThread = Thread::current();

    if (currentThread)
	this->currentAction(currentThread->currentAction());

    if (_attached)
    {
	this->addChild();
    }

    /* If thread should be running then make it so now */

    if (_running && (this->status() != THR_IS_RUNNING))
	this->run();
}

Boolean PoolThread::kill ()
{
    cerr << "WARNING - a thread from a thread pool is being killed." << endl;

    return Thread::kill();
}

#ifdef NO_INLINES
#  define POOLTHREAD_CC_
#  include <Common/Thread/PoolThread.n>
#  undef POOLTHREAD_CC_
#endif
