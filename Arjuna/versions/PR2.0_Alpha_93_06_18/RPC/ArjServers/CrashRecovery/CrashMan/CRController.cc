/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CRController.cc,v 1.13 1993/10/06 14:30:52 ndbi Exp $
 */


/*******************************************************************************
TO DO:
	1. CHECK FOR OUT OF DATE DATABASE
	2. CONTROL THE MANAGER STARTUP
	3. INCORPORATE MECHANISM FOR PURELY LOCAL ACTIONS
	4. PUT TIMING IN
	5. EXIT WHEN ALL WORK IS DONE
*******************************************************************************/


#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef FCNTL_H_
#  include <System/fcntl.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef SYS_TIME_H_
#include <System/sys/time.h>
#endif

#ifndef SYS_TYPES_H_
#include <System/sys/types.h>
#endif

#ifndef SYS_STAT_H_
#include <System/sys/stat.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef FSTREAM_H_
#  include <System/fstream.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
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

#ifndef LPOBJSTORE_H_
#  include <Arjuna/LPObjStore.h>
#endif

#ifndef SERVERAA_H_
#  include <Arjuna/ServerAA.h>
#endif

#ifndef OBJSTOREI_H_
#  include <Arjuna/ObjStoreI.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef CRCONTROLLER_H_
#  include "CRController.h"
#endif

/*
 * Crash recovery Controller Class
 */

/*
 * Declare statics
 */

Boolean CRController::setupGroupView = FALSE;
NSInterface* CRController::GV = 0;
int CRController::NSIRefCount = 0;
Boolean CRController::managerRunning = FALSE;

CRController::CRController ()
                           : workList(new CRRecordList),
                             recoveringHost(0),
                             firstPass(TRUE),
                             gotSAAWork(FALSE),
                             gotRepWork(FALSE),
                             gotAAWork(FALSE),
                             groupviewRecover(FALSE),
                             gotExclGVAccess(FALSE)

{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRController::CRController()\n" << flush;
#endif

    recoveringHost = new char[MAXHOSTNAMELEN];
    ::memset(recoveringHost, '\0', MAXHOSTNAMELEN);
    ::gethostname(recoveringHost, MAXHOSTNAMELEN);

    // Increment reference count for NSInterface pointer
    NSIRefCount++;
}

CRController::~CRController()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRController::~CRController()\n" << flush;
#endif

    if (recoveringHost)
        delete recoveringHost;

    if (workList)
        delete workList;

    if ((--NSIRefCount == 0) && GV)
        delete GV;
}

Boolean CRController::checkAndSetupGV()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRController::checkAndSetupGV()\n" << flush;
#endif

    if (!setupGroupView)
    {
        int result;

        cout << "Attempting to set up groupview database..." << endl;

        GV = new NSInterface(result);
        if (result != 0)
        {
            cerr << "** WARNING ** Failed to create groupview database" << endl;
            GV = 0;
            setupGroupView = FALSE;
        }
        else
        {
            cout << "Set up groupview database OK" << endl;
            setupGroupView = TRUE;
        }
    }
    return (setupGroupView);
}

void CRController::getSAAWork()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRController::getSAAWork()\n" << flush;
#endif

    if (!gotSAAWork)
    {
        LocalPersistentObjectStore store; 
        Uid *saa_uid;
        ServerAtomicAction temp_saa(NIL_UID);
        ObjectStoreIterator get_next_saa(store, temp_saa.type());

        cout << "Searching for server atomic actions..." << endl;

        while ( (saa_uid = get_next_saa()) != 0 )
        {
            cout << "Found SAA with uid = " << *saa_uid << endl;

            ServerAARecord *saar = new ServerAARecord(*saa_uid);
            workList->insert(saar);
         }
         cout << "Got all server atomic actions" << endl;
         gotSAAWork = TRUE;
    }
}

void CRController::getRepWork()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRController::getRepWork()\n" << flush;
#endif

    if ( checkAndSetupGV() && !gotRepWork)
    {
        OutCome GVResult;
        ReplicaDescriptor needsUpdate;
        ReplicaDescriptor mayNeedUpdate;
        AtomicAction A;

        cout << "Querying database for inconsistent replicas..." << endl;

        A.Begin();

        GVResult = GV->getNodeUpdateList(needsUpdate,
                                         mayNeedUpdate,
                                         recoveringHost);

        if (GVResult != Done)
        {
            cerr << "** WARNING ** GroupView getNodeUpdateList failed" << endl;
            A.Abort();
            gotRepWork = FALSE;
        }
        else
        {
            if (A.End() != COMMITTED)
            {
                cerr << "** WARNING ** AtomicAction commit failed" << endl;
                gotRepWork = FALSE;
            }
            else
            {
                ReplicaDescriptor *temp;
                Boolean sure = TRUE;

                /*
                 * Add the replicas that are known to require update
                 */

                if ( needsUpdate != NIL_RD )
                {
                    temp = &needsUpdate;

                    cout << "Replicas known to require update:\n"
                         << needsUpdate << endl;

                    while (temp)
                    {
                         ReplicaRecord *reprec = new ReplicaRecord(temp, sure, 
                                                                   GV);
                         workList->insert(reprec);
                         temp = temp->next;
                    }
                }

                /*
                 * Now add the replicas that may require update
                 */

                if ( mayNeedUpdate != NIL_RD )
                {
                    sure = FALSE;
                    temp = &mayNeedUpdate;

                    cout << "Replicas that may require update:\n"
                         << mayNeedUpdate << endl;

                    while (temp)
                    {
                         ReplicaRecord *reprec = new ReplicaRecord(temp, sure,
                                                                   GV);
                         workList->insert(reprec);
                         temp = temp->next;
                    } 
                }
                cout << "Got all replication work" << endl;
                gotRepWork = TRUE;
            }
        }
    }
}

ostream& CRController::printStatus ( ostream& strm ) const
{
    strm << "\n----------------------------------------"
         << "--------------------------------------" << endl;
    strm << "Current status of crash controller:\n" << endl;
    strm << "recovering host    = " << recoveringHost << endl;
    strm << "First pass         = " << firstPass << endl;
    strm << "got SAA Work       = " << gotSAAWork << endl;
    strm << "got Rep Work       = " << gotRepWork << endl;
    strm << "got Local AA Work  = " << gotAAWork << endl;
    strm << "setup GroupView    = " << setupGroupView << endl;
    strm << "GroupView recover  = " << groupviewRecover << endl;
    strm << "Got Excl GV Access = " << gotExclGVAccess << endl;
    strm << "manager running    = " << managerRunning << endl;
    strm << endl << "Work list follows:" << endl;
    workList->print(strm);
    strm << "----------------------------------------"
         << "--------------------------------------\n" << endl;
    return strm;
}

Boolean CRController::makeRecoveryPass()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRController::makeRecoveryPass()\n" << flush;
#endif

/*
	NEED TO SORT OUT HOW THE GETEXCLUSIVEGROUPVIEW SHOULD BE USED
	AND HOW THE MANAGER SHOULD BE STARTED
*/

    time_t theTime = ::time(0);

    cout << "\n****************************************"
         << "**************************************" << endl;
    cout << "ARJUNA CRASH RECOVERY MANAGER";
    if (firstPass)
        cout << " - FIRST PASS SINCE REBOOT";
    cout << endl;
    cout << "Beginning recovery pass at " << ctime(&theTime);
    cout << "****************************************"
         << "**************************************\n" << endl;

    /*
     * OK, If we haven't got the work already then get it now.
     * First, the unresolved server atomic actions
     * Second, the inconsistent replicas, third the local AAs
     */

    if (!gotSAAWork)
        getSAAWork();

    enableManager(); // for testing only

    if (!gotRepWork)
        getRepWork();

    if (!gotAAWork)
        getLocalAAWork();

    if (!groupviewRecover)
        recoverGroupView();

    workList->resolve();

    if (gotExclGVAccess)
        releaseExclusiveGV();

    workList->purge();
 
    if (firstPass)
        firstPass = FALSE;

    printStatus(cout);

    if ( gotSAAWork && gotRepWork && gotAAWork && groupviewRecover &&
         (workList->getNoEntries() == 0) )
    {
        enableManager();

        time_t theTime = ::time(0);
        cout << "\n****************************************"
             << "**************************************\n"
             << "ARJUNA CRASH RECOVERY MANAGER - Completed work at "
             << ctime(&theTime) 
             << "****************************************"
             << "**************************************\n" << endl;
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

Boolean CRController::getExclusiveGV()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRController::getExclusiveGV()\n" << flush;
#endif

    if ( !gotExclGVAccess && checkAndSetupGV() && GV->getExclusiveAccess() )
        gotExclGVAccess = TRUE;
    else
        gotExclGVAccess = FALSE;

    return (gotExclGVAccess);
}

Boolean CRController::releaseExclusiveGV()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRController::releaseExclusiveGV()\n" << flush;
#endif

    return (TRUE);
}

Boolean CRController::recoverGroupView()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRController::recoverGroupView()\n" << flush;
#endif

    /*
     * Only attempt recover if we have a GroupView database and we haven't
     * previously recovered.
     */

    if (checkAndSetupGV() && !groupviewRecover)
    {
        OutCome GVResult;
        GVResult = GV->recover(recoveringHost);

        if (GVResult != Done)
        {
            cerr << "** WARNING ** GroupView recover failed" << endl;
            groupviewRecover = FALSE;
        }
        else
        {
            cout << "GroupView recover OK" << endl;
            groupviewRecover = TRUE;
        }
    }
    return(setupGroupView && groupviewRecover);
}

long CRController::getccount ()
{
/* 
 * Read C_COUNT file and return the crash count of the local node,
 * if successful; otherwise -1.
 */

  static long   crashcount = -1L;
  int           ccountfd;

  if (crashcount < 0)
  {
    if ((ccountfd = open(C_COUNT_Q, O_RDWR, 0444)) >= 0)
    {
      struct flock arg;

      arg.l_whence = 0;
      arg.l_start = 0;
      arg.l_len = 0;
      arg.l_type = F_WRLCK;
      if (fcntl(ccountfd, F_SETLKW, (int)&arg) >= 0)
      {         /* Read C_COUNT */
        if(read(ccountfd, (char *) & crashcount, sizeof(crashcount))
           != sizeof(crashcount))
        {    /* Read failed - reset crashcount to error code */
          crashcount = -1L;
        }
        arg.l_type = F_UNLCK;
        fcntl(ccountfd, F_SETLKW, (int)&arg);
      }
      close(ccountfd);
    }
  }
  return(crashcount);
}


Boolean CRController::enableManager()
{
    Boolean success = FALSE;

    if (!managerRunning)
    {
        int managerPid = getManagerPid();

        if (managerPid != -1)
        {
            /*
             *  Send the manager process a signal to change its user mode
             */

            if (kill(managerPid, SIGUSR1) != 0)
            {
                cerr << "** WARNING ** Signalling manager process failed"
                     << endl;
            }
            else
            {
                success = TRUE;
                managerRunning = TRUE;
            }
        }
    }
    else
    {
        success = TRUE;
    }

    return (success);
}

int CRController::getManagerPid ()
{

/* 
 * Read MANPID_FILE file and return the id of the manager process
 * if successful; otherwise -1.
 */

    static int managerPid = -1;

    ifstream in(MANPID_FILE_Q);

    if ( !in )
    {
        cerr << "** WARNING ** Cannot open MANPID_FILE" << endl;
        managerPid = -1;
    }
    else
    {
        in >> managerPid;
    }

    return(managerPid);
}


void CRController::getLocalAAWork()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "CRController::getLocalAAWork()\n" << flush;
#endif

    if (!gotAAWork)
    {
        LocalPersistentObjectStore store; 
        Uid *aa_uid;
        AtomicAction temp_aa(NIL_UID);
        ObjectStoreIterator get_next_aa(store, temp_aa.type());

        cout << "Searching for local atomic actions..." << endl;

        while ( (aa_uid = get_next_aa()) != 0 )
        {
            cout << "Found AtomicAction with uid = " << *aa_uid << endl;

            AARecord *aar = new AARecord(*aa_uid);
            workList->insert(aar);
         }
         cout << "Got all local atomic actions" << endl;
         gotAAWork = TRUE;
    }
}










