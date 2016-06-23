/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NoneRWLock.h,v 1.1 1997/09/25 15:32:34 nmcl Exp $
 */
#ifndef NONERWLOCK_H_
#define NONERWLOCK_H_

/*
 * Read/Write locks for null threads
 */

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef THREADRWLOCK_H_
#  include <Thread/ThreadRWLock.h>
#endif

class NoneRWLock : public ThreadRWLock
{
    friend ThreadRWLock;
    
public:
    virtual ~NoneRWLock ();
    
    virtual Boolean readLock ();
    virtual Boolean writeLock ();
    virtual Boolean unlock ();
    virtual Boolean tryReadLock ();
    virtual Boolean tryWriteLock ();

private:
    NoneRWLock ();
};

#endif
