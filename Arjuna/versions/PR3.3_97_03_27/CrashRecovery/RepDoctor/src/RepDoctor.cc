/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RepDoctor.cc,v 1.4 1994/08/30 08:17:33 ngdp Exp $
 */

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

#ifndef LOCALOSTYPES_H_
#  include <ObjectStore/LocalOSTypes.h>
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

#ifndef REPDOCTOR_H_
#  include <RepDoctor.h>
#endif

#ifndef CRRECTYPE_H_
#  include <CRRecType.h>
#endif

#ifndef REPLICAR_H_
#  include <ReplicaR.h>
#endif

#ifndef DOZER_H_
#  include <Dozer.h>
#endif

/*
 * Enum: Progress
 * Records the progress of RepDoctor processing
 */

ostream& operator<< (ostream& strm, Progress p)
{
    switch (p)
    {
        case RD_WORKING:
            strm << "working";
            break;
        case RD_FAILED:
            strm << "failed";
            break;
        case RD_COMPLETE:
            strm << "complete";
            break;
        default:
            strm << "unknown";
            break;
    }
    return strm;
}

/*
 * Declare statics
 */

RepInterface* RepDoctor::RI = 0;
int RepDoctor::refCount = 0;
Boolean RepDoctor::setupRepInterface = FALSE;

/*
 * Class: Replication Doctor (RepDoctor)
 * Responsible for the co-ordination of replication recovery
 */

//-Public-Functions------------------------------------------------------------

RepDoctor::RepDoctor (ostream& strm, char* recoveringHost, CRRecordList* work)
                     : outStrm(strm),
                       host(recoveringHost),
                       workList(work)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "RepDoctor::RepDoctor()" << endl;
#endif

    // Increment reference count
    refCount++;
}

RepDoctor::~RepDoctor()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "RepDoctor::~RepDoctor()" << endl;
#endif

    if (--refCount == 0)
    {
        if (RI)
            delete RI;
    }
}

Boolean RepDoctor::phase1()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "RepDoctor::phase1()" << endl;
#endif

    Progress status = RD_WORKING;
    Dozer dozer(outStrm, 1);
    outStrm << "RepDoctor: Commencing phase 1 of recovery process" << endl;

    while ( !(RI = createRepInterface()) )
        dozer.doze();
    dozer.reset();

    if ( !(RI->repRecoveryRequired()) )
    {
        outStrm << "RepDoctor: No replication recovery is required." << endl;
        status = RD_COMPLETE;
    }
    
    if ( (status == RD_WORKING) && (!recoverGroupView()) )
        dozer.doze();
    dozer.reset();

    if ( (status == RD_WORKING) && (!getRepWork()) )
        dozer.doze();

    dozer.reset();

    if (status == RD_WORKING) 
        workList->resolve();

    if ( (status == RD_WORKING) && (!(RI->updateState())) )
        dozer.doze();
    dozer.reset();

    if ( (status == RD_WORKING) || (status == RD_COMPLETE) )
    {
        outStrm << "RepDoctor: Completed phase 1 recovery" << endl;
        return(TRUE);
    }
    else
    {
        outStrm << "RepDoctor: *WARNING* Phase 1 recovery incomplete" << endl;
        return(FALSE);
    }
}

//-Private-Functions-----------------------------------------------------------

RepInterface *RepDoctor::createRepInterface()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PRIVATE;
    debug_stream << "RepDoctor::createRepInterface()" << endl;
#endif
    int result;
    Boolean success = FALSE;

    outStrm << "RepDoctor: Creating replication database interface ..." << endl;

    RepInterface *repInterface = new RepInterface(result, outStrm);
    if (result != 0)
    {
        outStrm << "RepDoctor: ** WARNING ** "
               << "Failed to replication create database interface" << endl;

        delete repInterface;
        repInterface = 0;
    }
    else
    {
        outStrm << "RepDoctor: Created interface successfully" << endl;
        success = TRUE;
    }
    return(repInterface);
}


Boolean RepDoctor::recoverGroupView()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PRIVATE;
    debug_stream << "RepDoctor::recoverGroupView()" << endl;
#endif

    OutCome result;
    Boolean success = FALSE;
    outStrm << "RepDoctor: Attempting GroupView recover operation..." << endl;
    result = RI->recover(host);

    if (result != DONE)
    {
        outStrm << "RepDoctor: ** WARNING ** GroupView recover failed" << endl;
    }
    else
    {
        outStrm << "RepDoctor: GroupView recover successful" << endl;
        success = TRUE;
    }
    return(success);
}


Boolean RepDoctor::getRepWork()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PRIVATE;
    debug_stream << "RepDoctor::getRepWork()" << endl;
#endif

    Boolean success = FALSE;
    Boolean foundAtReboot = TRUE;
    OutCome result;
    ReplicaDescriptor needsUpdate;
    ReplicaDescriptor mayNeedUpdate;
    AtomicAction A;

    outStrm << "RepDoctor: Querying database for inconsistent replicas..."
            << endl;

    A.Begin();
    result = RI->getNodeUpdateList(needsUpdate, mayNeedUpdate, host);

    if (result != DONE)
    {
        outStrm << "RepDoctor: ** WARNING ** database query failed" << endl;
        A.Abort();
    }
    else
    {
        if (A.End() != COMMITTED) 
        {
            outStrm << "RepDoctor: ** WARNING ** database query failed" << endl;
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
                outStrm << "RepDoctor: Replicas known to require update:\n"
                        << needsUpdate << endl;

                while (temp)
                {
                     ReplicaRecord *reprec = new ReplicaRecord(outStrm,
                                                               foundAtReboot,
                                                               temp, sure, RI);
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
                outStrm << "RepDoctor: Replicas that may require update:\n"
                        << mayNeedUpdate << endl;

                while (temp)
                {
                     ReplicaRecord *reprec = new ReplicaRecord(outStrm,
                                                               foundAtReboot,
                                                               temp, sure, RI);
                     workList->insert(reprec);
                     temp = temp->next;
                }
            }
            outStrm << "RepDoctor: Got all replication work" << endl;
            success = TRUE;
        }
    }
    return (success);
}

