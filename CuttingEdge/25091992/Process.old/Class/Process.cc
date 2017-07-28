#include <iostream.h>
#include <stdlib.h>

#include "common.h"
#include "Process.h"
#include "ProcessIterator.h"
#include "ProcessList.h"
#include "ProcessCons.h"
#include "ProcessHeap.h"

static double SimulatedTime = 0.0;
//static ProcessList ReadyQueue;
static ProcessHeap ReadyQueue;
static Process *Current = 0;

extern long qtime;

//
// create scheduler task at MINPRIO
//

#define DEBUG

Scheduler::Scheduler() : LWP_Thread(MINPRIO) {}

Scheduler::~Scheduler() {}

double Scheduler::CurrentTime() { return SimulatedTime; }

void Scheduler::Body()
{
    for (;;)
    {
	Current = ReadyQueue.Remove();
	if (Current == 0)    // all done
	{
#ifdef DEBUG
	    cerr << "Scheduler queue is empty.\n";
#endif
	    exit(0);
	}

	if (Current->evtime() < 0)
	    cerr << "Scheduler Error: Process WakeupTime Invalid.\n";
	else
	    SimulatedTime = Current->evtime();

#ifdef DEBUG
	cerr << "Simulated time is now " << SimulatedTime << "\n";
#endif

	Current->Resume();
    }
}


//
// Class Process
//

const int Process::Never=-1;

Process::~Process() {}

double Process::Time() { return SimulatedTime; }

void Process::set_evtime(double time) { wakeuptime = time; }

void Process::Activate() { ReActivate(); }

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

void Process::ReActivate()
{
    if (idle())
	ReadyQueue.Remove(this);
    set_evtime(CurrentTime());
    Resume();
}

// cancels next burst of activity, process becomes idle
void Process::Cancel()
{
    if (idle())
	ReadyQueue.Remove(this);
    else
	Suspend();

    wakeuptime = Process::Never;
}

double  Process::evtime() { return wakeuptime; }

boolean Process::idle()
{
    if (wakeuptime == CurrentTime())
	return false;
    else
	return true;
}

boolean Process::terminated() { return Terminated; }

Process::Process() : LWP_Thread()
{
    wakeuptime = CurrentTime();
}

// suspend current process for simulated time t
void Process::Hold(double t) 
{
    struct timeval stime, ftime;

    gettimeofday(&stime, 0);
    ActivateDelay(t);
    gettimeofday(&ftime, 0);

    qtime += ((ftime.tv_sec*1000000)+ftime.tv_usec) - ((stime.tv_sec*1000000)+stime.tv_usec);
    Suspend();
}

// suspend current process indefinitely (i.e., make idle)
void Process::Passivate()
{
    wakeuptime = Process::Never;
    this->Terminated = true;
    Suspend();
}

double Process::CurrentTime() { return SimulatedTime; }
