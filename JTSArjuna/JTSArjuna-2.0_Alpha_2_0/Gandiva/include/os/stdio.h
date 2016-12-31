/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: stdio.h,v 1.4 1996/05/14 15:13:13 nmcl Exp $
 */

#ifndef OS_STDIO_H_
#define OS_STDIO_H_

#include <stdio.h>

#ifdef NEED_RENAME
extern "C" int rename (const char*, const char*);
#endif

#endif
