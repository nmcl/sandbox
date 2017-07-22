#include "lwp_thread.h"

LWP_Thread::LWP_Thread(boolean start_now)
{
    caddr_t p1;

    p1 = (caddr_t) this;
    (void) lwp_create(&mid, LWP_Thread::Execute, MAXPRIO, 0, lwp_newstk(), 1, p1);
    if (!start_now)
	(void) lwp_suspend(mid);
}

LWP_Thread::~LWP_Thread() {}

void LWP_Thread::Execute(LWP_Thread *p1) { p1->Body(); }

void LWP_Thread::Suspend() { (void) lwp_suspend(mid); }

void LWP_Thread::Resume() { (void) lwp_resume(mid); }

long LWP_Thread::Identity() { return mid.thread_key; }

thread_t LWP_Thread::Thread_ID() { return mid; }
