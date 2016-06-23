/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadSpecific.cc,v 1.1 1997/09/25 15:32:28 nmcl Exp $
 */

/*
 * Implementation of thread specific state
 */

#ifndef THREAD_H_
#  include <Thread/Thread.h>
#endif

#ifndef THREADMUTEX_H_
#  include <Thread/ThreadMutex.h>
#endif

#ifndef THREADSPECIFIC_H_
#  include <Thread/ThreadSpecific.h>
#endif

ThreadSpecific::ThreadSpecific ()
			       : valid_(FALSE),
				 lock_(ThreadMutex::create())
{
}

ThreadSpecific::~ThreadSpecific ()
{
    if (lock_)
	delete lock_;
}

Boolean ThreadSpecific::addToOverflow ( void *arg )
{
    Uid u = Thread::current()->get_uid();
    Boolean result = FALSE;
    
    if (lock_)
    {
	if (lock_->lock())
	{
	    ThreadSpecificData *td = overflow.lookFor(u);

	    if (td)
	    {
		td->data = arg;
		result = TRUE;
	    }
	    else
		result = overflow.insert(new ThreadSpecificData(u,arg));
	    
	    lock_->unlock();
	}
    }

    return result;
}

Boolean ThreadSpecific::getFromOverflow ( void *& arg )
{
    Uid u = Thread::current()->get_uid();
    Boolean result = FALSE;
    
    if (lock_)
    {
	if (lock_->lock())
	{
	    ThreadSpecificData *td = overflow.lookFor(u);

	    if (td)
	    {
		arg = td->data;
	    }
	    else
	    {
		arg = 0;
	    }
	    
	    result = TRUE;	    
	    lock_->unlock();
	}
    }

    return result;
}

Boolean ThreadSpecific::removeFromOverflow ( )
{
    Uid u = Thread::current()->get_uid();
    Boolean result = FALSE;
    
    if (lock_)
    {
	if (lock_->lock())
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


