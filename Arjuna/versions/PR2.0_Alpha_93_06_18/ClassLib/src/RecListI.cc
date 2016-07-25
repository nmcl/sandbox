/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecListI.cc,v 1.3 1993/03/22 09:20:07 ngdp Exp $
 */

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef RECLIST_H_
#  include "RecList.h"
#endif

#ifndef RECLISTI_H_
#  include "RecListI.h"
#endif

static const char RCSid[] = "$Id: RecListI.cc,v 1.3 1993/03/22 09:20:07 ngdp Exp $";

#ifdef NO_INLINES
#  define RECLISTI_CC_
#  include "RecListI.n"
#  undef RECLISTI_CC_
#endif
