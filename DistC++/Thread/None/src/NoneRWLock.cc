/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NoneRWLock.cc,v 1.1 1997/09/25 15:32:40 nmcl Exp $
 */

/*
 * Implementation of R/W locks for Solaris threads
 */

#ifndef NONERWLOCK_H_
#  include "NoneRWLock.h"
#endif

ThreadRWLock *ThreadRWLock::create ()
{
    return new NoneRWLock();
}

NoneRWLock::NoneRWLock ()
{
}

NoneRWLock::~NoneRWLock ()
{
}

Boolean NoneRWLock::readLock ()
{
    return TRUE;
}

Boolean NoneRWLock::writeLock ()
{
    return TRUE;
}

Boolean NoneRWLock::unlock ()
{
    return TRUE;
}

Boolean NoneRWLock::tryReadLock ()
{
    return TRUE;
}

Boolean NoneRWLock::tryWriteLock ()
{
    return TRUE;
}
