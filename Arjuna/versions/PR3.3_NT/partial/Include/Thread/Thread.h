/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Thread.h,v 1.3 1995/07/26 14:50:53 ngdp Exp $
 */

#ifndef THREAD_H_
#define THREAD_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef INITTHREADS_H_
#  include <Thread/InitThreads.h>
#endif

/*
 *
 * Common simple interface to various thread systems
 *
 */

class ThreadID;
class ThreadData;
class AtomicAction;
class ThreadList;
class ThreadListEntry;

class Thread
{
    friend class InitThreads;
    
public:
    enum ThreadStatus { THR_IS_RUNNING, THR_IS_BLOCKED, THR_IS_TERMINATED,
			THR_IS_SUSPENDED };

    enum ThreadAttachStatus { THR_IS_ATTACHED, THR_IS_DETACHED };
    
    Thread (void * (*func)(void*), void *arg, ThreadStatus = THR_IS_SUSPENDED,
	    ThreadAttachStatus = THR_IS_ATTACHED, void * = 0, size_t = 0);
    ~Thread ();

    /* Non static methods */
    
    AtomicAction *currentAction () const; /* retreive current action */
    void currentAction (AtomicAction *); /* set current action */
    
    Boolean join ();    
    Boolean resume ();			/* resume this thread */
    Boolean suspend ();			/* suspend this thread */
    
    ThreadStatus status () const;
    const ThreadID& threadId () const;
    
    /* Static methods */
    
    static Thread *current ();		/* pointer to current thread */
    static void current (ThreadID&);	/* id of current thread */
    static void exit ();

    static Boolean join (const ThreadID&);
    static Boolean sleep (unsigned int);
    static Boolean yield ();


private:
    Thread ();				/* used by init to create the 'main' thread */

    Boolean terminate ();

    static void *execute (void *);
    static void init ();		/* init thread system */
    static void shutdown ();		/* shutdown thread system */
    
    
    ThreadData *theThread;		/* thread system specific */
};

extern ThreadList *allThreads;

#endif
