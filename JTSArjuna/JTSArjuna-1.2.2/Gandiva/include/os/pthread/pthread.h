/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: pthread.h,v 1.13 1998/01/16 10:27:03 nmcl Exp $
 */

#ifndef OS_PTHREAD_PTHREAD_H_
#define OS_PTHREAD_PTHREAD_H_

#ifndef PTHREAD_DRAFT_6
#  include <pthread.h>
#else
extern "C"
{
#include <pthread/pthread.h>
}
#endif

#ifdef PTHREAD_DRAFT_6
#define PTHREAD_CREATE_JOINABLE 0
#define PTHREAD_CREATE_DETACHED 1
#endif

#ifdef PTHREAD_DRAFT_8
extern "C"
{
    int pthread_mutexattr_init (pthread_mutexattr_t*);
    int pthread_mutexattr_settype (pthread_mutexattr_t*, unsigned int);
    int pthread_mutexattr_destroy (pthread_mutexattr_t*);
}
#endif

#ifndef PTHREAD_HPUX
#  define pthread_mutexattr_default NULL
#  define pthread_mutexcond_default NULL
#endif

#endif
