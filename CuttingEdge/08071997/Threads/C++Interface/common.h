#ifndef Common_
#define Common_

#include <iostream.h>
#include <lwp/lwp.h>

#define MAXPRIO 10

class Process;

extern "C"
{
#include <lwp/stackdep.h>

    int pod_setmaxpri(int);
    int lwp_setstkcache(int, int);
#ifdef Main_
    int lwp_create(thread_t*, void (*func)(), int, int, stkalign_t*, int);
#else
    int lwp_create(thread_t*, void (*func)(Process *), int, int, stkalign_t*, int, caddr_t);
#endif
    int lwp_yield(thread_t);
    int lwp_suspend(thread_t);
    int lwp_resume(thread_t);
    int lwp_setpri(thread_t, int);
}

#ifndef true
typedef int boolean;
#define true 1
#define false 0
#endif

#define NOW -1.0

#endif
