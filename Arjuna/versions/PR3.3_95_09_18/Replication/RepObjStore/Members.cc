/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Members.cc,v 1.4 1996/07/05 13:00:29 nmcl Exp $
 */


#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef MEMBERS_H_
#  include <Members.h>
#endif


/*
 * The Members class holds the relevant information on which replicas (primary and backups)
 * are available i.e., responded to initiate.
 */

Members::Members ()
                  : CRPC(0),
		    RPCCont(0),
		    rpobjstore_daemon(0),
		    pingTopLevel(NIL_UID),
		    next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Members::Members ()\n" << flush;
#endif
}

Members::~Members ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Members::~Members ()\n" << flush;
#endif
}

