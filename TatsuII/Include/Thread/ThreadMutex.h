/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadMutex.h,v 1.1 1997/06/09 19:52:04 nmcl Exp $
 */

#ifndef THREADMUTEX_H_
#define THREADMUTEX_H_

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

class ThreadID;

class ThreadMutex
{
public:
    virtual ~ThreadMutex () {};
    
    virtual Boolean lock () = 0;
    virtual Boolean unlock () = 0;
    virtual Boolean tryLock () = 0;

    static ThreadMutex *create (Boolean = FALSE);
};

#endif
