/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef PROCESSCONS_H_
#include "ProcessCons.h"
#endif


ProcessCons::ProcessCons (Process &p, ProcessCons *n) { Proc = &p; Next = n; }


#ifdef NO_INLINES
#  define PROCESSCONS_CC_
#  include "ProcessCons.n"
#  undef PROCESSCONS_CC_
#endif
