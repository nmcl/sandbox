/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ReplicaR.cc,v 1.12 1995/02/22 09:09:33 ngdp Exp $
 */

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef SERVERAA_H_
#  include <Arjuna/ServerAA.h>
#endif

#ifndef NAMESERV_H_
#  include <ArjServers/NameServ_stub.h>
#endif

#ifndef RPOBJSTORE_H_
#  include <ArjServers/RPObjStore_stub.h>
#endif

#ifndef LPOBJSTORE_H_
#  include <ObjectStore/FragStore.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif 

#ifndef REPLICAR_H_
#  include <ReplicaR.h>
#endif

#include <AAStatus_stub.h>

/*
 * Enum: Replica Status (RepStatus)
 * Status of a particular replica
 */

ostream& operator<< (ostream& strm, RepStatus s)
{
    switch (s)
    {
        case NEEDSUPDATE:
            strm << "needs update";
            break;
        case MAYNEEDUPDATE:
            strm << "may need update";
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

RepInterface* ReplicaRecord::GV = 0;
int ReplicaRecord::NSIRefCount = 0;
int ReplicaRecord::repThreshold = 2;

/*
 * Class: ReplicaRecord
 * Responsible for the recovery of object replicas
 */

//-Public-Functions------------------------------------------------------------

ReplicaRecord::ReplicaRecord ( ostream& strm,
                               Boolean safe,
                               ReplicaDescriptor *rep, 
                               Boolean needsUpdate,
                               RepInterface* GVD)
			     : CrashRecRecord(strm, safe)

{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ReplicaRecord::ReplicaRecord()" << endl;
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
    debug_stream << "ReplicaRecord::~RelicaRecord()" << endl;
#endif

    if (--NSIRefCount == 0)
        delete GV;
}

void ReplicaRecord::print(ostream& strm)
{
    CrashRecRecord::print(strm);
    strm << "ReplicaRecord Status = " << status << endl;
    strm << "Hidden?    = " << hidden << endl;
    strm << "Host       = " << repDes.getHost() << endl;
    strm << "Uid        = " << repDes.getUid() << endl;
    strm << "GroupUid   = " << repDes.getGroupUid() << endl;
    strm << "Active?    = " << repDes.getActive() << endl;
}

void ReplicaRecord::id(ostream& strm)
{
    strm << "ReplicaRecord for replica uid = " << repDes.getUid() << endl;
}

Boolean ReplicaRecord::resolve()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ReplicaRecord::resolve()" << endl;
#endif

    if (getRecoveryStatus() == COMPLETE)
        return(TRUE);

    ReplicaDescriptor validReplicaList;

    outStrm << "RepRecord: Beginning recovery (" << repDes.getUid() << ")"
            << endl;

    switch (status)
    {
        case NEEDSUPDATE:
        {
             AtomicAction A;
             Boolean testAndSetResult;

             outStrm << "RepRecord: Replica is known to require update" << endl;

             A.Begin();

             testAndSetResult = GV->testAndSet( validReplicaList, 
                                                repDes.getGroupUid(),
                                                repDes.getHost(), ::getpid() );

             if (testAndSetResult != DONE)
             {
                 outStrm << "RepRecord: *WARNING* "
                         << "Could not access database entry for replica "
                         << repDes.getUid() << " of group " 
                         << repDes.getGroupUid() << endl;
                 A.Abort();
             }
             else
             {
                 if (  !( GV->releaseGroup( repDes.getGroupUid(),
                                            repDes.getHost(),
                                            ::getpid() ) ) )
                 {
		     outStrm << "RepRecord: *WARNING* "
			     << "Could not release database entry for replica "
			     << repDes.getUid() << " of group " 
			     << repDes.getGroupUid() << endl;
                     A.Abort();
                 }
                 else
                 {
                     if (A.End() != COMMITTED)
                     {
			 outStrm << "RepRecord: *WARNING* "
				 << "Action commit failed for replica "
				 << repDes.getUid() << " of group " 
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

            outStrm << "RepRecord: Replica may require update" << endl;

            B.Begin();

            needsUpdatingResult = GV->needsUpdating(repDes.getGroupUid(),
                                                    isModified,
                                                    validReplicaList,
                                                    repDes.getHost(),
                                                    ::getpid());

            if (needsUpdatingResult != DONE)
            {
                 outStrm << "RepRecord: *WARNING* "
                         << "Could not access database entry for replica "
                         << repDes.getUid() << " of group " 
                         << repDes.getGroupUid() << endl;
                B.Abort();
            }
            else
            {
                if (  !( GV->releaseGroup( repDes.getGroupUid(),
                                           repDes.getHost(),
                                           ::getpid() ) ) )
                {
		    outStrm << "RepRecord: *WARNING* "
			    << "Could not release database entry for replica "
			    << repDes.getUid() << " of group " 
			    << repDes.getGroupUid() << endl;
                    B.Abort();
                }
                else
                {
                    if (B.End() != COMMITTED)
                    {
			outStrm << "RepRecord: *WARNING* "
				<< "Action commit failed for replica "
				<< repDes.getUid() << " of group " 
				<< repDes.getGroupUid() << endl;
                    }
                    else
                    {
                        if (isModified == FALSE)
                        {
                            outStrm << "RepRecord: "
                                    << "Replica has not been modified" << endl;

                            if ( revealReplica() && GVInclude() )
                                setRecoveryStatus(COMPLETE);
                        }
                        else
                        {
                            if (updateReplica(validReplicaList))
                                setRecoveryStatus(COMPLETE);
                        }               
                    }
                }
            }
        }
	break;
    }

    Boolean success = ( (getRecoveryStatus() == COMPLETE) ? TRUE : FALSE );

    if(success)
        outStrm << "AARecord: Recovery successful (" << repDes.getUid() 
                << ")" << endl;
    else
        outStrm << "AARecord: Recovery unsuccessful (" << repDes.getUid()
                << ")" << endl;

    return (success);
}

const CRRecordType ReplicaRecord::type() const
{
    return REPRECORD;
}


const Uid& ReplicaRecord::value() const
{
    return(repDes.getUid());
}


//-Private-Functions-----------------------------------------------------------

Boolean ReplicaRecord::hideReplica()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PRIVATE;
    debug_stream << "ReplicaRecord::hideReplica()" << endl;
#endif

    ObjectStore *store = ObjectStore::create(DEFAULT_OBJECTSTORE_Q,
					     repDes.getStoreRoot() );
    Boolean success = FALSE;

    if (hidden)
    {
        outStrm << "ReplicaR: *WARNING* Attempt to hide hidden replica " 
                << "ignored (" << repDes.getUid() << ")" << endl;
        success = TRUE;
    }
    else
    {
        if (store->hide_state( repDes.getUid(), 
			      (const TypeName) repDes.getObjectName() ) )
	{
	    outStrm << "ReplicaR: Replica " << repDes.getUid()
                    << " is now hidden" << endl;
            success = TRUE;
        }
        else
        {
            outStrm << "ReplicaR: *WARNING* Attempt to hide replica " 
                    << repDes.getUid() << " failed" << endl;
        }
    }

    if (success)
        hidden = TRUE;

    if (store)
	delete store;
	    
    return (success);
}

Boolean ReplicaRecord::revealReplica()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PRIVATE;
    debug_stream << "ReplicaRecord::revealReplica()" << endl;
#endif

    ObjectStore *store = ObjectStore::create( "FragmentedStore",
					      repDes.getStoreRoot() );
    Boolean success = FALSE;
    
    if (!hidden)
    {
        outStrm << "ReplicaR: *WARNING* Attempt to reveal not-hidden replica "
                << "ignored (" << repDes.getUid() << ")" << endl;
        success = TRUE;
    }
    else
    {
        if (store->reveal_state(repDes.getUid(), 
			       (const TypeName) repDes.getObjectName()) )
	{
	    outStrm << "ReplicaR: Replica " << repDes.getUid()
                    << " has been revealed" << endl;
            success = TRUE;
        }
        else
        {
            outStrm << "ReplicaR: *WARNING* Attempt to reveal replica " 
                    << repDes.getUid() << " failed" << endl;
        }
    }

    if (success)
        hidden = FALSE;

    if (store)
	delete store;
    
    return (success);
}

Boolean ReplicaRecord::GVInclude()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PRIVATE;
    debug_stream << "ReplicaRecord::GVInclude()" << endl;
#endif

    OutCome includeResult;
    ReplicaDescriptor tempRD;
    AtomicAction A;
    Boolean success = FALSE;

    tempRD = repDes;

    outStrm << "RepRecord: About to include " << tempRD.getUid() 
            << " into the database..." << endl;

    A.Begin();

    tempRD.setActive(TRUE);
    includeResult = GV->include(tempRD, repDes.getGroupUid());

    if (includeResult == DONE)
    {
        if (A.End() == COMMITTED)
        {
            outStrm << "RepRecord: Included replica ("
                    << tempRD.getUid() << ") succcessfully" << endl;
            success = TRUE;
        }
        else
        {
            A.Abort();
            outStrm << "RepRecord: *WARNING* Failed to include replica " 
                    << tempRD.getUid() << endl;
        }
    }
    else
    {
        outStrm << "RepRecord: *WARNING* Failed to include replica " 
                << tempRD.getUid() << endl;
    }

    return (success);
}


Boolean ReplicaRecord::updateReplica(const ReplicaDescriptor &validList)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PRIVATE;
    debug_stream << "ReplicaRecord::updateReplica()" << endl;
#endif

    ReplicaDescriptor *copyFrom = (ReplicaDescriptor*) &validList;
    Buffer* goodState = 0;
    Boolean updated = FALSE;
    Boolean noMore = FALSE;
    Boolean readState = FALSE;
    Boolean writtenState = FALSE;

    while ( !readState && !noMore)
    {
        outStrm << "RepRecord: About to activate remote object store on host "
                << copyFrom->getHost() << endl;

        Boolean (*oldHandler) (RPC_Status, Int32);
        int startOK = -1;
        ClientRpc* RpcClient = new ClientRpc("RPObjStore");
        RpcClient->setHost( (char *) copyFrom->getHost() );
        oldHandler = setHandler(CrashRecRecord::rpc_handler);
        ClientRpcManager* RpcCon = new ClientRpcManager(RpcClient);
        RPObjStore *RPOS = new RPObjStore(startOK, RpcCon);

        if (startOK == 0)
        {
            // RPObjStore started OK

            int res = -1;

            outStrm << "RepRecord: About to readstate of "
                    << copyFrom->getUid() << endl;

            goodState = RPOS->read_committed(copyFrom->getUid(),
					     (const TypeName)
					     copyFrom->getObjectName(),
					     copyFrom->getStoreRoot(), res);

            if ( res == 0 )
            {
                // OK, we've got a state
                readState = TRUE;
            }
            else
            {
                outStrm << "RepRecord: *WARNING* Could not read "
                        << "state of " << copyFrom->getUid() << endl;
            }
        }
        else
        {
            outStrm << "RepRecord: *WARNING* Failed to start statedaemon "
                    << "on host " << copyFrom->getHost() << endl;
        }

        // Tidy up loose ends

        if (RPOS)
            delete RPOS;

        if (RpcCon)
            delete RpcCon;

        if (RpcClient)
            delete RpcClient;

        setHandler(oldHandler);

        if (!readState)
        {
            if (copyFrom->next != 0)
            {
                outStrm << "RepRecord: Trying next copy..." << endl;
                copyFrom = copyFrom->next;
            }
            else
            {
                outStrm << "RepRecord: There are no more copies to use, "
                        << "try later" << endl;
                noMore = TRUE;
            }

	    if (goodState)
	    {
		delete goodState;
		goodState = 0;
	    }
        }
    }

    if (readState)
    {
	ObjectStore *locStore = ObjectStore::create( "FragmentedStore",
						     repDes.getStoreRoot() );
        ObjectState oState;

        outStrm << "RepRecord: Attempting to write state to " 
                << repDes.getUid() << endl;

        writtenState = ( oState.unpackFrom(*goodState) &&
                         locStore->write_committed(repDes.getUid(),
                         (const TypeName) repDes.getObjectName(),
                         oState) );

        if (writtenState && GVInclude())
        {
            if ( GVInclude() )
                updated = TRUE;
        }
        else
        {
            outStrm << "RepRecord: *WARNING* Could not write state to " 
                    << repDes.getUid() << endl;
        }

	if (locStore)
	    delete locStore;
    }

    if (goodState)
	delete goodState;
    
    return(updated);
}

void ReplicaRecord::setThreshold(int t)
{
    repThreshold = t;
}

int ReplicaRecord::getThreshold()
{
    return(repThreshold);
}

void ReplicaRecord::alert()
{
    outStrm << "ReplicaRecord: ALERT!!!!" << endl;
}

void ReplicaRecord::calm()
{
    outStrm << "ReplicaRecord: CALM!!!!" << endl;
    resetAlarms();
}

