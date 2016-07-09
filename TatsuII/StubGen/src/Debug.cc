/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Debug.cc,v 1.1 1997/06/09 19:52:20 nmcl Exp $
 */

/*
 * Stub generator debug system
 *
 */

#ifndef WIN32
#  include <strstream>
#else
#  include <strstrea.h>
#endif

#ifndef DEBUG_H_
#  include "Debug.h"
#endif

static const char RCSid[] = "$Id: Debug.cc,v 1.1 1997/06/09 19:52:20 nmcl Exp $";

/*
 * The following global variables are used by the debugging operations to
 * determine whether debugging information is produced and the stream they
 * are sent to.
 */

/* The debug control class */
DebugController _debug;

ostrstream* DebugController::dbgStream = 0;

/*
 * Flush is UGLY. There is no way to ignore the contents of the stream
 * so we discard it entirely!
 */

void DebugController::dbgFlush ( DebugLevel dbg,
				 FacilityCode fac, VisibilityLevel vis )
{
    if ((currentLevel & dbg) && (currentFac & fac) && (currentVis & vis))
	cerr << dbgStream->rdbuf() << flush;
    else
    {
	if (dbgStream)
	{
	    delete dbgStream;
	    dbgStream = 0;
	}
    }
}


#ifdef NO_INLINES
#  define DEBUG_CC_
#  include "Debug.n"
#  undef DEBUG_CC_
#endif
