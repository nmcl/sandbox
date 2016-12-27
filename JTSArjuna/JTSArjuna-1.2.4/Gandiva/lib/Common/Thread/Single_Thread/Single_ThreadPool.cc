/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Single_ThreadPool.cc,v 1.2 1998/01/19 13:06:07 nmcl Exp $
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

#ifndef COMMON_THREAD_THREADPOOL_H_
#  include <Common/Thread/ThreadPool.h>
#endif

#ifndef COMMON_THREAD_POOLTHREAD_H_
#  include <Common/Thread/PoolThread.h>
#endif

#ifndef COMMON_THREAD_SINGLE_THREAD_SINGLE_THREAD_H_
#  include <Common/Thread/Single_Thread/Single_Thread.h>
#endif

/*
 * Implementation specific ThreadPool methods.
 *
 * For single threaded system we simply guarantee that a given
 * pool object cannot create an arbitrary number of threads.
 */

void ThreadPool::initialize ()
{
    for (int i = 0; i < _numberOfThreads; i++)
    {
	(*_threadPool)[i] = 0;
    }
}

void PoolThread::assignThread (void* (*)(void*), void*)
{
}
    
Thread* ThreadPool::create (void* (*body)(void*), void* param, Boolean)
{
    if (_numberOfThreads <= 0)
	return (Thread*) 0;
    
    array_t index = 0;
    array_t i = 0;
    Boolean found = FALSE;
    Thread* t = (Thread*) 0;

    for (i = 0; (i < _numberOfThreads) && (!found); i++)
    {
	if ((*_threadPool)[i] == NULL)
	{
	    index = i;
	    found = TRUE;
	}
	else
	{
	    t = (Thread*) (*_threadPool)[i];

	    Thread::ThreadStatus status = t->status();

	    if ((status == Thread::THR_IS_PASSIVATED) ||
		(status == Thread::THR_IS_TERMINATED))
	    {
		index = i;
		found = TRUE;
	    }
	}
    }

    if (found)
    {
	t = (Thread*) (*_threadPool)[index];
	Resource::unref(t);

	t = Thread::create(body, param);
	Resource::ref(t);
	
	(*_threadPool)[index] = t;

	return t;
    }
    
    return (Thread*) 0;
}

Boolean PoolThread::waitForWork ()
{
    /*
     * If status is not THR_IS_RUNNING, then make it so to
     * allow us to call suspend.
     */

    if (_theThread->commonData.threadStat == THR_IS_TERMINATED)
    {
	Thread::yield();
	
	_theThread->commonData.threadStat = THR_IS_RUNNING;
    }

    return Thread::suspend();
}
