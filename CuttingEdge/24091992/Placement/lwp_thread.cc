#define INIT_

#include "common.h"
#include "lwp_thread.h"

/* These are the Sun C routines which give access to threads. They have to be
 * defined in this way. The multiple definitions for lwp_create are as a result
 * of the way in which C++ allows C routines to be declared and the way in which
 * Sun's lwp_create is defined.
 */

extern "C"
{
#include <lwp/stackdep.h>

    int pod_setmaxpri(int);
    int lwp_setstkcache(int, int);
#ifndef Scheduler_
#ifdef Main_
    int lwp_create(thread_t*, void (*func)(), int, int, stkalign_t*, int);
#else
    int lwp_create(thread_t*, void (*func)(LWP_Thread *), int, int, stkalign_t*, int, caddr_t);
#endif
#endif
    int lwp_yield(thread_t);
    int lwp_suspend(thread_t);
    int lwp_resume(thread_t);
    int lwp_setpri(thread_t, int);
    int lwp_self(thread_t*);
    int lwp_ping(thread_t);
    int lwp_sleep(struct timeval);
}

//
// Class LWP_Thread
//

const int LWP_Thread::MaxPriority=10;

LWP_Thread::LWP_Thread (int prio)
{
    caddr_t p1;

    p1 = (caddr_t) this;
    (void) lwp_create(&mid, LWP_Thread::Execute, prio, 0, lwp_newstk(), 1, p1);
    thread_key = mid.thread_key;
    (void) lwp_suspend(mid);
}

// For creating the LWP_Thread for "main"
LWP_Thread::LWP_Thread (thread_t tid)
{
    thread_key = tid.thread_key;
}

LWP_Thread::~LWP_Thread () {}

void LWP_Thread::Execute (LWP_Thread *p1) { p1->Body(); }

void LWP_Thread::Suspend () { (void) lwp_suspend(mid); }

void LWP_Thread::Resume () { (void) lwp_resume(mid); }

void LWP_Thread::Sleep (struct timeval doze) { (void) lwp_sleep(doze); }

long LWP_Thread::Current_Thread ()
{
    thread_t tid;

    (void) lwp_self(&tid);

    return tid.thread_key;
}

thread_t LWP_Thread::Thread_ID () { return mid; }

//
// Getting the main thread into the thread list...
//

class LWP_Main_Thread : public LWP_Thread
{
public:
    LWP_Main_Thread (thread_t t) : LWP_Thread(t) {}
    ~LWP_Main_Thread ();

    void Body ();
};

LWP_Main_Thread::~LWP_Main_Thread () {}

void LWP_Main_Thread::Body () {}

void LWP_Thread::Initialize ()
{
    (void) pod_setmaxpri(MaxPriority);
    (void) lwp_setstkcache(1000, 40);

    thread_t me;
    lwp_self(&me);
    new LWP_Main_Thread(me);
}

