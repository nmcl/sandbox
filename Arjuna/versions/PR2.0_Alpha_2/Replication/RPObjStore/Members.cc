/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Members.cc,v
 */


#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef MEMBERS_H_
#  include <Replication/Members.h>
#endif


/*
 * The Members class holds the relevant information on which replicas (primary and backups)
 * are available i.e., responded to initiate.
 */

Members::Members ()
                  : CRPC(0),
		    RPCCont(0),
		    r_state_daemon(0),
		    next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Members::Members ()\n" << flush;
#endif
}

Members::~Members ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Members::~Members ()\n" << flush;
#endif
}

