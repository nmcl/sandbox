#include <iostream.h>

#define _INIT_
#include <gnulwp.h>
#include "gnu_thread.h"
    
const int GNU_Thread::MaxPriority = 7;

GNU_Thread::GNU_Thread (int prio)
{
    to_wait = 0;
    caddr_t p1;

    initlp(MaxPriority);
    p1 = (caddr_t) this;
    thread_key = base_key++;
    my_block = creatp(prio, GNU_Thread::Execute, 8000, 0, 0, p1);
}

GNU_Thread::~GNU_Thread () {}

void GNU_Thread::Execute (int dummy1, char** dummy2, GNU_Thread* p1)
{
    // Do this to remove compiler warnings.
    dummy1 = 0;
    dummy2 = 0;

    if ((p1->thread_key == 1) && (Self()->Current_Thread() == 0))
    {
	signals(p1->to_wait);
	p1->Body();
    }
    else
    {
	p1->to_wait = creats(0);
	p1->Suspend();
	p1->Body();
    }
}
    
void GNU_Thread::Suspend ()
{
    if ((thread_key == 0) && (Self()->Current_Thread() == 0) && (count < 2))
	count++;

    if ((count == 2) && (!SuspendMain))
    {
	SuspendMain = true;
	struct sem* p = creats(0);
	waits(p);
    }

    if ((thread_key == 1) && (Self()->Current_Thread() == 0))
    {
	if (DoWait)
	{
	    DoWait = false;
	    waits(to_wait);
	}
    }	
    else
	waits(to_wait);
}

void GNU_Thread::Resume ()
{
    if ((thread_key == 1) && (Self()->Current_Thread() == 0))
    {
	readyp(my_block);
	to_wait = creats(0);
	Suspend();
    }
    else
    	signals(to_wait);
}

long GNU_Thread::Current_Thread () { return thread_key; }

void GNU_Thread::Initialize () { initlp(1); }
