/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: lwp.h,v 1.6 1998/01/12 13:11:17 nmcl Exp $
 */

#ifndef OS_LWP_LWP_H_
#define OS_LWP_LWP_H_

#include <lwp/lwp.h>

/*
 * These are the Sun C routines which give access to threads. They have to be
 * defined in this way because some implementations of lwp.h do not
 * contain the prototypes.
 */

#ifdef BROKEN_LWP_H
extern "C"
{
#include <lwp/stackdep.h>
#include <lwp/lwpmachdep.h>

    int pod_setmaxpri(int);
    int lwp_stkcswset(thread_t, caddr_t);
    int lwp_setstkcache(int, int);
    int lwp_create(thread_t*, void* (*func)(void*), int, int, stkalign_t*, int, caddr_t);
    int lwp_destroy(thread_t);
    int lwp_yield(thread_t);
    int lwp_suspend(thread_t);
    int lwp_resume(thread_t);
    int lwp_setpri(thread_t, int);
    int lwp_self(thread_t*);
    int lwp_ping(thread_t);
    int lwp_sleep(struct timeval*);
    int lwp_join(thread_t);
    void pod_exit(int);

    int mon_create(mon_t*);
    int mon_destroy(mon_t);
    int mon_enter(mon_t);
    int mon_exit(mon_t);
    int mon_cond_enter(mon_t);
    int mon_waiters(mon_t, thread_t*, thread_t[], int);
    int mon_break(mon_t);

    cv_t cv_create(cv_t*, mon_t);
    int cv_destroy(cv_t);
    int cv_wait(cv_t);
    int cv_notify(cv_t);
    int cv_send(cv_t, thread_t);
    int cv_broadcast(cv_t);
    int cv_enumerate(cv_t[], int);
    int cv_waiters(cv_t, thread_t[], int);
}
#endif

#endif
