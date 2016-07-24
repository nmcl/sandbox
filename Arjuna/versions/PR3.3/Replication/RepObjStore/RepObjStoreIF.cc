/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RepObjStoreIF.cc,v 1.19 1995/09/30 10:23:39 nmcl Exp $
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

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef REPOBJSTOREIF_H_
#  include <Replication/RepObjStoreIF.h>
#endif

#ifndef REPDESC_H_
#  include <ArjServers/RepDesc_stub.h>
#endif

#ifndef REPLICATIONR_H_
#  include "ReplicationR.h"
#endif


/*
 * This class handles most of the complex object replication work. It is
 * also a preliminary test of the work for abstract interfaces and concrete
 * implementations.
 * Caching is achieved here and uses a write-through cache: if a replica failure
 * is detected during a read operation then it is "ignored", otherwise a new
 * view must be assembled.
 */


RepObjStoreList* RepObjStoreInterface::headOfList = (RepObjStoreList*) 0;
RepObjStoreList* RepObjStoreInterface::tailOfList = (RepObjStoreList*) 0;



RepObjStoreList::RepObjStoreList ()
				 : instance(new ReplicatedPersistentObjectStore),
				   derivedFrom(0),
				   next(0),
				   prev(0),
				   useCount(1)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_REPLICATION << VIS_PROTECTED;
    debug_stream << "RepObjStoreList::RepObjStoreList ()" << endl;
#endif
}

RepObjStoreList::~RepObjStoreList ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_REPLICATION << VIS_PROTECTED;
    debug_stream << "RepObjStoreList::~RepObjStoreList ()" << endl;
#endif

    if (instance)
        delete instance;
}

ReplicaDescriptor* RepObjStoreList::getCachedView (const Uid& u, RepObjStoreList*& rpPtr) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RepObjStoreList::getCachedView ( " << u << " ) const" << endl;
#endif

    RepObjStoreList *indx = (RepObjStoreList*) 0;
    ReplicaDescriptor *rdPtr = (ReplicaDescriptor*) 0;
    Boolean found = FALSE;

    /* search from tail of list. */

    rpPtr = RepObjStoreInterface::tailOfList;
    
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

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RepObjStoreList::getCachedView - " << ((found == FALSE) ? "no cached view" : "have cached view")
		 << endl;
#endif

    return rdPtr;
}
    
ReplicatedPersistentObjectStore* RepObjStoreList::getStore (const Uid& u, const TypeName tn)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RepObjStoreList::getStore ( " << u << ", " << tn << " )" << endl;
#endif

    RepObjStoreList* rpPtr = (RepObjStoreList*) 0;
    RepObjStoreList* indx = new RepObjStoreList;
    ReplicaDescriptor* rdPtr = getCachedView(u, rpPtr);

    // add to head of list.

    indx->next = RepObjStoreInterface::headOfList;
    RepObjStoreInterface::headOfList->prev = indx;
    RepObjStoreInterface::headOfList = indx;
	
    if (rdPtr)
    {
	rdPtr->testAndSetObjectNames(tn);

	// rpPtr must be set since rdPtr != 0

	if (rpPtr->derivedFrom != (RepObjStoreList*) 0)
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

Boolean RepObjStoreList::removeStore (ReplicatedPersistentObjectStore*& toRemove, Boolean canDelete)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RepObjStoreList::removeStore ( " << ((canDelete == TRUE) ? "Top-level" : "Nested")
		 << " )" << endl;
#endif

    /*
     * We actually only delete object store instances when the object is deleted.
     * We end up with some garbage only if the user changes the object from being
     * replicated to being non-replicated, in which case the "old" store type
     * stays around.
     *
     * The Boolean parameter indicates whether we can delete the object store as well
     * as deactivate the database. Because object stores can be linked to other
     * object stores as a result of complex object replication, we can only actually
     * delete an object store if it directly belongs to the object calling this routine.
     * If removeStore is called on an object store because some other instance is linked to
     * it then we cannot delete it, even if the useCount gets to zero. We must leave this
     * to the destructor of the object it really belongs to.
     */

    RepObjStoreList* rpPtr = RepObjStoreInterface::headOfList;
    RepObjStoreList* indx = (RepObjStoreList*) 0;
    Boolean found = FALSE, result = FALSE;

    if (!toRemove)
	return TRUE;

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
        error_stream << WARNING << "RepObjStoreList::removeStore - Attempt to destroy invalid RPObjStore" << endl;
#endif
	result = ((toRemove->isRemoteOnly) ? TRUE : toRemove->deactivateDatabase());
	delete toRemove;
	toRemove = (ReplicatedPersistentObjectStore*) 0;
	return result;
    }

    /*
     * We may not have been able to delete this object store the first time we tried to
     * as we may have been within an atomic action. So some object stores may have zero
     * use counts any yet still be on the list. We only decrement the use counts if it
     * makes sense to do so, i.e., if the count is > 0.
     */
    
    if (rpPtr->useCount > 0)
	rpPtr->useCount--;

    if (rpPtr->useCount == 0)
    {
	if (rpPtr->derivedFrom)
	{
	    result = removeStore(rpPtr->derivedFrom->instance, FALSE);
	    rpPtr->derivedFrom = (RepObjStoreList*) 0;  // guarantees we will only do this once, even if we have to retry.
	}
	else
	    result = ((toRemove->isRemoteOnly) ? TRUE : toRemove->deactivateDatabase());

	if ((result) && (canDelete))
	{
	    if (indx == (RepObjStoreList*) 0)
	    {
		if (RepObjStoreInterface::tailOfList == RepObjStoreInterface::headOfList)
		    RepObjStoreInterface::tailOfList = RepObjStoreInterface::headOfList->next;
		
		RepObjStoreInterface::headOfList = RepObjStoreInterface::headOfList->next;
		
		if (RepObjStoreInterface::headOfList)
		    RepObjStoreInterface::headOfList->prev = (RepObjStoreList*) 0;
	    }
	    else
	    {
		if (RepObjStoreInterface::tailOfList == rpPtr)
		    RepObjStoreInterface::tailOfList = indx;
		
		indx->next = rpPtr->next;
		if (rpPtr->next)
		    rpPtr->next->prev = indx;
	    }
	
	    delete rpPtr;
	    toRemove = (ReplicatedPersistentObjectStore*) 0;
	}
    }
    else
	result = TRUE;

    return result;
}

/*
 * It is possible that multiple objects may obtain overlapping sets of group views.
 * In this case it is also possible that one of these objects could have a more
 * up-to-date version, and if this is so the derivedFrom pointer (which indicates the
 * "owner" of the relevant cached view) may change between actions.
 * Note: the typical reason for this change is when the "owner" changes between top-level
 * atomic actions.
 * Calling this routine may allow an object store instance to be deleted: some instances
 * may have been held back from deletion because their use_count was not zero, despite
 * the fact that the object within which they were created may have been deleted.
 */

void RepObjStoreList::checkDerivedFrom (RepObjStoreList* toCheck, ReplicatedPersistentObjectStore* rpOS)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "RepObjStoreList::checkDerivedFrom (RepObjStoreList* toCheck, ReplicatedPersistentObjectStore* rpOS)" << endl;
#endif

    RepObjStoreList* tmpRP = this;
    
    while (tmpRP)
    {
	if (tmpRP->instance == rpOS)
	{
	    if (tmpRP->derivedFrom)
	    {
		if (tmpRP->derivedFrom == toCheck)
		    return;
		tmpRP->removeStore(tmpRP->derivedFrom->instance);
	    }
	    tmpRP->derivedFrom = toCheck;
	    tmpRP->derivedFrom->useCount++;

	    return;
	}
	else
	    tmpRP = tmpRP->next;
    }
}

/*
 * This is all for complex object replication. We only get to this
 * point if we need to re-evaluate a group view for a replica because
 * of detecting a failure.
 * Note: in the current implementation the cached copy is allowed to
 * get out-of-date with the db copy. If failures are detected by
 * group members they only update the db view and not the cached copy.
 * We do this to cut the overhead as failures are rare and the cached
 * copy is currently only used once per replica group.
 */

Boolean RepObjStoreList::reSyncCachedView (const Uid& u, const TypeName tn, ReplicatedPersistentObjectStore* rpOS)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "RepObjStoreList::reSyncCachedView ( " << u << ", " << tn << " )" << endl;
#endif

    if (rpOS->isRemoteOnly)
	return TRUE;

    /*
     * Exclude failed replicas. This call will only ensure that failed
     * replicas are marked and excluded from the database. The view is
     * not obtained [will factorise this out eventually].
     */

    if (rpOS->failureDetected)
    {
	if (rpOS->getViewAndExclude(u, tn, TRUE) == FALSE)
	    return FALSE;
    }
    
    RepObjStoreList* rpPtr = (RepObjStoreList*) 0;
    ReplicaDescriptor* rdPtr = RepObjStoreInterface::headOfList->getCachedView(u, rpPtr);

    /*
     * Finished with old view, so reset everything now.
     * We always delete the group view list (although not
     * the actual replica servers) so that we can create
     * a fresh one and benefit from any other failures (*servers*)
     * which other objects within this process may have detected.
     */

    rpOS->failureDetected = FALSE;
    if (rpOS->replica_members)
    {
	delete rpOS->replica_members;
	rpOS->replica_members = (ViewData*) 0;
    }

    if (rpPtr)
    {
        /*
	 * Got cached view.
	 */

	if (rpPtr->instance != rpOS)
	{
	    Boolean updateCache = TRUE;
	    ReplicatedPersistentObjectStore* toUse = ((rpPtr->derivedFrom != (RepObjStoreList*) 0) ? rpPtr->derivedFrom->instance
						                                                   : rpPtr->instance);

	    /*
	     * If the top-level action within which the current view was obtained is
	     * not the current one then that action must have ended and we need to
	     * go back to the db (or cache) to get the "current" view.
	     * Note: this is no longer required because of replication
	     * abstract records, which clean up when a top-level action ends.
	     * However, since the insertion of abstract records could fail, we
	     * leave this test in to be on the safe side - the operation is
	     * idempotent.
	     */

	    if (toUse->myData.actionID)
	    {
		if ((AtomicAction::Current()) &&
		    (AtomicAction::Current()->topLevelAction() == *toUse->myData.actionID))
		    updateCache = FALSE;
	    }

	    /*
	     * rdPtr can only be different if cache "owner" has detected failures
	     * at some point and returned to db to register them and obtain a
	     * new (composite) view. Therefore, if this is different it has to be
	     * more up-to-date than the one the replica currently possesses, so
	     * we go with it.
	     */

	    if (*rpOS->myData.group_view != *rdPtr)
	    {
		rdPtr->testAndSetObjectNames(tn);
		if (rpOS->myData.group_view)
		    delete rpOS->myData.group_view;
		rpOS->myData.group_view = new ReplicaDescriptor;
		*rpOS->myData.group_view = *rdPtr;
		RepObjStoreInterface::headOfList->checkDerivedFrom(rpPtr, rpOS);
	    }
	    else
	    {
		if (updateCache)
		{
		    if (rpOS->myData.group_view)
		    {
			delete rpOS->myData.group_view;
			rpOS->myData.group_view = (ReplicaDescriptor*) 0;
		    }

		    /*
		     * If group view was obtained through cache, then get cache "owner" to
		     * resync view and get new view from there. To do this we must force
		     * the "owner" to go back to the db and recreate its group view.
		     * Note: recreation of the group view for this object is not strictly
		     * necessary, but we do it anyway since it will probably be used.
		     */
		    
		    if (rpPtr->derivedFrom)
		    {
		        rpPtr->derivedFrom->instance->cleanUp();
			if (rpPtr->derivedFrom->instance->getViewAndExclude(u, tn, TRUE) == FALSE)
			{
			    if (rdPtr)
				delete rdPtr;
			    
			    return FALSE;
			}
		
			rpOS->myData.group_view = rpPtr->derivedFrom->instance->getCompositeView(u);
			rdPtr->testAndSetObjectNames(tn);
			rpOS->myData.group_view = new ReplicaDescriptor;
			*rpOS->myData.group_view = *rdPtr;
		    }
		}
		else  // cached copy is same as this and there has been no action change
		    RepObjStoreInterface::headOfList->checkDerivedFrom(rpPtr, rpOS);
	    }
	}
	else
	{
	    if (rpOS->myData.group_view)
	    {
		delete rpOS->myData.group_view;
		rpOS->myData.group_view = (ReplicaDescriptor*) 0;
	    }
	}
    }
    else
    {
	/*
	 * No cached view, so go back to database.
	 */

	if (rpOS->myData.group_view)
	{
	    delete rpOS->myData.group_view;
	    rpOS->myData.group_view = (ReplicaDescriptor*) 0;
	}
    }

    if (rdPtr)
	delete rdPtr;

    return rpOS->getViewAndExclude(u, tn, TRUE);
}
    


/*
 * If cacheLong is TRUE then we hold the group view lock until the object is
 * deleted, i.e., it is not released at the end of each top-level action.
 * storeRoot indicates the root of the object store to use.
 */

RepObjStoreInterface::RepObjStoreInterface (const char* storeRoot, Boolean cacheLong)
					   : instanceRPOS(0),
					     locationToSet(0),
					     numberOfLocations(0),
					     delayedAction(FALSE),
					     useStore(FALSE),
					     holdCache(cacheLong),
					     tName(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RepObjStoreInterface::RepObjStoreInterface ( " << storeRoot << ", "
		 << ((holdCache == TRUE) ? "Cache ON" : "Cache OFF") << " )" << endl;
#endif

    localStoreName = ::new char[::strlen(storeRoot)+1];
    ::strcpy(localStoreName, storeRoot);
}

RepObjStoreInterface::~RepObjStoreInterface ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RepObjStoreInterface::~RepObjStoreInterface ()" << endl;
#endif

    if (tName)
#ifndef GCC_ARRAY_DELETE_BUG
	::delete [] tName;
#else
        ::delete tName;
#endif

    if (localStoreName)
#ifndef GCC_ARRAY_DELETE_BUG
        ::delete [] localStoreName;
#else
        ::delete localStoreName;
#endif

    if (locationToSet)
    {
	for (int i = 0; i < numberOfLocations; i++)
#ifndef GCC_ARRAY_DELETE_BUG
	    ::delete [] locationToSet[i];
#else
	    ::delete locationToSet[i];
#endif

#ifndef GCC_ARRAY_DELETE_BUG	
	::delete [] locationToSet;
#else
	::delete locationToSet;
#endif	
    }	

    deactivate(TRUE); /* Deactivate and indicate this is for the destructor. */
}

void RepObjStoreInterface::setObjectData (const Uid& u, const TypeName tn)
{
    if (!tName)
    {
	gUID = u;
	tName = ::new char[::strlen(tn)+1];
	::memset(tName, '\0', ::strlen(tn)+1);
	::memcpy(tName, tn, ::strlen(tn));
    }

    /*
     * Add replication record to action list if we are using the db.
     * We do this here for new persistent objects because we cannot always do this
     * in setupStore as the atomic action may not be in the RUNNING state.
     */

    if (AtomicAction::Current() != 0)
    {
	if (AtomicAction::Current()->status() == RUNNING)
	{
	    ReplicationRecord *rr = new ReplicationRecord(this, gUID, tName, useStore);
	    if (AtomicAction::Current()->add(rr) != AtomicAction::ADDED)
		delete rr;
	}
    }
}

Boolean RepObjStoreInterface::deactivate (Boolean isDestructor)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RepObjStoreInterface::deactivate ( "
		 << ((isDestructor == TRUE) ? "Destructor" : "Normal") << " )" << endl;
#endif

    Boolean result = TRUE;
    
    if (instanceRPOS)
    {
	/*
	 * If this is called from the destructor then regardless of cache flag we must
	 * deactivate the object store. Otherwise, the cache flag dominates the
	 * final decision. (We obviously cannot cache if we are being deleted, though.
	 */

	if (isDestructor)
	    result = RepObjStoreInterface::headOfList->removeStore(instanceRPOS);

	/*
	 * We must only deactivate the database, rather than delete the
	 * object store instance.
	 */
	
	if ((!isDestructor) && (!holdCache))
	    result = instanceRPOS->deactivateDatabase();
    }

    return result;
}

/*
 * Assign the correct remote object store to the interface.
 * If a failure was detected prior to this, i.e., in another (aborted) action,
 * then we will exclude it first.
 */

void RepObjStoreInterface::setupStore (const Uid& u, const TypeName tn)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "RepObjStoreInterface::setupStore ( " << u << ", " << tn << " )" << endl;
#endif

    Boolean needsResync = FALSE;

    /*
     * Try to add replication record to action list.  We do this so that we
     * can guarantee a tidy up, even if the action aborts  or the object is
     * deleted in the atomic action. The ReplicationRecord will tidy up for
     * us and will eventually delete this object store if necessary. (Calls
     * to  delete it because the StateManager  has been deleted will simply
     * decrement   the     reference  count,  which   ReplicationRecord has
     * incremented, and not actually delete the store  since the count will
     * be greater than 0).
     */

    if (AtomicAction::Current() != 0)
    {
	if (AtomicAction::Current()->status() == RUNNING)
	{
	    ReplicationRecord *rr = new ReplicationRecord(this, u, tn, useStore);
	    if (AtomicAction::Current()->add(rr) != AtomicAction::ADDED)
		delete rr;
	}
    }

    /*
     * The following will always be true the first time a replica is used
     * within an atomic action, so we guarantee we must go back to the
     * db (or newly cached copy) for the group view, regardless of how often
     * this replica group has been used within the same application.
     * [ReplicationRecord does this cleanup for us].
     */

    if (instanceRPOS)
    {
	if (instanceRPOS->isRemoteOnly)
	    return;
	
	needsResync = (Boolean) (instanceRPOS->replica_members == (ViewData*) 0);
    }
	
    /* initialise object store instance. */

    if (!instanceRPOS)
    {
	if (RepObjStoreInterface::headOfList == (RepObjStoreList*) 0)
	{
	    RepObjStoreInterface::headOfList = new RepObjStoreList;
	    RepObjStoreInterface::tailOfList = RepObjStoreInterface::headOfList;
	    instanceRPOS = RepObjStoreInterface::headOfList->instance;
	}
	else
	    instanceRPOS = RepObjStoreInterface::headOfList->getStore(u, tn);

	instanceRPOS->setRootOfStore(localStoreName);
	
	if (delayedAction)
	{
	    delayedAction = FALSE;
	    instanceRPOS->storeLocation(locationToSet, numberOfLocations);
	    instanceRPOS->useStoreLocation(useStore);
	}
    }
    else
    {
	/*
	 * This is an "old" object store, but may have detected a failure and so
	 * may have to either go back to db or get new cached view.
	 * failureDetected will only be TRUE if a critical failure occurred.
	 */
	
	if ((instanceRPOS->failureDetected) || (needsResync))
	    RepObjStoreInterface::headOfList->reSyncCachedView(u, tn, instanceRPOS);
    }
}

void RepObjStoreInterface::cleanUp ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "RepObjStoreInterface::cleanUp ()" << endl;
#endif

    if (instanceRPOS)
	instanceRPOS->cleanUp();
}

Boolean RepObjStoreInterface::allObjUids ( const TypeName tn, ObjectState& state, ObjectStore::StateStatus _type )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RepObjStoreInterface::allObjUids ( " << tn << ", " << _type << " )" << endl;
#endif
    
    if (!instanceRPOS)
    {
        error_stream << WARNING
		     << "RepObjStoreInterface::allObjUids - cannot be called without assigning group Uid" << endl;
	return FALSE;
    }
    else
        return instanceRPOS->allObjUids(tn, state, _type);
}

Boolean RepObjStoreInterface::commit_state (const Uid& u, const TypeName tn)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RepObjStoreInterface::commit_state ( " << u << ", " << tn << " )" << endl;
#endif
    
    setupStore(u, tn);
    return instanceRPOS->commit_state(u, tn);
}

ObjectState* RepObjStoreInterface::read_committed (const Uid& u, const TypeName tn)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RepObjStoreInterface::read_committed ( " << u << ", " << tn << " )" << endl;
#endif
    
    setupStore(u, tn);
    return instanceRPOS->read_committed(u, tn);
}

ObjectState* RepObjStoreInterface::read_uncommitted (const Uid& u, const TypeName tn)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RepObjStoreInterface::read_uncommitted ( " << u << ", " << tn << " )" << endl;
#endif
    
    setupStore(u, tn);
    return instanceRPOS->read_uncommitted(u, tn);
}

Boolean RepObjStoreInterface::remove_committed (const Uid& u, const TypeName tn)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RepObjStoreInterface::remove_committed ( " << u << ", " << tn << " )" << endl;
#endif
   
    setupStore(u, tn);
    return instanceRPOS->remove_committed(u, tn);
}

Boolean RepObjStoreInterface::remove_uncommitted (const Uid& u, const TypeName tn)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RepObjStoreInterface::remove_uncommitted ( " << u << ", " << tn << " )" << endl;
#endif
    
    setupStore(u, tn);
    return instanceRPOS->remove_uncommitted(u, tn);
}

Boolean RepObjStoreInterface::write_committed (const Uid& u, const TypeName tn, const ObjectState& os)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RepObjStoreInterface::write_committed ( " << u << ", " << tn
		 << ", " << os << " )" << endl;
#endif
    
    setupStore(u, tn);
    return instanceRPOS->write_committed(u, tn, os);
}

Boolean RepObjStoreInterface::write_uncommitted (const Uid& u, const TypeName tn, const ObjectState& os)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RepObjStoreInterface::write_uncommitted ( " << u << ", " << tn
		 << ", " << os << " )" << endl;
#endif
    
    setupStore(u, tn);
    return instanceRPOS->write_uncommitted(u, tn, os);
}

/*
 * We must make copies of these items as the StateManager instance which
 * passes them to us may be deleted before we need to use them again.
 */

void RepObjStoreInterface::storeLocation (char** location, int number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RepObjStoreInterface::storeLocation ()" << endl;
#endif

    if (locationToSet)
    {
	for (int i = 0; i < numberOfLocations; i++)
#ifndef GCC_ARRAY_DELETE_BUG
	    ::delete [] locationToSet[i];
#else
	    ::delete locationToSet[i];
#endif

#ifndef GCC_ARRAY_DELETE_BUG	
	::delete [] locationToSet;
#else
	::delete locationToSet;
#endif	
    }

    numberOfLocations = number;

    if (numberOfLocations > 0)
    {
	locationToSet = ::new char* [numberOfLocations];
	
	for (int i = 0; i < numberOfLocations; i++)
	{
	    locationToSet[i] = ::new char[::strlen(location[i])+1];
	    ::strcpy(locationToSet[i], location[i]);
	}
    }
    else
	locationToSet = NULL;
	
    if (!instanceRPOS)
	delayedAction = TRUE;
    else
	instanceRPOS->storeLocation(locationToSet, numberOfLocations);
}

void RepObjStoreInterface::useStoreLocation (const Boolean flag)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RepObjStoreInterface::useStoreLocation ( "
		 << ((flag) ? "TRUE" : "FALSE") << " )" << endl;
#endif
    
    if (!instanceRPOS)
    {
	delayedAction = TRUE;
	useStore = flag;
    }
    else
	instanceRPOS->useStoreLocation(flag);
}

/*
 * We don't need to pack anything from the implementation as
 * it should be reflected in the interface.
 */

Boolean RepObjStoreInterface::packInto (Buffer& buff) const
{
    Boolean res = buff.pack(useStore);
    
    if (numberOfLocations > 0)
    {
	res = buff.pack(numberOfLocations);

	for (int i = 0; (i < numberOfLocations) && (res); i++)
	    res = buff.packMappedString(locationToSet[i]);
    }
    else
	res = buff.pack(0);

    if (res)
	res = buff.packMappedString(localStoreName);
    
    return res;
}

/*
 * We should only call this if we are recovering an action.
 * In this case we may not have an instanceRPOS pointer, so
 * we create it. We need to copy some of this information back
 * into the interface itself.
 */

Boolean RepObjStoreInterface::unpackFrom (Buffer& buff)
{
  int i;
  
  if (instanceRPOS == (ReplicatedPersistentObjectStore*) 0)
    {
	RepObjStoreList* rpPtr = new RepObjStoreList;

	rpPtr->next = RepObjStoreInterface::headOfList;
	RepObjStoreInterface::headOfList->prev = rpPtr;
	RepObjStoreInterface::headOfList = rpPtr;
	
	instanceRPOS = rpPtr->instance;
    }
    else                                // cleanup old instance before unpacking saved state.
	instanceRPOS->cleanUp();

    for (i = 0; i < numberOfLocations; i++)
    {
	if (locationToSet[i])
#ifndef GCC_ARRAY_DELETE_BUG
	    ::delete [] locationToSet[i];
#else
	    ::delete locationToSet[i];
#endif
    }

    if (locationToSet)
#ifndef GCC_ARRAY_DELETE_BUG
	::delete [] locationToSet;
#else
        ::delete locationToSet;
#endif

    locationToSet = NULL;

    Boolean res = buff.unpack(useStore) && buff.unpack(numberOfLocations);
	
    if (res)  // now setup RepObjStoreInterface
    {
	locationToSet = ::new char*[numberOfLocations];
	
	for (i = 0; (i < numberOfLocations) && (res); i++)
	{
	    locationToSet[i] = (char*) 0;
	    res = buff.unpack(locationToSet[i]);
	}

	if (res)
	{
	    if (localStoreName)
#ifndef GCC_ARRAY_DELETE_BUG
		::delete [] localStoreName;
#else
	        ::delete localStoreName;
#endif
	    localStoreName = (char*) 0;

	    res = buff.unpack(localStoreName);
	}
    }
    
    if (res)
    {
	instanceRPOS->storeLocation(locationToSet, numberOfLocations);
	instanceRPOS->useStoreLocation(useStore);
	instanceRPOS->setRootOfStore(localStoreName);
    }
    
    return res;
}

Boolean RepObjStoreInterface::hide_state (const Uid&, const TypeName)
{
    error_stream << WARNING << "RepObjStoreInterface::hide_state: operation not supported." << endl;
    return FALSE;
}

Boolean RepObjStoreInterface::reveal_state (const Uid&, const TypeName)
{
    error_stream << WARNING << "RepObjStoreInterface::reveal_state: operation not supported." << endl;
    return FALSE;
}

ObjectStore::StateStatus RepObjStoreInterface::currentState (const Uid&, const TypeName)
{
    error_stream << WARNING << "RepObjStoreInterface::currentState: operation not supported." << endl;
    return ObjectStore::OS_UNKNOWN;
}


Boolean RepObjStoreInterface::allTypes (ObjectState&) const
{
    error_stream << WARNING << "RepObjStoreInterface::allTypes: operation not supported." << endl;
    return FALSE;
}


#ifdef NO_INLINES
#  define REPOBJSTOREIF_CC_
#  include <Replication/RepObjStoreIF.n>
#  undef REOBJSTOREIF_CC_
#endif
