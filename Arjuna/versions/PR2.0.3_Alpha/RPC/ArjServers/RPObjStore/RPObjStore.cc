/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RPObjStore.cc,v 
 */



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

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef ARJNAME_H_
#  include <RPC/ArjServers/ArjName.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef OBJSTORE_H_
#  include <Arjuna/ObjStore.h>
#endif

#ifndef ABORT_H_
#  include <StubGen/Abort.h>
#endif

#ifndef RPOBJSTORE_H_
#  include <RPC/ArjServers/RPObjStore.h>
#endif

#ifndef RPOBJSTOREIF_H_
#  include <RPC/ArjServers/RPObjStoreIF.h>
#endif

#ifndef DAEMONLIST_H_
#  include <RPC/ArjServers/DaemonList.h>
#endif

#ifndef DAEMONDATA_H_
#  include <RPC/ArjServers/DaemonData.h>
#endif

#ifndef MEMBERS_H_
#  include <RPC/ArjServers/Members.h>
#endif

#ifndef STATEDAEMON_H_
#  include <RPC/ArjServers/StateDaemon_stub.h>
#endif

#ifndef NSINTERFACE_H_
#  include <RPC/ArjServers/NSInterface.h>
#endif

#ifndef GROUPDATA_H_
#  include <RPC/ArjServers/GroupData.h>
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif

#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
#  ifndef RAJDOOT_RPCUSER_H_
#    include <RPC/Rajdoot/rpcuser.h>
#  endif
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif 

static const char RCSid[] = "$Id: RPObjStore.cc,v 1.22 1993/10/29 16:27:40 nmcl Exp $";


Boolean RPObjStoreSetUp::setUp = FALSE;

int RemotePersistentObjectStore::use_count = 0;
DaemonList* RemotePersistentObjectStore::daemons = 0;
char* RemotePersistentObjectStore::hostName = 0;


/*
 * The following class has an equivalent in every type of object store.
 * This is used to cut down the size of the binaries at link time by
 * dynamically assembling a linked list of only those types of object stores
 * which the application needs to use.
 */

RPObjStoreSetUp::RPObjStoreSetUp ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RPObjStoreSetUp::RPObjStoreSetUp ()" << endl;
#endif

    if (!setUp)
    {
	ObjectStore::addToList(RPObjStoreSetUp::create, RPObjStoreSetUp::destroy, "RemotePersistentObjectStore");
	setUp = TRUE;
    }
}

RPObjStoreSetUp::~RPObjStoreSetUp () {}

ObjectStore* RPObjStoreSetUp::create (const char*)
{
    return new RPObjStoreInterface;
}

// deletion gets done by ObjectStore

void RPObjStoreSetUp::destroy (const char*) {}


/*
 * Since we cannot run exclude operation within commit phase, and
 * all objects must be accessed using "atomic" RPC, we cannot
 * tolerate a failure within an action without aborting the action
 * and retrying.
 */

client_data::client_data ()
                          : groupCreated(FALSE),
			    pid(::getpid()),
			    group_view(0),
			    compositeView(0),
			    group_uid(0)
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
}


ViewData::ViewData ()
                    : daemonInfo(0),
		      gd(0),
		      next(0)
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
 * Public constructors and destructor
 */

	
RemotePersistentObjectStore::RemotePersistentObjectStore ()
                                                          : storeName(OBJECTSTOREDIR_Q),
                                                            storeValid(TRUE),
							    replica_members(0),
							    NS(0),
							    failureDetected(FALSE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::RemotePersistentObjectStore ()\n" << flush;
#endif

    isRemoteOnly = FALSE;
    StoreLocation = 0;
    
    if (RemotePersistentObjectStore::daemons == 0)
	RemotePersistentObjectStore::daemons = new DaemonList;
    RemotePersistentObjectStore::use_count++;

    if (RemotePersistentObjectStore::hostName == 0)
    {
	RemotePersistentObjectStore::hostName = ::new char[MAXHOSTNAMELEN+1];
	::memset(RemotePersistentObjectStore::hostName, '\0', MAXHOSTNAMELEN+1);
	(void) ::gethostname(RemotePersistentObjectStore::hostName, MAXHOSTNAMELEN);
    }
}

RemotePersistentObjectStore::~RemotePersistentObjectStore ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::~RemotePersistentObjectStore()\n" << flush;
#endif

    // Cannot release group here as other (complex) objects may have implicit lock.

    if (NS)
	delete NS;

    RemotePersistentObjectStore::use_count--;
    if (RemotePersistentObjectStore::use_count == 0)
    {
	if (RemotePersistentObjectStore::daemons)
	{
	    delete RemotePersistentObjectStore::daemons;
	    RemotePersistentObjectStore::daemons = 0;
	}
    }

    if (StoreLocation)
#ifndef __GNUG__
	::delete [] StoreLocation;
#else
	::delete StoreLocation;
#endif

    undo();
    setHandler(old_handler);
}

Boolean RemotePersistentObjectStore::rpc_handler () { return FALSE; }

void RemotePersistentObjectStore::storeLocation (const char* location)
{
    int size = ((location != 0) ? ::strlen(location) : MAXHOSTNAMELEN);
    const char* tempPtr = ((location != 0) ? location : RemotePersistentObjectStore::hostName);
    StoreLocation = ::new char[size+1];
    ::memset(StoreLocation, '\0', size+1);
    ::memcpy(StoreLocation, tempPtr, size);
}


/*
 * Private functions
 */



Boolean RemotePersistentObjectStore::createDaemon (ReplicaDescriptor* indx)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "void RemotePersistentObjectStore::createDaemon (ReplicaDescriptor* indx)" << endl;
#endif

    int res = -1;
    ArjunaName AN2;
    ViewData* ptrToView = new ViewData;
    Boolean activated = FALSE;
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    UNIQUEID rUid;                                   // Uid for Rajdoot
    rUid.host = rUid.sec = rUid.msec = STATEDAEMONUID;
#endif

    AN2.SetObjName("StateDaemon");
    AN2.SetServiceName("StateDaemon");
    AN2.SetHostName(indx->getHost());

    ptrToView->daemonInfo = new Members;
    
    ptrToView->daemonInfo->CRPC = new ClientRpc(&AN2);
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
#ifdef DEBUG
    ptrToView->daemonInfo->CRPC->SetTimeout(DebugTimeout);
    ptrToView->daemonInfo->CRPC->SetRetry(DebugRetry);
    ptrToView->daemonInfo->CRPC->SetObjectUid(rUid);
#else
    ptrToView->daemonInfo->CRPC->SetTimeout(NormalTimeout);
    ptrToView->daemonInfo->CRPC->SetRetry(NormalRetry);
    ptrToView->daemonInfo->CRPC->SetObjectUid(rUid);
#endif
#endif

    old_handler = setHandler(RemotePersistentObjectStore::rpc_handler);
    ptrToView->daemonInfo->RPCCont = new RpcControl(ptrToView->daemonInfo->CRPC);
    ptrToView->daemonInfo->r_state_daemon = new RemoteStateDaemon(res, FALSE, ptrToView->daemonInfo->RPCCont);
    setHandler(old_handler);

    if (res == 0)
    {
	ptrToView->gd = new GroupData;
	*ptrToView->gd = *indx;
	ptrToView->next = replica_members;
	replica_members = ptrToView;
	RemotePersistentObjectStore::daemons->put(ptrToView->daemonInfo, indx->getHost());
	activated = TRUE;
    }
    else
    {
	indx->setActive(FALSE);
	delete ptrToView->daemonInfo;
    }

    return activated;
}

Boolean RemotePersistentObjectStore::tryOldDaemon (Members* touse, ReplicaDescriptor* indx)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean RemotePersistentObjectStore::tryOldDaemon ( " << indx->getHost() << " )" << endl;
#endif

    Boolean IsAlive = FALSE, activated = FALSE;
    ViewData* ptrToView = 0;

    touse->r_state_daemon->areYouAlive(IsAlive);
    
    if (IsAlive)
    {
	ptrToView = new ViewData;
	ptrToView->gd = new GroupData;
	ptrToView->daemonInfo = touse;
	*ptrToView->gd = *indx;
	ptrToView->next = replica_members;
	replica_members = ptrToView;
	activated = TRUE;
    }
    else
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
	debug_stream << "RemotePersistentObjectStore::activateDaemon - old StateDaemon non-responsive\n" << flush;
#endif
	// remove daemon from cached list

	RemotePersistentObjectStore::daemons->remove(indx->getHost());
    }

    return activated;
}
    
Boolean RemotePersistentObjectStore::activateDaemon (ReplicaDescriptor* indx)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean RemotePersistentObjectStore::activateDaemon (ReplicaDescriptor* indx)" << endl;
#endif

    Members* touse = 0;

    if (indx->getActive())
    {
	touse = RemotePersistentObjectStore::daemons->get(indx->getHost());

	if (!touse)
	{
	    // statedaemon not currently registered, so create new one.
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
	    debug_stream << "RemotePersistentObjectStore::activateDaemon - trying to create StateDaemon\n" << flush;
#endif
	    return createDaemon(indx);
	}
	else
	{
	    // statedaemon already registered so try to use it.
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
	    debug_stream << "RemotePersistentObjectStore::activateDaemon - trying old StateDaemon\n" << flush;
#endif
	    return tryOldDaemon(touse, indx);
	}
    }

    return FALSE;
}

/*
 * Try to activate replica group. Check for entire group failure at this stage rather
 * than later at invocation stage to try to make fail-over time fast.
 */

Boolean RemotePersistentObjectStore::activateGroup (Boolean& replicaFailure, Boolean criticalFailure)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean RemotePersistentObjectStore::activateGroup ("
		 << ((criticalFailure == TRUE) ? "TRUE" : "FALSE") << ")" << endl;
#endif

    ReplicaDescriptor* ptr = myData.group_view;

    if (replica_members)
    {
	delete replica_members;
	replica_members = 0;
    }
    myData.groupCreated = FALSE;

    while (ptr != 0)
    {
	if (ptr->getActive())
	{
	    if (activateDaemon(ptr))
		myData.groupCreated = TRUE;
	    else
	    {
		makeInactive(ptr, criticalFailure);
		if (criticalFailure)                           // only register failure if it is important
		    replicaFailure = TRUE;
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
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean RemotePersistentObjectStore::activateGroup () returning "
		 << (myData.groupCreated == TRUE ? "OK" : "Not OK") << "\n" << flush;
#endif
    
    return myData.groupCreated;
}

/*
 * Public non-virtual functions
 */

Boolean RemotePersistentObjectStore::createNewObject (const Uid& u)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean RemotePersistentObjectStore::createNewObject ( " << u << " )\n" << flush;
#endif

    Boolean groupFailure, replicaFailure = TRUE;

    isRemoteOnly = TRUE;
    
    if (myData.group_view)
	delete myData.group_view;

    myData.group_view = new ReplicaDescriptor;
    
    if (!StoreLocation)
        storeLocation(0);

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "RemotePersistentObjectStore - Object store location is " << StoreLocation << "\n" << flush;
#endif

    myData.group_view->setHost(StoreLocation);
    myData.group_view->setUid(u);
    myData.group_view->setActive(TRUE);

    groupFailure = !activateGroup(replicaFailure, FALSE);
    
    if (groupFailure)
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
	debug_stream << "RemotePersistentObjectStore - Could not activate group" << endl;
#endif
	undo();
	storeValid = FALSE;
    }
    else
	myData.groupCreated = TRUE;

    return groupFailure;
}

Boolean RemotePersistentObjectStore::deactivateDatabase ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean RemotePersistentObjectStore::deactivateDatabase ()" << endl;
#endif

    Boolean status = FALSE;
    
    if ((NS) && (myData.group_uid))
    {
	TopLevelAction TL;
	
	// Start nested top-level action

	TL.Begin();
	
	OutCome result = NS->releaseGroup(*myData.group_uid, RemotePersistentObjectStore::hostName, myData.pid);
	if (result != Done)
	    error_stream << WARNING << "Could not release group "
	                 << *myData.group_uid << " at database" << endl;
	else
	    status = TRUE;

	TL.End();
    }

    return status;
}
    
Boolean RemotePersistentObjectStore::activateDatabase ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean RemotePersistentObjectStore::activateDatabase ()" << endl;
#endif
    
    TopLevelAction T;
    Boolean result = FALSE;
    int res = -1;
    
    if (!NS)
    {
	T.Begin();

	NS = new NSInterface(res);

	if (res != 0)
	{
	    error_stream << WARNING << "RemotePersistentObjectStore - Could not create groupview database\n" << flush;
	    T.Abort();
	    undo();
	    storeValid = FALSE;
	}

	if (T.End() != COMMITTED)
	    undo();
	else
	    result = TRUE;
    }
    else
	result = TRUE;
    
    return result;
}

/*
 * Check group to determine whether any failures have been detected since the last
 * time it was used - since replicas may be shared some other user may have detected
 * the failure so saving us the time!
 */

Boolean RemotePersistentObjectStore::deadServers (Boolean criticalFailure)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean RemotePersistentObjectStore::deadServers ("
		 << ((criticalFailure == TRUE) ? "TRUE" : "FALSE") << ")" << endl;
#endif

    ViewData* tempVDptr = replica_members;
    ServerStatus status;

    while (tempVDptr)
    {
	status = tempVDptr->daemonInfo->CRPC->GetServerStatus();

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
	debug_stream << "RemotePersistentObjectStore::deadServers - node "
		     << tempVDptr->gd->getHost() << " with status " << status << endl;
#endif	
	if (status != OPERATIONAL)
	{
	    makeInactive(tempVDptr->gd, criticalFailure);
	    if (criticalFailure)                                     // only register failure if it is important
		failureDetected = TRUE;
	}

	tempVDptr = tempVDptr->next;
    }

    if ((!failureDetected) && (criticalFailure))
    {
	ReplicaDescriptor* tmpRD = myData.group_view;
	
	while ((tmpRD) && (!failureDetected))
	{
	    if (tmpRD->getActive() == FALSE)
		failureDetected = TRUE;
	    else
		tmpRD = tmpRD->next;
	}
    }
    
    return failureDetected;
}

Boolean RemotePersistentObjectStore::getViewAndExclude (const Uid& u, const TypeName tn, Boolean criticalFailure)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean RemotePersistentObjectStore::getViewAndExclude (" << u << ", " << tn << ", "
		 << ((criticalFailure == TRUE) ? "TRUE" : "FALSE") << ")" << endl;
#endif

    Boolean opcode = TRUE, replicaFailure = FALSE, result = FALSE;
    TopLevelAction TLA;

    if (activateDatabase())
    {
	TLA.Begin();
	
	if (myData.group_view == 0)
	{
	    if (myData.compositeView)
		delete myData.compositeView;
	    
	    myData.compositeView = new ReplicaDescriptor;
	    myData.compositeView->setUid(u);
	    myData.group_uid = new Uid(u);

	    opcode = NS->getView(*myData.compositeView, u, RemotePersistentObjectStore::hostName, myData.pid);

	    if (opcode)
	    {
		myData.compositeView->getGroupList(u, myData.group_view);
		myData.group_view->testAndSetObjectNames(tn);
	    }
	    else
	    {
		delete myData.compositeView;
		myData.compositeView = 0;
	    }
	}
	else
	    (void) deadServers(criticalFailure);

	if (opcode)  // if getView fails, no point in going on.
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
		    result = ((NS->exclude(*myData.group_view, u) == Done) ? TRUE : FALSE);
		    if (result)
			failureDetected = FALSE;
		}
	    }
	}

	if (result)
	{
	    if (TLA.End() == COMMITTED)
		return TRUE;
	}
	else
	    TLA.Abort();

	undo();
	storeValid = FALSE;

	return FALSE;
    }

    return ((createNewObject(u) == FALSE) ? TRUE : FALSE);
}

/*
 * Create new object or access non-replicated object without going to
 * database. Objects can have remote state without being logged at
 * database.
 */

Boolean RemotePersistentObjectStore::createGroupView (const Uid& u, const TypeName tn, Boolean criticalFailure)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "RemotePersistentObjectStore::createGroupView (" << u << ", " << tn << ", "
		 << ((criticalFailure == TRUE) ? "TRUE" : "FALSE") << ")\n" << flush;
#endif

    Boolean new_object = (Boolean) (AtomicAction::Current() != 0);

    if (new_object)
	new_object = ((AtomicAction::Current()->status() == PREPARING) && (replica_members == 0));

    if ((new_object) || (isRemoteOnly))
	return ((createNewObject(u) == FALSE) ? TRUE : FALSE);

    if ((failureDetected) || (deadServers(criticalFailure)))
    {
	failureDetected = FALSE;
	return RPObjStoreInterface::headOfList->reSyncCachedView(u, tn, this);
    }

    return getViewAndExclude(u, tn, criticalFailure);
}

void RemotePersistentObjectStore::makeInactive (ReplicaData* replicaName, Boolean criticalFailure)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "void RemotePersistentObjectStore::makeInactive (" << replicaName->getHost() << ", "
		 << ((criticalFailure == TRUE) ? "TRUE" : "FALSE") << ")" << endl;
#endif

    ReplicaDescriptor* tmpRD = myData.group_view;

    // only flag failure detection if it is important, i.e., for write operations

    if (criticalFailure)
	failureDetected = TRUE;
    
    while (tmpRD)
    {
	if (::strcmp(tmpRD->getHost(), replicaName->getHost()) == 0)
	    tmpRD->setActive(FALSE);

	tmpRD = tmpRD->next;
    }

    tmpRD = myData.compositeView;
    
    while (tmpRD)
    {
	if (::strcmp(tmpRD->getHost(), replicaName->getHost()) == 0)
	    tmpRD->setActive(FALSE);

	tmpRD = tmpRD->next;
    }

    replicaName->setActive(FALSE);
}
    
/*
 * RemotePersistentObjectStore is not persistent but some of its state must be
 * undone if a failure occurs which would cause the action to abort. This is
 * so that if the action is retried then it appears as though the
 * RemotePersistentObjectStore was untouched by the last action to use it.
 */

void RemotePersistentObjectStore::undo ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "void RemotePersistentObjectStore::undo ()\n" << flush;
#endif

    myData.groupCreated = FALSE;
    if (replica_members)
    {
	delete replica_members;
	replica_members = 0;
    }
}

/*
 * Public non-virtual functions
 */

/*
 * Given a type name return an objectstate that contains all of the
 * uids of objects of that type
 */

ObjectState *RemotePersistentObjectStore::allObjUids ( const TypeName tName )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::all_objuids ( " << tName << " )\n" << flush;
#endif

    Buffer buff;
    ObjectState* objstate = 0;
    int res = -1;
    ViewData* ptView = 0;

    if (!myData.groupCreated)
    {
	error_stream << WARNING << "RemotePersistentObjectStore - Attempt to use operation without creating group."
		     << endl;
	return 0;
    }

    ptView = replica_members;
    
    while (ptView)
    {
	old_handler = setHandler(RemotePersistentObjectStore::rpc_handler);
	buff = replica_members->daemonInfo->r_state_daemon->allObjUids(tName, res);
	setHandler(old_handler);

	if (res == 0)
	{
	    objstate = new ObjectState;
	    if (!objstate->unpackFrom(buff))
		delete objstate;
	    else
		return objstate;
	}
	else
	{
	    RemotePersistentObjectStore::daemons->remove(replica_members->gd->getHost());
	    makeInactive(replica_members->gd, FALSE);
	}

	ptView = ptView->next;
    }

    return 0;
}

/*
 * The following operation commits a previous write_state operation which
 * was made with the SHADOW FileType argument. This is achieved by
 * renaming the shadow and removing the hidden version.
 */

Boolean RemotePersistentObjectStore::commit_state ( const Uid& objUid, const TypeName tName )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::commit_state ( " << objUid << ", " << tName << " )\n" << flush;
#endif

    int res;
    Boolean result;
    ViewData* ptView = 0;

    if (!createGroupView(objUid, tName, TRUE))
	return FALSE;

    ptView = replica_members;

    while (ptView != 0)
    {
	res = -1;

	old_handler = setHandler(RemotePersistentObjectStore::rpc_handler);
	result = ptView->daemonInfo->r_state_daemon->commitState(ptView->gd->getUid(), tName, res);
	setHandler(old_handler);

	if (res != 0)
	{
	    RemotePersistentObjectStore::daemons->remove((char*) ptView->gd->getHost());
	    makeInactive(ptView->gd, TRUE);
	    undo();
	    return FALSE;
	}

	ptView = ptView->next;
    }

    return TRUE;
}

/*
 * Remove the ObjectState/file from the object store.
 */

Boolean RemotePersistentObjectStore::remove_state ( const Uid& objUid, const TypeName name)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::remove_state ( " << objUid << ", " << name << " )\n" << flush;
#endif

    Boolean result = FALSE;
    int res;
    ViewData* ptView = 0;

    if (!createGroupView(objUid, name, TRUE))
	return FALSE;

    ptView = replica_members;

    while (ptView != 0)
    {
	res = -1;

	old_handler = setHandler(RemotePersistentObjectStore::rpc_handler);
	(void) ptView->daemonInfo->r_state_daemon->removeState(ptView->gd->getUid(), name, res);
	setHandler(old_handler);

	if (res == 0)
	    result = TRUE;
	else
	{
	    RemotePersistentObjectStore::daemons->remove((char*) ptView->gd->getHost());
	    makeInactive(ptView->gd, TRUE);
	    undo();
	    return FALSE;
	}

	ptView = ptView->next;
    }

    return result;
}

Boolean RemotePersistentObjectStore::remove_uncommitted (const Uid& storeUid, const TypeName tName)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::remove_uncommitted ( " << storeUid << ", " << tName << " )\n" << flush;
#endif

    Boolean result = FALSE;
    int res;
    ViewData* ptView = 0;

    if (!createGroupView(storeUid, tName, TRUE))
	return FALSE;

    ptView = replica_members;

    while (ptView != 0)
    {
	if (ptView->gd->getActive())
	{
	    res = -1;

	    old_handler = setHandler(RemotePersistentObjectStore::rpc_handler);
	    (void) ptView->daemonInfo->r_state_daemon->removeUnCommitted(ptView->gd->getUid(), tName, res);
	    setHandler(old_handler);

	    if (res == 0)
		result = TRUE;
	    else
	    {
		RemotePersistentObjectStore::daemons->remove((char*) ptView->gd->getHost());
		makeInactive(ptView->gd, TRUE);
		undo();
		return FALSE;
	    }
	}

	ptView = ptView->next;
    }

    return result;
}

/*
 * read an  uncommitted instance of ObjectState out of the object store. 
 * The instance is identified by the unique id and type
 */

ObjectState* RemotePersistentObjectStore::read_uncommitted (const Uid& storeUid, const TypeName tName)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::read_uncommitted ( " << storeUid << ", " << tName << " )\n" << flush;
#endif

    Buffer buff;
    int res;
    ObjectState* os = 0;
    ViewData* ptView = 0;

    if (!createGroupView(storeUid, tName, FALSE))
	return 0;

    ptView = replica_members;
    
    while (ptView != 0)
    {
	res = -1;
	
	old_handler = setHandler(RemotePersistentObjectStore::rpc_handler);
	buff = ptView->daemonInfo->r_state_daemon->readUnCommitted(replica_members->gd->getUid(), tName, res);
	setHandler(old_handler);

	if (res == 0)
	{
	    os = new ObjectState;
	    os->unpackFrom(buff);
	    return os;
	}
	else
	{
	    RemotePersistentObjectStore::daemons->remove((char*) replica_members->gd->getHost());
	    makeInactive(replica_members->gd, FALSE);
	}

	ptView = ptView->next;
    }

    undo();
    return 0;
}

/*
 * read a committed instance of ObjectState out of the object store. The
 * instance is identified by the unique id and type
 */

ObjectState* RemotePersistentObjectStore::read_state ( const Uid& objUid, const TypeName tName)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::read_state ( " << objUid << ", " << tName << " )\n" << flush;
#endif

    Buffer buff;
    int res;
    ObjectState* os;
    Boolean ok = FALSE;
    ViewData* ptView = 0;

    if (!createGroupView(objUid, tName, FALSE))
	return 0;

    ptView = replica_members;

    while ((ptView != 0) && (!ok))
    {
	if (ptView->gd->getActive())
	{
	    res = -1;

	    old_handler = setHandler(RemotePersistentObjectStore::rpc_handler);
	    buff = ptView->daemonInfo->r_state_daemon->readState(ptView->gd->getUid(), tName, res);
	    setHandler(old_handler);

	    if (res == 0)
		ok = TRUE;
	    else
	    {
		RemotePersistentObjectStore::daemons->remove((char*) ptView->gd->getHost());
		makeInactive(ptView->gd, FALSE);
	    }
	}
	
	ptView = ptView->next;
    }

    if (ok)
    {
	os = new ObjectState;
	os->unpackFrom(buff);
	return os;
    }
    else
	undo();

    return 0;
}

Boolean RemotePersistentObjectStore::write_committed (const Uid& objUid, const TypeName tName,
						      const ObjectState& state)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::write_committed ( " << objUid << ", " << tName
	         << ", " << state << " )\n" << flush;
#endif

    Buffer buff;
    int res;
    Boolean result;
    ViewData* ptView = 0;

    if (!createGroupView(objUid, tName, TRUE))
	return FALSE;

    ptView = replica_members;
    state.packInto(buff);

    while (ptView != 0)
    {
	res = -1;

	old_handler = setHandler(RemotePersistentObjectStore::rpc_handler);
	result = ptView->daemonInfo->r_state_daemon->writeCommitted(ptView->gd->getUid(), tName, buff, res);
	setHandler(old_handler);

	if (res != 0)
	{
	    RemotePersistentObjectStore::daemons->remove((char*) ptView->gd->getHost());
	    makeInactive(ptView->gd, TRUE);
	    undo();
	    return FALSE;
	}

	ptView = ptView->next;
    }

    return TRUE;
}

/*
 * write_state saves the ObjectState in a file named by the type and Uid
 * of the ObjectState. If the second argument is SHADOW, then the file name
 * is different so that a subsequent commit_state invocation will rename
 * the file. If writting a shadow the original file is hidden.
 */

Boolean RemotePersistentObjectStore::write_state ( const Uid& storeUid, const TypeName tName,
						   const ObjectState& state)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::write_state ( " << storeUid << ", "
	         << tName << ", " << state << " )\n" << flush;
#endif

    Buffer buff;
    int res;
    Boolean result;
    ViewData* ptView = 0;

    if (!createGroupView(storeUid, tName, TRUE))
	return FALSE;

    ptView = replica_members;
    state.packInto(buff);

    while (ptView != 0)
    {
	res = -1;

	old_handler = setHandler(RemotePersistentObjectStore::rpc_handler);
	result = ptView->daemonInfo->r_state_daemon->writeState(ptView->gd->getUid(), tName, buff, res);
	setHandler(old_handler);

	/*
	 * Assumes that non-responsive nodes have failed. Recovery daemon will
	 * bring them up-to-date.
	 */

	if (res != 0)
	{
	    RemotePersistentObjectStore::daemons->remove((char*) ptView->gd->getHost());
	    makeInactive(ptView->gd, TRUE);
	    undo();
	    return FALSE;
	}

	ptView = ptView->next;
    }

    return TRUE;
}

ReplicaDescriptor* RemotePersistentObjectStore::getCompositeView (const Uid& u) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "ReplicaDescriptor* RemotePersistentObjectStore::getCompositeView ( " << u << " ) const" << endl;
#endif

    ReplicaDescriptor* ptr = 0;
    
    if (myData.compositeView != 0)
	myData.compositeView->getCachedGroupList(u, ptr);
    
    return ptr;
}

#ifdef NO_INLINES
#  define RPOBJSTORE_CC_
#  include <RPC/ArjServers/RPObjStore.n>
#  undef RPOBJSTORE_CC_
#endif
