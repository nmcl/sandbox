/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: signal.h,v 1.4 1993/03/22 09:43:05 ngdp Exp $
 */

#ifndef SIGNAL_H_
#define SIGNAL_H_

#ifdef STUB
#  pragma @NoRename
#endif

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

#endif
