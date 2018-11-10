/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: unistd.h,v 1.2 1995/07/19 14:04:24 ngdp Exp $
 */

#ifndef UNISTD_H_
#define UNISTD_H_

#ifdef STUB
#  pragma @NoRename
#endif

#include <unistd.h>

/*
 * Some machines have different names for the same thing...
 */

#ifndef _SC_PAGESIZE
#  if defined _SC_PAGE_SIZE
#    define _SC_PAGESIZE _SC_PAGE_SIZE
#  endif
#endif

#endif
