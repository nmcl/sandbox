/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadCleanup.cc,v 1.2 1996/01/02 13:14:00 nsmw Exp $
 */

#ifndef BASE_THREADCLEANUP_H_
#  include <Base/ThreadCleanup.h>
#endif


/*
 * These actions do their work when they are deleted.
 */

ThreadCleanup::ThreadCleanup ()
			     : _next(0)
{
}

ThreadCleanup::ThreadCleanup (ThreadCleanup* n)
			     : _next(n)
{
}

ThreadCleanup::~ThreadCleanup ()
{
    if (_next)
	delete _next;
}

ThreadCleanup* ThreadCleanup::next () const
{
    return _next;
}

void ThreadCleanup::next (ThreadCleanup* n)
{
    _next = n;
}
