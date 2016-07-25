/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Debug.cc,v 1.7 1993/07/27 10:43:59 ngdp Exp $
 */

/*
 * Stub generator debug system
 *
 */

#include <strstream.h>

#ifndef DEBUG_H_
#  include "Debug.h"
#endif

static const char RCSid[] = "$Id: Debug.cc,v 1.7 1993/07/27 10:43:59 ngdp Exp $";

/*
 * The following global variables are used by the debugging operations to
 * determine whether debugging information is produced and the stream they
 * are sent to.
 */

/* The debug control class */
DebugController _debug;

/* The ostrstream it uses */

ostrstream DebugController::dbgStream;

/* Another name for the same stream - for convenience */
ostrstream& debugStream = DebugController::dbgStream;


void DebugController::dbgFlush ( DebugLevel dbg,
				 FacilityCode fac, VisibilityLevel vis )
{
    if ((currentLevel & dbg) && (currentFac & fac) && (currentVis & vis))
	cerr << dbgStream.rdbuf() << flush;
    else
    {
	dbgStream.seekp(0,ios::beg);
    }
}


#ifdef NO_INLINES
#  define DEBUG_CC_
#  include "Debug.n"
#  undef DEBUG_CC_
#endif
