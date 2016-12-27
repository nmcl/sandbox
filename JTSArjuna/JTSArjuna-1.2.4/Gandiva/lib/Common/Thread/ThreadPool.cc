/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadPool.cc,v 1.2 1998/01/16 10:28:15 nmcl Exp $
 */

#ifndef GANDIVA_H_
#  include <Gandiva.h>
#endif

#ifndef COMMON_THREAD_THREADSETUP_H_
#  include <Common/Thread/ThreadSetup.h>
#endif

#ifndef COMMON_THREAD_POOLTHREAD_H_
#  include <Common/Thread/PoolThread.h>
#endif

#ifndef COMMON_THREAD_THREADPOOL_H_
#  include <Common/Thread/ThreadPool.h>
#endif

ThreadPool::ThreadPool (unsigned int numberOfThreads, ThreadSetup* setup)
		       : _setup(0),
			 _threadPool(0),
			 _numberOfThreads(numberOfThreads)
{
    if (_numberOfThreads > 0)
    {
	if (setup)
	    _setup = new ThreadSetup(*setup);

	_threadPool = new Array<void*>(_numberOfThreads);

	/*
	 * If the threads are to be created running, then do this
	 * after they are created: PoolThread is derived from Thread and
	 * may not have been setup prior to the thread running.
	 */

	Boolean running = FALSE;

	if (_setup && _setup->startState == Thread::THR_IS_RUNNING)
	{
	    _setup->startState = Thread::THR_IS_SUSPENDED;
	    running = TRUE;
	}

	initialize();

	/*
	 * Now start the threads running if required.
	 */

	if (running)
	{
	    _setup->startState = Thread::THR_IS_RUNNING;

	    for (unsigned int i = 0; i < _numberOfThreads; i++)
	    {
		PoolThread* t = (PoolThread*) (*_threadPool)[i];
		t->run();
	    }
	}
    }
}

ThreadPool::~ThreadPool ()
{
    if (_threadPool)
    {
	for (unsigned int i = 0; i < _numberOfThreads; i++)
	{
	    if ((*_threadPool)[i])
	    {
		PoolThread* t = (PoolThread*) (*_threadPool)[i];

		t->setDestroyed();
		Resource::unref(t);
		
		(*_threadPool)[i] = NULL;
	    }
	}

	delete _threadPool;
    }
    
    if (_setup)
	delete _setup;
}

Resource* ThreadPool::control (const ClassName&)
{
    return (Resource*) 0;
}

const ClassName& ThreadPool::name ()
{
    return Gandiva::Common::ThreadPool::name();
}

const ClassName& ThreadPool::className () const
{
    return Gandiva::Common::ThreadPool::name();
}

void* ThreadPool::castup (const ClassName& _type) const
{
    if (_type == ThreadPool::name())
	return (void*) this;
    else
	return (void*) 0;
}

ThreadPool* ThreadPool::castup (Resource* toCast)
{
    if (toCast == (Resource*) 0)
	return (ThreadPool*) 0;
    else
	return (ThreadPool*) toCast->castup(ThreadPool::name());
}    
