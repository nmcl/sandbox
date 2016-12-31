/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: unistd.h,v 1.16 1996/05/14 15:13:14 nmcl Exp $
 */

#ifndef OS_UNISTD_H_
#define OS_UNISTD_H_

#ifndef WIN32
#  include <unistd.h>
#else
#  include <process.h>
#endif

#if defined(NEED_INT_USLEEP) || defined(SOLARIS2) || (defined(NEED_GCC_PROTOTYPES) && !defined(HAS_VOID_USLEEP))
extern "C" int usleep(unsigned int);
#endif

#ifdef NEED_VOID_USLEEP
extern "C" void usleep(unsigned int);
#endif

#ifdef SUNOS4
extern "C" int gethostname(char*, int);
#endif

#endif
