/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SolarisMutex.h,v 1.1 1997/09/25 15:32:58 nmcl Exp $
 */

#ifndef SOLARISMUTEX_H_
#define SOLARISMUTEX_H_

#ifndef SOLARISTHREAD_H_
#  include <System/SolarisThread.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef THREADMUTEX_H_
#  include <Thread/ThreadMutex.h>
#endif

/*
 *
 * Solaris thread system simple mutex
 *
 */

class SolarisMutex : public ThreadMutex
{
    friend ThreadMutex;
    
public:
    virtual ~SolarisMutex ();
    
    virtual Boolean lock ();
    virtual Boolean unlock ();
    virtual Boolean tryLock ();
    
private:
    SolarisMutex (Boolean);

    mutex_t mutex;
    thread_t owner;
    int count;
    Boolean recursive;
    Boolean valid;
};

#endif
