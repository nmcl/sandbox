#ifndef LWP_THREAD_H
#define LWP_THREAD_H

#include <lwp/lwp.h>
#include "thread.h"

/* This is the Sun thread implementation of the Thread virtual class. It provides
 * an implementation for all of the pure virtual functions declared in the Thread
 * base class.
 * The Initialize function must be called at the start of any program which uses the
 * threads implementation as it calls necessary Sun thread initialization routines.
 */

class LWP_Thread : public Thread
{
public:
    virtual void Suspend (); // Suspend an active thread
    virtual void Resume (); // Resume a suspended thread
    virtual void Sleep (struct timeval); // Put active thread to sleep for duration specified

    virtual void Body () = 0; // Body of "active" object (defined in the deriving class)
    virtual long Current_Thread (); // Returns current thread id

    thread_t Thread_ID (); // Returns current thread's Sun thread_key

    // Initialize must be called exactly once at the start of the program
    static void Initialize ();

protected:
    static const int MaxPriority; // Maximum priority of a thread

    LWP_Thread (int priority = MaxPriority); // Create thread with given (or maximum) priority
    LWP_Thread (thread_t); // Create thread with given Sun thread_key

private:
    virtual ~LWP_Thread ();
    static void Execute (LWP_Thread*); // This routine calls the 'main' object code
    thread_t mid;
};

#endif
