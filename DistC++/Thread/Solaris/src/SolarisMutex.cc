/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SolarisMutex.cc,v 1.2 1997/10/03 14:36:04 nmcl Exp $
 */

/*
 * Implementation of thread ids for Solaris threads
 */

#ifndef SOLARISTHREAD_H_
#  include <System/SolarisThread.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef SOLARISMUTEX_H_
#  include "SolarisMutex.h"
#endif

ThreadMutex *ThreadMutex::create ( Boolean recurs )
{
    return new SolarisMutex(recurs);
}

SolarisMutex::SolarisMutex ( Boolean recurs )
			   : owner(0),
			     count(0),
			     recursive(recurs),
			     valid(FALSE)
{
    if (mutex_init(&mutex, USYNC_THREAD, 0) == 0)
	valid = TRUE;
}

SolarisMutex::~SolarisMutex ()
{
    if (valid)
	mutex_destroy(&mutex);
}

Boolean SolarisMutex::lock ()
{
    if (valid)
    {
	thread_t me = thr_self();
	
	if ((recursive) && (owner == me))
	{
	    count++;
	    return TRUE;
	}

	if (mutex_lock(&mutex) == 0)
	{
	    count = 1;
	    owner = me;
	    return TRUE;
	}
	else
	    valid = FALSE;
    }
    
    return FALSE;
    
}

Boolean SolarisMutex::unlock ()
{
    if (valid)
    {
	thread_t me = thr_self();
	
	if (recursive)
	{
	    if (owner == me)
	    {
		if (--count > 0)
		    return TRUE;
	    }
	    else
		return FALSE;  // not owner, so don't allow unlock!
	}
	
	owner = 0;
	count = 0;

	if (mutex_unlock(&mutex) == 0)
	    return TRUE;
    }
    
    return FALSE;
    
}

Boolean SolarisMutex::tryLock ()
{
    if (valid)
    {
	thread_t me = thr_self();
	int status = 0;

	if ((recursive) && (owner == me))
	{
	    count++;
	    return TRUE;
	}
	
	if ((status = mutex_trylock(&mutex)) == 0)
	{
	    owner = me;
	    count = 1;
	    return TRUE;	
	}
	else
	    if (status != EBUSY)
		valid = FALSE;
    }

    return FALSE;
}
