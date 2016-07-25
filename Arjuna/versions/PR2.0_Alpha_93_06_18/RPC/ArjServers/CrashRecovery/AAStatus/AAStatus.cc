/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AAStatus.cc,v 1.1 1993/08/16 10:40:56 ndbi Exp $
 */

#include <iostream.h>

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef LPOBJSTORE_H_
#  include <Arjuna/LPObjStore.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#define AASTATUS_SERVER
#include "AAStatus_stub.h"

AAStatus::AAStatus()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "AAStatus::AAStatus()\n" << flush;
#endif
}

AAStatus::~AAStatus()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "AAStatus::~AAStatus()\n" << flush;
#endif
}

Boolean AAStatus::isCommitted(Uid u)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "AAStatus::isCommitted(Uid u)\n" << flush;
#endif

    AtomicAction A;
    LocalPersistentObjectStore store;
    ObjectState *state;

    state = store.read_state(u, A.type());

    if (A.activate(&store))
        cout << "Successful activate\n";
    else
        cout << "Unsuccessful activate\n";

    if (state != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
