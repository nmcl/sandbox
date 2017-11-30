/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NoneMutex.cc,v 1.2 1995/07/03 14:30:26 ngdp Exp $
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
