/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: stdlib.h,v 1.5 1996/03/28 15:56:35 nmcl Exp $
 */

#ifndef OS_STDLIB_H_
#define OS_STDLIB_H_

#include <stdlib.h>

#ifdef NEED_GCC_PROTOTYPES
extern "C"
{
    void bzero (void*, int);
}
#endif

#endif
