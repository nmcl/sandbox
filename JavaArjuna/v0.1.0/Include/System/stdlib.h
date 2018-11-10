/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: stdlib.h,v 1.4 1995/02/13 11:35:33 ngdp Exp $
 */

#ifndef STDLIB_H_
#define STDLIB_H_

#ifdef STUB
#  pragma @NoRename
#endif

#include <stdlib.h>

#if defined(NEED_GCC_PROTOTYPES) || defined(__ultrix)
extern "C" {
    int usleep (unsigned int);
    void bzero (void *, int);
    long lrand48 ();
    void srand48 (long);
}
#endif
#endif
