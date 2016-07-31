/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ShadowR.cc,v 1.7 1995/02/10 11:41:49 ngdp Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef LOCALOSTYPES_H_
#  include <ObjectStore/LocalOSTypes.h>
#endif

#ifndef SERVERAA_H_
#  include <Arjuna/ServerAA.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

#ifndef SHADOWR_H_
#  include <ShadowR.h>
#endif

#ifndef MAILER_H_
#  include <Mailer.h>
#endif

/*
 * Enum: ShadowStatus
 * Records status of a shadow state managed by the associated ShadowRecord
 */

ostream& operator<< (ostream& strm, ShadowStatus s)
{
    switch (s)
    {
        case UNRESOLVED_STATE:
            strm << "unresolved";
            break;
        case COMMITTED_STATE:
            strm << "committed";
            break;
        case ABORTED_STATE:
            strm << "aborted";
            break;
        case IGNORE_STATE:
            strm << "ignored";
            break;
        default:
            strm << "unknown";
            break;
    }
    return strm;
}

/*
 * Class: ShadowRecord
 * Responsible for the recovery of unresolved shadow states.
 */

/*
 * Initialise statics
 */

int ShadowRecord::shadowThreshold = 2;

//-Public-Functions------------------------------------------------------------

ShadowRecord::ShadowRecord ( ostream& strm, Boolean safe, const Uid& recUid,
                             const TypeName recType, const TypeName OSType,
			     const char *OSRoot)
                           : CrashRecRecord(strm, safe),
			     storeType(0),
			     storeRoot(0),
                             objType(0),
                             objUid(recUid),
                             status(UNRESOLVED_STATE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ShadowRecord::ShadowRecord(" << recUid << ", "
                 << recType << ", " << OSType << ", " << OSRoot << ")" << endl;
#endif
    // Initialise storeType
    storeType = clone((char*) OSType);

    // Initialise storeRoot
    const char *location = 0;
    if (OSRoot == 0)
	location = "";
    else
	location = OSRoot;
    storeRoot = new char[::strlen(location) + 1];
    ::strcpy(storeRoot, location);

    // Initialise objType
    objType = clone((char*) recType);
}

ShadowRecord::~ShadowRecord()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ShadowRecord::~ShadowRecord()" << endl;
#endif
    if (storeType)
	delete storeType;

    if (storeRoot)
	delete storeRoot;

    if (objType)
	delete objType;
}

Boolean ShadowRecord::resolve()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ShadowRecord::resolve()" << endl;
#endif

    if (getRecoveryStatus() == COMPLETE)
        return(TRUE);

    outStrm << "ShadowRecord: Beginning recovery (" << objUid << ")" << endl;

    // Create the appropriate objectstore type
    ObjectStore *store = ObjectStore::create(storeType, storeRoot);

    // Check that this state is still uncommitted
    if (store->isType(objUid, objType, ObjectStore::OS_UNCOMMITTED))
    {
	// The state is still shadowed, abort it
	if (store->remove_uncommitted(objUid, objType))
	{
	    status = ABORTED_STATE;
	    setRecoveryStatus(COMPLETE);
	}
	else
	{
	    outStrm << "ShadowRecord: *WARNING* Attempt to remove uncommitted "
		    << "state failed" << endl;
	}
    }
    else
    {
	outStrm << "ShadowRecord: Object is no longer uncommitted, "
		<< "no further work required." << endl;
	status = IGNORE_STATE;
	setRecoveryStatus(COMPLETE);
    }
    
    ObjectStore::destroy(store);

    Boolean success = ( (getRecoveryStatus() == COMPLETE) ? TRUE : FALSE );

    if(success)
        outStrm << "ShadowRecord: Recovery successful (" << objUid << ")"
                << endl;
    else
        outStrm << "ShadowRecord: Recovery unsuccessful (" << objUid << ")"
                << endl;

    checkAlarms();
    return (success);
}

void ShadowRecord::print(ostream& strm)
{
    CrashRecRecord::print(strm);
    strm << "ShadowRecord for uid: " << objUid << endl;
    strm << "\tType = " << objType << endl;
    strm << "\tObject store type = " << storeType << endl;
    strm << "\tRoot = " << storeRoot << endl;
    strm << "\tStatus = " << status << endl;
}

void ShadowRecord::id (ostream& strm)
{
    strm << "ShadowRecord for uid: " << objUid << ", Type = " << objType
	 << endl;
}

const CRRecordType ShadowRecord::type() const
{
    return SHADOWRECORD;
}

const Uid& ShadowRecord::value() const
{
    return(objUid);
}

const TypeName& ShadowRecord::getType() const
{
    return(objType);
}

//-Private-Functions---------------------------------------------------------

void ShadowRecord::setThreshold(int t)
{
    shadowThreshold = t;
}

int ShadowRecord::getThreshold()
{
    return(shadowThreshold);
}

void ShadowRecord::alert()
{
    Mailer m(outStrm);

    m << "Crash recovery has made " << numberAttempts()
      << " attempts to resolve the following uncommitted object state:\n"
      << endl;
    
    m << "\tObject uid  = " << objUid << endl;
    m << "\tObject type = " << objType << endl;
    m << "\tObjectStore type = " << storeType << endl;
    
    m << "\nAttempts are continuing. "
      << "If this matter is resolved you will be informed." << endl;

    m.sendMessage();
}

void ShadowRecord::calm()
{
    Mailer m(outStrm);

    m << "Crash recovery has successfully resolved the following "
      << "object state after " << numberAttempts() << "attempts.\n" << endl;

    m << "\tObject uid  = " << objUid << endl;
    m << "\tObject type = " << objType << endl;
    m << "\tObjectStore type = " << storeType << endl;

    m.sendMessage();
    resetAlarms();
}








