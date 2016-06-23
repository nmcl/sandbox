/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PosixRWLock.h,v 1.1 1997/09/25 15:32:51 nmcl Exp $
 */

#ifndef POSIXRWLOCK_H_
#define POSIXRWLOCK_H_

/*
 * Read/Write locks for Posix
 */

#ifndef POSIXTHREAD_H_
#  include <System/PosixThread.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef THREADRWLOCK_H_
#  include <Thread/ThreadRWLock.h>
#endif

class PosixRWLock : public ThreadRWLock
{
    friend ThreadRWLock;
    
public:
    virtual ~PosixRWLock ();
    
    virtual Boolean readLock ();
    virtual Boolean writeLock ();
    virtual Boolean unlock ();
    virtual Boolean tryReadLock ();
    virtual Boolean tryWriteLock ();

private:
    PosixRWLock ();

    pthread_mutex_t mutex;		/* consistency */
    pthread_cond_t readers;		/* waiting readers/writers */
    pthread_cond_t writers;
    int readCount;			/* -1 -> its a writer */
    int waitCount;			/* number of waiting writers */
};

#endif
