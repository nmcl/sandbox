#ifndef PROCESS_H
#define PROCESS_H
#include "common.h"

void Scheduler();

class Process {
public:
    double Time();	// returns the simulated time at which the current process is active

    // There are four ways to activate a process:
    //   1) before another process, 
    //   2) after another process, 
    //   3) at a specified (simulated) time, or 
    //   4) after a specified (simulated) delay

    void ActivateBefore (Process &);
    void ActivateAfter  (Process &);
    void ActivateAt     (double AtTime = NOW, boolean prior = false);
    void ActivateDelay  (double AtTime = NOW, boolean prior = false);

    // Similarly, there are four ways to reactivate
    // Note that if a process is already scheduled, the reactivate
    // will simply re-schedule the process.

    void ReActivateBefore (Process &);
    void ReActivateAfter  (Process &);
    void ReActivateAt     (double AtTime = NOW, boolean prior = false);
    void ReActivateDelay  (double AtTime = NOW, boolean prior = false);

    void Cancel();	// cancels next burst of activity, process becomes idle
    double  evtime();	// time at which process is scheduled to be active
    void set_evtime(double);  // set wakeuptime (used by Scheduler)

    boolean idle();	// true if process is not currently scheduled to wake up
    boolean terminated();

    void SetMid(thread_t);
    thread_t GetMid();
    void Resume();

    // The pure virtual function, Body, defines the code that executes in the process
    virtual void Body() = 0;
   static void Execute(Process*);

protected:
    Process(boolean);
    void Hold(double t);// suspend current process for simulated time t
    void Passivate();	// suspend current process (i.e., make idle)

private:
    double   wakeuptime;
    boolean  Terminated;
    thread_t mid;
};

#endif // PROCESS_H
