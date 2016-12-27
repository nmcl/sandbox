/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NT_Sem.h,v 1.1 1998/01/12 13:07:09 nmcl Exp $
 */

#ifndef COMMON_THREAD_NT_THREAD_NT_SEM_H_
#define COMMON_THREAD_NT_THREAD_NT_SEM_H_

#ifndef OS_WINDOWS_H_
#  include <os/windows.h>
#endif

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

#ifndef COMMON_THREAD_SEMAPHORE_H_
#  include <Common/Thread/Semaphore.h>
#endif

class MutexSetup;
class SemaphoreSetup;

class SemaphoreData
{
public:
    SemaphoreData (unsigned int, SemaphoreSetup*);
    ~SemaphoreData ();

    HANDLE sem;
    Boolean dead;
};

class MutexData
{
public:
    MutexData (MutexSetup*);
    ~MutexData ();

    HANDLE mutex;
    Boolean dead;
};

#endif
