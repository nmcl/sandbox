/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadReaper.cc,v 1.2 1998/01/16 10:28:27 nmcl Exp $
 */

#include <os/iostream.h>

#ifndef COMMON_THREAD_THREADREAPER_H_
#  include <Common/Thread/ThreadReaper.h>
#endif

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

#ifndef TEMPLATE_HASHEDITERATOR_H_
#  include <Template/HashedIterator.h>
#endif

ThreadReaper::ThreadReaper (size_t numberToReap)
			   : _numberToReap(numberToReap),
			     _mutex(Mutex::create()),
			     _threadList(new HashedList<Thread, Uid>)

{
    Resource::ref(_mutex);
}

ThreadReaper::~ThreadReaper ()
{
    Resource::unref(_mutex);
    delete _threadList;
}

Boolean ThreadReaper::add (Thread* t)
{
    if (t)
    {
	if (_mutex->lock() == Mutex::MX_DONE)
	{
	    Resource::ref(t);
	    _threadList->insert(t, t->uid());
	
	    _mutex->unlock();

	    return TRUE;
	}
    }
    
    return FALSE;
}

long ThreadReaper::size () const
{
    long value = -1;
    
    if (_mutex->lock() == Mutex::MX_DONE)
    {
	value = _threadList->size();

	_mutex->unlock();
    }
    
    return value;
}

void ThreadReaper::purge ()
{
    if (!_threadList)
	return;

    HashedIterator<Thread, Uid> iter(*_threadList);
    size_t removed = 0;

    if (_mutex->lock() == Mutex::MX_DONE)
    {
	Thread* t = iter();

	while ((t) && (removed != _numberToReap))
	{
	    if (t->status() == Thread::THR_IS_TERMINATED)
	    {
		/*
		 * Assume threads are ref/unref-ed correctly, so this
		 * unref should remove the thread if no other thread or
		 * part of the application is waiting for it (in which
		 * case it should have done a ref!)
		 */

		_threadList->remove(t, t->uid());

		Resource::unref(t);

		removed++;
	    }

	    t = iter();
	}

	_mutex->unlock();
    }
}

ostream& ThreadReaper::print (ostream& strm) const
{
    if (!_threadList)
    {
        strm << "ThreadReaper: <NULL>";
	return strm;
    }

    strm << "ThreadReaper:\n";
    
    HashedIterator<Thread, Uid> iter(*_threadList);

    for (Thread* t = iter(); t; t = iter())
    {
	t->print(strm);
	strm << "\n";
    }

    return strm;    
}

ostream& operator<< (ostream& strm, const ThreadReaper& t)
{
    return t.print(strm);
}
