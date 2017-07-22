/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * Various useful definitions.
 */


#ifndef COMMON_H_
#define COMMON_H_

#include <iostream.h>

// Do not forget to change the Makefile accordingly for the right thread library!

// Can choose from LWP_Thread or GNU_Thread

#define Thread_Type LWP_Thread

// Can choose from LWPTHREAD or GNUTHREAD

#define LWPTHREAD

#ifndef true
typedef int boolean;
#define true 1
#define false 0
#endif

#endif
