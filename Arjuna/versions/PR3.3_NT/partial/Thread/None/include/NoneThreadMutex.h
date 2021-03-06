/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NoneThreadMutex.h,v 1.1 1995/06/26 10:46:58 ngdp Exp $
 */

#ifndef NONEMUTEX_H_
#define NONEMUTEX_H_

extern "C"
{
#include <thread.h>
#include <synch.h>
}

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef THREADMUTEX_H_
#  include <Thread/ThreadMutex.h>
#endif

/*
 *
 * None thread system simple mutex
 *
 */

class NoneMutex : public ThreadMutex
{
    friend ThreadMutex;
    
public:
    virtual ~NoneMutex ();
    
    virtual Boolean lock ();
    virtual Boolean unlock ();
    virtual Boolean tryLock ();
    
private:
    NoneMutex (Boolean);
};

#endif
