/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>
#include <stdlib.h>

#ifndef COMMON_H_
#include "common.h"
#endif

#ifndef PROCESS_H_
#include "Process.h"
#endif

#ifndef PROCESSITERATOR_H_
#include "ProcessIterator.h"
#endif

#ifndef PROCESSLIST_H_
#include "ProcessList.h"
#endif

#ifndef PROCESSCONS_H_
#include "ProcessCons.h"
#endif

static ProcessList ReadyQueue;
static Process *Current = 0;

//
// create scheduler task at MINPRIO
//

Scheduler::Scheduler () : Thread_Type(MINPRIO) {}

Scheduler::~Scheduler () {}

void Scheduler::Body ()
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

const int Process::Never=-1; // Process will never awaken.

Process::~Process () {}

double Process::CurrentTime () { return SimulatedTime; }

void Process::ActivateBefore (Process &p)
{
    // No op if already scheduled
    if (idle()) return;

    if (ReadyQueue.InsertBefore(*this, p))
	wakeuptime = p.wakeuptime;
    else
	cerr << "ActivateBefore failed because 'before' process is not scheduled" << endl;
}

void Process::ActivateAfter (Process &p)
{
    // No op if already scheduled
    if (idle()) return;

    if (ReadyQueue.InsertAfter(*this, p))
	wakeuptime = p.wakeuptime;
    else
	cerr << "ActivateAfter failed because 'after' process is not scheduled" << endl;
}

void Process::ActivateAt (double AtTime, boolean prior)
{
    // No op if already scheduled
    if (idle()) return;

    wakeuptime = AtTime;
    ReadyQueue.Insert(*this, prior);
}

void Process::ActivateDelay (double Delay, boolean prior)
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

void Process::ReActivateAt (double AtTime, boolean prior)
{
    Cancel();
    ActivateAt(AtTime, prior);
}

void Process::ReActivateDelay (double Delay, boolean prior)
{
    Cancel();
    ActivateDelay(Delay, prior);
}

void Process::ReActivate ()
{
    if (idle())
	ReadyQueue.Remove(this);
    set_evtime(CurrentTime());
    Resume();
}

// cancels next burst of activity, process becomes idle
void Process::Cancel ()
{
    if (idle())
	ReadyQueue.Remove(this);
    else
	Suspend();

    wakeuptime = Process::Never;
}

boolean Process::idle ()
{
    if (wakeuptime == CurrentTime())
	return false;
    else
	return true;
}

Process::Process() : Thread_Type()
{
    wakeuptime = CurrentTime();
}

// suspend current process for simulated time t
void Process::Hold (double t) 
{
    ActivateDelay(t);
    Suspend();
}

// suspend current process indefinitely (i.e., make idle)
void Process::Passivate ()
{
    wakeuptime = Process::Never;
    this->Terminated = true;
    Suspend();
}


#ifdef NO_INLINES
#  define PROCESS_CC_
#  include "Process.n"
#  undef PROCESS_CC_
#endif
