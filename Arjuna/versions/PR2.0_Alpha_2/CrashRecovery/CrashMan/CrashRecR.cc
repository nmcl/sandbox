/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CrashRecR.cc,v 1.1 1993/11/03 14:17:02 nmcl Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#include "CrashRecR.h"

CrashRecRecord::CrashRecRecord ()
                               : recoveryStatus(NOT_COMPLETE),
                                 next(0),
                                 previous(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CrashRecRecord::CrashRecRecord ()\n" << flush;
#endif
}

CrashRecRecord::~CrashRecRecord()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CrashRecRecord::~CrashRecRecord()\n" << flush;
#endif

    if (previous)
        previous->next = next;

    if (next)
        next->previous = previous;



}

ostream& CrashRecRecord::print ( ostream& strm ) const
{
    strm << "Recovery Status = " << recoveryStatus << endl;
    return (strm);
}

Boolean CrashRecRecord::resolve()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CrashRecRecord::resolve()\n" << flush;
#endif

    return TRUE;
}

void CrashRecRecord::setRecoveryStatus(RecoveryStatus newStatus)
{
    recoveryStatus = newStatus;
}


RecoveryStatus CrashRecRecord::getRecoveryStatus()
{
    return(recoveryStatus);
}

Boolean CrashRecRecord::rpc_handler ()
{
    /*
     * Prevent core dumps on RPC failures
     */

    return FALSE;
}
