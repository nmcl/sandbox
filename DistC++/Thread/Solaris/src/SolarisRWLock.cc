/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SolarisRWLock.cc,v 1.1 1997/09/25 15:33:01 nmcl Exp $
 */

/*
 * Implementation of R/W locks for Solaris threads
 */

#ifndef SOLARISTHREAD_H_
#  include <System/SolarisThread.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef SOLARISRWLOCK_H_
#  include "SolarisRWLock.h"
#endif

ThreadRWLock *ThreadRWLock::create ()
{
    return new SolarisRWLock();
}

SolarisRWLock::SolarisRWLock ()
			     : theLock()
{
    rwlock_init(&theLock, USYNC_THREAD, 0);
}

SolarisRWLock::~SolarisRWLock ()
{
    rwlock_destroy(&theLock);
}

Boolean SolarisRWLock::readLock ()
{
    int result = rw_rdlock(&theLock);
    
    return (result == 0 ? TRUE : FALSE);
}

Boolean SolarisRWLock::writeLock ()
{
    int result = rw_wrlock(&theLock); 

    return (result == 0 ? TRUE : FALSE);
}

Boolean SolarisRWLock::unlock ()
{
    rw_unlock(&theLock);

    return TRUE;
}

Boolean SolarisRWLock::tryReadLock ()
{
    int  result = rw_tryrdlock(&theLock);

    return (result == 0 ? TRUE : FALSE);
}

Boolean SolarisRWLock::tryWriteLock ()
{
    int  result = rw_trywrlock(&theLock);

    return (result == 0 ? TRUE : FALSE);
}
