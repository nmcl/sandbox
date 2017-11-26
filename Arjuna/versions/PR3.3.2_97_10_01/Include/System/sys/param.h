/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: param.h,v 1.1.6.1 1996/10/10 12:26:04 ngdp Exp $
 */

#ifndef SYS_PARAM_H_
#define SYS_PARAM_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifndef WIN32

#include <sys/param.h>

#else

#define MAXHOSTNAMELEN 128

#endif

#endif
