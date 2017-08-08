/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne, 
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SemaphoreSetup.cc,v 1.1 1998/01/12 13:06:47 nmcl Exp $
 */

#ifndef COMMON_THREAD_SEMAPHORESETUP_H_
#  include <Common/Thread/SemaphoreSetup.h>
#endif

SemaphoreSetup::SemaphoreSetup ()
                               : processBased(FALSE),
                                 initialOwner(FALSE),
				 reentrant(TRUE),
				 key(0)
{
}

SemaphoreSetup::~SemaphoreSetup ()
{
}
