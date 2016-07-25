/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CadaverR.cc,v 1.10 1994/03/08 09:19:29 ngdp Exp $
 */

/*
 *
 * Cadaver Record Class
 *
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef CADAVERLR_H_
#  include "CadaverLR.h"
#endif

#ifndef CADAVERR_H_
#  include "CadaverR.h"
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

static const char RCSid[] = "$Id: CadaverR.cc,v 1.10 1994/03/08 09:19:29 ngdp Exp $";


Boolean CadaverSetUp::setUp = FALSE;


/*
 * Cadaver records are created whenever a persistent object is deleted while
 * still in the scope of an atomic action. This ensures that if the
 * action commits the state of the persistent objects gets properly
 * reflected back in the object store. For objects that are only
 * recoverable such work is unnecessary. Cadaver records replace
 * PersistenceRecords in the record list of an atomic action so they must
 * be merged with such records to enable both commits and aborts to occur.
 */


CadaverSetUp::CadaverSetUp ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverSetUp::CadaverSetUp ()" << endl;
#endif
    
    if (!setUp)
    {
        CadaverRecord tr;
	AbstractRecord::addToList(CadaverSetUp::create, CadaverSetUp::remove, tr.type());
	setUp = TRUE;
    }
}

CadaverSetUp::~CadaverSetUp () {}

AbstractRecord* CadaverSetUp::create ()
{
    return new CadaverRecord;
}

void CadaverSetUp::remove (AbstractRecord*& toDelete)
{
    delete toDelete;
    toDelete = 0;
}


/*
 * Public constructors and destructor
 */

CadaverRecord::CadaverRecord ( ObjectState *os,
			       ObjectStore *objStore,
			       StateManager *sm )
                             : PersistenceRecord(os, objStore, sm),
			       newStateIsValid(os != 0 ? TRUE : FALSE),
			       oldState(0),
			       oType(NONE_RECORD),
			       storeUsed(0),
			       osUsed(0)
{
#ifdef DEBUG
    /* 
     * The void * casts in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverRecord::CadaverRecord(" << (void *)os << ", "
	         << sm->get_uid() << ")" << endl;
#endif
    
    size_t len = 0;
    
    if (objStore)
    {
	TypeName storeType = objStore->type();
	
	len = ::strlen(storeType);
	
	storeUsed = ::new char[len+1];
	::strcpy(storeUsed, storeType);

	if (::strcmp(storeUsed, "LocalVolatileObjectStore") == 0)
	    osUsed = ObjectStore::create(storeUsed);
    }
}

CadaverRecord::CadaverRecord ()
                              : PersistenceRecord(),
				newStateIsValid(FALSE),
				oldState(0),
				oType(NONE_RECORD),
				storeUsed(0),
				osUsed(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PROTECTED;
    debug_stream << "CadaverRecord::CadaverRecord ()" << endl;
#endif
}

CadaverRecord::~CadaverRecord ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverRecord::~CadaverRecord() for " << order() << endl;
#endif

    if (oldState)
	delete oldState;

    if (storeUsed)
#ifndef __GNUG__
	delete [] storeUsed;
#else
        delete storeUsed;
#endif

    if (osUsed)
        ObjectStore::destroy(osUsed);
}

/*
 * Public virtual functions. These are all re-implementations of inherited
 * functions
 */

/*
 * Pretend to be a PersistenceRecord
 */

Boolean CadaverRecord::propagateOnAbort () const
{
    return TRUE;
}

RecordType CadaverRecord::typeIs () const
{
    return PERSISTENCE;
}

Boolean CadaverRecord::nestedAbort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverRecord::nestedAbort() for " << order() << endl;
#endif

    if (oldState)
	newStateIsValid = FALSE;
    
    if (oType == RECOVERY)
	error_stream << WARNING << "Attempted abort operation on deleted object id "
		     << order() << " of type " << getTypeOfObject()
		     << " ignored" << endl;
    return TRUE;
}

/*
 * If we have no new state then we cannot commit and must force an
 * abort. Do this by failing the prepare phase
 */

PrepareOutcome CadaverRecord::nestedPrepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverRecord::nestedPrepare() for " << order() << endl;
#endif

    if (newStateIsValid)
	return (PersistenceRecord::nestedPrepare());
    else
	return (PREP_NOTOK);
}

Boolean CadaverRecord::topLevelAbort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverRecord::topLevelAbort() for " << order() << endl;
#endif

    newStateIsValid = FALSE;

    if (oType == RECOVERY)
	error_stream << WARNING << "Attempted abort operation on deleted object id "
		     << order() << " of type " << getTypeOfObject()
		     << " ignored" << endl;

    return TRUE;
}

/*
 * At topLevelCommit we commit the uncommitted version already saved
 * into object store.
 * Cannot use inherited version since that assumes object is alive
 * instead talk directly to the object store itself
 */

Boolean CadaverRecord::topLevelCommit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverRecord::topLevelCommit() for " << order() << endl;
#endif

    Boolean res = TRUE;
    ObjectState *state = getState();

    if (state && oType == PERSISTENCE)
    {
	ObjectStore *store = ObjectStore::create(storeUsed);
	if (store)
	{
	    res = store->commit_state(state->stateUid(), state->type());
	    ObjectStore::destroy(store);
	}
	else
	    res = FALSE;
    }

    return res;
}

/*
 * At topLevelPrepare write uncommitted version into object store.
 * Cannot use inherited version since that assumes object is alive
 * instead talk directly to the object store itself
 */

PrepareOutcome CadaverRecord::topLevelPrepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverRecord::topLevelPrepare() for " << order() << endl;
#endif

    PrepareOutcome tlpOk = PREP_NOTOK;
    ObjectState *state = (newStateIsValid ? getState() : oldState);

    if (state)
    {
	if (oType == PERSISTENCE)
	{
	    ObjectStore *store = ObjectStore::create(storeUsed);

	    if (store)
	    {
		if (store->write_state(state->stateUid(), state->type(), *state))
		    tlpOk = PREP_OK;
		ObjectStore::destroy(store);
	    }
	    else
		tlpOk = PREP_NOTOK;
	}
	else
	    tlpOk = PREP_OK;
    }
    
    return (tlpOk);
}

ostream& CadaverRecord::print (ostream& strm ) const
{
    strm << "Cadaver for:\n";
    return PersistenceRecord::print(strm);
}

const TypeName CadaverRecord::type() const
{
    return ("/StateManager/AbstractRecord/RecoveryRecord/PersistenceRecord/CadaverRecord");
}


/*
 * merge takes the information from the incoming PersistenceRecord and
 * uses it to initialise the oldState information. This is required
 * for processing of action aborts since CadaverRecords maintain the
 * final state of an object normally - which is required if the action
 * commits
 */
 
void CadaverRecord::merge ( const AbstractRecord *mergewith )
{
    /*
     *	Following assumes that value returns a pointer to the 
     *  old state maintained in the PersistenceRecord (as an ObjectState).
     *  Here we create a copy of that state allowing the original
     *  to be deleted
     */

    oType = mergewith->typeIs();
    
    if (oldState)
    {
	if (newStateIsValid)
	    delete oldState;
	else
	{ 
	    setValue(oldState);
	    newStateIsValid = TRUE;
	}
    }
    
    oldState = new ObjectState(*(ObjectState *)(mergewith->value()));
}

Boolean CadaverRecord::shouldMerge ( const AbstractRecord *ar ) const
{
    return (((order() == ar->order()) &&
	     ((ar->typeIs() == PERSISTENCE) || (ar->typeIs() == RECOVERY)))
	    ? TRUE : FALSE);
}

Boolean CadaverRecord::shouldReplace ( const AbstractRecord *ar ) const
{
    return (((order() == ar->order()) &&
	     ((ar->typeIs() == PERSISTENCE) || (ar->typeIs() == RECOVERY)))
	? TRUE : FALSE);
}
