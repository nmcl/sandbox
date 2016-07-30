/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PersistR.cc,v 1.21 1995/07/03 14:29:06 ngdp Exp $
 */

/*
 *
 * PersistenceRecord Class
 *
 */

#ifndef STDDEF_H_
#  include <System/stddef.h>
#endif

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

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef PERSISTR_H_
#  include "PersistR.h"
#endif

static const char RCSid[] = "$Id: PersistR.cc,v 1.21 1995/07/03 14:29:06 ngdp Exp $";

static const size_t MAX_OBJECT_SIZE = 4096;

Boolean PersistSetUp::setUp = FALSE;

/*
 * PersistenceRecords manage old states of persistent objects. They are
 * created when a persistent object is first modified inside the scope of
 * an atomic action
 */

PersistSetUp::PersistSetUp ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "PersistSetUp::PersistSetUp ()" << endl;
    debug_stream.unlock();
    
#endif
    
    if (!setUp)
    {
        PersistenceRecord tr;
	AbstractRecord::addToList(PersistSetUp::create, PersistSetUp::remove, tr.type());
	setUp = TRUE;
    }
}

PersistSetUp::~PersistSetUp () {}

AbstractRecord* PersistSetUp::create ()
{
    return new PersistenceRecord;
}

void PersistSetUp::remove (AbstractRecord*& toDelete)
{
    delete toDelete;
    toDelete = 0;
}

/*
 * Public constructors and destructors
 */

/*
 * This constructor is used to create a new instance of
 * PersistenceRecord.
 */

PersistenceRecord::PersistenceRecord ( ObjectState *os,
				       ObjectStore *objStore,
				       StateManager *sm )
                                     : RecoveryRecord(os,sm),
                                       shadowMade(FALSE),
				       store(objStore),
				       topLevelState(0)
{
#ifdef DEBUG
    debug_stream.lock();
    
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "PersistenceRecord::PersistenceRecord("
		 << (void *)os << ", " << sm->get_uid() << ")" << endl;
    debug_stream.unlock();
    
#endif

    /* Make sure the store will not vanish */
    
    if (store)
	ObjectStore::reference(store);
}

PersistenceRecord::~PersistenceRecord ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "PersistenceRecord::~PersistenceRecord() for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    if (store)
	ObjectStore::destroy(store);

    if (topLevelState)
	delete topLevelState;
}

/*
 * Redefintions of public virtual functions inherited from RecoveryRecord
 */

RecordType PersistenceRecord::typeIs () const
{
    return PERSISTENCE;
}

/*
 * topLevelAbort may have to remove the persistent state that was written
 * into the object store during the processing of topLevelPrepare.
 * It then does the standard abort processing.
 */

Boolean PersistenceRecord::topLevelAbort ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream <<"PersistenceRecord::topLevelAbort() for " << order() << endl;
    debug_stream.unlock();
    
#endif

    if (shadowMade)
    {
	if ((topLevelState == 0) ||
	    !store->remove_uncommitted(topLevelState->stateUid(), topLevelState->type()))
	{
	    return FALSE;
	}
    }
    
    return nestedAbort();
}

/*
 * commit the state saved during the prepare phase.
 */

Boolean PersistenceRecord::topLevelCommit ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "PersistenceRecord::topLevelCommit() for " << order() << endl;
    debug_stream.unlock();
    
#endif

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "PersistenceRecord::topLevelCommit() : About to commit state, uid = "
                 << order()
                 << ", ObjType = " << getTypeOfObject();

    if (store)
	debug_stream << ", store = " << store
		     << "(" << store->type() << ")";
    
    debug_stream << endl;
    debug_stream.unlock();
    
#endif

    Boolean result = FALSE;

    if (store != 0)
    {
	if (shadowMade)
	    result = store->commit_state(order(), getTypeOfObject());
	else
	    if (topLevelState)
		result = store->write_committed(order(), getTypeOfObject(), *topLevelState);
    }

    if (!result)
    {
	addError(new Error(FAC_ABSTRACT_REC, OS_COMMIT_FAILED,
			   "PersistenceRecord::topLevelCommit - commit_state error\n"));
    }
    
    return result;
}

/*
 * topLevelPrepare attempts to save the object.
 * It will either do this in the action intention list or directly
 * in the object store by using the 'deactivate' function of the object
 * depending upon the size of the state.
 * To ensure that objects are correctly hidden while they are in an uncommitted
 * state if we use the abbreviated protocol then we write an EMPTY object state
 * as the shadow state - THIS MUST NOT BE COMMITTED. Instead we write_committed
 * the one saved in the intention list.
 * If the store cannot cope with being given an empty state we revert to the old
 * protocol
 */

PrepareOutcome PersistenceRecord::topLevelPrepare ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream <<"PersistenceRecord::topLevelPrepare() for " << order() << endl;
    debug_stream.unlock();
    
#endif

    PrepareOutcome result = PREP_NOTOK;
    StateManager *sm = getObjectAddr();
    
    if (sm && store)
    {
	topLevelState = new ObjectState(sm->get_uid(), sm->type());
	
	if (!store->fullCommitNeeded() &&
	    (sm->save_state(*topLevelState, ANDPERSISTENT)) &&
	     (topLevelState->size() <= MAX_OBJECT_SIZE))
	{
	    ObjectState *dummy = new ObjectState(sm->get_uid(), sm->type());

	    /* Write an empty shadow state to the store to indicate one exists! */
	    
	    store->write_uncommitted(sm->get_uid(), sm->type(), *dummy);
	    delete dummy;
	    
	    result = PREP_OK;
	}
	else 
	    if (sm->deactivate(store->getStoreName(), FALSE))
	    {
		shadowMade = TRUE;
		result = PREP_OK;
	    }
    }

    return result;
}

/*
 * topLevelCleanup must leave the persistent state that was written
 * into the object store during the processing of topLevelPrepare intact.
 * Crash recovery will take care of its resolution
 */

Boolean PersistenceRecord::topLevelCleanup ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream <<"PersistenceRecord::topLevelCleanup() for " << order() << endl;
    debug_stream.unlock();
    
#endif

    return TRUE;
}

Boolean PersistenceRecord::restore_state ( ObjectState& os, ObjectType ot)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream <<"PersistenceRecord::restore_state for " << order() << endl;
    debug_stream.unlock();
    
#endif

    Boolean res = FALSE;
    char* objStoreType = 0;

    if (os.unpack(objStoreType) && (objStoreType != 0))
    {
#ifdef DEBUG
	debug_stream.lock();
	
	debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
	debug_stream <<"PersistenceRecord::restore_state: "
		     << "Just unpacked object store type = "
		     << objStoreType << endl;
	debug_stream.unlock();
	
#endif

	/* discard old store before creating new */
	
	if (store)
	    ObjectStore::destroy(store);
   
	store = ObjectStore::create(objStoreType);
	
#ifdef GCC_ARRAY_DELETE_BUG
	::delete objStoreType;
#else	
	::delete [] objStoreType;
#endif

	if (store->unpackFrom(os) && os.unpack(shadowMade))
	{
	    if (topLevelState)
	    {
		delete topLevelState;
		topLevelState = 0;
	    }
	    
	    if (!shadowMade)
	    {
		topLevelState = new ObjectState();
		res = topLevelState->unpackFrom(os);
		
	    }
	    
	    return (res && RecoveryRecord::restore_state(os, ot));
	}
    }
    else
    {
	addError(new Error(FAC_ABSTRACT_REC, SM_RESTORE_FAILED,
			   "PersistenceRecord::restore_state: Failed to unpack object store type\n"));
    }
    
    return res;
}

Boolean PersistenceRecord::doSave () const
{
    return TRUE;
}

Boolean PersistenceRecord::save_state ( ObjectState& os, ObjectType ot)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream <<"PersistenceRecord::save_state() for " << order() << endl;
    debug_stream.unlock();
    
#endif
    
    Boolean res = FALSE;

    if (store)
    {
	if ((store->type() == 0) || (::strcmp(store->type(), "") == 0))
	{
	    addError(new Error(FAC_ABSTRACT_REC, SM_SAVE_FAILED,
			      "Type of store is unknown\n"));
	}
	else
	{
	    if ((res = os.packNonMappedString(store->type())))
	    {
#ifdef DEBUG
		debug_stream.lock();
		
		debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
		debug_stream <<"PersistenceRecord::restore_state: "
			     << "Packed object store type = "
			     << store->type() << endl;
		debug_stream.unlock();
		
#endif
	    
		if ((res = store->packInto(os)))
		{
#ifdef DEBUG
		    debug_stream.lock();
		    
		    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
		    debug_stream <<"PersistenceRecord::save_state: "
				 << "Packed object store root" << endl;
		    debug_stream.unlock();
		    
#endif

		    if ((res = os.pack(shadowMade)))
		    {
			if (!shadowMade)
			{
			    res = (topLevelState != 0);
			    
			    if (!res || !(res = topLevelState->packInto(os)))
			    {
				addError(new Error(FAC_ABSTRACT_REC, SM_SAVE_FAILED,
						   "Packing top level state failed\n"));
			    }
			}
		    }
		    else 
		    {
			addError(new Error(FAC_ABSTRACT_REC, SM_SAVE_FAILED,
					   "Packing shadow flag failed\n"));
		    }
		}
		else 
		{
		    addError(new Error(FAC_ABSTRACT_REC, SM_SAVE_FAILED,
				       "Packing object store root failed\n"));
		}
	    }
	    else
	    {
		addError(new Error(FAC_ABSTRACT_REC, SM_SAVE_FAILED,
				   "Packing object store type failed\n"));
	    }	
	}
    }
    else
    {
	addError(new Error(FAC_ABSTRACT_REC, SM_SAVE_FAILED,
			  "No object store defined for object\n"));
	
	os.packMappedString(0);
    }
    
    return(res && RecoveryRecord::save_state(os, ot));
}

ostream& PersistenceRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);	/* bypass RecoveryRecord */

    strm << "PersistenceRecord with state:\n" << *getState() << "\n";
    return strm;
}

const TypeName PersistenceRecord::type () const
{
    return ("/StateManager/AbstractRecord/RecoveryRecord/PersistenceRecord");
}

/*
 * Creates a 'blank' persistence record. This is used during crash recovery
 * when recreating the prepared list of a server atomic action.
 */

PersistenceRecord::PersistenceRecord ()
                                     : RecoveryRecord() ,
                                       shadowMade(FALSE),
                                       store(0),
				       topLevelState(0)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PROTECTED;
    debug_stream << "PersistenceRecord::PersistenceRecord()"
		 << "- crash recovery constructor" << endl;

    debug_stream.unlock();
    
#endif
}

/*
 * Cadaver records force write shadows.
 * This operation supresses to abbreviated commit
 * This should never return FALSE
 */

Boolean PersistenceRecord::shadowForced ()
{
    if (topLevelState == 0)
    {
	shadowMade = TRUE;
	return TRUE;
    }

    /* I've already done the abbreviated protocol so its too late */
    
    return FALSE;
}
