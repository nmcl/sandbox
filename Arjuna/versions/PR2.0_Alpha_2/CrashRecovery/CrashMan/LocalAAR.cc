/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LocalAAR.cc,v 1.1 1993/11/03 14:17:05 nmcl Exp $
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

#ifndef LPOBJSTORE_H_
#  include <ObjectStore/LPObjStore.h>
#endif

#ifndef SERVERAA_H_
#  include <Arjuna/ServerAA.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef LOCALAAR_H_
#  include "LocalAAR.h"
#endif

AARecord::AARecord ( const Uid& actUid )
                   : CrashRecRecord(),
                     AAUid(actUid),
                     status(UNRESOLVED_AA)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "AARecord::AARecord(const Uid&)\n" << flush;
#endif
}

AARecord::~AARecord()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "AARecord::~AARecord()\n" << flush;
#endif
}

ostream& AARecord::print ( ostream& strm ) const
{
    CrashRecRecord::print(strm);
    strm << "AARecord for uid : " << AAUid << endl;
    strm << "AARecord Status = " << status << endl;
    return strm;
}

ostream& AARecord::id ( ostream& strm ) const
{
    strm << "AARecord for AA uid = " << AAUid << endl;
    return strm;
}

Boolean AARecord::resolve()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "AARecord::resolve()\n" << flush;
#endif

    cout << "\nBEGINNING AA RECOVERY (" << AAUid << ")" << endl;

    LocalPersistentObjectStore store; 
    AtomicAction aa(AAUid);
    aa.activate(&store);

    cout << "Attempting to commit action..." << endl;

    aa.crashRecoveryCommit();

    setRecoveryStatus(COMPLETE);

    cout << "\nFINISHED AA RECOVERY (" << AAUid << ")" << endl;

    return(TRUE);
}

