/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPlus_Sem.h,v 1.2 1998/01/19 10:35:11 nmcl Exp $
 */

#ifndef COMMON_THREAD_ORBPLUS_THREAD_ORBPLUS_SEM_H_
#define COMMON_THREAD_ORBPLUS_THREAD_ORBPLUS_SEM_H_

#include <os/msd/thread.hh>
#include <os/msd/tsd.hh>
#include <os/msd/time.hh>
#include <os/msd/sighandler.hh>

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMON_THREAD_SEMAPHORE_H_
#  include <Common/Thread/Semaphore.h>
#endif

#ifndef COMMON_THREAD_MUTEX_H_
#  include <Common/Thread/Mutex.h>
#endif

class MutexSetup;
class SemaphoreSetup;

/*
 *
 * MSD thread system simple mutex
 *
 */

class MutexData
{
public:
    MutexData (MutexSetup*);
    ~MutexData ();

    MSD_Mutex* mutex;
    unsigned int owner;
    int count;
    Boolean recursive;
    Boolean valid;
};

#endif
