/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id:
 */

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef DAEMONDATA_H_
#  include <Replication/DaemonData.h>
#endif


DaemonData::DaemonData ()
                        : memb(0),
			  hostName(0),
			  available(TRUE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "DaemonData::DaemonData ()\n" << flush;
#endif
}

DaemonData::~DaemonData ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "DaemonData::~DaemonData ()\n" << flush;
#endif
}
