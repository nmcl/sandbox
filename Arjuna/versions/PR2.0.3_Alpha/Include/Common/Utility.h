/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 * 
 * $Id: Utility.h,v
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#ifndef COMMONT_H_
#include "CommonT.h"
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

/*
 * Prototypes for some generally useful routines
 */

extern key_t typeToKey ( const TypeName tname );

#if defined (hpux) || defined(SYSV)
extern unsigned int usleep ( unsigned int );
#endif

#endif
