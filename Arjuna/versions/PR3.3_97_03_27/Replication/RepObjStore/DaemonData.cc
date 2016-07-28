/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: DaemonData.cc,v 1.2 1996/07/05 13:00:27 nmcl Exp $
 */

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef DAEMONDATA_H_
#  include <DaemonData.h>
#endif


DaemonData::DaemonData ()
                        : memb(0),
			  hostName(0),
			  available(TRUE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "DaemonData::DaemonData ()\n" << flush;
#endif
}

DaemonData::~DaemonData ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "DaemonData::~DaemonData ()\n" << flush;
#endif
}
