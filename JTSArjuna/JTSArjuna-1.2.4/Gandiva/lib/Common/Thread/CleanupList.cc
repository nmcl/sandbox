/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CleanupList.cc,v 1.1 1998/01/12 13:06:42 nmcl Exp $
 */

#ifndef BASE_THREADCLEANUP_H_
#  include <Base/ThreadCleanup.h>
#endif

#ifndef COMMON_THREAD_CLEANUPLIST_H_
#  include <Common/Thread/CleanupList.h>
#endif

CleanupList::CleanupList ()
			 : _cleanup(0)
{
}

/*
 * Deleting the list will cause each element to perform its
 * necessary cleanup work.
 */

CleanupList::~CleanupList ()
{
    if (_cleanup)
	delete _cleanup;
}

Boolean CleanupList::add (ThreadCleanup* cl)
{
    cl->next(_cleanup);
    _cleanup = cl;
    
    return TRUE;
}
