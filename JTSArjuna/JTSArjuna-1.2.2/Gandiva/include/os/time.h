/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: time.h,v 1.5 1996/01/08 12:14:47 nmcl Exp $
 */

#ifndef OS_TIME_H_
#define OS_TIME_H_

#include <time.h>

#if defined(SOLARIS2) && !defined(HAS_BSD_GETTIMEOFDAY)

extern "C" int gettimeofday(struct timeval *tp);

#endif

#endif
