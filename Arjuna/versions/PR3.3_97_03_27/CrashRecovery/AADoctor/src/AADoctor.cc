/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AADoctor.cc,v 1.5 1994/08/18 15:24:00 ndbi Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef SYS_TIME_H_
#include <System/sys/time.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef LOCALOSTYPES_H_
#  include <ObjectStore/LocalOSTypes.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef OBJSTOREI_H_
#  include <ObjectStore/ObjStoreI.h>
#endif

#ifndef SERVERAA_H_
#  include <Arjuna/ServerAA.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef LOCALAAR_H_
#  include <LocalAAR.h>
#endif

#ifndef SERVERAAR_H_
#  include <ServerAAR.h>
#endif

#ifndef SHADOWR_H_
#  include <ShadowR.h>
#endif

#ifndef AADOCTOR_H_
#  include <AADoctor.h>
#endif

/*
 * Class: AADoctor
 * Responsible for the co-ordination of atomic action recovery
 */

//-Public-Functions------------------------------------------------------------

AADoctor::AADoctor (ostream& strm, char* recoveringHost, CRRecordList *work)
                   : outStrm(strm),
                     host(recoveringHost),
                     workList(work)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "AADoctor::AADoctor(" << recoveringHost << ")" << endl;
#endif
}

AADoctor::~AADoctor ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "AADoctor::~AADoctor()" << endl;
#endif
}


Boolean AADoctor::phase1()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "AADoctor::phase1()" << endl;
#endif

    /*
     * Any unresolved AAs or SAAs that are detected at this phase (just
     * after a reboot, before the manager is up) are known to be safe for
     * recovery.
     */
    Boolean safe = TRUE;
    Boolean anyWork = FALSE;

    outStrm << "\nAADoctor: Commencing phase 1 recovery scan"  << endl;

    if (findUnresolvedAAs(safe) > 0)
        anyWork = TRUE;

    if (findUnresolvedSAAs(safe) > 0)
        anyWork = TRUE;

    if (findShadowedStates(safe) > 0)
        anyWork = TRUE;

    outStrm << "AADoctor: Completed phase 1 recovery scan\n" << endl;
    return(anyWork);
}

Boolean AADoctor::phase2()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "AADoctor::phase2()" << endl;
#endif

    outStrm << "\nAADoctor: Commencing phase 2 recovery scan"  << endl;

    Boolean safe = FALSE;
    Boolean anyWork = FALSE;

    if (findUnresolvedAAs(safe) > 0)
        anyWork = TRUE;

    if (findUnresolvedSAAs(safe) > 0)
        anyWork = TRUE;

    outStrm << "AADoctor: Completed phase 2 recovery scan\n" << endl;
    return(anyWork);
}

//-Private-Functions-----------------------------------------------------------

int AADoctor::findUnresolvedAAs(Boolean safe)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PRIVATE;
    debug_stream << "AADoctor::findUnresolvedAAs()" << endl;
#endif

    Uid aa_uid(NIL_UID);
    AtomicAction tempAA;
    ObjectStore *aaStore = tempAA.store();
    ObjectStoreIterator get_next_aa(*aaStore, tempAA.type());
    int numberFound = 0;

    outStrm << "AADoctor: Searching for local atomic actions..." << endl;

    while ( (aa_uid = get_next_aa()) != NIL_UID )
    {
        AARecord *aar = new AARecord(outStrm, safe, aa_uid);

        if (workList->scan(aar))
        {
            delete aar;
        }
        else
        {
            outStrm << "AADoctor: Found AtomicAction with uid = "
                    << aa_uid << endl;
            workList->insert(aar);
            numberFound++;
        }
    }

    if (numberFound == 0)
    {
        outStrm << "AADoctor: No local atomic actions were found" << endl;
    }
    else
    {
        outStrm << "AADoctor: Found " << numberFound
                << " local atomic action(s)" << endl;
    }

    return (numberFound);
}

int AADoctor::findUnresolvedSAAs(Boolean safe)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PRIVATE;
    debug_stream << "AADoctor::findUnresolvedSAAs()" << endl;
#endif

    Uid saa_uid(NIL_UID);
    ServerAtomicAction tempSAA(NIL_UID);;
    ObjectStore *saaStore = tempSAA.store();
    ObjectStoreIterator get_next_saa(*saaStore, tempSAA.type());
    int numberFound = 0;

    outStrm << "AADoctor: Searching for server atomic actions..." << endl;

    while ( (saa_uid = get_next_saa()) != NIL_UID )
    {
        ServerAARecord *saar = new ServerAARecord(outStrm, safe, saa_uid,
						  host);

        if (workList->scan(saar))
        {
            delete saar;
        }
        else
        {
            outStrm << "AADoctor: Found ServerAtomicAction with uid = "
                    << saa_uid << endl;
            workList->insert(saar);
            numberFound++;
        }
    }

    if (numberFound == 0)
    {
        outStrm << "AADoctor: No server atomic actions were found" << endl;
    }
    else
    {
        outStrm << "AADoctor: Found " << numberFound
                  << " server atomic action(s)" << endl;
    }

    return (numberFound);
}

int AADoctor::findShadowedStates(Boolean safe)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PRIVATE;
    debug_stream << "AADoctor::findShadowedStates()" << endl;
#endif

    int numberFound = 0;

    TypeName osType = DEFAULT_OBJECTSTORE_Q;
    ObjectState typeState;
    ObjectState uidState;
    Uid uidFound;

    outStrm << "AADoctor: Searching for uncommitted object states..." << endl;

    ObjectStore *os = ObjectStore::create(osType);

    if (os->allTypes(typeState))
    {
	char *p = 0;
	typeState.unpack(p);
	while (p && ::strlen(p) > 0)
	{
	    if (os->allObjUids(p, uidState, ObjectStore::OS_UNCOMMITTED))
	    {
		uidFound.unpack(uidState);
		
		while (uidFound != NIL_UID)
		{
		    ShadowRecord *sr = new ShadowRecord(outStrm, safe,
							uidFound, p, osType,
							0);
		    if (workList->scan(sr))
		    {
			delete sr;
		    }
		    else
		    {
			outStrm << "AADoctor: Found (" << p << ", "
				<< uidFound << ")" <<  endl;
			workList->insert(sr);
			numberFound++;
		    }
		    uidFound.unpack(uidState);
		}
	    }
	
	    delete [] p;
	    p = 0;
	    typeState.unpack(p);
	}
    }

    ObjectStore::destroy(os);

    if (numberFound == 0)
    {
        outStrm << "AADoctor: No uncommitted object states were found" << endl;
    }
    else
    {
        outStrm << "AADoctor: Found " << numberFound
                << " uncommitted object state(s)" << endl;
    }

    return (numberFound);
}



