/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: malloc.h,v 1.1 1993/11/03 14:32:48 nmcl Exp $
 */

#ifndef MALLOC_H_
#define MALLOC_H_

#ifdef STUB
#  pragma @NoRename
#endif

#include <malloc.h>

#if defined(hpux) || defined(SYSV) || defined(_AIX) || defined(__linux__)
typedef void* malloc_t;
#endif

#endif
