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
#  include "CommonT.h"
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SYS_IPC_H_
/* This is for key_t */
#  include <System/sys/ipc.h>
#endif

/*
 * Prototypes for some generally useful routines
 */

extern unsigned long checkSum (const void *, size_t);
extern char *clone (const char *const );
extern key_t typeToKey (const char *tname);

#if defined(NEED_USLEEP_PROTO)
extern "C" int usleep (unsigned int);
#endif

#endif