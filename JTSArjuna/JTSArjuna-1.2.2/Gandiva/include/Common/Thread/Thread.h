/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Thread.h,v 1.6 1998/08/29 10:36:36 nmcl Exp $
 */

#ifndef COMMON_THREAD_THREAD_H_
#define COMMON_THREAD_THREAD_H_

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef OS_TYPES_H_
#  include <os/sys/types.h>
#endif

#ifndef COMMON_CLASSNAME_H_
#  include <Common/ClassName.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

#ifndef CORE_RESOURCE_H_
#  include <Core/Resource.h>
#endif

#ifndef COMMON_THREAD_INITTHREADS_H_
#  include <Common/Thread/InitThreads.h>
#endif

#ifndef TEMPLATE_BASICLIST_H_
#  include <Template/BasicList.h>
#endif

#ifndef TEMPLATE_HASHEDLIST_H_
#  include <Template/HashedList.h>
#endif

class ThreadCleanup;
class CleanupList;
class ThreadSetup;
class Mutex;

class ThreadData;
class Action;

class Thread : public Resource
{
    friend class ThreadControl;
    friend InitThreads;
    friend ThreadData;
    
public:
    enum ThreadStatus { THR_IS_RUNNING, THR_IS_SUSPENDED, THR_IS_BLOCKED, THR_IS_WAITING,
			THR_IS_TERMINATED, THR_IS_CREATED, THR_IS_PASSIVATED };

    enum ThreadAttachStatus { THR_IS_ATTACHED, THR_IS_DETACHED };

    /*
     * Some useful status values and thread-specific values, such as
     * the priorities and stack sizes allowed.
     */
    
    static const int MaxPriority;
    static const int MinPriority;
    static const int InvalidPriority;

    static const int defaultPriority;
    static const unsigned long defaultStackSize;

    /*
     * Action specific methods.
     */

    virtual Action* currentAction ();     /* retrieve current action */
    virtual void currentAction (Action*);

    /*
     * Now the actual thread methods.
     */

    static Thread* create (void* (*body)(void*), void*, ThreadSetup* = 0);
    
    /*
     * Various methods for controlling threads.
     */

    virtual Boolean resume  ();      // Resume a suspended thread
    virtual Boolean suspend ();      // Suspend an active thread
    
    virtual Boolean kill    ();      // kill the thread, and its children
    virtual Boolean run     ();      // Start a (suspended) created thread

    /*
     * Terminate the thread. This is a clean termination, i.e.,
     * the thread will be forced to wait for its children to terminate.
     */
    
    virtual Boolean terminate ();
    
    ThreadStatus status () const;
    const Uid&   uid    () const;

    /*
     * Static methods.
     * These methods operate on the current thread only.
     */

    static Thread* current ();		/* pointer to current thread */
    static Thread* find (const Uid&);
    static void exit ();
    
    /*
     * Wait for a thread(s) to terminate.
     */
    
    static void*   wait (Thread*);
    static void*   wait (Thread*, unsigned long waittime, Boolean& timeout);
    static void    waitForAllChildren ();

    static Boolean sleep (unsigned long); // time in microseconds
    static Boolean yield ();              // yield current thread

    /*
     * Miscellaneous routines.
     */
    
    Boolean addCleanup (ThreadCleanup*);
    virtual Resource* control (const ClassName&);

    /*
     * Some basic control methods we can support. Currently we return valid
     * answers for all implementations, even those which do not actually
     * support priorites or stacks, e.g., single threads. We need to change
     * this eventually to differentiate between implementations for which these
     * operations make sense.
     */

    int priority () const;
    
    /*
     * Print information about running threads.
     */
    
    virtual ostream& print    (ostream&) const;
    static  ostream& printAll (ostream&);

    /*
     * We may need to add some methods to allow explicit
     * (though limited) access to the child list for
     * a thread. At the moment you can only print it.
     */

    virtual ostream& printChildren (ostream&) const;

protected:
    Thread (ThreadAttachStatus = THR_IS_ATTACHED);
    Thread (void* (*func)(void*), void* param, ThreadSetup* = 0);
    virtual ~Thread ();

    void addChild ();
    
    void killChildren ();
    void cleanup ();      // run through thread cleanup list

    static HashedList<Thread, Uid>& list ();
    static Boolean initialize ();
    static void shutdown ();

    ThreadData* _theThread;		/* thread system specific */
    
    static Mutex* _mutex;
    
private:
    static void* execute (void *);
    
    BasicList<Thread, Uid>* _childList;
    CleanupList*            _cleanupList;

    static HashedList<Thread, Uid>* _list;
    static Boolean                  _initialized;
    
public:
    static  const ClassName& name ();
    virtual const ClassName& className () const;
    
    virtual void*   castup (const ClassName&) const;
    static  Thread* castup (Resource*);
};

#include <Common/Thread/Thread.n>

#endif
