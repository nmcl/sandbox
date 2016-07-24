/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CrashRecR.cc,v 1.7 1995/02/10 11:42:41 ngdp Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SYS_SOCKET_H_
#  include <System/sys/socket.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef LOCALOSTYPES_H_
#  include <ObjectStore/LocalOSTypes.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef CRASHRECR_H_
#  include <CrashRecR.h>
#endif

/*
 * Operators for CrashRecRecord enums
 */

ostream& operator<< (ostream& strm, RecoveryStatus s)
{
    switch (s)
    {
        case NOT_COMPLETE:
            strm << "not complete";
            break;
        case COMPLETE:
            strm << "complete";
            break;
        default:
            strm << "unknown";
            break;
    }
    return strm;
}

/*
 * Class: Crash Recovery Record (CrashRecRecord)
 * Base class of recovery records. The inherited classes are responsible
 * for performing specific recovery operations. These records are managed
 * by the CRRecordList class.
 */

//-Public-Functions------------------------------------------------------------

void CrashRecRecord::print(ostream& strm)
{
    strm << "Recovery Status = " << recoveryStatus;
    strm << " [this= " << (void *) this << " next= " << next
         << " previous= " << previous << "]" << endl;
}

void CrashRecRecord::setRecoveryStatus(RecoveryStatus newStatus)
{
    recoveryStatus = newStatus;
}

RecoveryStatus CrashRecRecord::getRecoveryStatus()
{
    return(recoveryStatus);
}

Boolean CrashRecRecord::operator== (const CrashRecRecord *crr) const
{
    return ( ( type() == crr->type() ) && ( value() == crr->value() ) );
}

//-Protected-Functions---------------------------------------------------------

CrashRecRecord::CrashRecRecord (ostream& strm, Boolean safe)
                               : outStrm(strm),
                                 foundAtReboot(safe),
                                 recoveryStatus(NOT_COMPLETE),
				 noAttempts(0),
				 alerted(FALSE),
                                 next(0),
                                 previous(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PROTECTED;
    debug_stream << "CrashRecRecord::CrashRecRecord(" << safe << ")" << endl;
#endif
}

CrashRecRecord::~CrashRecRecord()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CRASH_RECOVERY << VIS_PROTECTED;
    debug_stream << "CrashRecRecord::~CrashRecRecord()" << endl;
#endif

    if (previous)
        previous->next = next;

    
    if (next)
        next->previous = previous;
}

Boolean CrashRecRecord::rpc_handler ( RPC_Status, Int32 )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PROTECTED;
    debug_stream << "CrashRecRecord::rpc_handler()" << endl;
#endif

    /*
     * Prevent core dumps on RPC failures
     */

    return FALSE;
}


Boolean CrashRecRecord::compareHosts(const char *host1, const char *host2)
{
    Boolean sameHost = FALSE;
    struct hostent *host;
    char *host1OfficialName = 0;

    /*
     * We need to copy the first name to another string as gethostbyname
     * returns a pointer to a static area.
     */

    if ( (host = gethostbyname((char*)host1)) != 0 )
    {
        int nameLen = sizeof(host->h_name) + 1;
        host1OfficialName = new char[nameLen];
        ::memset(host1OfficialName, '\0', nameLen);
        ::strncpy(host1OfficialName, host->h_name, sizeof(host->h_name));

        if ( (host = gethostbyname((char *)host2)) != 0 )
        {
            if (::strncmp(host1OfficialName,
                          host->h_name, sizeof(host->h_name)) == 0)
            {
                sameHost = TRUE;
            }
        }
        delete host1OfficialName;
    }
    return(sameHost);
}

void CrashRecRecord::checkAlarms()
{
    if (getRecoveryStatus() != COMPLETE)
    {
	noAttempts++;

	if ( (noAttempts > getThreshold()) && !(alerted) )
	{
	    alert(); // fire alert routine
	    alerted = TRUE;
	}
    }
    else
    {
	if (alerted)
	{
	    calm(); // fire calm routine
	}
    }
}

void CrashRecRecord::resetAlarms()
{
    alerted = FALSE;
    noAttempts = 0;
}

int CrashRecRecord::numberAttempts()
{
    return(noAttempts);}


//-Private-Functions-----------------------------------------------------------

void CrashRecRecord::setNext(CrashRecRecord *crr)
{
    next = crr;
}

void CrashRecRecord::setPrevious(CrashRecRecord *crr)
{
    previous = crr;
}
