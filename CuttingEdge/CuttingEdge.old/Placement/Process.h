#ifndef PROCESS_H
#define PROCESS_H

#include "common.h"
#include "lwp_thread.h"

/* This is the main class for the simulation package. All objects which are to possess
 * independent threads of control but which are to be controlled by the simulation
 * scheduler MUST be derived from the Process class. The Scheduler, which is also an
 * object which possesses an independent thread and which exists within the simulation
 * environment, is shown here, but is only derived from the lwp_thread class and not Process
 * as it is the central controller for the simulation environment and as such runs independently
 * of everything else.
 * If debugging is turned on (-DDEBUG added to the CFLAGS for the Makefile) then the scheduler
 * will print out the current simulation time and other information.
 */

class Scheduler : public LWP_Thread
{
public:
    Scheduler ();
    ~Scheduler ();

    void Body ();
    double CurrentTime ();

private:
    double ReActivateTime;
    Thread* mainthread;
};

class Process : public LWP_Thread
{
public:
    static const int Never;

    ~Process ();

    double Time ();	// returns the simulated time at which the current process is active
    static double CurrentTime (); // returns the current simulation time

    // There are four ways to activate a process:
    //   1) before another process, 
    //   2) after another process, 
    //   3) at a specified (simulated) time, or 
    //   4) after a specified (simulated) delay

    void Activate ();
    void ActivateBefore (Process &);
    void ActivateAfter  (Process &);
    void ActivateAt     (double AtTime = CurrentTime(), boolean prior = false);
    void ActivateDelay  (double AtTime = CurrentTime(), boolean prior = false);

    // Similarly, there are four ways to reactivate
    // Note that if a process is already scheduled, the reactivate
    // will simply re-schedule the process.

    void ReActivateBefore (Process &);
    void ReActivateAfter  (Process &);
    void ReActivateAt     (double AtTime = CurrentTime(), boolean prior = false);
    void ReActivateDelay  (double AtTime = CurrentTime(), boolean prior = false);
    void ReActivate ();


    void Cancel ();	// cancels next burst of activity, process becomes idle
    double  evtime ();	// time at which process is scheduled to be active
    void set_evtime (double);  // set wakeuptime (used by Scheduler)

    boolean idle ();	// true if process is not currently scheduled to wake up
    boolean terminated (); // returns whether or not the object has been passivated

    // The pure virtual function, Body, defines the code that executes in the process
    virtual void Body () = 0;

protected:
    Process ();
    void Hold (double t);// suspend current process for simulated time t
    void Passivate ();	// suspend current process (i.e., make idle)

private:
    double   wakeuptime;
    boolean  Terminated;
};

#endif // PROCESS_H
