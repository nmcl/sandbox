/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: types.h,v 1.8 1993/08/20 14:57:06 ngdp Exp $
 */

#ifndef SYS_TYPES_H_
#define SYS_TYPES_H_ 

#ifdef STUB
#  pragma @NoRename
#endif

#include <sys/types.h>

#ifdef __linux__
#  ifndef key_t
#    define key_t unsigned long
#  endif
#endif

#ifdef __linux__
#  ifndef pid_t
#    define pid_t int
#  endif
#endif

#endif
