/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 */

/*
 * $Id: RepObjStore.cc,v 1.34 1997/02/16 16:15:45 n048z Exp $
 */


#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef SYSENT_H_
#  include <System/unistd.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif 

#ifndef NETDB_H_
#   include <System/netdb.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef ARJNAME_H_
#  include <ArjServers/ArjName.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef ABORT_H_
#  include <StubGen/Abort.h>
#endif

#ifndef NSINTERFACE_H_
#  include <ArjServers/NSInterface.h>
#endif

#ifndef GROUPDATA_H_
#  include <ArjServers/GroupData.h>
#endif

#ifndef REPOBJSTORE_H_
#  include <Replication/RepObjStore.h>
#endif

#ifndef REPOBJSTOREIF_H_
#  include <Replication/RepObjStoreIF.h>
#endif

#ifndef RPOBJSTORE_H_
#  include <ArjServers/RPObjStore_stub.h>
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif

#ifndef MEMBERS_H_
#  include "Members.h"
#endif

#ifndef DAEMONLIST_H_
#  include "DaemonList.h"
#endif

#ifndef DAEMONDATA_H_
#  include "DaemonData.h"
#endif

#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
#  ifndef RAJDOOT_RPCUSER_H_
#    include <RPC/Rajdoot/rpcuser.h>
#  endif
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif 


static const char RCSid[] = "$Id: RepObjStore.cc,v 1.34 1997/02/16 16:15:45 n048z Exp $";

static osCreatorFunc createInstance;

#ifdef STRUCT_INIT_BUG
static SetUpInfo RepSetup = { 0, 1, 0, &createInstance, ReplicatedStoreType, 0 };
#else
static SetUpInfo RepSetup = { FALSE, TRUE, 0, &createInstance, ReplicatedStoreType, 0 };
#endif

int         ReplicatedPersistentObjectStore::use_count = 0;
DaemonList* ReplicatedPersistentObjectStore::daemons = (DaemonList*) 0;
char*       ReplicatedPersistentObjectStore::hostName = (char*) 0;


ObjectStore* createInstance ( const char* locationOfStore )
{
    return new RepObjStoreInterface(locationOfStore);
}

/*
 * The following class has an equivalent in every type of object store.
 * This is used to cut down the size of the binaries at link time by
 * dynamically assembling a linked list of only those types of object stores
 * which the application needs to use. [Initial interface/implementation work].
 */

RepObjStoreSetUp::RepObjStoreSetUp ()
				   : ObjectStoreSetUp(RepSetup)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RepObjStoreSetUp::RepObjStoreSetUp ()" << endl;
#endif

#ifdef GCC_STATIC_INIT_BUG
    if (!RepSetup._setUp)
    {
	RepSetup._new = &createInstance;
	RepSetup._alwaysCreate = TRUE;
	RepSetup._tn = ReplicatedStoreType;
	RepSetup._setUp = TRUE;
	ObjectStore::addToList(&RepSetup);
    }
#endif 
}

RepObjStoreSetUp::~RepObjStoreSetUp () {}

/*
 * Since we cannot run exclude operation within commit phase, and
 * all objects must be accessed using "atomic" RPC, we cannot
 * tolerate a failure within an action without aborting the action
 * and retrying.
 * However, we still work on the read-from-one/write-to-all policy,
 * in which case a group will still be considered successfully activated
 * if at least one member is available. The atomic action system will
 * potentially cause the action to abort later, depending upon where
 * the failure occurred.
 * In addition, since there is currently no failure detection mechanism
 * (apart from timeout and retry built in to the RPC), we try to make use
 * of failures of nodes detected by other replicated objects within this
 * process. This can mean that a replica is excluded spuriously (for
 * example if a node fails and recovers quickly), but the probability
 * of this is proportional to the period of the application. Currently
 * we optimise for short application, and possibly take an availability
 * "miss".
 */


client_data::client_data ()
			 : groupCreated(FALSE),
			   pid(getpid()),
			   group_view(0),
			   compositeView(0),
			   group_uid(0),
			   actionID(0)
{
}

client_data::~client_data ()
{
    if (group_view)
	delete group_view;
    if (compositeView)
	delete compositeView;
    if (group_uid)
	delete group_uid;
    if (actionID)
	delete actionID;
}


ViewData::ViewData ()
		   : gd(0),
		     daemonInfo(0),
		     next(0),
		     isAlive(TRUE)
{
}

ViewData::~ViewData ()
{
    if (gd)
	delete gd;
    if (next)
	delete next;
}


/*
 * If an object is replicated/remote then it must have an associated group
 * view. This can be registered at the database or provided (for new objects
 * only) via StateManager's operation setStoreInformation. If a group view
 * is not given then any state access will fail, rather than defaulting to
 * the current local host. This is because it is unlikely that such a default
 * is going to be correct and so we inform the user of this error immediately
 * and let him sort it out. Rather this than write the state to an object store
 * which is unwanted and/or unknown about by the user.
 */


/*
 * Public constructors and destructor
 */

	
ReplicatedPersistentObjectStore::ReplicatedPersistentObjectStore (const char* storeRoot)
								 : old_handler(0),
								   isRemoteOnly(FALSE),
								   storeValid(TRUE),
								   failureDetected(FALSE),
								   doRelease(FALSE),
								   replica_members(0),
								   _failedReplicas(0),
								   StoreLocation(0),
								   localStoreName(0),
								   numberOfStores(0),
								   NS(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "ReplicatedPersistentObjectStore::ReplicatedPersistentObjectStore ()\n" << flush;
#endif

    localStoreName = ::new char[::strlen(storeRoot)+1];
    ::strcpy(localStoreName, storeRoot);
							       
    if (ReplicatedPersistentObjectStore::daemons == (DaemonList*) 0)
	ReplicatedPersistentObjectStore::daemons = new DaemonList;
    
    ReplicatedPersistentObjectStore::use_count++;

    if (ReplicatedPersistentObjectStore::hostName == (char*) 0)
    {
	ReplicatedPersistentObjectStore::hostName = ::new char[MAXHOSTNAMELEN+1];
	::memset(ReplicatedPersistentObjectStore::hostName, '\0', MAXHOSTNAMELEN+1);
	(void) ::gethostname(ReplicatedPersistentObjectStore::hostName, MAXHOSTNAMELEN);
    }
}

ReplicatedPersistentObjectStore::~ReplicatedPersistentObjectStore ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "ReplicatedPersistentObjectStore::~ReplicatedPersistentObjectStore ()" << endl;
#endif

    /* Cannot release group here as other (complex) objects may have implicit lock. */

    if (NS)
	delete NS;

    ReplicatedPersistentObjectStore::use_count--;
    
    if (ReplicatedPersistentObjectStore::use_count == 0)
    {
	if (ReplicatedPersistentObjectStore::daemons)
	{
	    delete ReplicatedPersistentObjectStore::daemons;
	    ReplicatedPersistentObjectStore::daemons = (DaemonList*) 0;
	}
    }

    if (localStoreName)
#ifndef GCC_ARRAY_DELETE_BUG
        ::delete [] localStoreName;
#else
        ::delete localStoreName;
#endif

    if (StoreLocation)
    {
	for (int i = 0; i < numberOfStores; i++)
#ifndef GCC_ARRAY_DELETE_BUG
	    ::delete [] StoreLocation[i];
#else
	    ::delete StoreLocation[i];
#endif

#ifndef GCC_ARRAY_DELETE_BUG	
	::delete [] StoreLocation;
#else
	::delete StoreLocation;
#endif	
    }

    undo();
    setHandler(old_handler);
}

Boolean ReplicatedPersistentObjectStore::rpc_handler ( RPC_Status, Int32 )
{
    error_stream << WARNING << "ReplicatedPersistentObjectStore::rpc_handler - RPObjStore daemon timeout occurred." << endl;
    return FALSE;
}

void ReplicatedPersistentObjectStore::setRootOfStore (const char* storeRoot)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "void ReplicatedPersistentObjectStore::setRootOfStore ( " << storeRoot << " )" << endl;
#endif

    if (localStoreName)
#ifndef GCC_ARRAY_DELETE_BUG
        ::delete [] localStoreName;
#else
        ::delete localStoreName;
#endif

    localStoreName = ::new char[::strlen(storeRoot)+1];
    ::strcpy(localStoreName, storeRoot);
}

/*
 * We must make our own copies of these items as the interface may be
 * deleted before we use them last.
 */

void ReplicatedPersistentObjectStore::storeLocation (char** location, int number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "void ReplicatedPersistentObjectStore::storeLocation (char** location, " << number << " )" << endl;
#endif

    if (StoreLocation)
    {
	for (int i = 0; i < numberOfStores; i++)
#ifndef GCC_ARRAY_DELETE_BUG
	    ::delete [] StoreLocation[i];
#else
	    ::delete StoreLocation[i];
#endif

#ifndef GCC_ARRAY_DELETE_BUG	
	::delete [] StoreLocation;
#else
	::delete StoreLocation;
#endif	
    }

    numberOfStores = number;

    if (numberOfStores > 0)
    {
	StoreLocation = ::new char* [numberOfStores];
	
	for (int i = 0; i < numberOfStores; i++)
	{
	    StoreLocation[i] = ::new char[::strlen(location[i])+1];
	    ::strcpy(StoreLocation[i], location[i]);
	}
    }
    else
	StoreLocation = NULL;    
}

/*
 * Private functions
 */

ReplicatedPersistentObjectStore::DaemonStatus ReplicatedPersistentObjectStore::createDaemon (ReplicaDescriptor* indx)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "ReplicatedPersistentObjectStore::createDaemon ( " << indx->getHost() << " )" << endl;
#endif

    int res = -1;
    ArjunaName AN2;
    ViewData* ptrToView = new ViewData;
    DaemonStatus status = UNAVAILABLE;
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    UNIQUEID rUid;                                   // Uid for Rajdoot
    rUid.host = rUid.sec = rUid.msec = RPOBJSTOREUID;
#endif

    AN2.setObjectName("RPObjStore");
    AN2.setServiceName("RPObjStore");
    AN2.setHostName(indx->getHost());

    ptrToView->daemonInfo = new Members;
    
    ptrToView->daemonInfo->CRPC = new ClientRpc(&AN2);
    ptrToView->daemonInfo->CRPC->disableAtomic();
    
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
#ifdef DEBUG
    ptrToView->daemonInfo->CRPC->setTimeout(RepDebugTimeout);
    ptrToView->daemonInfo->CRPC->setRetry(RepDebugRetry);
#else
    ptrToView->daemonInfo->CRPC->setTimeout(RepNormalTimeout);
    ptrToView->daemonInfo->CRPC->setRetry(RepNormalRetry);
#endif
    ptrToView->daemonInfo->CRPC->setObjectUid(rUid);
#endif
#if RPC_SYSTEM_TYPE == RPC_RajdootIIAction_ || RPC_SYSTEM_TYPE == RPC_RajdootII_ || RPC_SYSTEM_TYPE == RPC_SimpleRpcAction_ || RPC_SYSTEM_TYPE == RPC_SimpleRpc_
#ifdef DEBUG
    ptrToView->daemonInfo->CRPC->control().setInitTermTimeout(20000);
    ptrToView->daemonInfo->CRPC->control().setInitTermRetry(6);
    ptrToView->daemonInfo->CRPC->control().setTimeout(DebugTimeout);
    ptrToView->daemonInfo->CRPC->control().setRetry(DebugRetry);
#else
    ptrToView->daemonInfo->CRPC->control().setInitTermTimeout(5000);
    ptrToView->daemonInfo->CRPC->control().setInitTermRetry(6);
    ptrToView->daemonInfo->CRPC->control().setTimeout(NormalTimeout);
    ptrToView->daemonInfo->CRPC->control().setRetry(NormalRetry);
#endif
    ptrToView->daemonInfo->CRPC->control().setServerID(RPOBJSTOREUID);
    ptrToView->daemonInfo->CRPC->control().setProtocol(STREAM);
#endif 

    old_handler = setHandler(ReplicatedPersistentObjectStore::rpc_handler);
    ptrToView->daemonInfo->RPCCont = new ClientRpcManager(ptrToView->daemonInfo->CRPC);
    ptrToView->daemonInfo->rpobjstore_daemon = new RemoteRPObjStore(res, ptrToView->daemonInfo->RPCCont);
    setHandler(old_handler);

    /*
     * If creation of the daemon fails but the machine is available, then we return this
     * as a failure and let the rest of the protocol determine whether or not it can
     * continue despite this failure, i.e., whether the failure is critical or not.
     * For replication which does not involve the GVD we must distinguish between these
     * types of initiate failure.
     */

    ptrToView->gd = new GroupData;
    *ptrToView->gd = *indx;
    ptrToView->next = replica_members;
    replica_members = ptrToView;

    if (res == 0)
    {
	ReplicatedPersistentObjectStore::daemons->put(ptrToView->daemonInfo, indx->getHost());
	status = AVAILABLE;
    }
    else
    {
	if ((replica_members->daemonInfo->CRPC->getServerStatus() == INIT_UNABLE) ||
	    (replica_members->daemonInfo->CRPC->getServerStatus() == UNABLE))
	{
	    status = V_PARTITION;
	
	    error_stream << WARNING << "RepObjStore: initiate virtual failed for RPObjStore daemon on machine "
			 << indx->getHost() << " for UID " << indx->getGroupUid()
			 << " and status " << replica_members->daemonInfo->CRPC->getServerStatus()
			 << endl;
	}
	else
	    error_stream << WARNING << "RepObjStore: initiate failed for RPObjStore daemon on machine "
			 << indx->getHost() << " for UID " << indx->getGroupUid() << endl;	    
	
	/*
	 * We must remember this failure for the duration of this action in case
	 * lock propogation occurs, i.e., the failure may not be critical at this
	 * time, but may later (within the same action) become so.
	 */
	
	replica_members->isAlive = FALSE;
	indx->setActive(FALSE);

	/*
	 * Add replica to list of failed replicas for deletion when the action has ended.
	 */

	if (!_failedReplicas)
	    _failedReplicas = new DaemonList();
	
	_failedReplicas->put(ptrToView->daemonInfo, indx->getHost());
    }

    return status;
}

Boolean ReplicatedPersistentObjectStore::tryOldDaemon (Members* currentDaemon, ReplicaDescriptor* indx)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "ReplicatedPersistentObjectStore::tryOldDaemon ( " << indx->getHost() << " )" << endl;
#endif

    Boolean IsAlive = FALSE, activated = FALSE;
    Boolean doPing = TRUE, isAction = (Boolean) (AtomicAction::Current() != (AtomicAction*) 0);
    ViewData* ptrToView = (ViewData*) 0;
    
    /*
     * Instead of pinging for every object on the same machine within the
     * same top-level action we only ping on the first object on each new
     * machine. This may cause us to assume a machine is available when it
     * has in fact failed, but this should be a rare accurrence (especially
     * for short-duration atomic actions. If we get it wrong then the action
     * will abort, so it is still safe.
     */
    
    if ((isAction) && (currentDaemon->pingTopLevel != NIL_UID))
    {
	/*
	 * We have pinged this machine before, but not within this top-level action.
	 */
	
	if (AtomicAction::Current()->topLevelAction() != currentDaemon->pingTopLevel)
	    doPing = TRUE;
	else
	    doPing = FALSE;
    }
    
    if (doPing)
    {
	IsAlive = currentDaemon->CRPC->ping();

	/*
	 * Effectively cache this result for later.
	 */
	
	if (isAction)
	    currentDaemon->pingTopLevel = AtomicAction::Current()->topLevelAction();
    }
    else
	IsAlive = TRUE;

    ptrToView = new ViewData;
    ptrToView->gd = new GroupData;
    ptrToView->daemonInfo = currentDaemon;
    *ptrToView->gd = *indx;
    ptrToView->next = replica_members;
    replica_members = ptrToView;
    
    if (IsAlive)
	activated = TRUE;
    else
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
	debug_stream << "ReplicatedPersistentObjectStore::tryOldDaemon - old RPObjStore unresponsive\n" << flush;
#endif

	/*
	 * We must remember this failure for the duration of this action in case
	 * lock propogation occurs, i.e., the failure may not be critical at this
	 * time, but may later (within the same action) become so.
	 */
	
	replica_members->isAlive = FALSE;

	// remove daemon from cached list

	ReplicatedPersistentObjectStore::daemons->remove(indx->getHost());
    }

    return activated;
}
    
ReplicatedPersistentObjectStore::DaemonStatus ReplicatedPersistentObjectStore::activateDaemon (ReplicaDescriptor* indx)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "ReplicatedPersistentObjectStore::activateDaemon ( " << indx->getHost() << " )" << endl;
#endif

    Members* currentDaemon = (Members*) 0;

    if (indx->getActive())
    {
	currentDaemon = ReplicatedPersistentObjectStore::daemons->get(indx->getHost());

	if (!currentDaemon)
	{
	    // statedaemon not currently registered, so create new one.
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
	    debug_stream << "ReplicatedPersistentObjectStore::activateDaemon - trying to create RPObjStore\n" << flush;
#endif
	    return createDaemon(indx);
	}
	else
	{
	    // statedaemon already registered so try to use it.
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
	    debug_stream << "ReplicatedPersistentObjectStore::activateDaemon - trying old RPObjStore\n" << flush;
#endif
	    return ((tryOldDaemon(currentDaemon, indx)) ? AVAILABLE : UNAVAILABLE);
	}
    }

    return UNAVAILABLE;
}

/*
 * Try to activate replica group. Check for entire group failure at this stage rather
 * than later at invocation stage to try to make fail-over time fast.
 */

Boolean ReplicatedPersistentObjectStore::activateGroup (Boolean& replicaFailure, Boolean criticalFailure)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "ReplicatedPersistentObjectStore::activateGroup ("
		 << ((criticalFailure == TRUE) ? "Critical" : "Non-critical") << ")" << endl;
#endif

    ReplicaDescriptor* ptr = myData.group_view;

    replicaFailure = FALSE;
    
    if (replica_members)
    {
	delete replica_members;
	replica_members = (ViewData*) 0;
    }
    myData.groupCreated = FALSE;

    while (ptr != (ReplicaDescriptor*) 0)
    {
	if (ptr->getActive())
	{
	    DaemonStatus status = activateDaemon(ptr);
	    
	    if (status == AVAILABLE)
		myData.groupCreated = TRUE;
	    else
	    {
		if (isRemoteOnly)
		{
		    /*
		     * If the user supplies the group view then we assume that we can
		     * cope with complete failures, but virtual partitions will cause
		     * write actions to abort.
		     */
		    
		    if (status != V_PARTITION)
			makeInactive(ptr, FALSE);

		    if (criticalFailure)            // only register failure if it is important
			replicaFailure = TRUE;
		}
		else
		{
		    /*
		     * Regardless of error type, exclude failed replica.
		     */
		    
		    makeInactive(ptr, criticalFailure);
		    if (criticalFailure)           // only register failure if it is important
			replicaFailure = TRUE;
		}
	    }
	}
	else
	{
	    if (criticalFailure)
		failureDetected = TRUE;
	}
	
	ptr = ptr->next;
    }

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "ReplicatedPersistentObjectStore::activateGroup - returning "
		 << (myData.groupCreated == TRUE ? "OK" : "Not OK") << "\n" << flush;
#endif
    
    return myData.groupCreated;
}

/*
 * Public non-virtual functions
 */

/*
 * This method returns a Boolean indicating whether a failure was detected during group
 * activation. Therefore a return of FALSE is a *successful* activation, and TRUE means
 * an *error* occurred.
 */

Boolean ReplicatedPersistentObjectStore::createNewObject (const Uid& u, const TypeName tn, Boolean criticalFailure)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "ReplicatedPersistentObjectStore::createNewObject ( " << u << ", "
		 << tn << " )\n" << flush;
#endif

    if (replica_members != (ViewData*) 0)
	return FALSE;     // already created this within this action, so return OK
    
    Boolean groupFailure = FALSE, replicaFailure = FALSE;
    ReplicaDescriptor *tmpRD = (ReplicaDescriptor*) 0, *trailRD = (ReplicaDescriptor*) 0;

    if (myData.group_view)
    {
	delete myData.group_view;
	myData.group_view = (ReplicaDescriptor*) 0;
    }

    /*
     * If this is a new replicated object but no group view is given via StateManager variables
     * "locationsOfStores" and "storeNumbers" then we must go to the database - this is
     * possibly a template object, or the user may have pre-created the replicas and
     * registered them separately.
     */

    if (!StoreLocation)
	return FALSE;

    for (int x = 0; x < numberOfStores; x++)
    {
        trailRD = new ReplicaDescriptor;
	trailRD->setHost(StoreLocation[x]);

	// if this is remote only then name all Uids the same.
	
	if (isRemoteOnly)
	    trailRD->setUid(u);
	else
	{
	    Uid tmpUID;
	    trailRD->setUid(tmpUID);
	}
	
	trailRD->setGroupUid(u);
	trailRD->setObjectName(tn);
	trailRD->setStoreRoot(localStoreName);
	trailRD->next = tmpRD;
	tmpRD = trailRD;
    }
	
    myData.group_view = tmpRD;

    if (!isRemoteOnly)
    {
        if (activateDatabase())
	{
	    TopLevelAction TLC;

	    TLC.Begin();

	    if (NS->addReplicas(*myData.group_view, u) == DONE)
	    {
	        if (TLC.End() == COMMITTED)
		    return FALSE;
	    }
	    else
	        TLC.Abort();
	}

	return TRUE;
    }

    if (!activateGroup(replicaFailure, criticalFailure))
	groupFailure = TRUE;

    if (groupFailure)
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
	debug_stream << "ReplicatedPersistentObjectStore::createNewObject - Could not activate group" << endl;
#endif
	undo();
	storeValid = FALSE;
    }
    else
	myData.groupCreated = TRUE;

    return groupFailure;
}

Boolean ReplicatedPersistentObjectStore::deactivateDatabase ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "ReplicatedPersistentObjectStore::deactivateDatabase ()" << endl;
#endif

    Boolean status = FALSE;
    TopLevelAction TLA;
    
    if ((NS) && (myData.group_uid) && (doRelease))
    {
	TLA.Begin();

	OutCome result = NS->releaseGroup(*myData.group_uid, ReplicatedPersistentObjectStore::hostName, myData.pid);

	if (result != DONE)
	    error_stream << WARNING << "ReplicatedPersistentObjectStore::deactivateDatabase - Could not release group "
	                 << *myData.group_uid << " at database." << endl;
	else
	{
	    doRelease = FALSE;
	    status = TRUE;
	}

	if (status)
	{
	    if (TLA.End() != COMMITTED)
		status = FALSE;
	}
	else
	    TLA.Abort();
    }
    else
	status = TRUE;

    return status;
}
    
Boolean ReplicatedPersistentObjectStore::activateDatabase ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "ReplicatedPersistentObjectStore::activateDatabase ()" << endl;
#endif
    
    TopLevelAction T;
    Boolean result = FALSE;
    int res = -1;
    
    if (!NS)
    {
	T.Begin();

	NS = new NSInterface(res);

	if (res == 0)
	{
	    if (T.End() == COMMITTED)
		result = TRUE;
	    
	}
	else
	    T.Abort();
	
	if (!result)
	{
	    error_stream << WARNING << "ReplicatedPersistentObjectStore - Could not create groupview database\n" << flush;
	    undo();
	    storeValid = FALSE;
	}
    }
    else
	result = TRUE;
    
    return result;
}

/*
 * Check group to determine whether any failures have been detected since the last
 * time it was used - since replicas may be shared some other user may have detected
 * the failure so saving us the time!
 * Note: this could result in a spurious exclusion, but most of the time it will
 * allow us to detect failures much quicker. What we could do at this point is
 * to try to recreate a new server for any "failed" one, but since the node is
 * probably down this will impose an unnecessary overhead on us. When we have a
 * separate failure detection mechanism this will be removed.
 */

Boolean ReplicatedPersistentObjectStore::deadServers (Boolean criticalFailure)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "ReplicatedPersistentObjectStore::deadServers ( "
		 << ((criticalFailure == TRUE) ? "Critical" : "Non-critical") << " )" << endl;
#endif

    ViewData* tempVDptr = replica_members;
    ServerStatus status;

    /*
     * First check the members (servers) within the current group view being used.
     * This will always be a subset of the group view returned from the db.
     */

    while (tempVDptr)
    {
	status = tempVDptr->daemonInfo->CRPC->getServerStatus();

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
	debug_stream << "ReplicatedPersistentObjectStore::deadServers - node "
		     << tempVDptr->gd->getHost() << " with status " << status << endl;
#endif	
	if (status != OPERATIONAL)
	{
	    makeInactive(tempVDptr->gd, criticalFailure);
	    if (criticalFailure)                          // only register failure if it is important
		failureDetected = TRUE;
	}

	tempVDptr = tempVDptr->next;
    }

    /*
     * If no failure was detected check the group view itself. Entries on this
     * list may have been flagged as being failed during some other action, and
     * such failures may not have been deemed critical. They may be critical
     * failures at this point.
     */

    if ((!failureDetected) && (criticalFailure))
    {
	ReplicaDescriptor* tmpRD = myData.group_view;
	
	while ((tmpRD) && (!failureDetected))
	{
	    if (! tmpRD->getActive())
		failureDetected = TRUE;
	    else
		tmpRD = tmpRD->next;
	}
    }

    return failureDetected;
}

/*
 * Get the group view and attempt to exclude any failed members at this point.
 */

Boolean ReplicatedPersistentObjectStore::getViewAndExclude (const Uid& u, const TypeName tn, Boolean criticalFailure)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "ReplicatedPersistentObjectStore::getViewAndExclude ( " << u << ", " << tn << ", "
		 << ((criticalFailure == TRUE) ? "Critical" : "Non-critical") << " )" << endl;
#endif

    Boolean opcode = TRUE, replicaFailure = FALSE, result = FALSE;
    TopLevelAction TLA, TLB;

    if (AtomicAction::Current())
    {
	if (myData.actionID == (Uid*) 0)
	    myData.actionID = new Uid(AtomicAction::Current()->topLevelAction());
	else
	    *myData.actionID = AtomicAction::Current()->topLevelAction();
    }

    // if we do not have a view then get one (either remotely or from cache)

    if (myData.group_view == (ReplicaDescriptor*) 0)
    {
	if (activateDatabase())
	{
	    TLA.Begin();

	    if (!myData.compositeView)
	    {
		myData.compositeView = new ReplicaDescriptor;
		myData.compositeView->setUid(u);
	    
		if (myData.group_uid == (Uid*) 0)
		    myData.group_uid = new Uid(u);
		else
		    *myData.group_uid = u;

		/* If we cannot get the group view then we cannot continue. */
	    
		opcode = NS->getView(*myData.compositeView, u, ReplicatedPersistentObjectStore::hostName, myData.pid);
	    }

	    if (opcode)
	    {
		if (TLA.End() == COMMITTED)
		{
		    myData.compositeView->getGroupList(u, myData.group_view);
		    
		    if (myData.group_view)
		    {
		        myData.group_view->testAndSetObjectNames(tn);
			doRelease = TRUE;
#ifdef DEBUG
			debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
			debug_stream << "ReplicatedPersistentObjectStore::getViewAndExclude - View for group " << u
				     << " is:\n" << *myData.group_view << endl;
#endif			
		    }
		    else
		        opcode = FALSE;
		}
		else
		    opcode = FALSE;
	    }
	    else
	    {
		TLA.Abort();
		error_stream << FATAL << "ReplicatedPersistentObjectStore::getViewAndExclude - could not get replica view for object "
			     << u << ", " << tn << " from database or StateManager set-up." << endl;
		opcode = result = FALSE;
		ArjunaFatal();
	    }
	}
	else
	    opcode = FALSE;
    }
    else
	(void) deadServers(criticalFailure);  // mark any failed replicas

    if (opcode)
    {
	/*
	 * failureDetected : failure of already existing group member.
	 * replicaFailure  : failure of replica during initial group start up.
	 */

	if (!failureDetected)
	    result = activateGroup(replicaFailure, criticalFailure);
	else
	    result = TRUE;

	if (result)  // activated group ok
	{
	    if ((failureDetected) || (replicaFailure))
	    {
		if (activateDatabase())
		{
		    TLB.Begin();
		    
		    result = ((NS->exclude(*myData.group_view, u) == DONE) ? TRUE : FALSE);
		    if (result)
		    {
			if (TLB.End() == COMMITTED)
			    failureDetected = FALSE;
			else
			    result = FALSE;
		    }
		    else
			TLB.Abort();
		}
		else
		    result = FALSE;
	    }
	}
    }
    else
	result = FALSE;
    
    if (!result)
    {
	undo();
	storeValid = FALSE;

	return FALSE;
    }

    return TRUE;
}

/*
 * This routine will typically create a replicated state group. The necessary
 * information will either be obtained from the db, or, in the case of a new
 * object, can be supplied via the appropriate StateManager methods.
 * It is also possible to simply access a non-replicated object store remotely
 * using this scheme. This is treated as a specific case of a replica though,
 * i.e., a group with a single member.
 */

Boolean ReplicatedPersistentObjectStore::createGroupView (const Uid& u, const TypeName tn, Boolean criticalFailure)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "ReplicatedPersistentObjectStore::createGroupView (" << u << ", " << tn << ", "
		 << ((criticalFailure == TRUE) ? "TRUE" : "FALSE") << ")\n" << flush;
#endif

    if (isRemoteOnly)
	return ((createNewObject(u, tn, criticalFailure) == FALSE) ? TRUE : FALSE);

    Boolean new_object = (Boolean) (AtomicAction::Current() != 0);
    
    if (new_object)
	new_object = ((AtomicAction::Current()->status() == PREPARING) && (replica_members == (ViewData*) 0));

    /*
     * If this is a new object and the database is not to be contacted for
     * its group view, then create the view using information provided and
     * register it at the database.
     * Also, even though this is a new object, if it is a composite object
     * or a member of a template group, then we may have already obtained
     * its group view from the cache, in which case we do not need to contact
     * the database.
     */

    if ((new_object) && (myData.group_view == (ReplicaDescriptor*) 0))
    {
        if (createNewObject(u, tn, criticalFailure))
	    return FALSE;
    }

    if ((failureDetected) || (deadServers(criticalFailure)))
	return RepObjStoreInterface::headOfList->reSyncCachedView(u, tn, this);

    return getViewAndExclude(u, tn, criticalFailure);
}

/*
 * Mark as unavailable any failed replicas within this replica's group view.
 * We cannot modify the cached composite view directly in the same way as this
 * could make the failure of a node "invisible" to a subsequent replica which
 * uses the cached copy to build up its own group view. Therefore, if the composite
 * view is required to be updated we must return to the db to recache it.
 * Although modification of the composite can be performed to reflect what has
 * happened at the db, this is not strictly required. [Such modifications need to
 * be done on a per replica basis and not on the host, for obvious reasons].
 */

void ReplicatedPersistentObjectStore::makeInactive (ReplicaData* replicaName, Boolean criticalFailure)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "ReplicatedPersistentObjectStore::makeInactive ( " << replicaName->getHost() << ", "
		 << ((criticalFailure == TRUE) ? "Critical" : "Non-critical") << " )" << endl;
#endif

    ReplicaDescriptor* tmpRD = myData.group_view;

    // only flag failure detection if it is important, i.e., for write operations

    if (criticalFailure)
	failureDetected = TRUE;
    
    /*
     * We cannot simply remove the entry from the group view as it may be
     * necessary later to remember the failure occurred, e.g., the failure
     * is detected during a non-critical section, and then a critical section
     * arises where the failure must be known about and reflected at the db.
     */

    while (tmpRD)
    {
	if (::strcmp(tmpRD->getHost(), replicaName->getHost()) == 0)
	    tmpRD->setActive(FALSE);

	tmpRD = tmpRD->next;
    }

    replicaName->setActive(FALSE);
}

/*
 * This routine cleans up more of the state than undo, which it calls. The two
 * routines are necessary in order to accomplish partial and total "rollback".
 */

void ReplicatedPersistentObjectStore::cleanUp ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "ReplicatedPersistentObjectStore::cleanUp ()" << endl;
#endif

    undo();
    if (myData.compositeView)
    {
	delete myData.compositeView;
	myData.compositeView = (ReplicaDescriptor*) 0;
    }
    if (myData.actionID)
    {
	delete myData.actionID;
	myData.actionID = (Uid*) 0;
    }
}
    
/*
 * ReplicatedPersistentObjectStore is not persistent but some of its state must be
 * undone if a failure occurs which would cause the action to abort. This is
 * so that if the action is retried then it appears as though the
 * ReplicatedPersistentObjectStore was untouched by the last action to use it.
 */

void ReplicatedPersistentObjectStore::undo ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "ReplicatedPersistentObjectStore::undo ()" << endl;
#endif

    myData.groupCreated = FALSE;
    if (replica_members)
    {
	delete replica_members;
	replica_members = (ViewData*) 0;
    }
    if (_failedReplicas)
    {
	delete _failedReplicas;
	_failedReplicas = (DaemonList*) 0;
    }
}

/*
 * Public non-virtual functions
 * If a failure is detected for a modified object then we do not attempt
 * to exclude it here but wait until the user retries the operation.
 * This cuts down the overhead for the case where the user will not retry
 * the operation, and then a replica would be excluded erroneously.
 */

/*
 * Given a type name return an objectstate that contains all of the
 * uids of objects of that type
 */

Boolean ReplicatedPersistentObjectStore::allObjUids (const TypeName tName, ObjectState& state,
						     ObjectStore::StateStatus _type)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "ReplicatedPersistentObjectStore::all_objuids ( " << tName << " )\n" << flush;
#endif

    ViewData* ptView = (ViewData*) 0;

    if (!myData.groupCreated)
    {
	error_stream << WARNING << "ReplicatedPersistentObjectStore - Attempt to use operation without creating group." << endl;
	return FALSE;
    }

    ptView = replica_members;
    
    while (ptView != (ViewData*) 0)
    {
	int res = -1;
	
	old_handler = setHandler(ReplicatedPersistentObjectStore::rpc_handler);
	Boolean outcome = replica_members->daemonInfo->rpobjstore_daemon->allObjUids(tName, state,
										     _type, ptView->gd->getStoreRoot(), res);
	setHandler(old_handler);

	if (res == 0)
	    return outcome;
	else
	{
	    /*
	     * If the machine has failed then exclude it. Otherwise (the machine
	     * was available, but no reply was received) ignore it for now. Since
	     * we are only reading, and not modifying the data this will not cause
	     * any consistency problems.
	     */
	    
	    if ((ptView->daemonInfo->CRPC->getServerStatus() != UNABLE) &&
		(ptView->daemonInfo->CRPC->getServerStatus() != INIT_UNABLE))
	    {
		error_stream << WARNING << "ReplicatedPersistentObjectStore::allObjUids: RPObjStore on machine " << ptView->gd->getHost()
			     << " failed." << endl;
	    
		ReplicatedPersistentObjectStore::daemons->remove(ptView->gd->getHost());
		makeInactive(ptView->gd, FALSE);
	    }
	    else
		error_stream << WARNING << "ReplicatedPersistentObjectStore::allObjUids: RPObjStore on machine "
			     << ptView->gd->getHost() << " virtual fail." << endl;
	}

	ptView = ptView->next;
    }

    return FALSE;
}

/*
 * The following operation commits a previous write_state operation which
 * was made with the SHADOW FileType argument. This is achieved by
 * renaming the shadow and removing the hidden version.
 */

Boolean ReplicatedPersistentObjectStore::commit_state ( const Uid& objUid, const TypeName tName )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "ReplicatedPersistentObjectStore::commit_state ( " << objUid << ", " << tName << " )\n" << flush;
#endif

    int numberContacted = 0;
    ViewData* ptView = (ViewData*) 0;

    if (!createGroupView(objUid, tName, TRUE))
	return FALSE;

    ptView = replica_members;

    while (ptView != (ViewData*) 0)
    {
	int res = -1;
	Boolean result = FALSE;
	
	if (ptView->isAlive)
	{

	    old_handler = setHandler(ReplicatedPersistentObjectStore::rpc_handler);
	    result = ptView->daemonInfo->rpobjstore_daemon->commit_state(ptView->gd->getUid(),
									 tName, ptView->gd->getStoreRoot(), res);
	    setHandler(old_handler);
	}

	if (res != 0)
	{
	    // if we cannot talk to daemon but machine is alive we had better abort
	    
	    if ((ptView->daemonInfo->CRPC->getServerStatus() == UNABLE) ||
		(ptView->daemonInfo->CRPC->getServerStatus() == INIT_UNABLE))
	    {
		error_stream << WARNING << "ReplicatedPersistentObjectStore::commit_state: RPObjStore on machine " << ptView->gd->getHost()
			     << " virtual fail." << endl;		
		undo();
		return FALSE;
	    }
	    
	    error_stream << WARNING << "ReplicatedPersistentObjectStore::commit_state: RPObjStore on machine "
			 << ptView->gd->getHost() << " failed." << endl;
	    
	    ReplicatedPersistentObjectStore::daemons->remove(ptView->gd->getHost());
	    makeInactive(ptView->gd, TRUE);

	    /*
	     * If this is a remote objstore only then we don't have anything to exclude, so
	     * we can continue to attempt to contact any other members. However, if this
	     * is a replicated object then we should exclude replicas.
	     */
	    
	    if (!isRemoteOnly)
	    {
		undo();
		return FALSE;
	    }
	}
	else
	{
	    // if the RPC succeeded but something else went wrong then we should abort
	    
	    if (! result)
	    {
		error_stream << WARNING << "ReplicatedPersistentObjectStore::commit_state: RPObjStore on machine "
			     << ptView->gd->getHost() << " returned error status." << endl;
		
		undo();
		return FALSE;
	    }
	    
	    numberContacted++;
	}

	ptView = ptView->next;
    }

    if (isRemoteOnly)
	return ((numberContacted > 0) ? TRUE : FALSE);
    
    return TRUE;
}

/*
 * read an  uncommitted instance of ObjectState out of the object store. 
 * The instance is identified by the unique id and type
 */

ObjectState* ReplicatedPersistentObjectStore::read_committed (const Uid& storeUid, const TypeName tName)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "ReplicatedPersistentObjectStore::read_committed ( " << storeUid << ", " << tName << " )\n" << flush;
#endif

    ObjectState* os = (ObjectState*) 0;
    ViewData* ptView = (ViewData*) 0;

    if (!createGroupView(storeUid, tName, FALSE))
	return (ObjectState*) 0;

    ptView = replica_members;
    
    while (ptView != (ViewData*) 0)
    {
	int res = -1;

	if (ptView->isAlive)
	{
	    old_handler = setHandler(ReplicatedPersistentObjectStore::rpc_handler);
	    os = ptView->daemonInfo->rpobjstore_daemon->read_committed(ptView->gd->getUid(), tName,
								       ptView->gd->getStoreRoot(), res);
	    setHandler(old_handler);
	}

	if (res == 0)
	    return os;
	else
	{
	    if ((ptView->daemonInfo->CRPC->getServerStatus() != UNABLE) &&
		(ptView->daemonInfo->CRPC->getServerStatus() != INIT_UNABLE))
	    {
		error_stream << WARNING << "ReplicatedPersistentObjectStore::read_committed: RPObjStore on machine "
			     << ptView->gd->getHost() << " failed." << endl;
		
		ReplicatedPersistentObjectStore::daemons->remove(ptView->gd->getHost());
		makeInactive(ptView->gd, FALSE);
	    }
	    else
		error_stream << WARNING << "ReplicatedPersistentObjectStore::read_committed: RPObjStore on machine "
			     << ptView->gd->getHost() << " virtual fail." << endl;
	}

	if (os)
	    delete os;
	ptView = ptView->next;
    }

    undo();
    if (os)
	delete os;
    return (ObjectState*) 0;
}

/*
 * read a committed instance of ObjectState out of the object store. The
 * instance is identified by the unique id and type
 */

ObjectState* ReplicatedPersistentObjectStore::read_uncommitted ( const Uid& objUid, const TypeName tName)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "ReplicatedPersistentObjectStore::read_uncommitted ( " << objUid << ", " << tName << " )\n" << flush;
#endif

    ObjectState* os = (ObjectState*) 0;
    ViewData* ptView = (ViewData*) 0;

    if (!createGroupView(objUid, tName, FALSE))
	return (ObjectState*) 0;

    ptView = replica_members;

    while (ptView != (ViewData*) 0)
    {
	int res = -1;
	
	if (ptView->isAlive)
	{
	    old_handler = setHandler(ReplicatedPersistentObjectStore::rpc_handler);
	    os = ptView->daemonInfo->rpobjstore_daemon->read_uncommitted(ptView->gd->getUid(), tName,
									 ptView->gd->getStoreRoot(), res);
	    setHandler(old_handler);
	}

	if (res == 0)
	    return os;
	else
	{
	    if ((ptView->daemonInfo->CRPC->getServerStatus() != UNABLE) &&
		(ptView->daemonInfo->CRPC->getServerStatus() != INIT_UNABLE))		    
	    {
		error_stream << WARNING << "ReplicatedPersistentObjectStore::read_uncommitted: RPObjStore on machine "
			     << ptView->gd->getHost() << " failed." << endl;
	    
		ReplicatedPersistentObjectStore::daemons->remove(ptView->gd->getHost());
		makeInactive(ptView->gd, FALSE);
	    }
	    else
		error_stream << WARNING << "ReplicatedPersistentObjectStore::read_uncommitted: RPObjStore on machine "
			     << ptView->gd->getHost() << " virtual fail." << endl;		
	}

	if (os)
	    delete os;
	ptView = ptView->next;
    }

    undo();
    if (os)
	delete os;
    return (ObjectState*) 0;
}

/*
 * Remove the ObjectState/file from the object store.
 */

Boolean ReplicatedPersistentObjectStore::remove_committed ( const Uid& objUid, const TypeName name)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "ReplicatedPersistentObjectStore::remove_committed ( " << objUid << ", " << name << " )\n" << flush;
#endif

    int numberContacted = 0;
    ViewData* ptView = (ViewData*) 0;
    Boolean result = FALSE;

    if (!createGroupView(objUid, name, TRUE))
	return FALSE;

    ptView = replica_members;

    while (ptView != (ViewData*) 0)
    {
	int res = -1;

	if (ptView->isAlive)
	{
	    old_handler = setHandler(ReplicatedPersistentObjectStore::rpc_handler);
	    result = ptView->daemonInfo->rpobjstore_daemon->remove_committed(ptView->gd->getUid(), name,
									     ptView->gd->getStoreRoot(), res);
	    setHandler(old_handler);
	}

	if (res == 0)
	{
	    // if the RPC succeeded but something else went wrong then we should abort
	    
	    if (result)
		numberContacted++;
	    else
	    {
		error_stream << WARNING << "ReplicatedPersistentObjectStore::remove_committed: RPObjStore on machine "
			     << ptView->gd->getHost()
			     << " returned error status." << endl;
		undo();
		return FALSE;
	    }
	}
	else
	{
	    if ((ptView->daemonInfo->CRPC->getServerStatus() == UNABLE) ||
		(ptView->daemonInfo->CRPC->getServerStatus() == INIT_UNABLE))	    
	    {
		error_stream << WARNING << "ReplicatedPersistentObjectStore::remove_committed: RPObjStore on machine "
			     << ptView->gd->getHost()
			     << " virtual fail." << endl;	    
		undo();
		return FALSE;
	    }
	    
	    error_stream << WARNING << "ReplicatedPersistentObjectStore::remove_committed: RPObjStore on machine "
			 << ptView->gd->getHost() << " failed." << endl;
	    
	    ReplicatedPersistentObjectStore::daemons->remove(ptView->gd->getHost());
	    makeInactive(ptView->gd, TRUE);
	    
	    /*
	     * If this is a remote objstore only then we don't have anything to exclude, so
	     * we can continue to attempt to contact any other members. However, if this
	     * is a replicated object then we should exclude replicas.
	     */
	    	    
	    if (!isRemoteOnly)
	    {
		undo();
		return FALSE;
	    }
	}

	ptView = ptView->next;
    }

    if (isRemoteOnly)
	return ((numberContacted > 0) ? TRUE : FALSE);
    
    return result;
}

/*
 * We attempt to remove all of the uncommitted states (typically the action
 * is aborting). If we fail to do so for whatever reason we simply output an
 * error message and continue. Crash recovery should sort this out for us
 * eventually.
 */

Boolean ReplicatedPersistentObjectStore::remove_uncommitted (const Uid& storeUid, const TypeName tName)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "ReplicatedPersistentObjectStore::remove_uncommitted ( " << storeUid
		 << ", " << tName << " )\n" << flush;
#endif

    int numberContacted = 0;
    ViewData* ptView = (ViewData*) 0;
    Boolean result = FALSE;

    if (!createGroupView(storeUid, tName, FALSE))
	return FALSE;

    ptView = replica_members;

    while (ptView != (ViewData*) 0)
    {
	if (ptView->gd->getActive())
	{
	    int res = -1;
	    
	    if (ptView->isAlive)
	    {
		old_handler = setHandler(ReplicatedPersistentObjectStore::rpc_handler);
		result = ptView->daemonInfo->rpobjstore_daemon->remove_uncommitted(ptView->gd->getUid(), tName,
										   ptView->gd->getStoreRoot(), res);
		setHandler(old_handler);
	    }

	    if (res == 0)
	    {
		if (result)
		    numberContacted++;
#ifdef DEBUG		
		else
		    error_stream << WARNING << "ReplicatedPersistentObjectStore::remove_uncommitted: RPObjStore on machine "
				 << ptView->gd->getHost()
				 << " returned error status." << endl;
#endif		
	    }
	    else
	    {
		if ((ptView->daemonInfo->CRPC->getServerStatus() == UNABLE) ||
		    (ptView->daemonInfo->CRPC->getServerStatus() == INIT_UNABLE))
		    error_stream << WARNING << "ReplicatedPersistentObjectStore::remove_uncommitted: RPObjStore on machine "
				 << ptView->gd->getHost()
				 << " virtual fail." << endl;		
		else
		{
		    error_stream << WARNING << "ReplicatedPersistentObjectStore::remove_uncommitted: RPObjStore on machine "
				 << ptView->gd->getHost() << " failed." << endl;
	    
		    ReplicatedPersistentObjectStore::daemons->remove(ptView->gd->getHost());
		    makeInactive(ptView->gd, TRUE);
		}		
	    }
	}

	ptView = ptView->next;
    }

    if (isRemoteOnly)
	return ((numberContacted > 0) ? TRUE : FALSE);
        
    return result;
}

Boolean ReplicatedPersistentObjectStore::write_committed (const Uid& objUid, const TypeName tName,
							  const ObjectState& state)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "ReplicatedPersistentObjectStore::write_committed ( " << objUid << ", " << tName
	         << ", " << state << " )\n" << flush;
#endif

    int numberContacted = 0;
    ViewData* ptView = (ViewData*) 0;

    if (!createGroupView(objUid, tName, TRUE))
	return FALSE;

    ptView = replica_members;

    while (ptView != (ViewData*) 0)
    {
	int res = -1;
	Boolean result = FALSE;

	if (ptView->isAlive)
	{
	    old_handler = setHandler(ReplicatedPersistentObjectStore::rpc_handler);
	    result = ptView->daemonInfo->rpobjstore_daemon->write_committed(ptView->gd->getUid(), tName,
									    state, ptView->gd->getStoreRoot(), res);
	    setHandler(old_handler);
	}

	if (res != 0)
	{
	    if ((ptView->daemonInfo->CRPC->getServerStatus() == UNABLE) ||
		(ptView->daemonInfo->CRPC->getServerStatus() == INIT_UNABLE))	    
	    {
		error_stream << WARNING << "ReplicatedPersistentObjectStore::write_committed: RPObjStore on machine "
			     << ptView->gd->getHost()
			     << " virtual fail." << endl;	    
		undo();
		return FALSE;
	    }
	    
	    error_stream << WARNING << "ReplicatedPersistentObjectStore::write_committed: RPObjStore on machine "
			 << ptView->gd->getHost() << " failed." << endl;
	    
	    ReplicatedPersistentObjectStore::daemons->remove(ptView->gd->getHost());
	    makeInactive(ptView->gd, TRUE);

	    /*
	     * If this is a remote objstore only then we don't have anything to exclude, so
	     * we can continue to attempt to contact any other members. However, if this
	     * is a replicated object then we should exclude replicas.
	     */
	    
	    if (!isRemoteOnly)
	    {
		undo();
		return FALSE;
	    }
	}
	else
	{
	    /*
	     * If the RPC succeeded but something else went wrong then we should abort to
	     * guarantee consistency of the replicas.
	     */
	    
	    if (result)
		numberContacted++;
	    else
	    {
		error_stream << WARNING << "ReplicatedPersistentObjectStore::write_committed: RPObjStore on machine "
			     << ptView->gd->getHost()
			     << " returned error status." << endl;		
		undo();
		return FALSE;
	    }
	}

	ptView = ptView->next;
    }

    if (isRemoteOnly)
	return ((numberContacted > 0) ? TRUE : FALSE);
        
    return TRUE;
}

/*
 * write_state saves the ObjectState in a file named by the type and Uid
 * of the ObjectState. If the second argument is SHADOW, then the file name
 * is different so that a subsequent commit_state invocation will rename
 * the file. If writing a shadow the original file is hidden.
 */

Boolean ReplicatedPersistentObjectStore::write_uncommitted (const Uid& storeUid, const TypeName tName,
							    const ObjectState& state)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "ReplicatedPersistentObjectStore::write_uncommitted ( " << storeUid << ", "
	         << tName << ", " << state << " )\n" << flush;
#endif

    int numberContacted = 0;
    ViewData* ptView = (ViewData*) 0;

    if (!createGroupView(storeUid, tName, TRUE))
	return FALSE;

    ptView = replica_members;

    while (ptView != (ViewData*) 0)
    {
	int res = -1;
	Boolean result = FALSE;
	
	if (ptView->isAlive)
	{
	    old_handler = setHandler(ReplicatedPersistentObjectStore::rpc_handler);
	    result = ptView->daemonInfo->rpobjstore_daemon->write_uncommitted(ptView->gd->getUid(), tName,
									      state, ptView->gd->getStoreRoot(), res);
	    setHandler(old_handler);
	}

	/*
	 * Assumes that unresponsive nodes have failed. Recovery daemon will
	 * bring them up-to-date.
	 */

	if (res != 0)
	{
	    if ((ptView->daemonInfo->CRPC->getServerStatus() == UNABLE) ||
		(ptView->daemonInfo->CRPC->getServerStatus() == INIT_UNABLE))
	    {
		error_stream << WARNING << "ReplicatedPersistentObjectStore::write_uncommitted: RPObjStore on machine "
			     << ptView->gd->getHost()
			     << " virtual fail." << endl;	    
		undo();
		return FALSE;
	    }
		    
	    error_stream << WARNING << "ReplicatedPersistentObjectStore::write_uncommitted: RPObjStore on machine "
			 << ptView->gd->getHost() << " failed." << endl;
	    
	    ReplicatedPersistentObjectStore::daemons->remove(ptView->gd->getHost());
	    makeInactive(ptView->gd, TRUE);

	    /*
	     * If this is a remote objstore only then we don't have anything to exclude, so
	     * we can continue to attempt to contact any other members. However, if this
	     * is a replicated object then we should exclude replicas.
	     */
	    
	    if (!isRemoteOnly)
	    {
		undo();
		return FALSE;
	    }	    
	}
	else
	{
	    // if the RPC succeeded but something else went wrong then we should abort

	    if (result)
		numberContacted++;
	    else
	    {
		error_stream << WARNING << "ReplicatedPersistentObjectStore::write_uncommitted: RPObjStore on machine "
			     << ptView->gd->getHost()
			     << " returned error status." << endl;		
		undo();
		return FALSE;
	    }
	}

	ptView = ptView->next;
    }

    if (isRemoteOnly)
	return ((numberContacted > 0) ? TRUE : FALSE);
        
    return TRUE;
}

ReplicaDescriptor* ReplicatedPersistentObjectStore::getCompositeView (const Uid& u) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "ReplicatedPersistentObjectStore::getCompositeView ( " << u << " ) const" << endl;
#endif

    ReplicaDescriptor* ptr = (ReplicaDescriptor*) 0;
    
    if (myData.compositeView != (ReplicaDescriptor*) 0)
	myData.compositeView->getCachedGroupList(u, ptr);
    
    return ptr;
}

Boolean ReplicatedPersistentObjectStore::packInto (Buffer& buff) const
{
    Boolean res = buff.pack(isRemoteOnly);

    if (numberOfStores > 0)
    {
	res = buff.pack(1) && buff.pack(numberOfStores);
	for (int i = 0; (res) && (i < numberOfStores); i++)
	    res = buff.packMappedString(StoreLocation[i]);
    }
    else
	res = buff.pack(-1);

    res = res && buff.packMappedString(localStoreName);

    return res;
}

Boolean ReplicatedPersistentObjectStore::unpackFrom (Buffer& buff)
{
    Boolean res = buff.unpack(isRemoteOnly);
    int ptrType = 0;

    if (res)
    {
	res = buff.unpack(ptrType);
	
	if (res == 1)
	{
	    res = buff.unpack(numberOfStores);
	    
	    if (res)
	    {
		StoreLocation = ::new char* [numberOfStores];
		
		for (int i = 0; (res) && (i < numberOfStores); i++)
		{
		    char* location = (char*) 0;

		    res = buff.unpack(location);
		    
		    if (res)
		    {
			StoreLocation[i] = ::new char[::strlen(location) +1];
			::memset(StoreLocation[i], '\0', ::strlen(location)+1);
			::memcpy(StoreLocation[i], location, ::strlen(location));
			::delete [] location;
		    }
		}
	    }
	}
    }

    if (res)
    {
	if (localStoreName)
	{
#ifndef GCC_ARRAY_DELETE_BUG	    
	    ::delete [] localStoreName;
#else
	    ::delete localStoreName;
#endif	    
	    localStoreName = (char*) 0;
	}
	
	res = buff.unpack(localStoreName);
    }
    
    return res;
}

Boolean ReplicatedPersistentObjectStore::hide_state (const Uid&, const TypeName)
{
    error_stream << WARNING << "ReplicatedPersistentObjectStore::hide_state: operation not supported." << endl;
    return FALSE;
}

Boolean ReplicatedPersistentObjectStore::reveal_state (const Uid&, const TypeName)
{
    error_stream << WARNING << "ReplicatedPersistentObjectStore::reveal_state: operation not supported." << endl;
    return FALSE;
}

ObjectStore::StateStatus ReplicatedPersistentObjectStore::currentState (const Uid&, const TypeName)
{
    error_stream << WARNING << "ReplicatedPersistentObjectStore::currentState: operation not supported." << endl;
    return ObjectStore::OS_UNKNOWN;
}
    
    
Boolean ReplicatedPersistentObjectStore::allTypes (ObjectState&) const
{
    error_stream << WARNING << "ReplicatedPersistentObjectStore::allTypes: operation not supported." << endl;
    return FALSE;
}

Boolean ReplicatedPersistentObjectStore::supressEntry (const char *) const
{
    return FALSE;
}


#ifdef NO_INLINES
#  define REPOBJSTORE_CC_
#  include <Replication/RepObjStore.n>
#  undef REPOBJSTORE_CC_
#endif
