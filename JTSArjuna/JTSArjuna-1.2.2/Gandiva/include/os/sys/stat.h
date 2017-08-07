/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: stat.h,v 1.4 1996/03/28 12:41:25 nsmw Exp $
 */

#ifndef OS_SYS_STAT_H_
#define OS_SYS_STAT_H_

#ifndef OS_SYS_TYPES_H_
#   include <os/sys/types.h>
#endif

#include <sys/stat.h>

#ifdef NEED_FCHMOD
extern "C" int fchmod(int, mode_t);
#endif

#ifdef NEED_FSYNC
extern "C" int fsync(int);
#endif

#endif
