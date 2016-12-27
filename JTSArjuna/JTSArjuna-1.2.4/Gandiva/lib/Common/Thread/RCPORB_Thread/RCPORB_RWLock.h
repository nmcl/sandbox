/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RCPORB_RWLock.h,v 1.2 1998/01/16 14:32:09 nmcl Exp $
 */

#ifndef COMMON_THREAD_RCPORB_THREAD_RCPORB_RWLOCK_H_
#define COMMON_THREAD_RCPORB_THREAD_RCPORB_RWLOCK_H_

/*
 * Read/Write locks for ON
 */

#include <os/CORBA.ch>
#include <os/ON_init.h>
#include <os/ON_sync.h>

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMON_THREAD_THREADRWLOCK_H_
#  include <Common/Thread/ThreadRWLock.h>
#endif

class RCPORB_RWLock : public ThreadRWLock
{
    friend ThreadRWLock;
    
public:
    virtual ~RCPORB_RWLock ();
    
    virtual Boolean readLock ();
    virtual Boolean writeLock ();
    virtual Boolean unlock ();
    virtual Boolean tryReadLock ();
    virtual Boolean tryWriteLock ();

private:
    RCPORB_RWLock ();

    ODS_CORBA::Mutex* mutex;		/* consistency */
    ODS_CORBA::CondMutex* readers;      /* waiting readers/writers */
    ODS_CORBA::CondMutex* writers;
    int readCount;			/* -1 -> its a writer */
    int waitCount;			/* number of waiting writers */
};

#endif
