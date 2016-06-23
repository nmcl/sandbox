/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PosixMutex.cc,v 1.1 1997/09/25 15:32:54 nmcl Exp $
 */

/*
 * Implementation of thread ids for Posix threads
 */

#ifndef POSIXTHREAD_H_
#  include <System/PosixThread.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef POSIXMUTEX_H_
#  include "PosixMutex.h"
#endif

ThreadMutex *ThreadMutex::create ( Boolean recurs )
{
    return new PosixMutex(recurs);
}

#ifndef HAVE_RECURSIVE_MUTEX
PosixMutex::PosixMutex ( Boolean recurs )
		       : owner(0),
			 count(0),
			 recursive(recurs),
			 valid(FALSE)
#else
PosixMutex::PosixMutex ( Boolean recurs )
		       : recursive(recurs),
			 valid(FALSE)
#endif    
{
#ifndef HAVE_HPUX_PTHREADS
    if (pthread_mutex_init(&mutex, 0) == 0)
	valid = TRUE;
#else
    /*
     * Have recursive mutexes! (Non-portable implementation though!)
     */

    if (recurs)
    {
	pthread_mutexattr_t attr;

	pthread_mutexattr_create(&attr);
	pthread_mutexattr_setkind_np(&attr, MUTEX_RECURSIVE_NP);
    
	if (pthread_mutex_init(&mutex, attr) == 0)
	    valid = TRUE;
    }
    else
	if (pthread_mutex_init(&mutex, pthread_mutexattr_default) == 0)
	    valid = TRUE;	
#endif    
}

PosixMutex::~PosixMutex ()
{
    if (valid)
    {
	pthread_mutex_destroy(&mutex);
    }
}

Boolean PosixMutex::lock ()
{
    Boolean result = FALSE;
    
    if (valid)
    {
#ifndef HAVE_RECURSIVE_MUTEX	
	pthread_t me = pthread_self();

	if ((recursive) && (pthread_equal(owner, me)))
	{
	    count++;
	    result = TRUE;
	}
	else
	{
	    if (pthread_mutex_lock(&mutex) == 0)
	    {
		count = 1;
		owner = me;
		result = TRUE;
	    }
	}
#else
	if (pthread_mutex_lock(&mutex) == 0)
	{
	    result = TRUE;
	}	
#endif
    }
    
    return result;
}

Boolean PosixMutex::unlock ()
{
    Boolean result = FALSE;

    if (valid)
    {
#ifndef HAVE_RECURSIVE_MUTEX	
	pthread_t me = pthread_self();

	if (recursive)
	{
	    if (pthread_equal(owner, me))
	    {
		result = TRUE;
		
		if (--count == 0)
		{
		    owner = 0;
		    count = 0;

		    if (pthread_mutex_unlock(&mutex) != 0)
			result = FALSE;
		}
	    }
	    else
	    {
		/*
		 * Not the owner, so shouldn't be calling unlock.
		 */
	    }
	}
	else
	{
	    if (pthread_mutex_unlock(&mutex) != 0)
		result = FALSE;
	    else
		result = TRUE;
	}
#else
	if (pthread_mutex_unlock(&mutex) != 0)
	    result = FALSE;
	else
	    result = TRUE;
#endif	
    }
    
    return result;
}

Boolean PosixMutex::tryLock ()
{
    Boolean result = FALSE;

    if (valid)
    {
#ifndef HAVE_RECURSIVE_MUTEX	
	pthread_t me = pthread_self();
	int status = 0;

	if ((recursive) && (pthread_equal(owner, me)))
	{
	    count++;
	    result = TRUE;
	}
	else
	{
	    if ((status = pthread_mutex_trylock(&mutex)) == 0)
	    {
		owner = me;
		count = 1;
		result = TRUE;
	    }
	    else
		if (status != EBUSY)
		    valid = FALSE;
	}
#else
	if (pthread_mutex_trylock(&mutex) == 0)
	{
	    result = TRUE;
	}
#endif	
    }

    return result;
}
