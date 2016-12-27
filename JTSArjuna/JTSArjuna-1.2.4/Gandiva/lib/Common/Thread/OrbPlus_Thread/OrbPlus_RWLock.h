/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPlus_RWLock.h,v 1.1 1998/01/12 13:07:20 nmcl Exp $
 */

#ifndef COMMON_THREAD_ORBPLUS_THREAD_ORBPLUS_RWLOCK_H_
#define COMMON_THREAD_ORBPLUS_THREAD_ORBPLUS_RWLOCK_H_

/*
 * Read/Write locks for MSD
 */

#include <os/msd/thread.hh>
#include <os/msd/tsd.hh>
#include <os/msd/time.hh>
#include <os/msd/sighandler.hh>

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMON_THREAD_THREADRWLOCK_H_
#  include <Common/Thread/ThreadRWLock.h>
#endif

class OrbPlus_RWLock : public ThreadRWLock
{
    friend ThreadRWLock;
    
public:
    virtual ~OrbPlus_RWLock ();
    
    virtual Boolean readLock ();
    virtual Boolean writeLock ();
    virtual Boolean unlock ();
    virtual Boolean tryReadLock ();
    virtual Boolean tryWriteLock ();

private:
    OrbPlus_RWLock ();

    MSD_Mutex* mutex;		        /* consistency */
    MSD_CondVar* readers;		/* waiting readers/writers */
    MSD_CondVar* writers;
    int readCount;			/* -1 -> its a writer */
    int waitCount;			/* number of waiting writers */
};

#endif
