#ifndef GNU_THREAD_H_
#define GNU_THREAD_H_

#include "common.h"
#include "thread.h"

#define MINPRIO 2

class GNU_Thread : public Thread
{
public:
    virtual void Suspend (); // Suspend an active thread
    virtual void Resume (); // Resume a suspended thread

    virtual void Body () = 0; // Body of "active" object (defined in the deriving class)
    virtual long Current_Thread (); // Returns current thread id

    // Initialize must be called exactly once at the start of the program
    static void Initialize ();

protected:
    static const int MaxPriority; // Maximum priority of a thread
    static long base_key;

    GNU_Thread (int priority = MaxPriority); // Create thread with given (or maximum) priority

private:
    virtual ~GNU_Thread ();
    static void Execute (int, char**, GNU_Thread*); // This routine calls the 'main' object code
    static boolean DoWait;
    static boolean SuspendMain;
    static int count;

    long thread_key;
    struct sem* to_wait;
    struct pcb* my_block;
};

#ifdef _INIT_
#undef _INIT_
    long GNU_Thread::base_key = 0;
    boolean GNU_Thread::DoWait = true;
    boolean GNU_Thread::SuspendMain = false;
    int GNU_Thread::count = 0;
#endif
#endif
