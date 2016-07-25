/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: unistd.h,v 1.5 1996/07/01 11:59:29 ngdp Exp $
 */

#ifndef UNISTD_H_
#define UNISTD_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifndef WIN32
#  include <unistd.h>
#else
#  include <process.h>
#endif

#ifdef WIN32
extern "C"
{
	unsigned int sleep (unsigned int);
}
#endif

#ifdef NEED_USLEEP_PROTO
extern "C"
{
    int usleep(unsigned int);
}
#endif

#if defined(sun) && !defined(SVR4)
extern "C"
{
    int getpagesize ();
}
#endif

/*
 * Some machines have different names for the same thing...
 */

#ifndef _SC_PAGESIZE
#  if defined _SC_PAGE_SIZE
#    define _SC_PAGESIZE _SC_PAGE_SIZE
#  endif
#endif

#ifdef WIN32

#define F_OK 00
#define W_OK 02
#define R_OK 04
#define WR_OK 06

#endif

#endif