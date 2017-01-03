/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Thread.cc,v 1.4 1998/01/16 10:29:45 nmcl Exp $
 */

#include <os/iostream.h>

#ifndef BASE_ACTION_H_
#  include <Base/Action.h>
#endif

#ifndef BASE_THREADCLEANUP_H_
#  include <Base/ThreadCleanup.h>
#endif

#ifndef COMMON_THREAD_CLEANUPLIST_H_
#  include <Common/Thread/CleanupList.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

#ifndef TEMPLATE_BASICLISTITERATOR_H_
#  include <Template/BasicListIterator.h>
#endif

#ifndef TEMPLATE_HASHEDLIST_H_
#  include <Template/HashedList.h>
#endif

#ifndef TEMPLATE_HASHEDITERATOR_H_
#  include <Template/HashedIterator.h>
#endif

#ifndef COMMON_THREAD_THREADSETUP_H_
#  include <Common/Thread/ThreadSetup.h>
#endif

#ifndef GANDIVA_H_
#  include <Gandiva.h>
#endif

const int Thread::InvalidPriority = -9999;

HashedList<Thread, Uid>* Thread::_list = 0;
Mutex* Thread::_mutex = 0;
Boolean Thread::_initialized = 0;

Boolean Thread::addCleanup (ThreadCleanup* cleanup)
{
    if (cleanup)
    {
	if (!_cleanupList)
	    _cleanupList = new CleanupList;
    
	return _cleanupList->add(cleanup);
    }
    else
	return FALSE;
}

/*
 * Only add to child list if we have a parent!
 * Parent must be current thread.
 */

/*
 * This should use the mutex defined in the CommonData
 * structure.
 */

void Thread::addChild ()
{
    Thread* curr = Thread::current();

    if (curr && (Thread::_mutex->lock() == Mutex::MX_DONE))
    {
        if (!curr->_childList)
	    curr->_childList = new BasicList<Thread, Uid>;

        Resource::ref(this);
	curr->_childList->insert(this);
	Thread::_mutex->unlock();
    }
}

/*
 * Since this is a static it implicitly means that the current
 * thread will be made to wait for all of its children.
 */

void Thread::waitForAllChildren ()
{
    Thread* currentThread = Thread::current();
    
    if ((!currentThread) || (!currentThread->_childList))
        return;

    BasicListIterator<Thread, Uid> iter(*currentThread->_childList);
    
    for (Thread* marker = iter(); marker; marker = iter())
    {
        Thread::wait(marker);
	currentThread->_childList->remove(marker);
	Resource::unref(marker);
    }

    delete currentThread->_childList;
    currentThread->_childList = NULL;
}

void Thread::cleanup ()
{
    if (_cleanupList != (CleanupList*) 0)
    {
	delete _cleanupList;
	_cleanupList = (CleanupList*) 0;
    }
}

/*
 * Kill the thread's children.
 * We do not call waitForAllChildren as this method is
 * only invoked by kill, which we assume required immediate
 * termination of the thread.
 */

void Thread::killChildren ()
{
    if (_childList)
    {
	BasicListIterator<Thread, Uid> iter(*_childList);
    
	for (Thread* marker = iter(); marker; marker = iter())
	{
	    marker->kill();
	    _childList->remove(marker);
	    Resource::unref(marker);
	}
	
	delete _childList;
	_childList = NULL;
    }
}

ostream& Thread::printChildren (ostream& strm) const
{
    strm << "Child List : ";
    
    if (!_childList)
	strm << "Empty" << endl;
    else
	strm << "\n" << *_childList << endl;

    return strm;
}

/*
 * Go through list and print out information on all threads.
 */

ostream& Thread::printAll (ostream& strm)
{
    if (!_list)
    {
        strm << "Thread system not initialized." << endl;
	return strm;
    }

    HashedIterator<Thread, Uid> iter(list());

    for (Thread* t = iter(); t; t = iter())
    {
	t->print(strm);
	strm << endl;
    }

    return strm;
}

const ClassName& Thread::name ()
{
    return Gandiva::Common::Thread::name();
}

const ClassName& Thread::className () const
{
    return Gandiva::Common::Thread::name();    
}

void* Thread::castup (const ClassName& _type) const
{
    if (_type == Thread::name())
	return (void*) this;
    else
	return (void*) 0;
}

Thread* Thread::castup (Resource* toCast)
{
    if (toCast == (Resource*) 0)
	return (Thread*) 0;
    else
	return (Thread*) toCast->castup(Thread::name());
}

/*
 * Shutdown thread system. Eventually this needs to be
 * in each thread implementation.
 */

void Thread::shutdown ()
{
}

#ifdef NO_INLINES
#  define THREAD_CC_
#  include <Common/Thread/Thread.n>
#  undef THREAD_CC_
#endif
