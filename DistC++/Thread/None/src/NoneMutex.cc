/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NoneMutex.cc,v 1.1 1997/09/25 15:32:38 nmcl Exp $
 */

/*
 * Implementation of thread ids for None threads
 */


#ifndef NONEMUTEX_H_
#  include "NoneMutex.h"
#endif

ThreadMutex *ThreadMutex::create ( Boolean recurs )
{
    return new NoneMutex(recurs);
}

NoneMutex::NoneMutex ( Boolean )
{
}

NoneMutex::~NoneMutex ()
{
}

Boolean NoneMutex::lock ()
{
    return TRUE;
}

Boolean NoneMutex::unlock ()
{
    return TRUE;
}

Boolean NoneMutex::tryLock ()
{
    return TRUE;	
}
