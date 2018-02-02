/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AAStatus.cc,v 1.7 1994/09/26 20:41:39 ndbi Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef LOCALOSTYPES_H_
#  include <ObjectStore/LocalOSTypes.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#define AASTATUS_SERVER
#include "AAStatus_stub.h"

/* 
 * Enum: ActionCondition
 * Records the status of an atomic action
 */

ostream& operator<< (ostream& strm, ActionCondition s)
{
    switch (s)
    {
        case ACTION_COMMITTED:
            strm << "committed";
            break;
        case ACTION_ABORTED:
            strm << "aborted";
            break;
        case ACTION_ATTACHED:
            strm << "attached";
            break;
        default:
            strm << "unknown";
            break;
    }
    return strm;
};

/*
 * Class: AAStatus
 * Used to check for the existence of AtomicAction states in the object store
 */

//-Public-Functions------------------------------------------------------------

AAStatus::AAStatus(int& res)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "AAStatus::AAStatus()" << endl;
#endif

    res = 1;
}

AAStatus::~AAStatus()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "AAStatus::~AAStatus()" << endl;
#endif
}

ActionCondition AAStatus::isCommitted(const Uid& u, Boolean checkProcess)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "AAStatus::isCommitted(" << u << ", " << checkProcess
                 << ")" << endl;
#endif
    ActionCondition result = ACTION_STATUS_UNKNOWN;
    AtomicAction aa(u);

    if ( aa.activate() )
    {
        int aaPid = aa.getCreatorPid();
        if (pingProcess(aaPid))
        {
            result = ACTION_ATTACHED;
        }
        else
        {
            result = ACTION_COMMITTED;
        }
    }
    else
    {
        result = ACTION_ABORTED;
    }

    return(result);
}

//-Private-Functions-----------------------------------------------------------

Boolean AAStatus::pingProcess(int pid)
{
    Boolean alive = ((kill(pid, 0) != -1) ? TRUE : FALSE);
    return(alive);
}
