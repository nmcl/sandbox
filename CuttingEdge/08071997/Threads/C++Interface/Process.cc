#include <iostream.h>
#include <stdlib.h>
#include <lwp/lwp.h>

extern "C"
{
#include <lwp/stackdep.h>
}

#include "common.h"
#include "Process.h"
#include "ProcessIterator.h"
#include "ProcessList.h"
#include "ProcessCons.h"

static double SimulatedTime = 0.0;
static ProcessList ReadyQueue;
static Process *Current = 0;

// create scheduler task at MINPRIO

void Scheduler()
{
    for (;;)
    {
	Current = ReadyQueue.Remove();
	if (Current == 0)    // all done
	    exit(0);

	SimulatedTime = Current->evtime();
	Current->set_evtime(NOW);

	cerr << "Simulated time is now " << SimulatedTime << "\n";
	
	(void) lwp_resume(Current->GetMid());
    }
}

double Process::Time() { return SimulatedTime; }

void Process::set_evtime(double time) { wakeuptime = time; }

void Process::ActivateBefore (Process &p)
{
    // No op if already scheduled
    if (idle()) return;

    if (ReadyQueue.InsertBefore(*this, p))
	wakeuptime = p.wakeuptime;
    else
	cerr << "ActivateBefore failed because 'before' process is not scheduled" << endl;
}

void Process::ActivateAfter(Process &p)
{
    // No op if already scheduled
    if (idle()) return;

    if (ReadyQueue.InsertAfter(*this, p))
	wakeuptime = p.wakeuptime;
    else
	cerr << "ActivateAfter failed because 'after' process is not scheduled" << endl;
}

void Process::ActivateAt(double AtTime, boolean prior)
{
    // No op if already scheduled
    if (idle()) return;

    wakeuptime = AtTime;
    ReadyQueue.Insert(*this, prior);
}

void Process::ActivateDelay(double Delay, boolean prior)
{
    // No op if already scheduled
    if (idle()) return;

    wakeuptime = SimulatedTime + Delay;
    ReadyQueue.Insert(*this, prior);
}


// Similarly, there are four ways to reactivate
// Note that if a process is already scheduled, the reactivate
// will simply re-schedule the process.

void Process::ReActivateBefore (Process &p)
{
    Cancel();
    ActivateBefore(p);
}

void Process::ReActivateAfter  (Process &p)
{
    Cancel();
    ActivateAfter(p);
}

void Process::ReActivateAt(double AtTime, boolean prior)
{
    Cancel();
    ActivateAt(AtTime, prior);
}

void Process::ReActivateDelay(double Delay, boolean prior)
{
    Cancel();
    ActivateDelay(Delay, prior);
}


// cancels next burst of activity, process becomes idle
void Process::Cancel()
{
    if (idle()) {
	wakeuptime = NOW;
	ReadyQueue.Remove(this);
    }
}

double  Process::evtime() { return wakeuptime; }

boolean Process::idle()
{
    if (wakeuptime == NOW)
	return false;
    else
	return true;
}

boolean Process::terminated() { return Terminated; }

void Process::Execute(Process *p1)
{
    p1->Body();
    p1->Terminated = true;
}

void Process::SetMid(thread_t newthread)
{
    mid.thread_id = newthread.thread_id;
    mid.thread_key = newthread.thread_key;
}

thread_t Process::GetMid() { return mid; }

Process::Process(boolean start_now)
{
    thread_t new_thread;    // create task running body
    caddr_t p1;

    p1 = (caddr_t) this;
    ReadyQueue.Insert(*this);

    lwp_create(&new_thread, Process::Execute, MAXPRIO, 0, lwp_newstk(), 1, p1);
    SetMid(new_thread);

    if (!start_now)
	lwp_suspend(mid);
}

// suspend current process for simulated time t
void Process::Hold(double t) 
{
    ActivateDelay(t);
    (void)lwp_suspend(SELF);
}

// suspend current process indefinitely (i.e., make idle)
void Process::Passivate()
{
    wakeuptime = NOW;
    lwp_suspend(SELF);
}

void Process::Resume() { lwp_resume(mid); }
