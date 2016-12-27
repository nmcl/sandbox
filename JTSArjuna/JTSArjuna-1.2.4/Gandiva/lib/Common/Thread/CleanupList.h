/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CleanupList.h,v 1.1 1998/01/12 13:06:43 nmcl Exp $
 */

#ifndef COMMON_THREAD_CLEANUPLIST_H_
#define COMMON_THREAD_CLEANUPLIST_H_

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

class ThreadCleanup;

class CleanupList
{
public:
    CleanupList ();
    virtual ~CleanupList ();
    
    virtual Boolean add (ThreadCleanup*);

private:
    ThreadCleanup* _cleanup;
};


#endif
