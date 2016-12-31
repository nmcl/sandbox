/*
 * Copyright (C) 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadRWLock.h,v 1.1 1998/01/12 12:56:12 nmcl Exp $
 */

#ifndef COMMON_THREAD_THREADRWLOCK_H_
#define COMMON_THREAD_THREADRWLOCK_H_

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMON_THREAD_INITTHREADS_H_
#  include <Common/Thread/InitThreads.h>
#endif

/*
 *
 * Thread system simple readers/writers lock.
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
