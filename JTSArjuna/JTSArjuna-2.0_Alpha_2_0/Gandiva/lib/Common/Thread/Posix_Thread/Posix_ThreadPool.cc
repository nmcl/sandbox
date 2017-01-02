/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Posix_ThreadPool.cc,v 1.3 1998/01/19 13:06:06 nmcl Exp $
 */

#ifndef OS_ERRNO_H_
#  include <os/errno.h>
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

#if defined(PTHREAD_SUN) && !defined(OS_THREAD_H_)
#  include <os/thread.h>
#endif

#ifndef COMMON_THREAD_THREADPOOL_H_
#  include <Common/Thread/ThreadPool.h>
#endif

#ifndef COMMON_THREAD_POOLTHREAD_H_
#  include <Common/Thread/PoolThread.h>
#endif

#ifndef COMMON_THREAD_POSIX_THREAD_POSIX_THREAD_H_
#  include <Common/Thread/Posix_Thread/Posix_Thread.h>
#endif

void ThreadPool::initialize ()
{
    for (int i = 0; i < _numberOfThreads; i++)
    {
	PoolThread* pt = new PoolThread(_setup);
	Resource::ref(pt);
	(*_threadPool)[i] = pt;
    }
}

Thread* ThreadPool::create (void* (*body)(void*), void* param, Boolean)
{
    if (_numberOfThreads <= 0)
	return (Thread*) 0;

    array_t index = 0;
    Boolean found = FALSE;
    
    for (array_t i = 0; i < _numberOfThreads; i++)
    {
	PoolThread* t = (PoolThread*) (*_threadPool)[i];

	if (t && t->passivated())
	{
	    index = i;
	    found = TRUE;
	    break;
	}
	else
	{
	    if (t && t->status() == Thread::THR_IS_TERMINATED)
	    {
		cerr << "WARNING: thread in pool marked as terminated, but not finished." << endl;
	    }
	}
    }

    if (found)
    {
	PoolThread* availableThread = (PoolThread*) (*_threadPool)[index];
	availableThread->assignThread(body, param);

	return availableThread;
    }
    
    return (Thread*) 0;
}

void PoolThread::assignThread (void* (*body)(void*), void* arg)
{
    _arg = arg;
    _result = NULL;
    _threadBody = body;

    /*
     * Mark thread as unavailable, in case we try to assign another
     * thread from the pool before this one actually gets a chance
     * to run.
     */

    _passivated = FALSE;
    _theThread->commonData.threadStat = THR_IS_CREATED;
    
    setup();
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
