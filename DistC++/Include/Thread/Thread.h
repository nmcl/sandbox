/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Thread.h,v 1.1 1997/09/25 15:28:14 nmcl Exp $
 */

#ifndef THREAD_H_
#define THREAD_H_

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef INITTHREADS_H_
#  include <Thread/InitThreads.h>
#endif

/*
 *
 * Common simple interface to various thread systems
 *
 */

class ThreadData;

template <class T> class ThreadSafeHashList;

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
    
    Boolean join ();    
    Boolean resume ();			/* resume this thread */
    
    ThreadStatus status () const;
    const Uid& get_uid () const;
    
    /* Static methods */
    
    static Thread *current ();		/* pointer to current thread */
    static void current (Uid&);		/* id of current thread */
    static void exit ();

    static Boolean join (const Uid&);
    static Boolean sleep (unsigned int);
    static Boolean suspend ();		/* suspend this thread */
    static Boolean yield ();

private:
    Thread ();				/* used by init to create the 'main' thread */

    Boolean terminate ();

    static void *execute (void *);
    static void init ();		/* init thread system */
    static void shutdown ();		/* shutdown thread system */    
    
    ThreadData *theThread;		/* thread system specific */
};

extern ThreadSafeHashList<Thread> *allThreads;

#endif
