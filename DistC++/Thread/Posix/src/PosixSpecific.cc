/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PosixSpecific.cc,v 1.2 1998/02/06 12:12:31 nmcl Exp $
 */

/*
 * Implementation of thread specific state for Posix threads
 */

#ifndef POSIXTHREAD_H_
#  include <System/PosixThread.h>
#endif

#ifndef POSIXSPECIFIC_H_
#  include "PosixSpecific.h"
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

ThreadSpecific* ThreadSpecific::create ()
{
    PosixSpecific* temp = new PosixSpecific();

    if (temp)
    {
	if (!temp->valid())
	{
	    delete temp;
	    temp = 0;
	}
    }
	
    return temp;
}

PosixSpecific::PosixSpecific ()
			     : key_(0)
{
#ifndef HAVE_HPUX_PTHREADS    
    if (pthread_key_create(&key_, 0) == 0)
	valid(TRUE);
#else
    if (pthread_keycreate(&key_, 0) == 0)  // so much for standards!
	valid(TRUE);
#endif    
}

PosixSpecific::~PosixSpecific ()
{
    destroySpecific();
}

Boolean PosixSpecific::getSpecific ( void *& anyData )
{
    if (valid())
    {
#ifndef HAVE_HPUX_PTHREADS	
	if ((anyData = pthread_getspecific(key_)) != 0)
	    return TRUE;
#else
	if (pthread_getspecific(key_, &anyData) == 0)
	    return TRUE;
#endif	
        else
	{
            return getFromOverflow(anyData);
	}
    }
    
    return FALSE;
}

Boolean PosixSpecific::setSpecific ( void *anyData )
{
    if (valid())
    {
	int error = 0;

        if (((error = pthread_setspecific(key_, anyData)) != 0) &&
            ((error != ENOMEM) || !(addToOverflow(anyData))))
        {
            return FALSE;
        }

        return TRUE;
    }
    
    return FALSE;
}

Boolean PosixSpecific::destroySpecific ()
{
    if (valid())
    {
#ifndef HAVE_HPUX_PTHREADS
	pthread_key_delete(key_);
#else
	/*
	 * No delete of key, so all we can do is set to 0.
	 */
	
        (void) pthread_setspecific(key_, 0);
#endif
	removeFromOverflow();
	return TRUE;
    }

    return FALSE;
}
