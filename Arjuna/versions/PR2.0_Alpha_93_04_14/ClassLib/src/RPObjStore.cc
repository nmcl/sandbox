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
#include <System/protos.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef SYS_STAT_H_
#  include <System/sys/stat.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef CTYPE_H_
#  include <System/ctype.h>
#endif

#ifndef DIRENT_H_
#  include <System/dirent.h>
#endif

#ifndef FCNTL_H_
#  include <System/fcntl.h>
#endif

#ifndef SYS_FILE_H_
#  include <System/sys/file.h>
#endif

#ifndef SIGNAL_H_
#include <System/signal.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif 

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif 

#ifndef PROTOS_H_
#include <System/protos.h>
#endif

#ifndef BUFFER_H_
#include <Common/Buffer.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef ARJNAME_H_
#include <RPC/ArjServers/ArjName.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
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

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif

#include <Arjuna/RPObjStore.h>

#ifndef STATEDAEMON_H_
#include <RPC/ArjServers/StateDaemon_stub.h>
#endif

#ifndef NAMESERV_H_
#include <RPC/ArjServers/NameServ_stub.h>
#endif

#ifndef NSINTERFACE_H_
#include <RPC/ArjServers/NSInterface.h>
#endif

static const char RCSid[] = "$Id: RPObjStore.cc,v 1.18 1993/05/13 14:17:22 nmcl Exp $";

int RemotePersistentObjectStore::use_count = 0;
DaemonList* RemotePersistentObjectStore::daemons = 0;


/*
 * Public constructors and destructor
 */

/* The Members class holds the relevant information on which replicas (primary and backups)
 * are available i.e., responded to initiate.
 */

Members::Members ()
                  : CRPC(0),
		    RPCCont(0),
		    r_state_daemon(0),
		    next(0),
		    rd(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Members::Members ()\n" << flush;
#endif
}

Members::~Members ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Members::~Members ()\n" << flush;
#endif

    if (next)
	delete next;
}

DaemonData::DaemonData ()
                        : memb(0),
			  hostName(0),
			  available(TRUE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "DaemonData::DaemonData ()\n" << flush;
#endif
}

DaemonData::~DaemonData ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "DaemonData::~DaemonData ()\n" << flush;
#endif
}


DaemonList::DaemonList ()
                        : next(0),
			  data(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "DaemonList::DaemonList ()\n" << flush;
#endif
}

DaemonList::~DaemonList ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "DaemonList::~DaemonList ()\n" << flush;
#endif

    if ((data->memb->r_state_daemon) && (data->available))
	if (data->memb->CRPC->GetServerStatus() != DEAD)
	    delete data->memb->r_state_daemon;

    if (data->memb->rd)
	delete data->memb->rd;

    if (data->hostName)
	delete data->hostName;

    if (data)
	delete data;

    if (next)
	delete next;
}

RemoteStateDaemon* DaemonList::Get (const char* name)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemoteStateDaemon* DaemonList::Get (const char* name)\n" << flush;
#endif

    if ((name == 0) || (data == 0))
	return 0;

    DaemonList* dmarker = this;

    while (dmarker != 0)
    {
	if (dmarker->data->available)
	{
	    if (dmarker->data->memb->CRPC->GetServerStatus() == DEAD)
		dmarker->data->available = FALSE;
	}

	if ((strcmp(name, dmarker->data->hostName) == 0) && (dmarker->data->available))
	    return dmarker->data->memb->r_state_daemon;
	else
	   dmarker = dmarker->next;
    }

    return 0;
}

void DaemonList::Put (Members* toadd, const char* name)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "void DaemonList::Put (RemoteStateDaemon* toadd, const char* name)\n" << flush;
#endif

    DaemonData* adding = new DaemonData;

    adding->memb = toadd;
    adding->hostName = new char[MAXHOSTNAMELEN+1];
    memset(adding->hostName, '\0', MAXHOSTNAMELEN+1);
    memcpy(adding->hostName, name, strlen(name));

    if (data == 0)
	data = adding;
    else
    {
	DaemonList* dmarker = this;
	while (dmarker->next != 0)
	    dmarker = dmarker->next;

	dmarker->next = new DaemonList;
	dmarker->next->data = adding;
    }
}

void DaemonList::Delete (const char* name)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "void DaemonList::Delete (const char* name)\n" << flush;
#endif

    if ((name) && (data))
    {
	DaemonList *dmarker = this;
	Boolean found = FALSE;

	while ((dmarker != 0) && (!found))
	{
	    if (strcmp(name, dmarker->data->hostName) == 0)
	    {
		dmarker->data->available = FALSE;
		found = TRUE;
	    }
	    else
		dmarker = dmarker->next;
	}
    }
}

	
RemotePersistentObjectStore::RemotePersistentObjectStore ()
                                                          : storeValid(TRUE),
							    storeName(OBJECTSTOREDIR_Q),
							    replica_members(0),
							    group_view(0),
							    group_uid(0),
							    primary_uid(0),
							    NS(0),
							    created_primary(FALSE),
							    created_backups(FALSE),
							    replica_failure(FALSE),
							    group_failure(FALSE),
							    StoreLocation(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::RemotePersistentObjectStore()\n" << flush;
#endif

    if (RemotePersistentObjectStore::daemons == 0)
	RemotePersistentObjectStore::daemons = new DaemonList;
    use_count++;

    signal(SIGQUIT, SIG_IGN);
}

RemotePersistentObjectStore::~RemotePersistentObjectStore ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::~RemotePersistentObjectStore()\n" << flush;
#endif

    if (NS)
    {
	// This is commented out as use_count is not used at database at present.
//	(void) NS->Release_Group(*group_uid);
	delete NS;
    }

    use_count--;

    if (use_count == 0)
    {
	if (RemotePersistentObjectStore::daemons)
	    delete RemotePersistentObjectStore::daemons;
    }

    if (replica_members)
	delete replica_members;

    if (StoreLocation)
	delete StoreLocation;

    undo();
    set_handler(old_handler); // make sure stub generator abort is turned back on.
    signal(SIGQUIT, SIG_DFL);
}

Boolean RemotePersistentObjectStore::rpc_handler () { return FALSE; }

void RemotePersistentObjectStore::storeLocation (const char* location)
{
    int size = MAXHOSTNAMELEN;

    if (location)
	size = strlen(location);

    StoreLocation = new char[size+1];
    memset(StoreLocation, '\0', size+1);

    if (location)
	memcpy(StoreLocation, location, size);
    else
	(void) gethostname(StoreLocation, MAXHOSTNAMELEN);
}


const TypeName RemotePersistentObjectStore::type () const
{
    return "RemotePersistentObjectStore";
}

const char* RemotePersistentObjectStore::getStoreName () const
{
    return storeName;
}

/*
 * Private functions
 */


Boolean RemotePersistentObjectStore::activateDaemon (ReplicaDescriptor* indx, Members* markern)
{
    ArjunaName AN2;
    UNIQUEID rUid;                                   // Uid for Rajdoot
    Boolean activated = FALSE, isPrimary = FALSE;
    Boolean tryAgain = FALSE;
    RemoteStateDaemon* touse = 0;
    int res;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean RemotePersistentObjectsStore::activateDaemon ()\n" << flush;
#endif

    if (indx->GetActive()) // replica is flagged as being available at the groupview database
    {
	if (!replica_members)
	    isPrimary = TRUE;

	do
	{
	    if (!tryAgain)
	    {
		if (indx->GetHost())
		    touse = RemotePersistentObjectStore::daemons->Get(indx->GetHost());
		else
		    error_stream << FATAL << "RemotePersistentObjectsStore::activateDaemon - null host name" << endl;
	    }

	    if ((!touse) || (tryAgain)) // statedaemon not currently registered, so create new one.
	    {
#ifdef DEBUG
		debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
		debug_stream << "RemotePersistentObjectStore::activateDaemon - trying to create StateDaemon\n" << flush;
#endif

		if (tryAgain)
		    tryAgain = FALSE;

		res = -1;

		rUid.host = rUid.sec = rUid.msec = STATEDAEMONUID;

		AN2.SetObjName("StateDaemon");
		AN2.SetServiceName("StateDaemon");

		if (indx->GetHost())
		    AN2.SetHostName(indx->GetHost());
		else
		    error_stream << FATAL << "RemotePersistentObjectsStore::activateDaemon - null host name" << endl;

		if (!replica_members)
		{
		    replica_members = new Members;
		    markern = replica_members;
		}
		else
		{
		    markern->next = new Members;
		    markern = markern->next;
		}

		markern->CRPC = new ClientRpc(&AN2);
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
#ifdef DEBUG
		markern->CRPC->SetTimeout(8000);
		markern->CRPC->SetRetry(5);
		markern->CRPC->SetObjectUid(rUid);
#else
		markern->CRPC->SetTimeout(2000);
		markern->CRPC->SetObjectUid(rUid);
#endif
#endif
		// turn off stub generated abort if call fails.

		old_handler = set_handler(RemotePersistentObjectStore::rpc_handler);
		markern->RPCCont = new RpcControl(markern->CRPC);
		markern->r_state_daemon = new RemoteStateDaemon(res, FALSE, markern->RPCCont);

		// turn stub generator abort back on for other servers.

		set_handler(old_handler);

		if (!res)  // statedaemon successfully activated
		{
		    markern->rd = indx;
		    RemotePersistentObjectStore::daemons->Put(markern, indx->GetHost());
		    activated = TRUE;

		    if (!primary_uid)
			primary_uid = new Uid(*indx->GetUid());
		}
		else
		{
		    if (primary_uid)
		    {
			delete primary_uid;
			primary_uid = 0;
		    }

		    replica_failure = TRUE;
		    indx->SetActive(FALSE); // flag replica is unavailable
		    markern = 0;

		    if (isPrimary)
		    {
			if (replica_members)
			{
			    delete replica_members;
			    replica_members = 0;
			}
		    }
		}
	    }
	    else  // statedaemon already registered so try to use it.
	    {
#ifdef DEBUG
		debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
		debug_stream << "RemotePersistentObjectStore::activateDaemon - trying old StateDaemon\n" << flush;
#endif

		Boolean IsAlive = FALSE;

		touse->AreYouAlive(IsAlive);
		if (IsAlive)
		{
		    if (!replica_members)
		    {
			replica_members = new Members;
			markern = replica_members;
		    }
		    else
		    {
			markern->next = new Members;
			markern = markern->next;
		    }

		    markern->r_state_daemon = touse;
		    markern->rd = indx;
		    activated = TRUE;

		    if (!primary_uid)
			primary_uid = new Uid(*indx->GetUid());
		}
		else
		{
#ifdef DEBUG
		    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
		    debug_stream << "RemotePersistentObjectStore::activateDaemon - old StateDaemon non-responsive\n" << flush;
#endif
		    markern = 0;
		    tryAgain = TRUE; // server may have failed, try once to recreate it.
		    RemotePersistentObjectStore::daemons->Delete(indx->GetHost());

		    if (isPrimary)
		    {
			if (replica_members)
			{
			    delete replica_members;
			    replica_members = 0;
			}
		    }

		    if (primary_uid)
		    {
			delete primary_uid;
			primary_uid = 0;
		    }
		}
	    }
	} while (tryAgain);
    }

    return activated;
}

Boolean RemotePersistentObjectStore::activate_primary ()
{
    ReplicaDescriptor* ptr = group_view;
    Boolean initiated = FALSE;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "RemotePersistentObjectStore::activate_primary ()\n" << flush;
#endif

    while ((!initiated) && (ptr != 0))
    {
	if (replica_members)
	{
	    delete replica_members;
	    replica_members = 0;
	}

	if (ptr->GetActive()) // replica is flagged as being available at the groupview database
	    initiated = activateDaemon(ptr, replica_members);

	if (!initiated)
	    ptr = ptr->next;
    }

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean RemotePersistentObjectStore::activate_primary () returning "
	<< (initiated == TRUE ? "TRUE" : "FALSE") << "\n" << flush;
#endif
    
    if (initiated)
	created_primary = TRUE;

    return initiated;
}
						 
Boolean RemotePersistentObjectStore::activate_backups ()
{
    Members *marker1 = replica_members, *marker2 = 0;
    ReplicaDescriptor* ptr = group_view;
    Boolean initiated;
#ifdef DEBUG
    int replica_number = 0;
#endif

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "RemotePersistentObjectStore::activate_backups ()\n" << flush;
#endif

    if (!group_view)
    {
	error_stream << WARNING << "Attempt to active backups without contacting database first.\n" << flush;
	return FALSE;
    }
    else
	created_backups = TRUE;

    while (ptr != 0)
    {
#ifdef DEBUG
	replica_number++;
	debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
	debug_stream << "RemotePersistentObjectStore - Trying replica number " << replica_number << "\n" << flush;
#endif

	if (ptr->GetActive())
	{
	    if (*primary_uid != *ptr->GetUid())
	    {
		marker2 = marker1;
		initiated = activateDaemon(ptr, marker2);
		if (initiated)
		{
#ifdef DEBUG
		    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
		    debug_stream << "RemotePersistentObjectStore - successfully activated backup\n" << flush;
#endif
		    marker1 = marker1->next;
		}
#ifdef DEBUG
		else
		{
		    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
		    debug_stream << "RemotePersistentObjectStore - could not activate backup\n" << flush;
		}
#endif
	    }
	}

	ptr = ptr->next;
    }

    return TRUE;
}


/*
 * Public non-virtual functions
 */

Boolean RemotePersistentObjectStore::createNewObject (const Uid u)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean RemotePersistentObjectStore::createNewObject (const Uid u)\n" << flush;
#endif

    if (group_view)
	delete group_view;
    
    if (!StoreLocation)
    {
	char place[MAXHOSTNAMELEN];
	::memset(place, '\0', MAXHOSTNAMELEN);
	(void) gethostname(place, MAXHOSTNAMELEN);
	group_view = new ReplicaDescriptor(place);

#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
	debug_stream << "RemotePersistentObjectStore - Object store location is " << place << "\n" << flush;
#endif
    }
    else
    {
	group_view = new ReplicaDescriptor(StoreLocation);
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
	debug_stream << "RemotePersistentObjectStore - Object store location is " << StoreLocation << "\n" << flush;
#endif
    }

    group_view->SetUid(u);
    group_view->SetActive(TRUE);

    if (!activate_primary())
    {
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
	debug_stream << "Could not activate StateDaemon\n" << flush;
#endif
	undo();
	storeValid = FALSE;
	return FALSE;
    }

    return TRUE;
}

Boolean RemotePersistentObjectStore::create_groupview (const Uid u, const TypeName tn)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "RemotePersistentObjectStore::create_groupview (" << u << ", " << tn << ")\n" << flush;
#endif

    if ((group_failure) || (!storeValid))
	return FALSE;

    // Since StateDaemons are created using ClRajAct if the action within which they were created
    // aborts then they will be terminated, but created_primary will not be altered (RemotePersistentObjectStore
    // is not a persistent object). Thus we must check that the primary has not been terminated.
    // If it has then we will re-create it.

    if (created_primary)
    {
	if (replica_members->CRPC->GetServerStatus() == DEAD)
	{
#ifdef DEBUG
		error_stream << WARNING << "RemotePersistentObjectStore - An action has previously aborted and terminated StateDaemons.\n" << flush;
		error_stream << WARNING << "RemotePersistentObjectStore - Will have to recreate them\n" << flush;
#endif
		// should do this for entire group.
		RemotePersistentObjectStore::daemons->Delete(replica_members->rd->GetHost());
		created_primary = FALSE;
	}
    }

    if (!created_primary)
    {
	int res;
	Boolean new_object;

	// Since the use of a remote statedaemon requires contacting the groupview database within
	// atomic actions this cannot be done unless the action is RUNNING. If this is a new
	// persistent object then this routine will be called when the action is PREPARING and so
	// the resulting object state must be saved in the local objectstore.
	// Note that since StateDaemons are accessed as standard Arjuna applications, they go via
	// ClientActionRPC, and this means that we get some warnings during commit time. Since StateDaemons
	// possess no state as such this is ok, (but should be fixed eventually).

	new_object = (Boolean) (AtomicAction::Current() == 0);
	if (!new_object)
	    new_object = (Boolean) (AtomicAction::Current()->status() == PREPARING);

	if (new_object)
	    return createNewObject(u);

	Boolean opcode = FALSE;
	AtomicAction A, B;

// Nameserver should be created (and deleted) within the scope of a nested top-level
// atomic action.

	if (!NS)
	{
	    B.Begin();

	    res = -1;

	    NS = new NSInterface(res);

	    if (res != 0)
	    {
		error_stream << WARNING << "RemotePersistentObjectStore - Could not create groupview database\n" << flush;
		B.Abort();
		undo();
		storeValid = FALSE;
		return FALSE;
	    }

	    res = -1;
	    
	    if (B.End() != COMMITTED)
	    {
		undo();
		return FALSE;
	    }
	}

	group_view = new ReplicaDescriptor(0); // blank descriptor as database will fill it in
	group_view->SetUid(u);
	group_uid = new Uid(u);

	A.Begin(); // nested top-level atomic action
	
	opcode = NS->Get_View(*group_view, u);

	if (opcode)
	{
	    if (!activate_primary())
	    {
		error_stream << WARNING << "Could not create primary. All group members have not responded.\n" << flush;

		A.Abort();
		group_failure = TRUE;
		undo();
		storeValid = FALSE;
		return FALSE;
	    }
	    else
	    {
		(void) activate_backups();

		if (replica_failure)
		{
		    AtomicAction Z;
		    Boolean result_code = FALSE;
		    
		    Z.Begin();

		    result_code = NS->ChangeReplicaStatus(*group_view, u, FALSE);
		    
		    if (result_code)
		    {
			if (Z.End() != COMMITTED)
			{
			    A.Abort();
			    undo();
			    storeValid = FALSE;
			    return FALSE;
			}
		    }
		    else
		    {
			// Could not exclude out-of-date replicas from database
			Z.Abort(); // must abort or inconsistencies will result with different group views
			A.Abort();
			storeValid = FALSE;
			undo();
			return FALSE;
		    }
		}
	    }
	}
	else
	{
	    if (!createNewObject(u))
	    {
		A.Abort();
		undo();
		storeValid = FALSE;
		return FALSE;
	    }
	}

	if (A.End() != COMMITTED)
	{
	    undo();
	    storeValid = FALSE;
	    return FALSE;
	}
	else
	    return TRUE;
    }
    else
	return TRUE;
}

/* RemotePersistentObjectStore is not persistent but some of its state must be undone if a failure occurs
 * which would cause the action to abort. This is so that if the action is retried then
 * it appears as though the RemotePersistentObjectStore was untouched by the last action to use it.
 */

void RemotePersistentObjectStore::undo ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "void RemotePersistentObjectStore::undo ()\n" << flush;
#endif

    if (group_view)
    {
	delete group_view;
	group_view = 0;
    }

    if (primary_uid)
    {
	delete primary_uid;
	primary_uid = 0;
    }

    created_primary = FALSE;
    created_backups = FALSE;
    replica_failure = FALSE;
    group_failure = FALSE;
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
    Buffer buff;
    ObjectState* objstate;
    int res = -1;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::all_objuids ( " << tName << " )\n" << flush;
#endif

    if (!created_primary)
    {
	error_stream << WARNING << "Attempt to use RemotePersistentObjectStore operation without assigned UID\n" << flush;
	return 0;
    }

    old_handler = set_handler(RemotePersistentObjectStore::rpc_handler);
    buff = replica_members->r_state_daemon->AllObjUids(tName, res);
    set_handler(old_handler);

    if (res == 0)
    {
	objstate = new ObjectState;
	if (!objstate->unpackFrom(buff))
	    return 0;
	else
	    return objstate;
    }
    else
    {
	RemotePersistentObjectStore::daemons->Delete(replica_members->rd->GetHost());
	undo();
	return 0;
    }
}

/*
 * The following operation commits a previous write_state operation which
 * was made with the SHADOW FileType argument. This is achieved by
 * renaming the shadow and removing the hidden version.
 */

Boolean RemotePersistentObjectStore::commit_state ( const Uid& objUid, const TypeName tName )
{
    int res = -1;
    Boolean result, success = FALSE;
    Members* markery;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::commit_state ( " << objUid << ", " << tName << " )\n" << flush;
#endif

    if (!create_groupview(objUid, tName))
	return FALSE;

    markery = replica_members;

    while (markery != 0)
    {
	res = -1;
	old_handler = set_handler(RemotePersistentObjectStore::rpc_handler);
	result = markery->r_state_daemon->CommitState(*markery->rd->GetUid(), tName, res);
	set_handler(old_handler);

	if (res != 0)
	{
	    RemotePersistentObjectStore::daemons->Delete(markery->rd->GetHost());
	    undo();
	    return FALSE;
	}
	else
	    success = TRUE;

	markery = markery->next;
    }

    if (success)
	return result;
    else
    {
	undo();
	return FALSE;
    }
}

/*
 * Remove the ObjectState/file from the object store.
 */

Boolean RemotePersistentObjectStore::remove_state ( const Uid& objUid, const TypeName name)
{
    Boolean result = TRUE;
    int res = -1;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::remove_state ( " << objUid << ", " << name << " )\n" << flush;
#endif

    if (!create_groupview(objUid, name))
	return FALSE;

    Members* markery = replica_members;

    while (markery != 0)
    {
	res = -1;
	old_handler = set_handler(RemotePersistentObjectStore::rpc_handler);
	(void) markery->r_state_daemon->RemoveState(*markery->rd->GetUid(), name, res);
	set_handler(old_handler);

	if (res == 0)
	    result = TRUE;
	else
	    RemotePersistentObjectStore::daemons->Delete(markery->rd->GetHost());

	markery = markery->next;
    }

    return result;
}

Boolean RemotePersistentObjectStore::remove_uncommitted (const Uid& storeUid, const TypeName tName)
{
    Boolean result = TRUE;
    int res = -1;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::remove_uncommitted ( " << storeUid << ", " << tName << " )\n" << flush;
#endif

    if (!create_groupview(storeUid, tName))
	return FALSE;

    Members* markery = replica_members;

    while (markery != 0)
    {
	res = -1;
	old_handler = set_handler(RemotePersistentObjectStore::rpc_handler);
	(void) markery->r_state_daemon->RemoveUnCommitted(*markery->rd->GetUid(), tName, res);
	set_handler(old_handler);

	if (res == 0)
	    result = TRUE;
	else
	    RemotePersistentObjectStore::daemons->Delete(markery->rd->GetHost());

	markery = markery->next;
    }

    return result;
}

/*
 * read an  uncommitted instance of ObjectState out of the object store. 
 * The instance is identified by the unique id and type
 */

ObjectState* RemotePersistentObjectStore::read_uncommitted (const Uid& storeUid, const TypeName tName)
{
    Buffer buff;
    int res = -1;
    ObjectState* os;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::read_uncommitted ( " << storeUid << ", " << tName << " )\n" << flush;
#endif

    if (!create_groupview(storeUid, tName))
	return 0;

    old_handler = set_handler(RemotePersistentObjectStore::rpc_handler);
    buff = replica_members->r_state_daemon->ReadUnCommitted(*primary_uid, tName, res);
    set_handler(old_handler);

    if (res == 0)
    {
	os = new ObjectState;
	os->unpackFrom(buff);
	return os;
    }
    else
    {
	RemotePersistentObjectStore::daemons->Delete(replica_members->rd->GetHost());
	undo();
	return 0;
    }
}

/*
* read a committed instance of ObjectState out of the object store. The instance
 * is identified by the unique id and type
 */

ObjectState* RemotePersistentObjectStore::read_state ( const Uid& objUid, const TypeName tName)
{
    Buffer buff;
    int res = -1;
    ObjectState* os;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::read_state ( " << objUid << ", " << tName << " )\n" << flush;
#endif

    if (!create_groupview(objUid, tName))
	return 0;

    Boolean ok = FALSE;
    Members* markery = replica_members;
	
    while ((markery != 0) && (!ok))
    {
	res = -1;
	old_handler = set_handler(RemotePersistentObjectStore::rpc_handler);
	buff = markery->r_state_daemon->ReadState(*markery->rd->GetUid(), tName, res);
	set_handler(old_handler);

	if (res == 0)
	    ok = TRUE;
	else
	{
	    RemotePersistentObjectStore::daemons->Delete(markery->rd->GetHost());
	    markery = markery->next;
	}
    }

    if (res == 0)
    {
	os = new ObjectState;
	os->unpackFrom(buff);
	return os;
    }
    else
    {
	undo();
	return 0;
    }
}

Boolean RemotePersistentObjectStore::write_committed (const Uid& objUid, const TypeName tName,
						      const ObjectState& state)
{
    Buffer buff;
    int res = -1;
    Boolean result, success = FALSE;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::write_committed ( " << objUid << ", " << tName
	         << ", " << state << " )\n" << flush;
#endif

    if (!create_groupview(objUid, tName))
	return FALSE;

    state.packInto(buff);

    Members* markery = replica_members;

    while (markery != 0)
    {
	res = -1;
	old_handler = set_handler(RemotePersistentObjectStore::rpc_handler);
	result = markery->r_state_daemon->WriteCommitted(*markery->rd->GetUid(), tName, buff, res);
	set_handler(old_handler);

	if (res != 0)
	{
	    RemotePersistentObjectStore::daemons->Delete(markery->rd->GetHost());
	    undo();
	    return FALSE;
	}
	else
	    success = TRUE;

	markery = markery->next;
    }

    if (success)
	return result;
    else
	return FALSE;
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
    Buffer buff;
    int res = -1;
    Boolean result, success = FALSE;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore::write_state ( " << storeUid << ", "
	         << tName << ", " << state << " )\n" << flush;
#endif

    if (!create_groupview(storeUid, tName))
	return FALSE;

    state.packInto(buff);

    Members* markery = replica_members;

    while (markery != 0)
    {
	res = -1;
	old_handler = set_handler(RemotePersistentObjectStore::rpc_handler);
	result = markery->r_state_daemon->WriteState(*markery->rd->GetUid(), tName, buff, res);
	set_handler(old_handler);

	// Assumes that non-responsive nodes have failed. Recovery daemon will bring them up-to-date.
	// if (res == 0) success = TRUE;

	if (res != 0)
	{
	    RemotePersistentObjectStore::daemons->Delete(markery->rd->GetHost());
	    undo();
	    return FALSE; // abort operation and action and try again later with new groupview.
	}
	else
	    success = TRUE;
	
	markery = markery->next;
    }

    if (success)
	return result;
    else
	return FALSE;
}
