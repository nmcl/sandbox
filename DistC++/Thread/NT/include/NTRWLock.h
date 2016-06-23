/*
 * Copyright (C) 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NTRWLock.h,v 1.1 1998/09/16 11:03:29 nmcl Exp $
 */

#ifndef NTRWLOCK_H_
#define NTRWLOCK_H_

/*
 * Read/Write locks for NT
 */

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef THREADRWLOCK_H_
#  include <Thread/ThreadRWLock.h>
#endif

class NTRWLock : public ThreadRWLock
{
    friend ThreadRWLock;
    
public:
    virtual ~NTRWLock ();
    
    virtual Boolean readLock ();
    virtual Boolean writeLock ();
    virtual Boolean unlock ();
    virtual Boolean tryReadLock ();
    virtual Boolean tryWriteLock ();

private:
    NTRWLock ();

    HANDLE mutex;		/* consistency */
    HANDLE readers;		/* waiting readers/writers */
    HANDLE writers;
    int readCount;			/* -1 -> its a writer */
    int waitCount;			/* number of waiting writers */
};

#endif
