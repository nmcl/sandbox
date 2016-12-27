/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: signal.h,v 1.5 1996/10/24 11:49:48 nmcl Exp $
 */

#ifndef OS_SIGNAL_H_
#define OS_SIGNAL_H_

/*
 * Some typedefs for signal handling - these are ANSI C compliant
 * but may break on other systems
 */

#ifndef DOTDOTDOT
#  define DOTDOTDOT
#endif

#ifdef HAVE_INTSIG
typedef int SIG_FUNC_TYPE (int DOTDOTDOT);
#else
typedef void  SIG_FUNC_TYPE (int DOTDOTDOT);
#endif

typedef SIG_FUNC_TYPE *SIG_ARGTYPE;

#include <signal.h>

#ifdef NEED_GCC_PROTOTYPES
extern "C" {
    int sigprocmask(int, const sigset_t *, sigset_t *);
    int sigblock(int);
    int sigsetmask(int);
}
#endif

#endif
