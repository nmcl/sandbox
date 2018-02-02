/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CRController.cc,v 1.15.2.1 1996/03/06 11:34:37 ngdp Exp $
 */

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef SYS_TYPES_H_
#include <System/sys/types.h>
#endif

#ifndef SYS_TIME_H_
#include <System/sys/time.h>
#endif

#ifndef SYS_STAT_H_
#include <System/sys/stat.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef CRCONTROLLER_H_
#  include <CRController.h>
#endif

#ifndef CRRECTYPE_H_
#  include <CRRecType.h>
#endif

#ifndef AADOCTOR_H_
#include <AADoctor.h>
#endif

#ifndef DOZER_H_
#  include <Dozer.h>
#endif

#ifndef MAILER_H_
#  include <Mailer.h>
#endif

/*
 * Class: CrashRecoveryController (CRController)
 * Manages and controls the recovery operations performed by the recovery
 * doctors (i.e. AADoctor, RepDoctor...)
 */

/*
 * Declare statics
 */

char* CRController::host = 0;
int CRController::refCount = 0;
ConnectMan* CRController::connection = 0;

//-Public-Functions------------------------------------------------------------

CRController::CRController (ostream& strm)
                           : outStrm(strm),
                             workList(new CRRecordList(strm))
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRController::CRController()" << endl;
#endif

    // Check if we've got the hostname
    if (!host)
    {
        host = new char[MAXHOSTNAMELEN];
        memset(host, '\0', MAXHOSTNAMELEN);
        gethostname(host, MAXHOSTNAMELEN);
    }

    // Check we have a connection manager
    if (!connection)
        connection = new ConnectMan(outStrm);

    // Increment reference count
    refCount++;
}

CRController::~CRController()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRController::~CRController()" << endl;
#endif

    if (workList)
        delete workList;

    if (--refCount == 0)
    {
        if (connection)
            delete connection;

        if (host)
            delete host;
    }
}


void CRController::recover()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRController::recover()" << endl;
#endif

    banner(outStrm, TRUE);

    Dozer dozer(outStrm, 30);
    dozer.setThreshold(4);
    dozer.setWarningFunction(warnAdmin);

    // Make sure we have a connection but it is refusing external requests
    while(!connection->control(FALSE))
    {
        // There is a problem with the connection, sleep, then retry
        dozer.doze();
    }

    if (dozer.fired())
        calmAdmin();

    // Create the doctors, these actually perform the recovery
    AADoctor aaDoc(outStrm, host, workList);
    // RepDoctor repDoc(outStrm, host, workList);

    /*
     * Phase 1 recovery is concerned with determining the recovery work
     * required. Thw node is made available after phase 1 is complete.
     */

    aaDoc.phase1();
    //repDoc.phase1();

    // Make one recovery pass
    makeRecoveryPass();

    // It is now safe to allow external connections
    dozer.reset();
    while(!connection->control(TRUE))
    {
        // There is a problem with the connection, sleep, then retry
        dozer.doze();
    }

    /*
     * Phase 2 recovery performs continued attempts to resolve the work
     * that was found during phase 1 and also to find any subsequent work
     * that has arisen
     */

    while (TRUE)
    {
        aaDoc.phase2();
        makeRecoveryPass();
        phaseTwoSleep();
    }
}

//-Private-Functions-----------------------------------------------------------

Boolean CRController::makeRecoveryPass()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PRIVATE;
    debug_stream << "CRController::makeRecoveryPass()" << endl;
#endif
    Boolean res = TRUE;

    if (workList->getWorkLevel() > 0)
    {
        outStrm << "\nCrashRecovery: Commencing recovery pass" << endl;
        res = workList->resolve();
        outStrm << "CrashRecovery: Completed recovery pass" << endl;
    }
    return(res);
}

void CRController::printStatus(ostream& strm)
{
    strm << "Crash recovery controller status:" << endl;
    strm << "recovering host    = " << host << endl;
    strm << "connection status:   ";
    connection->print(strm);
    strm << "Work list follows:" << endl;
    workList->print(strm);
}

void CRController::banner(ostream& strm, Boolean first)
{
    time_t theTime = ::time(0);

    if (first)
    {
        strm << "\nARJUNA CRASH RECOVERY MANAGER STARTUP at "
             << ctime(&theTime) << endl;
    }
    else
    {
        int numJobs = workList->getWorkLevel();

        strm << "\nARJUNA CRASH RECOVERY MANAGER PERIODIC WAKEUP at "
             << ctime(&theTime) << flush;

        if (numJobs == 0)
        {
            strm << "There is currently no work pending recovery" << endl;
        }
        else
        {
            strm << "There are currently " << numJobs
                 << " item(s) of work pending recovery." << endl;
/*
            strm << "\nHere is the work:" << endl;
            workList->printWorkIds(strm);
*/
        }
    }
}

void CRController::phaseTwoSleep()
{
    int sleepTime = PHASE2SLEEP;
    time_t theTime = ::time(0);
    outStrm << "Crash recovery hibernating for " << sleepTime
            << " secs. Its now " << ctime(&theTime) << flush;
    ::sleep(sleepTime);
    banner(outStrm);
}


Boolean warnAdmin()
{
    Mailer m(cerr);

    m << "Crash recovery has made several"
      << " attempts to contact the rpc manager" << endl;
    
    m << "\nAttempts are continuing. "
      << "If this matter is resolved you will be informed." << endl;

    m.sendMessage();

    return (TRUE);
}

Boolean calmAdmin()
{
    Mailer m(cerr);

    m << "Re: Previous warning message\n" << endl;
    m << "Crash recovery has now successfully contacted the rpc manager."
      << endl;
    
    m.sendMessage();
    return(TRUE);
}
