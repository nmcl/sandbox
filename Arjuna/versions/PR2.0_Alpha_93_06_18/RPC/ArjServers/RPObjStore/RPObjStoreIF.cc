/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RPObjStoreIF.cc,v 
 */

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef RPOBJSTOREIF_H_
#  include <RPC/ArjServers/RPObjStoreIF.h>
#endif

#ifndef RPOBJSTORE_H_
#  include <RPC/ArjServers/RPObjStore.h>
#endif

#ifndef REPDESC_H_
#  include <RPC/ArjServers/RepDesc_stub.h>
#endif


/*
 * This class handles most of the complex object replication work. It is
 * also a preliminary test of the work for abstract interfaces and concrete
 * implementations.
 * Caching is achieved here and uses a write-through cache: if a replica failure
 * is detected during a read operation then it is "ignored", otherwise a new
 * view must be assembled.
 */

RPObjStoreList* RPObjStoreInterface::headOfList = 0;
RPObjStoreList* RPObjStoreInterface::tailOfList = 0;



RPObjStoreList::RPObjStoreList ()
                                : instance(new RemotePersistentObjectStore),
				  derivedFrom(0),
                                  useCount(1),
                                  next(0),
				  prev(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RPObjStoreList::RPObjStoreList ()" << endl;
#endif
}

RPObjStoreList::~RPObjStoreList ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RPObjStoreList::~RPObjStoreList ()" << endl;
#endif

    if (instance)
        delete instance;
}

ReplicaDescriptor* RPObjStoreList::getCachedView (const Uid& u, RPObjStoreList*& rpPtr) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor* RPObjStoreList::getCachedView ( " << u << " ) const" << endl;
#endif

    RPObjStoreList *indx = 0;
    ReplicaDescriptor *rdPtr = 0;
    Boolean found = FALSE;

    // search from tail of list.

    rpPtr = RPObjStoreInterface::tailOfList;
    
    while ((rpPtr) && (!found))
    {
	if (rpPtr->instance)
	    rdPtr = rpPtr->instance->getCompositeView(u);
	    
	if (rdPtr)
	    found = TRUE;
	else
	{
	    indx = rpPtr;
	    rpPtr = rpPtr->prev;
	}
    }

    return rdPtr;
}
    
RemotePersistentObjectStore* RPObjStoreList::getStore (const Uid& u, const TypeName tn)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RemotePersistentObjectStore* RPObjStoreList::getStore ( " << u << ", " << tn << " )" << endl;
#endif

    RPObjStoreList *rpPtr = 0, *indx = new RPObjStoreList;
    ReplicaDescriptor* rdPtr = getCachedView(u, rpPtr);
    Boolean found = FALSE;

    // add to head of list.

    indx->next = RPObjStoreInterface::headOfList;
    RPObjStoreInterface::headOfList->prev = indx;
    RPObjStoreInterface::headOfList = indx;
	
    if (rdPtr)
    {
	rdPtr->testAndSetObjectNames(tn);

	// rpPtr must be set since rdPtr != 0

	if (rpPtr->derivedFrom != 0)
	    indx->derivedFrom = rpPtr->derivedFrom;
	else
	    indx->derivedFrom = rpPtr;

	indx->derivedFrom->useCount++;
	indx->instance->myData.group_view = new ReplicaDescriptor;
	*indx->instance->myData.group_view = *rdPtr;

	delete rdPtr;
    }
	
    return indx->instance;
}    

void RPObjStoreList::removeStore (RemotePersistentObjectStore*& toRemove)
{
    RPObjStoreList *rpPtr = RPObjStoreInterface::headOfList, *indx = 0;
    Boolean found = FALSE;

    if (!toRemove)
        return;

    while ((rpPtr) && (!found))
    {
        if (rpPtr->instance == toRemove)
	    found = TRUE;
	else
	{
	    indx = rpPtr;
	    rpPtr = rpPtr->next;
	}
    }

    if (!found)
    {
#ifdef DEBUG
        error_stream << WARNING << "void RPObjStoreList::removeStore - Attempt to destroy invalid RPObjStore" << endl;
#endif
	(void) toRemove->deactivateDatabase();
	delete toRemove;
	toRemove = 0;
	return;
    }

    rpPtr->useCount--;

    if (rpPtr->useCount == 0)
    {
	Boolean haveRelease = FALSE;
	
	if (rpPtr->derivedFrom)
	{
	    rpPtr->derivedFrom->useCount--;
	    if (rpPtr->derivedFrom->useCount == 0)
	    {
		RPObjStoreList *top = RPObjStoreInterface::headOfList, *current = 0;
		found = FALSE;

		while ((top) && (!found))
		{
		    if (top == rpPtr->derivedFrom)
			found = TRUE;
		    else
		    {
			current = top;
			top = top->next;
		    }
		}
		
		if (current == 0)
		{
		    if (RPObjStoreInterface::tailOfList == RPObjStoreInterface::headOfList)
			RPObjStoreInterface::tailOfList = top->next;
		    
		    RPObjStoreInterface::headOfList = top->next;
		}
		else
		{
		    if (RPObjStoreInterface::tailOfList == top)
			RPObjStoreInterface::tailOfList = current;
		    
		    top->next->prev = current;
		    current->next = top->next;
		}
		
		(void) top->instance->deactivateDatabase();
		haveRelease = TRUE;
		
		delete top;
		rpPtr->derivedFrom = 0;
	    }
	}
		
        if (indx == 0)
	{
	    if (RPObjStoreInterface::tailOfList == RPObjStoreInterface::headOfList)
		RPObjStoreInterface::tailOfList = rpPtr->next;
	    
	    RPObjStoreInterface::headOfList = rpPtr->next;
	}
	else
	{
	    if (RPObjStoreInterface::tailOfList == rpPtr)
		RPObjStoreInterface::tailOfList = indx;

	    indx->next = rpPtr->next;
	}

	// postpone deactivate of db until os is deleted (probably after action commits)

	if (!haveRelease)
	    toRemove->deactivateDatabase();
	
	delete rpPtr;
	toRemove = 0;
    }
}

/*
 * A replica failure must have been detected to have called this routine.
 * This is all for complex object replication.
 */

Boolean RPObjStoreList::reSyncCachedView (const Uid& u, const TypeName tn, RemotePersistentObjectStore* rpOS)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean RPObjStoreList::reSyncCachedView ( " << u << ", " << tn << " )" << endl;
#endif
    
    RPObjStoreList* rpPtr = 0;
    ReplicaDescriptor* rdPtr = RPObjStoreInterface::headOfList->getCachedView(u, rpPtr);
    Boolean result;

    /*
     * If no group view currently exists then return and go to database.
     */

    if (rpOS->myData.group_view == 0)
	return FALSE;
    
    /*
     * Exclude failed replicas
     */

    result = rpOS->getViewAndExclude(u, tn, TRUE);
    if (!result)
	return FALSE;

    /*
     * Finished with old view, so reset everything now.
     */

    rpOS->failureDetected = FALSE;
    if (rpOS->replica_members)
    {
	delete rpOS->replica_members;
	rpOS->replica_members = 0;
    }
    
    /*
     * Got cached view.
     */
    
    if (rpPtr)
    {
	if (rpPtr->instance != rpOS)
	{
	    if (*rpOS->myData.group_view != *rdPtr)
	    {
		rdPtr->testAndSetObjectNames(tn);
		delete rpOS->myData.group_view;
		rpOS->myData.group_view = new ReplicaDescriptor;
		*rpOS->myData.group_view = *rdPtr;
	    }
	    else
	    {
		delete rpOS->myData.group_view;
		rpOS->myData.group_view = 0;

		/*
		 * If group view was obtained through cache, then get cache "owner" to
		 * resync view and get new view from there.
		 */

		if (rpPtr)
		{
		    if (rpPtr->derivedFrom)
		    {
			result = rpPtr->derivedFrom->instance->getViewAndExclude(u, tn, TRUE);
			if (!result)
			    return FALSE;
		
			rpOS->myData.group_view = rpPtr->derivedFrom->instance->getCompositeView(u);
			rdPtr->testAndSetObjectNames(tn);
			rpOS->myData.group_view = new ReplicaDescriptor;
			*rpOS->myData.group_view = *rdPtr;
		    }
		}
	    }
	}
    }
    else
    {
	/*
	 * No cached view, so go back to database.
	 */

	delete rpOS->myData.group_view;
	rpOS->myData.group_view = 0;
    }

    if (rdPtr)
	delete rdPtr;

    return rpOS->getViewAndExclude(u, tn, TRUE);
}
    



RPObjStoreInterface::RPObjStoreInterface ()
                                          : instanceRPOS(0),
					    delayedAction(FALSE),
					    useStore(FALSE),
					    locationToSet(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RPObjStoreInterface::RPObjStoreInterface ()" << endl;
#endif
}

RPObjStoreInterface::~RPObjStoreInterface ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "RPObjStoreInterface::~RPObjStoreInterface ()" << endl;
#endif

    if (instanceRPOS)
        RPObjStoreInterface::headOfList->removeStore(instanceRPOS);
}

void RPObjStoreInterface::setupStore (const Uid& u, const TypeName tn)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "void RPObjStoreInterface::setupStore ( " << u << ", " << tn << " )" << endl;
#endif

    // initialise object store instance

    if (!instanceRPOS)
    {
	if (RPObjStoreInterface::headOfList == 0)
	{
	    RPObjStoreInterface::headOfList = new RPObjStoreList;
	    RPObjStoreInterface::tailOfList = RPObjStoreInterface::headOfList;
	    instanceRPOS = RPObjStoreInterface::headOfList->instance;
	}
	else
	    instanceRPOS = RPObjStoreInterface::headOfList->getStore(u, tn);

	if (delayedAction)
	{
	    delayedAction = FALSE;
	    instanceRPOS->storeLocation(locationToSet);
	    instanceRPOS->useStoreLocation(useStore);
	}
    }
    else
    {
	/*
	 * This is an "old" object store, but may have detected a failure and so
	 * may have to either go back to db or get new cached view.
	 */
	
	if (instanceRPOS->failureDetected)
	    RPObjStoreInterface::headOfList->reSyncCachedView(u, tn, instanceRPOS);
    }
}

Boolean RPObjStoreInterface::commit_state (const Uid& u, const TypeName tn)
{
    setupStore(u, tn);
    return instanceRPOS->commit_state(u, tn);
}

ObjectState* RPObjStoreInterface::read_state (const Uid& u, const TypeName tn)
{
    setupStore(u, tn);
    return instanceRPOS->read_state(u, tn);
}

ObjectState* RPObjStoreInterface::read_uncommitted (const Uid& u, const TypeName tn)
{
    setupStore(u, tn);
    return instanceRPOS->read_uncommitted(u, tn);
}

Boolean RPObjStoreInterface::remove_state (const Uid& u, const TypeName tn)
{
    setupStore(u, tn);
    return instanceRPOS->remove_state(u, tn);
}

Boolean RPObjStoreInterface::remove_uncommitted (const Uid& u, const TypeName tn)
{
    setupStore(u, tn);
    return instanceRPOS->remove_uncommitted(u, tn);
}

Boolean RPObjStoreInterface::write_committed (const Uid& u, const TypeName tn, const ObjectState& os)
{
    setupStore(u, tn);
    return instanceRPOS->write_committed(u, tn, os);
}

Boolean RPObjStoreInterface::write_state (const Uid& u, const TypeName tn, const ObjectState& os)
{
    setupStore(u, tn);
    return instanceRPOS->write_state(u, tn, os);
}

ObjectState* RPObjStoreInterface::allObjUids (const TypeName tn)
{
    if (!instanceRPOS)
    {
        error_stream << WARNING << "Cannot call RPObjStoreInterface::allObjUids without assigning Uid" << endl;
	return 0;
    }
    else
        return instanceRPOS->allObjUids(tn);
}

const char* RPObjStoreInterface::getStoreName () const
{
    error_stream << FATAL << "RPObjStoreInterface::getStoreName () : Not Implemented" << endl;
    ArjunaFatal();
    return 0;
}

const TypeName RPObjStoreInterface::type () const { return "RemotePersistentObjectStore"; }

void RPObjStoreInterface::storeLocation (const char* location)
{
    if (!instanceRPOS)
    {
	delayedAction = TRUE;
	locationToSet = location;
    }
    else
	instanceRPOS->storeLocation(location);
}

void RPObjStoreInterface::useStoreLocation (const Boolean flag)
{
    if (!instanceRPOS)
    {
	delayedAction = TRUE;
	useStore = flag;
    }
    else
	instanceRPOS->useStoreLocation(flag);
}
