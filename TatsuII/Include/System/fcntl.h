/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: fcntl.h,v 1.1 1997/06/09 19:51:58 nmcl Exp $
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
