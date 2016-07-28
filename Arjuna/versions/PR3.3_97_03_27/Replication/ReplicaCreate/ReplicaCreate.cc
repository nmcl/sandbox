/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ReplicaCreate.cc,v 1.19 1994/10/10 11:13:47 ngdp Exp $
 */

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef RPOBJSTORE_H_
#  include <ArjServers/RPObjStore_stub.h>
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

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef REGISTER_H_
#  include <Replication/ReplicaCreate.h>
#endif

#ifndef REPDESC_H_
#  include <ArjServers/RepDesc_stub.h>
#endif

#ifndef GROUPDATA_H_
#  include <ArjServers/GroupData.h>
#endif

#ifndef NSINTERFACE_H_
#  include <ArjServers/NSInterface.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef NAMINGR_H_
#  include "NamingR.h"
#endif


StateData::StateData ()
                      : RpcClient(0),
		        RpcMan(0),
                        SD(0),
                        hostName(0),
			next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "StateData::StateData ()" << endl;
#endif
}

StateData::~StateData ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "StateData::~StateData ()" << endl;
#endif

    if (SD)
    {
	delete SD;
	SD = (RemoteRPObjStore*) 0;
	if (RpcMan)
	    delete RpcMan;
	if (RpcClient)
	    delete RpcClient;
    }
    if (next)
	delete next;
    if (hostName)
#ifndef GCC_ARRAY_DELETE_BUG
        ::delete [] hostName;
#else
        ::delete hostName;
#endif
}


ReplicaCreate::ReplicaCreate (const Uid& gUid, TypeName tName)
                             : register_uid(new Uid(gUid)),
                               tn(tName),
                               head(0),
                               rd(0),
			       endOfList(0),
                               GV(0),
                               doRelease(FALSE),
			       copyFrom(FALSE),
			       groupSize(0),
			       os(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "ReplicaCreate::ReplicaCreate ( " << gUid << " , " << tName << " )" << endl;
#endif
}

ReplicaCreate::ReplicaCreate ()
		              : register_uid(0),
                                tn(0),
		                head(0),
                                rd(0),
				endOfList(0),
                                GV(0),
                                doRelease(FALSE),
				copyFrom(FALSE),
				groupSize(0),
				os(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "ReplicaCreate::ReplicaCreate ()" << endl;
#endif
}

ReplicaCreate::~ReplicaCreate ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "ReplicaCreate::~ReplicaCreate ()" << endl;
#endif

    if (needToRelease())
        (void) releaseGroup();
    if (register_uid)
	delete register_uid;
    if (head)
        delete head;
    if (rd)
        delete rd;
    if (GV)
        delete GV;
    if (os)
	delete os;
}

RemoteRPObjStore* ReplicaCreate::createDaemon (const char* name)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "RemoteRPObjStore* ReplicaCreate::createDaemon ( " << name << " )" << endl;
#endif

    int res = -1;
    StateData* temp = head;

    /* Has this daemon been created already? */

    while (temp)
    {
        if (::strcmp(temp->hostName, name) == 0)
	    return temp->SD;
	else
	    temp = temp->next;
    }

    /* Create new daemon. */

#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    UNIQUEID rUid;
    rUid.host = rUid.sec = rUid.msec = RPOBJSTOREUID;
#endif

    temp = new StateData;
    temp->RpcClient = new ClientRpc;
    temp->RpcClient->setServiceName("RPObjStore");
    temp->RpcClient->setHost((char*) name);
    temp->RpcClient->disableAtomic();
    
#if (RPC_SYSTEM_TYPE == RPC_RajdootAction_) || (RPC_SYSTEM_TYPE == RPC_Radjoot_)
    temp->RpcClient->setTimeout(3000);
    temp->RpcClient->setRetry(1);
    temp->RpcClient->setObjectUid(rUid);
#endif
    
    temp->RpcMan = new ClientRpcManager(temp->RpcClient);
    temp->SD = new RemoteRPObjStore(res, temp->RpcMan);
    temp->hostName = ::new char[::strlen(name)+1];
    ::memset(temp->hostName, '\0', ::strlen(name)+1);
    ::memcpy(temp->hostName, name, ::strlen(name));

    if (res == 0)
    {
        temp->next = head;
	head = temp;
        return temp->SD;
    }

    delete temp;
    return (RemoteRPObjStore*) 0;
}

StoreState ReplicaCreate::readObjectState (ReplicaDescriptor* marker, ObjectState*& buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "StoreState ReplicaCreate::readObjectState ()\n" << flush;
#endif

    RemoteRPObjStore* SD = createDaemon(marker->getHost());
    
    if (SD)
    {
        int res = -2;

        marker->setActive(TRUE);
	if (marker->isNullType())
	    marker->setObjectName(tn);
	if (marker->getUid() == NIL_UID)
	    marker->setUid(*register_uid);
	if (marker->getGroupUid() == NIL_UID)
	    marker->setGroupUid(*register_uid);
	if (marker->getStoreRoot() == (char*) 0)
	    marker->setStoreRoot(objectStoreRoot);
	
	buff = SD->read_committed(marker->getUid(), (const TypeName) marker->getObjectName(), marker->getStoreRoot(), res);
	
	if (res == 0)
	    return OK;
	if (res == -2)
	    return NOTPRESENT;
    }

    marker->setActive(FALSE);
    return UNKNOWN;
}

StoreState ReplicaCreate::writeObjectState (ReplicaDescriptor* ptr, const ObjectState& buff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "StoreState ReplicaCreate::writeObjectState ()" << endl;
#endif

    Boolean outCome = FALSE, done = FALSE;
    RemoteRPObjStore* SD = (RemoteRPObjStore*) 0;
    int res = -1;
    Uid* Y = (Uid*) 0;
 
    while (ptr != (ReplicaDescriptor*) 0)
    {
        if (Y)
	    delete Y;
	Y = new Uid;

	// if we do not have a uid to write to then create one.

	if (ptr->getUid() == NIL_UID)
	    ptr->setUid(*Y);

	// likewise we may need to set the group uid.

	if (ptr->getGroupUid() == NIL_UID)
	    ptr->setGroupUid(*register_uid);

	if (ptr->getStoreRoot() == (char*) 0)
	    ptr->setStoreRoot(objectStoreRoot);

        ptr->setObjectName(tn);

        SD = createDaemon(ptr->getHost());
        if (SD)
	{
	    // write the state to the new uid.

	    done = SD->write_committed(ptr->getUid(), tn, buff, ptr->getStoreRoot(), res);

	    if ((res == 0) && (done))
	    {
	        outCome = TRUE;
		groupSize++;
	    }
	}

	/*
	 * If we could not create an appropriate daemon or write the state
	 * then we must reflect this fact at the group view.
	 */

	if (!done)
	    ptr->setActive(FALSE);

	ptr = ptr->next;
	done = FALSE;
	res = -1;
    }

    if (Y)
        delete Y;

    return ((outCome == TRUE) ? OK : UNKNOWN);
}

Boolean ReplicaCreate::addReplica (const char* hostName, const Uid& replicaUid,
				   Boolean isOriginal)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaCreate::addReplica ( " << hostName << ", "
		 << replicaUid << " )" << endl;
#endif

    if (!hostName)
	return FALSE;

    // can only have one original replica in list currently.
    
    if ((copyFrom) && (isOriginal))
	return FALSE;
    
    ReplicaDescriptor* tmpRD = new ReplicaDescriptor;
    Uid* u = ((replicaUid == NIL_UID) ? new Uid : new Uid(replicaUid));
    
    tmpRD->setHost(hostName);
    tmpRD->setUid(*u);

    if (isOriginal)
    {
        // make this the head of the list.

	tmpRD->next = rd;
	rd = tmpRD;

	// if state not given then read from replica.

	if (os == (ObjectState*) 0)
	    copyFrom = TRUE;

	if (endOfList == (ReplicaDescriptor*) 0)
	    endOfList = tmpRD;
    }
    else
    {
        // all other replicas have to be created and go at end of list.

	if (endOfList)
	    endOfList->next = tmpRD;
	else
	    rd = tmpRD;

	endOfList = tmpRD;
    }

    if (u)
	delete u;
    
    return TRUE;
}
    
Boolean ReplicaCreate::deleteReplica (const char* hostName, const Uid& replicaUid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaCreate::deleteReplica ( " << hostName << ", " << replicaUid << " )" << endl;
#endif

    if ((!hostName) || (!rd))
	return FALSE;

    ReplicaDescriptor *tmpRD = rd, *trail = (ReplicaDescriptor*) 0;
    Boolean found = FALSE;

    while ((!found) && (tmpRD))
    {
        if (::strcmp(tmpRD->getHost(), hostName) == 0)
	{
	    Boolean doDelete = FALSE;
	    
	    if (replicaUid == NIL_UID)
		doDelete = TRUE;
	    else
		doDelete = (Boolean) (replicaUid == tmpRD->getUid());

	    if (doDelete)
	    {
		if (trail == (ReplicaDescriptor*) 0)
		    rd = tmpRD->next;
		else
		    trail->next = tmpRD->next;

		tmpRD->next = (ReplicaDescriptor*) 0;
		found = TRUE;
		delete tmpRD;
	    }
	}
	else
	{
	    trail = tmpRD;
	    tmpRD = tmpRD->next;
	}
    }

    return found;
}
    
const ReplicaDescriptor& ReplicaCreate::getView () const
{
    if (rd)
	return *rd;
    else
	return NIL_RD;
}

StoreState ReplicaCreate::createReplicas (unsigned long minimumSize)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "StoreState ReplicaCreate::createReplicas ( " << minimumSize << " )" << endl;
#endif

    ObjectState* buff = (ObjectState*) 0;
    StoreState result = UNKNOWN;
    
    if ((register_uid == (Uid*) 0) || (tn == 0))
	return UNKNOWN;

    /*
     * If we have not been given a state to copy from then return FALSE.
     * This state can either be given directly, or we can try to copy it
     * from a remote object.
     */

    if ((copyFrom == FALSE) && (os == (ObjectState*) 0))
	return UNKNOWN;

    if (rd == (ReplicaDescriptor*) 0)
	return UNKNOWN;

    groupSize = 1;

    // if given state then use it, otherwise try to read from remote object.

    if (copyFrom)
    {
	if (rd->next)
	    result = readObjectState(rd, buff);
    }
    else
    {
	buff = os;
	result = OK;
    }
    
    if (result == OK)
	result = writeObjectState(rd->next, *buff);

    if (groupSize < minimumSize)
	result = SIZEERROR;
    
    return result;
}

StoreState ReplicaCreate::createReplicas (const ReplicaDescriptor& toUse, unsigned long minimumSize)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "StoreState ReplicaCreate::createReplicas ( " << minimumSize << " )\n" << flush;
#endif

    StoreState result = UNKNOWN;
    ReplicaDescriptor* ptr = rd;

    // we don't want to modify the rd passed from the caller.
    
    rd = (ReplicaDescriptor*) &toUse;
    copyFrom = TRUE;
    result = createReplicas(minimumSize);
    rd = ptr;

    return result;
}

void ReplicaCreate::reset ()
{
    if (register_uid)
    {
        delete register_uid;
	register_uid = (Uid*) 0;
    }
    if (rd)
    {
        delete rd;
	rd = (ReplicaDescriptor*) 0;
    }
    if (os)
    {
	delete os;
	os = (ObjectState*) 0;
    }
    endOfList = (ReplicaDescriptor*) 0;
    copyFrom = FALSE;
    tn = 0;
    groupSize = 0;
}

void ReplicaCreate::setObjectInfo (const Uid& u, TypeName t, const char* root)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "void ReplicaCreate::setObjectInfo ( " << u << ", " << t << " )" << endl;
#endif

    if (register_uid)
        delete register_uid;
    register_uid = new Uid(u);
    tn = t;
    objectStoreRoot = root;
}

void ReplicaCreate::setState (ObjectState* state)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "void ReplicaCreate::setState (ObjectState* state)" << endl;
#endif

    if (os)
	delete os;

    os = ((state == (ObjectState*) 0) ? (ObjectState*) 0 : new ObjectState(*state));

    // since we have been given state we will not copy it from replica.

    copyFrom = FALSE;
}

Boolean ReplicaCreate::activateDatabase ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "Boolean ReplicaCreate::activateDatabase ()" << endl;
#endif

    Boolean result = FALSE;
    int res = -1;

    if (GV)
        result = TRUE;
    else
    {
        GV = new NSInterface(res);
	if (res == 0)
	    result = TRUE;
	else
	{
	    delete GV;
	    GV = (NSInterface*) 0;
	}
    }

    return result;
}

Boolean ReplicaCreate::releaseGroup ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PROTECTED;
    debug_stream << "Boolean ReplicaCreate::releaseGroup ()" << endl;
#endif
    
    Boolean result = FALSE;

    if ((doRelease) && (register_uid) && (GV))
        return FALSE;

    result = (GV->releaseGroup(*register_uid, NSInterface::nodeName, NSInterface::processID) == DONE);

    if (!result)
        error_stream << WARNING << "ReplicaCreate::releaseGroup - could not release group " << *register_uid << endl;
    else
        doRelease = FALSE;

    return result;
}

Boolean ReplicaCreate::getReplicaInformation (LockMode lMode)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "Boolean ReplicaCreate::getReplicaInformation ( " << ((lMode == READ) ? "READ" : "WRITE") << " )" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;

    if (!register_uid)
        return FALSE;

    A.Begin();

    if (activateDatabase())
    {
        // try to lock group first.
        result = GV->lockGroup(*register_uid, NSInterface::nodeName, NSInterface::processID, lMode);

	// if we locked group then try to get group view.
	if (result)
	    result = GV->getView(*rd, *register_uid, NSInterface::nodeName, NSInterface::processID);
    }

    if (result)
    {
        doRelease = TRUE;
	NamingRecord* nr = new NamingRecord(*register_uid, tn, NSInterface::processID, (char*) NSInterface::nodeName);
	if (A.add(nr) != AtomicAction::ADDED)
	    delete nr;

        if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
        A.Abort();

    if (!result)
    {
        if (rd)
	{
	    delete rd;
	    rd = (ReplicaDescriptor*) 0;
	}
    }

    return result;
}

/*
 * To be able to modify the group view it is necessary to have locked the group
 * view at the database in an exclusive way to guarantee no inconsistencies can
 * be seen by other users.
 */

Boolean ReplicaCreate::addToGroup (const char* location, const Uid& replicaUid, LockMode lMode, Boolean isTemplate)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PROTECTED;
    debug_stream << "Boolean ReplicaCreate::addToGroup ( " << location << ", " << replicaUid << " )" << endl;
#endif

    Boolean result = FALSE;
    
    if (!location)
	return FALSE;

    result = ((rd == (ReplicaDescriptor*) 0) ? getReplicaInformation(lMode) : TRUE);

    if (result)
        result = addToGroup(rd, location, replicaUid, isTemplate);

    return result;
}

/*
 * Copy a state from group "from" to all members mentioned in group "to". All members
 * in "to" must be written to or the operation terminates. Also, there must be at
 * least one available copy of the state to copy for the operation to work.
 * [Could present some problems for template group. Change later.]
 */

Boolean ReplicaCreate::copyGroup (const ReplicaDescriptor& from, const GroupData& to)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaCreate::copyGroup ()" << endl;
#endif

    const GroupData* scanner = &to;

    if (rd)
    {
        delete rd;
	rd = (ReplicaDescriptor*) 0;
    }

    if (register_uid)
        delete register_uid;
    register_uid = new Uid(from.getGroupUid());

    tn = (char*) from.getObjectName();

    while (scanner)
    {
        while ((scanner) && (rd == (ReplicaDescriptor*) 0))
	{
	    if (scanner->getActive())
	        rd = new ReplicaDescriptor(scanner);
	    else
	        scanner = scanner->next;
	}
	
	if (rd)
	{
	    rd->next = (ReplicaDescriptor*) &from;
	    if (createReplicas() == OK)
	        return TRUE;

	    rd->next = (ReplicaDescriptor*) 0;
	    delete rd;
	    rd = (ReplicaDescriptor*) 0;
	    scanner = scanner->next;
	}
	else
	    return FALSE;
    }

    return FALSE;
}
	
Boolean ReplicaCreate::addToGroup (ReplicaDescriptor*& groupView, const char* location,
				   const Uid& replicaUid, Boolean isTemplate)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaCreate::addToGroup ( " << location << ", "
		 << replicaUid << " )" << endl;
#endif

    StoreState result = UNKNOWN;
    ReplicaDescriptor* trail = groupView;
    Uid* u = ((replicaUid == NIL_UID) ? new Uid : new Uid(replicaUid));

    if ((groupView) && (location))
    {
        if (register_uid)
	    delete register_uid;
	register_uid = new Uid(groupView->getUid());
	tn = (char*) groupView->getObjectName();
    }
    else
        return FALSE;
        
    while ((result != OK) && (trail != (ReplicaDescriptor*) 0))
    {
        if (rd)
	    delete rd;
	rd = new ReplicaDescriptor;

	rd->setGroupUid(trail->getGroupUid());
	rd->setUid(trail->getUid());
	rd->setHost(trail->getHost());
	rd->setObjectName(trail->getObjectName());
	rd->setStoreRoot(trail->getStoreRoot());
	rd->next = new ReplicaDescriptor;
	rd->next->setGroupUid(trail->getGroupUid());
	rd->next->setUid(*u);
	rd->next->setHost(location);
	rd->next->setObjectName(trail->getObjectName());  // should have same type anyway
	rd->next->setStoreRoot(trail->getStoreRoot());   // should also have same relative objstore

	result = createReplicas();
	if (result == UNKNOWN)
	    trail = trail->next;
	else
	{
	    /*
	     * If this is a template group then it is expected that some members may not
	     * exist yet, i.e., there may be no state to replicate. However, modification
	     * of the template should still be allowed. We are essentially modifying the
	     * template and not necessarily the members to which it refers.
	     */

	    if ((result == NOTPRESENT) && (isTemplate))
		result = OK;
	}
    }

    if (result == OK)
    {
        rd->next->next = groupView;
	groupView = rd->next;
	rd->next = (ReplicaDescriptor*) 0;
	delete rd;
    }

    if (u)
	delete u;
    
    return ((result == OK) ? TRUE : FALSE);
}

Boolean ReplicaCreate::removeFromGroup (const char* location, const Uid& replicaUid, LockMode lMode)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PROTECTED;
    debug_stream << "Boolean ReplicaCreate::removeFromGroup ( " << location << " )" << endl;
#endif

    Boolean result = FALSE;

    result = ((rd == (ReplicaDescriptor*) 0) ? getReplicaInformation(lMode) : TRUE);

    if (result)
        result = removeFromGroup(rd, location, replicaUid);

    return result;
}

Boolean ReplicaCreate::removeFromGroup (ReplicaDescriptor*& groupView, const char* location, const Uid& replicaUid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Boolean ReplicaCreate::removeFromGroup ( " << location << ", "
		 << replicaUid << " )" << endl;
#endif

    Boolean found = FALSE;
    ReplicaDescriptor *trail = groupView, *marker = (ReplicaDescriptor*) 0;

    if ((groupView == (ReplicaDescriptor*) 0) || (location == (char*) 0))
        return FALSE;

    while ((!found) && (trail))
    {
        if (::strcmp(trail->getHost(), location) == 0)
	{
	    if (replicaUid == NIL_UID)
		found = TRUE;
	    else
		found = (Boolean) (replicaUid == trail->getUid());
	}
	else
	{
	    marker = trail;
	    trail = trail->next;
	}
    }

    if (!found)
        return FALSE;

    /*
     * We don't really need to delete the state as the fact that the replica is
     * to be removed from the db will mean the state becomes inaccessible.
     * Also, if the user decides to abort the action we must have the state to go
     * back to.
     */

    if (marker == (ReplicaDescriptor*) 0)
        groupView = groupView->next;
    else
        marker->next = trail->next;

    trail->next = (ReplicaDescriptor*) 0;
    delete trail;

    return TRUE;
}


#ifdef NO_INLINES
#  define REPCREATE_CC_
#  include <Replication/ReplicaCreate.n>
#  undef REPCREATE_CC_
#endif
