/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: signal.h,v 1.1 1993/11/03 14:33:05 nmcl Exp $
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
