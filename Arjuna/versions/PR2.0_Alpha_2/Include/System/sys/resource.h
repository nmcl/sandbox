/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: resource.h,v 1.1 1993/11/03 14:34:22 nmcl Exp $
 */

#ifndef SYS_RESOURCE_H_
#define SYS_RESOURCE_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifndef SYS_TIME_H_
#  include <System/sys/time.h>
#endif

#include <sys/resource.h>

#ifdef __linux__
#  include <linux/limits.h>
#endif

#endif
