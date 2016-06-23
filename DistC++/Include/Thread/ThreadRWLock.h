/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadRWLock.h,v 1.1 1997/09/25 15:28:23 nmcl Exp $
 */

#ifndef THREADRWLOCK_H_
#define THREADRWLOCK_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef INITTHREADS_H_
#  include <Thread/InitThreads.h>
#endif

/*
 *
 * Thread system simple mutex
 *
 */

class ThreadRWLock
{
public:
    virtual ~ThreadRWLock () {};
    
    virtual Boolean readLock () = 0;
    virtual Boolean writeLock () = 0;
    virtual Boolean unlock () = 0;
    virtual Boolean tryReadLock () = 0;
    virtual Boolean tryWriteLock () = 0;

    static ThreadRWLock *create ();
};

#endif
