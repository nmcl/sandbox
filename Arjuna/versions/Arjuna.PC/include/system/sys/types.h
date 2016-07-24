/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: types.h,v 1.1 1993/11/03 14:34:45 nmcl Exp $
 */

#ifndef SYS_TYPES_H_
#define SYS_TYPES_H_ 

#ifdef STUB
#  pragma @NoRename
#endif

#include <sys\types.h>

#if defined(OS2) || defined(MSDOS)
#  define key_t unsigned long
#  define pid_t int
#  define mode_t int
#  define off_t long
#endif

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
