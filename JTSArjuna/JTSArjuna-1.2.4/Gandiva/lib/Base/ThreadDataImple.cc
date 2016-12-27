/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadDataImple.cc,v 1.3 1998/01/12 13:05:22 nmcl Exp $
 */

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

#ifndef BASE_THREADDATAIMPLE_H_
#  include <Base/ThreadDataImple.h>
#endif

ThreadDataImple::ThreadDataImple ()
				 : valid_(FALSE),
				   lock_(Mutex::create())
{
    Resource::ref(lock_);
}

ThreadDataImple::~ThreadDataImple ()
{
    Resource::unref(lock_);
}

Boolean ThreadDataImple::addToOverflow ( void *arg )
{
    Uid u = Thread::current()->uid();
    Boolean result = FALSE;
    
    if (lock_)
    {
	if (lock_->lock() == Mutex::MX_DONE)
	{
	    ThreadSpecificData *td = overflow.find(u);

	    if (td)
	    {
		td->data = arg;
	    }
	    else
		overflow.insert(new ThreadSpecificData(u, arg));

	    result = TRUE;
	    
	    lock_->unlock();
	}
    }

    return result;
}

Boolean ThreadDataImple::getFromOverflow ( void *& arg )
{
    Uid u = Thread::current()->uid();
    Boolean result = FALSE;
    
    if (lock_)
    {
	if (lock_->lock() == Mutex::MX_DONE)
	{
	    ThreadSpecificData *td = overflow.find(u);

	    if (td)
	    {
		arg = td->data;
		result = TRUE;
	    }
	    else
	    {
		arg = 0;
	    }
	    
	    lock_->unlock();
	}
    }

    return result;
}

Boolean ThreadDataImple::removeFromOverflow ( )
{
    Uid u = Thread::current()->uid();
    Boolean result = FALSE;
    
    if (lock_)
    {
	if (lock_->lock() == Mutex::MX_DONE)
	{
	    ThreadSpecificData *td = overflow.remove(u);

	    if (td)
	    {
		delete td;
		
		result = TRUE;
	    }
	    
	    lock_->unlock();
	}
    }

    return result;
}
