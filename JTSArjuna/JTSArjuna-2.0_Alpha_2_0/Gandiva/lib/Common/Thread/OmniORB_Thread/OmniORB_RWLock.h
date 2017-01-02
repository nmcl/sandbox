/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OmniORB_RWLock.h,v 1.1 1998/01/12 13:07:13 nmcl Exp $
 */

#ifndef COMMON_THREAD_OMNIORB_THREAD_OMNIORB_RWLOCK_H_
#define COMMON_THREAD_OMNIORB_THREAD_OMNIORB_RWLOCK_H_

/*
 * Read/Write locks for OmniOrb
 */

#include <os/omnithread.h>

#ifndef COMMONBOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMON_THREAD_THREADRWLOCK_H_
#  include <Common/Thread/ThreadRWLock.h>
#endif

class OmniORB_RWLock : public ThreadRWLock
{
    friend ThreadRWLock;
    
public:
    virtual ~OmniORB_RWLock ();
    
    virtual Boolean readLock ();
    virtual Boolean writeLock ();
    virtual Boolean unlock ();
    virtual Boolean tryReadLock ();
    virtual Boolean tryWriteLock ();

private:
    OmniORB_RWLock ();

    omni_mutex* mutex;  		/* consistency */
    omni_condition* readers;		/* waiting readers/writers */
    omni_condition* writers;
    int readCount;			/* -1 -> its a writer */
    int waitCount;			/* number of waiting writers */
};

#endif
