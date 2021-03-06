/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: MutexSetup.h,v 1.1 1998/01/12 12:56:06 nmcl Exp $
 */

#ifndef COMMON_THREAD_MUTEXSETUP_H_
#define COMMON_THREAD_MUTEXSETUP_H_

#ifndef OS_SYS_TYPES_H_
#  include <os/sys/types.h>
#endif

#ifndef WIN32

#ifndef OS_SYS_IPC_H_
#  include <os/sys/ipc.h>
#endif

#endif

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif


class MutexSetup
{
public:
    MutexSetup ();
    ~MutexSetup ();

    Boolean processBased;
    Boolean initialOwner;
    Boolean reentrant;
    key_t key;
};

#endif
