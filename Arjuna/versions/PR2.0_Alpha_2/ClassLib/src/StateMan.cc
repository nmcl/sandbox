/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StateMan.cc,v 1.1 1993/11/03 12:30:17 nmcl Exp $
 */

/*
 *
 * Object state management class
 *
 */

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
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

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef STATEMAN_H_
#  include <Arjuna/StateMan.h>
#endif

#ifndef ACTIVER_H_
#  include "ActiveR.h"
#endif

#ifndef CADAVERAR_H_
#  include "CadaverAR.h"
#endif

#ifndef RECOVERYR_H_
#  include "RecoveryR.h"
#endif

#ifndef PERSISTR_H_
#  include "PersistR.h"
#endif

#ifndef CADAVERR_H_
#  include "CadaverR.h"
#endif

static const char RCSid[] = "$Id: StateMan.cc,v 1.1 1993/11/03 12:30:17 nmcl Exp $";

/*
 * Public non-virtual functions and operators
 */

/*
 * This operation activates an object. Activation of an object may
 * entail loading its passive state from the object store and unpacking
 * it into the memory resident form, or it may simply be a no-op.
 * Full activation is only necessary if the object is currently marked
 * as being PASSIVE (that is, the object was constructed as being of
 * type ANDPERSISTENT with an existing uid and has not already been
 * activated).
 * Objects that are not of type ANDPERSISTENT or are persistent but
 * have not yet been saved in an object store (so-called new persistent
 * objects) are unaffected by this function.
 * Returns FALSE if PASSIVE object cannot be loaded from object store,
 * TRUE otherwise.
 */

Boolean LocalStateManager::activate ( ObjectStore *store )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_STATE_MAN << VIS_PUBLIC;
    debug_stream << "LocalStateManager::activate(" << store << ")"
	         << " for object-id " << objectUid << "\n" << flush;
#endif

    ObjectStatus oldStatus = currentStatus;
    Boolean result = TRUE;			/* assume 'succeeds' */

    if ((currentStatus == PASSIVE) ||
	(currentStatus == PASSIVE_NEW))
    {

	AtomicAction *action = AtomicAction::Current();

	/* Only really activate if object is PASSIVE */

	setupStore(store);
	if (currentStatus == PASSIVE)
	{
	    ObjectState *oldState = objectStore->read_state(objectUid, type());

	    if (oldState)
	    {
		if (result = restore_state(*oldState, ANDPERSISTENT))
		    currentStatus = ACTIVE;
		delete oldState;
	    } else
	    {
		error_stream << WARNING << "Activate of object with id = "
			     << objectUid << " and type '" << type()
			     << "' unexpectedly failed\n" << flush;
		result = FALSE;
	    }
	    
	}

	/*
	 * Create ActRec if status changed Passive->Active or if object
	 * is a new persistent object.
	 */
	
	if (((currentStatus == ACTIVE) || (currentStatus == PASSIVE_NEW)) &&
	    (action) &&
	    (action->status() == RUNNING))
	{
	    ActivationRecord *ar = new ActivationRecord(oldStatus,this);

	    if(action->add(ar) == FALSE)
		delete ar;
	}
    }
	
    return (result);
}

/*
 * This operation deactivates a persistent object.
 * It behaves in a similar manner to the activate operation, but has an
 * extra argument which defines whether the object's state should be
 * commited now or not.
 */

Boolean LocalStateManager::deactivate ( ObjectStore *store, Boolean commit )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_STATE_MAN << VIS_PUBLIC;
    debug_stream << "LocalStateManager::deactivate(" << store << ", "
	         << commit << ")"
	         << " for object-id " << objectUid << "\n" << flush;
#endif

    Boolean result = FALSE;

    if ((myType == ANDPERSISTENT) || ((myType == RECOVERABLE) && (shareable)))
    {
	if (currentStatus == ACTIVE_NEW)
	{
	    TypeName tn = type();

	    setupStore(store);
	    
	    ObjectState newState(objectUid, tn);

	    save_state(newState, myType);

	    if (commit)
		result = objectStore->write_committed(objectUid, tn,
						      newState);
	    else
		result = objectStore->write_state(objectUid, tn, newState);
	    

	    if (result)
		currentStatus = PASSIVE;
	}
    }
    else
	error_stream << WARNING << "LocalStateManager::deactivate() called for "
	             << "non-persistent object - ignored\n";
	
    return (result);
}

void LocalStateManager::resetState ( ObjectStatus newState )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_STATE_MAN << VIS_PUBLIC;
    debug_stream << "LocalStateManager::resetState(" << newState << ")"
	         << " for object-id " << objectUid << "\n" << flush;
#endif

    currentStatus = newState;
}

ObjectStatus LocalStateManager::status () const
{
    return currentStatus;
}

Uid LocalStateManager::get_uid () const
{
    return objectUid;
}

Boolean LocalStateManager::HasRemoteState () const
{
    return FALSE;
}

const char* LocalStateManager::StoreLocation () const
{
    return 0;
}

Boolean LocalStateManager::useStoreLocation () const
{
    return FALSE;
}


/*
 * Storage management for classes derived from LocalStateManager
 * These currently map to the standard management routines
 * but at a later date these will need doing properly
 */

void *LocalStateManager::operator new ( size_t amount )
{
    return ::new char[amount];
}

void LocalStateManager::operator delete ( void *memPtr )
{
#ifndef __GNUG__
    ::delete [] memPtr;
#else
    ::delete memPtr;
#endif
}

/*
 * Public virtual functions 
 */


void LocalStateManager::destroy ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_STATE_MAN << VIS_PUBLIC;
    debug_stream << "LocalStateManager::destroy for object-id " << objectUid << "\n" << flush;
#endif
    AtomicAction *action = AtomicAction::Current();

    if (action && action->status() == RUNNING)
    {
	error_stream << WARNING
	             << "Sorry destroy is not implemented at the moment!\n";
    }
}

/*
 * The following function disables recovery for an object by setting
 * the ObjectType to NEITHER (RECOVERABLE or ANDPERSISTENT).
 * The value of this variable is checked in the modified operation
 * so that no recovery information is created if myType is set to NEITHER.
 */

void LocalStateManager::disable ()
{
    myType = NEITHER;
}

ostream& LocalStateManager::print ( ostream& strm ) const
{
    strm << "Uid: " << objectUid << "\n"
	 << "Type: " << type() << "\n";
    return strm;
}

/*
 * Protected constructors and destructor
 */

/*
 * Create object with specific uid. This constructor
 * is primarily used when recreating an existing object. The object type
 * is set to 'ANDPERSISTENT' this is equivalent to invoking
 * persist in the object constructor
 */

LocalStateManager::LocalStateManager ( const Uid& objUid, ObjectType ot)
                                     : currentStatus(PASSIVE),
                                       deleteStore(FALSE),
				       myType(ot),
				       objectStore(0),
				       objectUid(objUid),
				       shareable(TRUE),
				       usingAction(NIL_UID)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_STATE_MAN << VIS_PROTECTED;
    debug_stream << "LocalStateManager::LocalStateManager(" << get_uid() << ")\n" << flush;
#endif
}

LocalStateManager::LocalStateManager ( ObjectType ot, Boolean isShareable )
                                     : currentStatus((isShareable == FALSE) && (ot == RECOVERABLE) ? ACTIVE : PASSIVE_NEW),
                                       deleteStore(FALSE),
				       myType(ot),
				       objectStore(0),
				       objectUid(),
				       shareable(isShareable),
				       usingAction(NIL_UID)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_STATE_MAN << VIS_PROTECTED;
    debug_stream << "LocalStateManager::LocalStateManager(" << ot << ")\n" << flush;
#endif
}

/*
 * Destructor.
 */

LocalStateManager::~LocalStateManager ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_STATE_MAN << VIS_PUBLIC;
    debug_stream << "LocalStateManager::~LocalStateManager() for object-id "
	         << get_uid() << "\n" << flush;
#endif

    AtomicAction *action = AtomicAction::Current();
 
   if (action &&
	action->status() == RUNNING &&
	currentStatus == ACTIVE_NEW)
    {
	error_stream << WARNING
	             << "LocalStateManager::terminate() should be invoked in every destructor\n";
	terminate();
    }

    if (deleteStore && objectStore)
        ObjectStore::destroy(objectStore);
}

/*
 * Protected non-virtual functions
 */

void LocalStateManager::modified ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_STATE_MAN << VIS_PROTECTED;
    debug_stream << "LocalStateManager::modified() for object-id " << get_uid()
	         << "\n" << flush;
#endif
    
    AtomicAction *action = AtomicAction::Current();
    RecoveryRecord *record = 0;
    ObjectStatus oldStatus;
    
    if (myType == NEITHER)		/*  NEITHER => no recovery info */
	return;
    
    if (currentStatus == PASSIVE)
    {
	error_stream << WARNING
		     << "LocalStateManager::modified() invocation on an object whose state"
		     << " has not been restored - activating object\n";
	activate();
    }
    
    oldStatus = currentStatus;
    currentStatus = ACTIVE_NEW;
    
    if (action && action->status() == RUNNING)
    {
	/* Check if this is the first call to modified in this action */
	
	if ((action->get_uid()) == usingAction)
	    return;
	
	/* If here then its a new action */
	
	ObjectState *state = new ObjectState(objectUid, type());
	
	/*
	 * Only save the state if this is a nested atomic action
	 * and state is valid (ie oldStatus was not PASSIVE_NEW)
	 */
	
	if ((oldStatus != PASSIVE_NEW) && action->parent())
	    save_state(*state, RECOVERABLE);
	
	if ((myType == RECOVERABLE) && (!shareable))
	    record = new RecoveryRecord(state, this);
	else
	    record = new PersistenceRecord(state, objectStore, this);
	
	if (action->add(record) == FALSE)
	    delete record;
	
	usingAction = action->get_uid();
    }
}

/*
 * The persist function changes the type of the object from RECOVERABLE
 * to ANDPERSISTENT.
 * No changes are made unless the status of the object is ACTIVE, so it
 * is not possible to change the type of the object if it has been
 * modified.
 */

void LocalStateManager::persist ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_STATE_MAN << VIS_PROTECTED;
    debug_stream << "LocalStateManager::persist() for object-id "
	         << get_uid() << "\n" << flush;
#endif

    if (currentStatus == ACTIVE)
    {
	currentStatus = PASSIVE_NEW;
	myType = ANDPERSISTENT;
    }
}

/*
 * The following function checks to see if the object is going out of
 * scope while an action is still running.
 */

void LocalStateManager::terminate ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_STATE_MAN << VIS_PROTECTED;
    debug_stream << "LocalStateManager::terminate() for object-id"
	         << get_uid() << "\n" << flush;
#endif

    AtomicAction *action = AtomicAction::Current();

    if (myType == NEITHER)
	return;

    if (currentStatus == ACTIVE_NEW &&
	action &&
	action->status() == RUNNING)
    {
	ObjectState *state = new ObjectState(objectUid, type());
	AbstractRecord *record = 0;

	/* This should be unnecessary - but just in case */
	setupStore(objectStore);
	
	save_state(*state, myType);
	record = new CadaverRecord(state, objectStore, StoreLocation(), this);

	if (action->add(record) == FALSE)
	    delete record;

	record = new CadaverActivationRecord(this);
	if (action->add(record) == FALSE)
	    delete record;

	currentStatus = PASSIVE;
    }
}

/*
 * Private non-virtual functions
 */

void LocalStateManager::setupStore (ObjectStore *store)
{
    if (store)
	objectStore = store;
    else if (objectStore == 0)
    {
        deleteStore = TRUE;
	if (HasRemoteState())
	{
	    objectStore = ObjectStore::create("RemotePersistentObjectStore");
	    objectStore->storeLocation(StoreLocation());
	    objectStore->useStoreLocation(useStoreLocation());
	}
	else
	{
	    if (myType == ANDPERSISTENT)
	    {
		objectStore = ObjectStore::create("LocalPersistentObjectStore");
	    }
	    else
	    {
		objectStore = ObjectStore::create("LocalVolatileObjectStore");
	    }
	}
    }
}

ostream& operator<< ( ostream& strm, const LocalStateManager& sm )
{
    return sm.print(strm);
}

ostream& operator<< ( ostream& strm, ObjectType ot )
{
    strm << (ot == RECOVERABLE ? "RECOVERABLE"
                               : (ot == ANDPERSISTENT ? "ANDPERSISTENT"
                                                      : "NEITHER"));
    return strm;
}

ostream& operator<< ( ostream& strm, ObjectStatus os )
{
    strm << (os == PASSIVE ? "PASSIVE"
                           : (os == PASSIVE_NEW ? "PASSIVE_NEW"
                                                : (os == ACTIVE ? "ACTIVE"
                                                                : "ACTIVE_NEW")));
    return strm;
}
