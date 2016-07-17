/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Debug.cc,v 1.5 1993/03/22 09:30:49 ngdp Exp $
 */

/*
 * Stub generator debug system
 *
 */

#include <strstream.h>

#ifndef DEBUG_H_
#  include "Debug.h"
#endif

static const char RCSid[] = "$Id: Debug.cc,v 1.5 1993/03/22 09:30:49 ngdp Exp $";

/*
 * The following global variables are used by the debugging operations to
 * determine whether debugging information is produced and the stream they
 * are sent to.
 */

/* The debug control class */
DebugController _debug;

/* The ostrstream it uses */

ostrstream DebugController::dbg_stream;

/* Another name for the same stream - for convenience */
ostrstream& debug_stream = DebugController::dbg_stream;


void DebugController::dbg_flush ( DebugLevel dbg,
			          FacilityCode fac, VisibilityLevel vis )
{
    if ((current_level & dbg) && (current_fac & fac) && (current_vis & vis))
	cerr << dbg_stream.rdbuf() << flush;
    else
    {
	streambuf *sb = dbg_stream.rdbuf();
	//	while (sb->snextc() != EOF) ;
	dbg_stream.seekp(0,ios::beg);
    }
}


#ifdef NO_INLINES
#  define DEBUG_CC_
#  include "Debug.n"
#  undef DEBUG_CC_
#endif
