/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CadaverR.cc,v 1.1 1993/11/03 12:29:49 nmcl Exp $
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

static const char RCSid[] = "$Id: CadaverR.cc,v 1.1 1993/11/03 12:29:49 nmcl Exp $";

/*
 * Cadaver records are created whenever a persistent object is deleted while
 * still in the scope of an atomic action. This ensures that if the
 * action commits the state of the persistent objects gets properly
 * reflected back in the object store. For objects that are only
 * recoverable such work is unnecessary. Cadaver records replace
 * PersistenceRecords in the record list of an atomic action so they must
 * be merged with such records to enable both commits and aborts to occur.
 */

/*
 * Public constructors and destructor
 */

CadaverRecord::CadaverRecord ( ObjectState *os,
			       ObjectStore *objStore,
			       const char *storeLoc,
			       StateManager *sm )
                             : PersistenceRecord(os, objStore, sm),
			       oldState(0),
			       oType(NONE_RECORD),
			       storeLocation(0),
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
	         << (void *)sm << ")\n" << flush;
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
    
    if (storeLoc)
    {
	len = ::strlen(storeLoc);
	
	if (len > 0)
	{
	    storeLocation = new char[len+1];
	    ::strcpy(storeLocation,storeLoc);
	}
    }
}

CadaverRecord::~CadaverRecord ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverRecord::~CadaverRecord()\n" << flush;
#endif

    if (oldState)
	delete oldState;
    if (storeLocation)
	delete storeLocation;
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
 * merge takes the information from the incoming PersistenceRecord and
 * uses it to initialise the oldState information. This is required
 * for processing of action aborts since CadaverRecords maintain the
 * final state of an object normally - which is required if the action
 * commits
 */
 
void CadaverRecord::merge ( const AbstractRecord *mergewith )
{
#ifdef DEBUG
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */


    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverRecord::merge(" << (void *)mergewith << ")\n" << flush;
#endif

    /*
     *	Following assumes that value returns a pointer to the 
     *  old state maintained in the PersistenceRecord (as an ObjectState).
     *  Here we create a copy of that state allowing the original
     * to be deleted
     */

    oType = mergewith->type_is();
    oldState = new ObjectState(*(ObjectState *)(mergewith->value()));
}

Boolean CadaverRecord::should_merge ( const AbstractRecord *ar ) const
{
#ifdef DEBUG
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverRecord::should_merge(" << (void *)ar << ")\n" << flush;
#endif

    return (((ordering() == ar->ordering()) &&
	     ((ar->type_is() == PERSISTENCE) || (ar->type_is() == RECOVERY)))
	    ? TRUE : FALSE);
}

Boolean CadaverRecord::should_replace ( const AbstractRecord *ar ) const
{
#ifdef DEBUG
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverRecord::should_replace(" << (void *)ar << ")\n" << flush;
#endif

    return (((ordering() == ar->ordering()) &&
	     ((ar->type_is() == PERSISTENCE) || (ar->type_is() == RECOVERY)))
	? TRUE : FALSE);
}

/*
 * Pretend to be a PersistenceRecord
 */

RecordType CadaverRecord::type_is () const
{
    return PERSISTENCE;
}

void CadaverRecord::nested_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverRecord::nested_abort()\n" << flush;
#endif

    if (oType == RECOVERY)
	error_stream << WARNING << "Attempted abort operation on deleted object id "
		     << getUidOfObject() << " of type " << getTypeOfObject()
		     << " ignored\n" << flush;
}

void CadaverRecord::top_level_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverRecord::top_level_abort()\n" << flush;
#endif

    if (oType == RECOVERY)
	error_stream << WARNING << "Attempted abort operation on deleted object id "
		     << getUidOfObject() << " of type " << getTypeOfObject()
		     << " ignored\n" << flush;
}

/*
 * At top_level_commit we commit the uncommitted version already saved
 * into object store.
 * Cannot use inherited version since that assumes object is alive
 * instead talk directly to the object store itself
 */

void CadaverRecord::top_level_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverRecord::top_level_commit()\n" << flush;
#endif

    if (oType == PERSISTENCE)
    {
	ObjectStore *store = ObjectStore::create(storeUsed);
    
	store->commit_state(state->stateUid(), state->type());

        ObjectStore::destroy(store);
    }
}

/*
 * At top_level_prepare write uncommitted version into object store.
 * Cannot use inherited version since that assumes object is alive
 * instead talk directly to the object store itself
 */

PrepareOutcome CadaverRecord::top_level_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverRecord::top_level_prepare()\n" << flush;
#endif

    PrepareOutcome tlpOk = PREP_NOTOK;

    if (oType == PERSISTENCE)
    {
	ObjectStore *store = ObjectStore::create(storeUsed);
    
	if (store->write_state(state->stateUid(), state->type(), *state))
	    tlpOk = PREP_OK;
    
        ObjectStore::destroy(store);
    }
    else
	tlpOk = PREP_OK;
    
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
