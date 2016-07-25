/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: fcntl.h,v 1.3 1995/01/05 09:06:09 ngdp Exp $
 */

#ifndef FCNTL_H_
#define FCNTL_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifdef HIDE_FCNTL_PROTO
extern "C" int fcntl (int, int, ...);
#  define fcntl _fcntl_hidden_
#endif

#include <fcntl.h>

#undef fcntl

#endif
