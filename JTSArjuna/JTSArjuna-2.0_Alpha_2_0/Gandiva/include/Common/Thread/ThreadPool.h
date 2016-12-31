/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadPool.h,v 1.2 1998/01/19 12:53:57 nmcl Exp $
 */

#ifndef COMMON_THREAD_THREADPOOL_H_
#define COMMON_THREAD_THREADPOOL_H_

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef TEMPLATE_ARRAY_H_
#  include <Template/Array.h>
#endif

class PoolThread;

/*
 * This is the thread pool controller. It creates instances of the
 * PoolThread class which do the actual threading work.
 */

class ThreadPool : public Resource
{
public:
    /*
     * Create a new thread pool object which can field the specified
     * number of threads. All threads from a given pool have the
     * same ThreadSetup paramaters. (Maybe too course? Set at creation
     * time as with standard threads?)
     */
    
    ThreadPool (unsigned int numberOfThreads, ThreadSetup* = 0);
    virtual ~ThreadPool ();

    /*
     * Create a new thread. If all of the threads available
     * from this pool object are in use then the Boolean
     * parameter indicates whether we should block until one is
     * available or simply return NULL (the default).
     */
    
    Thread* create (void* (*body)(void*), void*, Boolean block = FALSE);
    
private:
    void initialize ();
    
    ThreadSetup* _setup;
    Array<void*>* _threadPool;
    unsigned int _numberOfThreads;

public:
    /*
     * Gain access to implementation specific methods.
     */

    virtual Resource* control (const ClassName&);
    
    static  const ClassName& name ();
    virtual const ClassName& className () const;
    
    virtual void*       castup (const ClassName&) const;
    static  ThreadPool* castup (Resource*);
};

#endif
