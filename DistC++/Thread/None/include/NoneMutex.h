/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NoneMutex.h,v 1.1 1997/09/25 15:32:32 nmcl Exp $
 */

#ifndef NONEMUTEX_H_
#define NONEMUTEX_H_

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
