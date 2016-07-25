/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecListI.cc,v 1.1 1993/11/03 12:30:07 nmcl Exp $
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

static const char RCSid[] = "$Id: RecListI.cc,v 1.1 1993/11/03 12:30:07 nmcl Exp $";

#ifdef NO_INLINES
#  define RECLISTI_CC_
#  include "RecListI.n"
#  undef RECLISTI_CC_
#endif
