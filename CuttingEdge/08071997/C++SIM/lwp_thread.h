/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef LWP_THREAD_H_
#define LWP_THREAD_H_

#include <lwp/lwp.h>

#ifndef THREAD_H_
#include "thread.h"
#endif

/* This is the Sun thread implementation of the Thread virtual class. It provides
 * an implementation for all of the pure virtual functions declared in the Thread
 * base class.
 * The Initialize function must be called at the start of any program which uses the
 * threads implementation as it calls necessary Sun thread initialization routines.
 *
 * Note: if any problems occur when using the Sun thread package, try increasing the
 * stack size and/or the number of stacks to create in Initialize.
 */

class LWP_Thread : public Thread
{
public:
    virtual void Suspend ();                // Suspend an active thread
    virtual void Resume ();                 // Resume a suspended thread
    virtual void Sleep (struct timeval);    // Put active thread to sleep for duration specified

    virtual void Body () = 0;               // Body of "active" object (defined in the deriving class)
    virtual long Current_Thread () const;   // Returns current thread id

    thread_t Thread_ID () const;            // Returns current thread's Sun thread_key

    // Initialize must be called exactly once at the start of the program
    static void Initialize ();

protected:
    static const int MaxPriority;           // Maximum priority of a thread

    LWP_Thread (int priority = MaxPriority); // Create thread with given (or maximum) priority
    LWP_Thread (thread_t);                  // Create thread with given Sun thread_key
    virtual ~LWP_Thread ();

private:
    static void Execute (LWP_Thread*);     // This routine calls the 'main' object code
    thread_t mid;
};

#endif
