/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LocalAAR.cc,v 1.6 1994/09/26 20:41:25 ndbi Exp $
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

#ifndef MAILER_H_
#  include <Mailer.h>
#endif

#ifndef LOCALAAR_H_
#  include <LocalAAR.h>
#endif

/*
 * Enum: Atomic Action Status (AAStatus)
 * Records status of AA managed by the associated AARecord
 */

ostream& operator<< (ostream& strm, AAStatus s)
{
    switch (s)
    {
        case UNRESOLVED_AA:
            strm << "unresolved";
            break;
        case COMMITTED_AA:
            strm << "committed";
            break;
        default:
            strm << "unknown";
            break;
    }
    return strm;
}

/*
 * Initialise statics
 */

int AARecord::AAThreshold = 0; // default retry threshold for AARecords

/*
 * Class: AARecord
 * Responsible for the recovery of local atomic actions
 */

//-Public-Functions------------------------------------------------------------

AARecord::AARecord ( ostream& strm, Boolean safe, const Uid& actUid )
                   : CrashRecRecord(strm, safe),
                     AAUid(actUid),
                     status(UNRESOLVED_AA)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "AARecord::AARecord(" << actUid << ")" << endl;
#endif
}

AARecord::~AARecord()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "AARecord::~AARecord()" << endl;
#endif
}

Boolean AARecord::resolve()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "AARecord::resolve()" << endl;
#endif

    if (getRecoveryStatus() == COMPLETE)
        return(TRUE);

    outStrm << "AARecord: Beginning recovery (" << AAUid << ")" << endl;

    AtomicAction aa(AAUid);

    if ( !aa.activate() )
    {
        outStrm << "AARecord: Action was transient - no work necessary"
                << endl;
        setRecoveryStatus(COMPLETE);
    }
    else
    {
        if ( kill(aa.getCreatorPid(), 0) == -1 )
        {
            // The process associated with this action is dead, commit it
            aa.crashRecoveryCommit();
            status = COMMITTED_AA;
        }
        setRecoveryStatus(COMPLETE);
    }

    Boolean success = ( (getRecoveryStatus() == COMPLETE) ? TRUE : FALSE );

    if(success)
        outStrm << "AARecord: Recovery successful (" << AAUid << ")" << endl;
    else
        outStrm << "AARecord: Recovery unsuccessful (" << AAUid << ")" << endl;

    checkAlarms();
    return (success);
}

void AARecord::print(ostream& strm)
{
    CrashRecRecord::print(strm);
    strm << "AARecord for uid : " << AAUid << endl;
    strm << "AARecord Status = " << status << endl;
}

void AARecord::id (ostream& strm)
{
    strm << "AARecord for AA uid = " << AAUid << endl;
}

const CRRecordType AARecord::type() const
{
    return AARECORD;
}

const Uid& AARecord::value() const
{
    return(AAUid);
}

//-Private-Functions---------------------------------------------------------

void AARecord::setThreshold(int t)
{
    AAThreshold = t;
}

int AARecord::getThreshold()
{
    return(AAThreshold);
}

void AARecord::alert()
{
    Mailer m(outStrm);

    m << "Crash recovery has made " << numberAttempts()
      << " attempts to resolve a local atomic action " << endl;
    m << "with uid = " << AAUid << endl;
    m << "\nAttempts are continuing. "
      << "If this matter is resolved you will be informed." << endl;

    m.sendMessage();
}

void AARecord::calm()
{
    Mailer m(outStrm);

    m << "Re: Atomic Action with uid = " << AAUid << "\n" <<endl;
    
    m << "After " << numberAttempts() << "attempts crash recovery has "
      << " successfully resolved this action." << endl;
    
    m.sendMessage();
    resetAlarms();
}

