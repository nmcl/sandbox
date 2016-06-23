/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PosixMutex.h,v 1.1 1997/09/25 15:32:50 nmcl Exp $
 */

#ifndef POSIXMUTEX_H_
#  define POSIXMUTEX_H_

#ifndef POSIXTHREAD_H_
#  include <System/PosixThread.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef THREADMUTEX_H_
#  include <Thread/ThreadMutex.h>
#endif

/*
 *
 * POSIX thread system simple mutex
 *
 */

class PosixMutex : public ThreadMutex
{
    friend ThreadMutex;
    
public:
    virtual ~PosixMutex ();
    
    virtual Boolean lock ();
    virtual Boolean unlock ();
    virtual Boolean tryLock ();
    
private:
    PosixMutex (Boolean);

    pthread_mutex_t mutex;
#ifndef HAVE_RECURSIVE_MUTEX    
    pthread_t owner;
    int count;
#endif    
    Boolean recursive;
    Boolean valid;
};

#endif
