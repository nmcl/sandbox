/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SolarisRWLock.h,v 1.1 1997/09/25 15:32:58 nmcl Exp $
 */
#ifndef SOLARISRWLOCK_H_
#define SOLARISRWLOCK_H_

/*
 * Read/Write locks for Solaris
 */

#ifndef SOLARISTHREAD_H_
#  include <System/SolarisThread.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef THREADRWLOCK_H_
#  include <Thread/ThreadRWLock.h>
#endif

class SolarisRWLock : public ThreadRWLock
{
    friend ThreadRWLock;
    
public:
    virtual ~SolarisRWLock ();
    
    virtual Boolean readLock ();
    virtual Boolean writeLock ();
    virtual Boolean unlock ();
    virtual Boolean tryReadLock ();
    virtual Boolean tryWriteLock ();

private:
    SolarisRWLock ();

    rwlock_t theLock;
};

#endif
