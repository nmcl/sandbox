/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ReplicaR.cc,v 1.1 1993/11/03 14:17:07 nmcl Exp $
 */

#ifndef UNISTD_H_
#  include <System/unistd.h>
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

#ifndef NAMESERV_H_
#  include <ArjServers/NameServ_stub.h>
#endif

#ifndef STATEDAEMON_H_
#  include <ArjServers/StateDaemon_stub.h>
#endif

#ifndef SERVERAA_H_
#  include <Arjuna/ServerAA.h>
#endif

#ifndef REPLICAR_H_
#include "ReplicaR.h"
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif 

#include "AAStatus_stub.h"

/*
 * Declare statics
 */

NSInterface* ReplicaRecord::GV = 0;
int ReplicaRecord::NSIRefCount = 0;

ReplicaRecord::ReplicaRecord ( ReplicaDescriptor *rep, 
                               Boolean needsUpdate,
                               NSInterface* GVD)
                             : CrashRecRecord()

{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ReplicaRecord::ReplicaRecord()\n" << flush;
#endif

    if (needsUpdate == TRUE)
        status = NEEDSUPDATE;
    else
        status = MAYNEEDUPDATE;

    /* Check our static groupview pointer is setup */

    if (!GV && GVD)
    {
        GV = GVD;
    }

    repDes = *rep;
    hidden = FALSE;
    NSIRefCount++;
    hideReplica();
}

ReplicaRecord::~ReplicaRecord()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ReplicaRecord::~RelicaRecord()\n" << flush;
#endif

    removeHiddenCopy();

    if (--NSIRefCount == 0)
        delete GV;
}

ostream& ReplicaRecord::print ( ostream& strm ) const
{
    CrashRecRecord::print(strm);
    strm << "ReplicaRecord Status = " << status << endl;
    strm << "Hidden?    = " << hidden << endl;
    strm << "Hidden Uid = " << hiddenUid << endl;
    strm << "Host       = " << repDes.getHost() << endl;
    strm << "Uid        = " << repDes.getUid() << endl;
    strm << "GroupUid   = " << repDes.getGroupUid() << endl;
    strm << "Active?    = " << repDes.getActive() << endl;
    return strm;
}

ostream& ReplicaRecord::id ( ostream& strm ) const
{
    strm << "ReplicaRecord for replica uid = " << repDes.getUid() << endl;
    return strm;
}

Boolean ReplicaRecord::hideReplica()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ReplicaRecord::hideReplica()\n" << flush;
#endif

    LocalPersistentObjectStore store;
    ObjectState *oState = 0;
    Boolean success = FALSE;

    if (hidden)
    {
        cerr << "** WARNING ** Attempt to hide hidden replica ignored!" << endl;
        return (TRUE);
    }
    else
    {
        cout << "About to hide replica. Moving from "
             << repDes.getUid() << "\nto " << hiddenUid << endl;

        if ( (oState = store.read_state(repDes.getUid(),
                              (const TypeName)repDes.getObjectName())) )
        {
            if ( store.write_committed( hiddenUid,
                                        (const TypeName) repDes.getObjectName(),
                                        *oState ) )
            {
                if ( store.remove_state(repDes.getUid(),
                                       (const TypeName)repDes.getObjectName()) )
                {
                    success = TRUE;
                }
                else
                {
                    cerr << "** WARNING ** Could not remove state ("
                         << repDes.getUid() << ")" << endl;
                }
            }
            else
            {
                cerr << "** WARNING ** Could not write state ("
                     << hiddenUid << ")" << endl;
            }
        }
        else
        {
            cerr << "** WARNING ** Could not read state ("
                 << repDes.getUid() << ")" << endl;
        }
    }

    if (success)
        hidden = TRUE;

    return (success);
}

Boolean ReplicaRecord::revealReplica()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ReplicaRecord::revealReplica()\n" << flush;
#endif

    LocalPersistentObjectStore store;
    ObjectState *oState = 0;
    Boolean success = FALSE;

    if (!hidden)
    {
        cerr << "** WARNING ** Attempt to reveal un-hidden replica ignored!" 
             << endl;
        return (TRUE);
    }
    else
    {
        cout << "About to reveal replica. Moving from "
             << hiddenUid << "\nto " << repDes.getUid() << endl;

        if ( (oState = store.read_state(hiddenUid,
                              (const TypeName)repDes.getObjectName())) )
        {
            if ( store.write_committed( repDes.getUid(),
                                        (const TypeName) repDes.getObjectName(),
                                        *oState ) )
            {
                if (store.remove_state(hiddenUid,
                                       (const TypeName)repDes.getObjectName()) )
                {
                    success = TRUE;
                }
                else
                {
                    cerr << "** WARNING ** Could not remove state ("
                         << hiddenUid << ")" << endl;
                }
            }
            else
            {
                cerr << "** WARNING ** Could not write state ("
                     << repDes.getUid() << ")" << endl;
            }
        }
        else
        {
            cerr << "** WARNING ** Could not read state ("
                 << hiddenUid << ")" << endl;
        }
    }

    if (success)
        hidden = TRUE;

    return (success);
}

Boolean ReplicaRecord::removeHiddenCopy()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "removehiddenCopy()\n" << flush;
#endif

    LocalPersistentObjectStore store;
    Boolean success = FALSE;

    cout << "About to remove hidden state ("
         << hiddenUid << ")" << endl;

    if (store.remove_state(hiddenUid,
                           (const TypeName)repDes.getObjectName()) )
    {
        
        success = TRUE;
    }
    else
    {
        cerr << "** WARNING ** Could not remove state ("
             << hiddenUid << ")" << endl;
    }
    return(success);
}

Boolean ReplicaRecord::resolve()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ReplicaRecord::resolve()\n" << flush;
#endif

    ReplicaDescriptor validReplicaList;

    cout << "\nBEGINNING REPLICA UPDATE (" << repDes.getUid() << ")" << endl;

    switch (status)
    {
        case NEEDSUPDATE:
        {
             AtomicAction A;
             Boolean testAndSetResult;

             cout << "Replica is known to require update" << endl;

             A.Begin();

             testAndSetResult = GV->testAndSet( validReplicaList, 
                                                repDes.getGroupUid(),
                                                repDes.getHost(), ::getpid() );

             if (testAndSetResult != Done)
             {
                 cerr << "** WARNING ** testAndSet failed for replica "
                      << repDes.getUid() << " of group = " 
                      << repDes.getGroupUid() 
                      << "\nAborting action..." << endl;
                 A.Abort();
             }
             else
             {
                 if (  !( GV->releaseGroup( repDes.getGroupUid(),
                                            repDes.getHost(),
                                            ::getpid() ) ) )
                 {
                     cerr << "** WARNING ** releaseGroup failed for replica "
                          << repDes.getUid() << " of group = " 
                          << repDes.getGroupUid()
                          << "\nAborting action..." << endl;
                     A.Abort();
                 }
                 else
                 {
                     if (A.End() != COMMITTED)
                     {
                     cerr << "** WARNING ** Action commit failed for replica "
                          << repDes.getUid() << " of group = " 
                          << repDes.getGroupUid() << endl;
                     }
                     else
                     {
                         updateReplica(validReplicaList);
                     }
                }
            }
        }
	break;
        case MAYNEEDUPDATE:
        {
            AtomicAction B;
            OutCome needsUpdatingResult;
            Boolean isModified;

            cout << "Replica may require update" << endl;

            B.Begin();

            needsUpdatingResult = GV->needsUpdating(repDes.getGroupUid(),
                                                    isModified,
                                                    validReplicaList,
                                                    repDes.getHost(),
                                                    ::getpid());

            if (needsUpdatingResult != Done)
            {
                 cerr << "** WARNING ** needsUpdating failed for replica "
                      << repDes.getUid() << " of group = " 
                      << repDes.getGroupUid() 
                      << "\nAborting action..." << endl;
                B.Abort();
            }
            else
            {
                if (  !( GV->releaseGroup( repDes.getGroupUid(),
                                           repDes.getHost(),
                                           ::getpid() ) ) )
                {
                     cerr << "** WARNING ** releaseGroup failed for replica "
                          << repDes.getUid() << " of group = " 
                          << repDes.getGroupUid()
                          << "\nAborting action..." << endl;
                    B.Abort();
                }
                else
                {
                    if (B.End() != COMMITTED)
                    {
                     cerr << "** WARNING ** Action commit failed "
                          << "for replica "
                          << repDes.getUid() << "\nof group = " 
                          << repDes.getGroupUid() << endl;
                    }
                    else
                    {
                        if (isModified == FALSE)
                        {
                            cout << "Replica has not been modified" << endl;

                            if ( revealReplica() && GVInclude() )
                                setRecoveryStatus(COMPLETE);
                        }
                        else
                        {
                            updateReplica(validReplicaList);
                        }               
                    }
                }
            }
        }
	break;
    }
    return TRUE;
}

Boolean ReplicaRecord::GVInclude()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ReplicaRecord::GVInclude()\n" << flush;
#endif

    OutCome includeResult;
    ReplicaDescriptor tempRD;
    AtomicAction A;
    Boolean success = FALSE;

    tempRD = repDes;

    cout << "About to include " << tempRD.getUid() 
         << " in to the database..." << endl;

    A.Begin();

    tempRD.setActive(TRUE);
    includeResult = GV->include(tempRD, repDes.getGroupUid());

    if (includeResult == Done)
    {
        if (A.End() == COMMITTED)
        {
            cout << "Included replica ("
                 << tempRD.getUid() << ") in database OK" << endl;
            success = TRUE;
        }
        else
        {
            A.Abort();
            cerr << "** WARNING ** include action commit failed for " 
                 << tempRD.getUid() << endl;
        }
    }
    else
    {
        cerr << "** WARNING ** Could not include replica " 
             << tempRD.getUid() << endl;
    }

    return (success);
}


Boolean ReplicaRecord::updateReplica(const ReplicaDescriptor &validList)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ReplicaRecord::updateReplica()\n" << flush;
#endif

    ReplicaDescriptor *copyFrom = (ReplicaDescriptor*) &validList;
    Buffer goodState;
    Boolean updated = FALSE;
    Boolean noMore = FALSE;
    Boolean readState = FALSE;
    Boolean writtenState = FALSE;

    while ( !readState && !noMore)
    {
        cout << "About to activate remote statedaemon on host "
             << copyFrom->getHost() << endl;

        Boolean (*old_handler) ();
        int startOK = -1;
        Boolean isVolatile = FALSE;
        ClientRpc* RpcClient = new ClientRpc("StateDaemon");
        RpcClient->SetHost( (char *) copyFrom->getHost() );
        old_handler = setHandler(CrashRecRecord::rpc_handler);
        RpcControl* RpcCon = new RpcControl(RpcClient);
        setHandler(old_handler);
        StateDaemon *SD = new StateDaemon(startOK, isVolatile, RpcCon);

        if (startOK == 0)
        {
            // StateDaemon started OK

            int res = -1;

            cout << "Started remote statedaemon OK\n"
                 << "About to readstate of " << copyFrom->getUid() << endl;

            goodState = SD->readState(copyFrom->getUid(),
                                     (const TypeName) copyFrom->getObjectName(),
                                      res);

            if ( res == 0 )
            {
                // OK, we've got a state
                cout << "Got state OK from " << copyFrom->getUid() << endl;
                readState = TRUE;
            }
            else
            {
                cerr << "** WARNING ** Could not read state of " 
                     << copyFrom->getUid() << endl;
            }
        }
        else
        {
            cerr << "** WARNING ** Failed to start statedaemon on host "
                 << copyFrom->getHost() << endl;
        }

        // Tidy up loose ends

        if (SD)
            delete SD;

        if (RpcCon)
            delete RpcCon;

        if (RpcClient)
            delete RpcClient;


        if (!readState)
        {
            if (copyFrom->next != 0)
            {
                cout << "Trying next copy..." << endl;
                copyFrom = copyFrom->next;
            }
            else
            {
                cout << "There are no more copies to use, try later..." << endl;
                noMore = TRUE;
            }
        }
    }

    if (readState)
    {
        LocalPersistentObjectStore locStore;
        ObjectState oState;

        cout << "Attempting to write state to " << repDes.getUid() << endl;

        writtenState = ( oState.unpackFrom(goodState) &&
                         locStore.write_committed(repDes.getUid(),
                         (const TypeName) repDes.getObjectName(),
                         oState) );

        if (writtenState)
        {
            updated = GVInclude();
        }
        else
        {
            cerr << "** WARNING ** Could not write state to " 
                 << repDes.getUid() << endl;
        }
    }

    if (updated)
    {
        cout << "REPLICA UPDATE SUCCESSFUL (" << repDes.getUid() 
             << ")\n" << endl;
        setRecoveryStatus(COMPLETE);
    }
    else
    {
        cout << "REPLICA UPDATE UNSUCCESSFUL (" << repDes.getUid() 
             << ")\n" << endl;
    }
    return(updated);
}

