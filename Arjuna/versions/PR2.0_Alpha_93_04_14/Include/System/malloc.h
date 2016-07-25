/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: malloc.h,v 1.6 1993/04/27 14:45:02 ngdp Exp $
 */

#ifndef MALLOC_H_
#define MALLOC_H_

#ifdef STUB
#  pragma @NoRename
#endif

#include <malloc.h>

#if defined(hpux) || defined(SYSV)
typedef void* malloc_t;
#endif

#endif
