/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastlw upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: MutexSetup.cc,v 1.1 1998/01/12 13:06:45 nmcl Exp $
 */

#ifndef COMMON_THREAD_MUTEXSETUP_H_
#  include <Common/Thread/MutexSetup.h>
#endif

MutexSetup::MutexSetup ()
		       : processBased(FALSE),
			 initialOwner(FALSE),
			 reentrant(TRUE)
{
}

MutexSetup::~MutexSetup ()
{
}
