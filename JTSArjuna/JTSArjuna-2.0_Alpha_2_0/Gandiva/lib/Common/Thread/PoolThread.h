/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PoolThread.h,v 1.1 1998/01/19 12:54:24 nmcl Exp $
 */

#ifndef COMMON_THREAD_POOLTHREAD_H_
#define COMMON_THREAD_POOLTHREAD_H_

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

/*
 * Instances of this class represent the real thread which resides
 * within the pool.
 */

class PoolThread : public Thread
{
public:
    PoolThread (ThreadSetup* = 0);
    virtual ~PoolThread ();

    /*
     * Overloaded Thread methods.
     */
    
    Boolean terminate ();
    Boolean kill ();

    Boolean passivated () const;
    Boolean waitForWork ();
    
    Boolean getDestroyed () const;
    void    setDestroyed ();
    
    void assignThread (void* (*body)(void*), void* arg);

    void doWork ();

    static void* poolBody (void*);

protected:
    void setup ();

private:
    Boolean _passivated;
    Boolean _destroyed;
    Boolean _attached;
    Boolean _running;

    void* _arg;
    void* _result;
    void* (*_threadBody)(void*);
};

#include <Common/Thread/PoolThread.n>

#endif
