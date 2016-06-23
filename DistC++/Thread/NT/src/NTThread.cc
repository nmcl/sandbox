/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NTThread.cc,v 1.1 1998/09/16 11:03:31 nmcl Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef THREAD_H_
#  include <Thread/Thread.h>
#endif

#ifndef THREADMUTEX_H_
#  include <Thread/ThreadMutex.h>
#endif

#ifndef THREADCOMMON_H_
#  include <Thread/ThreadCommon.h>
#endif

#ifndef TSHASHLIST_H_
#  include <Template/TSHashList.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

static int MaxPriority = THREAD_PRIORITY_ABOVE_NORMAL;
static int MinPriority = THREAD_PRIORITY_BELOW_NORMAL;
static unsigned long defaultStackSize = 2048;
static ThreadSafeHashList<Thread>* overflowList = 0;

DWORD globalKey;

struct ThreadData
{
    ThreadData (Thread::ThreadAttachStatus, Thread*);
    ~ThreadData ();

    ThreadCommon commonData;

    Boolean onOverflow;
    DWORD   sysId;
    HANDLE  thrHandle;
};
    
ThreadData::ThreadData (Thread::ThreadAttachStatus aStat,
			Thread* th)
		       : commonData(sStat, th),
			 onOverflow(FALSE),
			 sysId(0),
			 thrHandle(0)
{
}

ThreadData::~ThreadData ()
{
}

    
Thread::Thread (void* (*body)(void *), void *arg, ThreadStatus startIn,
		ThreadAttachStatus aStat, void *stack, size_t stackSize)
{
    theThread = new ThreadData(aStat, this);

    theThread->commonData.threadBody = body;
    theThread->commonData.arg = arg;

    theThread->thrHandle = CreateThread(NULL, stackSize, (LPTHREAD_START_ROUTINE) body,
					arg, CREATE_SUSPENDED, &theThread->sysId);

    if (theThread->thrHandle)
    {
	allThreads->add(this);

	if (startIn == THR_IS_RUNNING)
	    resume();
    }
    else
    {
	theThread->commonData.threadStat = THR_IS_TERMINATED;
    }
}

Thread::Thread ()
{
    theThread = new ThreadData(THR_IS_ATTACHED, this);
    theThread->commonData.threadStat = THR_IS_RUNNING;
    theThread->sysID = GetCurrentThreadId();
    theThread->thrHandle = GetCurrentThread();
    TlsSetValue(globalKey, this);
    
    allThreads->add(this);
}

// tidy things up before we terminate thread

Thread::~Thread ()
{
    terminate();

    if (theThread->onOverflow)
	overflowList->remove(this);

    allThreads->remove(this);
    delete theThread;
}

Boolean Thread::join ()
{
    if ((theThread) && (theThread->sysId != GetCurrentThreadId()))
    {
	if (WaitForSingleObject(theThread->thrHandle, INFINITE) != WAIT_FAILED)
	{
	    if (theThread->commonData.threadStat == THR_IS_TERMINATED)
		return TRUE;
	    else
		return FALSE;
	}
	else
	    return FALSE;
    }
    else
	return FALSE;
}

Boolean Thread::resume ()
{
    if (theThread->commonData.threadStat == THR_IS_SUSPENDED)
    {
	theThread->commonData.threadStat = THR_IS_RUNNING;
	return (Boolean) (ResumeThread(thrHandle) > 0);
    }
    else
	return (Boolean) (theThread->commonData.threadStat == THR_IS_RUNNING);
}

Boolean Thread::suspend ()
{
    Boolean result = FALSE;
    Thread* th = Thread::current();

    if (th && th->theThread)
    {
	if (th->theThread->commonData.threadStat == THR_IS_RUNNING)
	{
	    th->theThread->commonData.threadStat = THR_IS_SUSPENDED;
	    SuspendThread(thrHandle);
	    result = TRUE;
	}
	else
	    result = (Boolean) (th->theThread->commonData.threadStat == Thread::SUSPENDED);
    }

    return result;
}

Thread::ThreadStatus Thread::status () const
{
    return theThread->commonData.threadStat;
}

const Uid& Thread::get_uid () const
{
    return theThread->commonData.threadId;
}

Thread* Thread::current ()
{
    LPVOID p1 = TlsGetValue(globalKey);

    if ((p1) || ((!p1) && (GetLastError() == NO_ERROR)))
	return (Thread*) p1;

    if (!Thread::overflowList)
	return (Thread*) 0;
    
    ThreadSafeHashListI<Thread> iter(*overflowList);
    DWORD tid = GetCurrentThreadId();
    Thread* t = 0;
    
    while ((t = iter()) != 0)
    {
	if (tid == t->sysID)
	{
	    break;
	}
    }

    return t;
}

void Thread::current ( Uid& theId )
{
    Thread *me = Thread::current();
    
    theId = me->theThread->commonData.threadId;
}

void Thread::exit (int retValue)
{
    Thread* th = Thread::current();

    if (th && th->theThread)
    {
	th->terminate();
	th->theThread->commonData.threadStat = THR_IS_TERMINATED;
	_exit(retValue);
    }
}

Boolean Thread::join ( const Uid& toJoin )
{
    Thread *th = allThreads->lookFor(toJoin);

    if (th)
    {
	return th->join();
    }
    
    return FALSE;
}

Boolean Thread::sleep (unsigned int sleepTime)
{
    Thread* currt = Thread::current();

    if (!currt)
	return FALSE;

    currt->theThread->commonData.threadStat = THR_IS_BLOCKED;
    Sleep(sleepTime);
    currt->theThread->commonData.threadStat = THR_IS_RUNNING;
	
    return TRUE;
}

/*
 * A yielded thread is still running, and is simply
 * rescheduled by the thread system.
 */

Boolean Thread::yield ()
{
    Sleep(0);
    return TRUE;
}

Boolean Thread::terminate ()
{
    NT_Thread* currt = (NT_Thread*) Thread::current();

    if (!currt)
	return;

    if (this != currt)
    {
	join();

	return (theThread->commonData.threadStat == THR_IS_TERMINATED);
    }
    else
    {
	theThread->commonData.threadStat = THR_IS_TERMINATED;
	TerminateThread(currt->thrHandle, 0);
	return TRUE;
    }
}

void* Thread::execute (void* startMe)
{
    if (startMe)
    {
	Boolean runThread = TRUE;
	Thread* run = (Thread*) startMe;
	
	if (!TlsSetValue(globalKey, run))
	{
	    // put on to overflow list

	    if (!overflowList)
		overflowList = new ThreadSafeHashList<Thread>(11);
	    
	    if (overflowList->insert(run))
		run->theThread->onOverflow = TRUE;
	}

	run->theThread->commonData.threadStat = THR_IS_RUNNING;
	if (run->theThread->commonData.threadBody)
	    run->theThread->commonData.result = run->theThread->commonData.threadBody(run->theThread->commonData.arg);

	run->terminate();
    }

    return 0;
}

void Thread::init ()
{
    if (mainThread == 0)
    {
	ThreadMutex* dummy = ThreadMutex::create();

	globalKey = TlsAlloc();

	mainThread = new Thread();

	delete dummy;
    }
}

void Thread::shutdown ()
{
    if (mainThread)
    {
	delete mainThread;
	mainThread = 0;
    }
}
