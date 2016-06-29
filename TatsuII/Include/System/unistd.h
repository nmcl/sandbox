/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: unistd.h,v 1.1 1997/06/09 19:52:00 nmcl Exp $
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

#endif
