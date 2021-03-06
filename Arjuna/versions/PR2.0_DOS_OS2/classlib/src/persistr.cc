/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PersistR.cc,v 1.4 1993/12/14 11:31:08 nmcl Exp $
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

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef PERSISTR_H_
#  include "PersistR.h"
#endif



static const char RCSid[] = "$Id: PersistR.cc,v 1.4 1993/12/14 11:31:08 nmcl Exp $";


/*
 * Public constructors and destructors
 */

/*
 * This constructor is used to create a new instance of
 * PersistenceRecord.
 */


PersistenceRecord::PersistenceRecord ( StateManager* sm )
                                     : RecoveryRecord(sm) ,
                                       shadowMade(FALSE),
				       store(0)
{
}

void PersistenceRecord::setState (ObjectState* os)
{
    RecoveryRecord::setState(os);
}

void PersistenceRecord::setStore (ObjectStore* objStore)
{
    store = objStore;
}


/*
 * Creates a 'blank' persistence record. This is used during crash recovery
 * when recreating the prepared list of a server atomic action.
 */

PersistenceRecord::PersistenceRecord ( )
                                     : RecoveryRecord() ,
                                       shadowMade(FALSE),
                                       store(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PROTECTED;
    debug_stream << "PersistenceRecord::PersistenceRecord()"
		 << "- crash recovery constructor" << endl;
#endif
}

PersistenceRecord::~PersistenceRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "PersistenceRecord::~PersistenceRecord() for " 
		 << order() << endl;
#endif
}

/*
 * Redefintions of public virtual functions inherited from RecoveryRecord
 */

RecordType PersistenceRecord::typeIs () const
{
    return PERSISTENCE;
}

/*
 * topLevelAbort must remove the persistent state that was written
 * into the object store during the processing of topLevelPrepare.
 * It then does the standard abort processing.
 */

Boolean PersistenceRecord::topLevelAbort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream <<"PersistenceRecord::topLevelAbort() for " << order() << endl;
#endif

    if (shadowMade)
    {
	ObjectState *state = getState();
	
	if ((state == 0) ||
	    !store->remove_uncommitted(state->stateUid(), state->type()))
	    return FALSE;
    }
    
    return nestedAbort();
}

/*
 * commit the state saved during the prepare phase.
 */

Boolean PersistenceRecord::topLevelCommit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "PersistenceRecord::topLevelCommit() for " << order() << endl;
#endif

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "PersistenceRecord::topLevelCommit() : About to commit state, uid = "
                 << order()
                 << ", ObjType = " << getTypeOfObject();

    if (store)
	debug_stream << ", store = " << store
		     << "(" << store->type() << ")";
    
    debug_stream << endl;
#endif

    if ((store == 0) ||
	(store->commit_state(order(),
			     getTypeOfObject()) == FALSE))
    {
	error_stream << WARNING 
		     << "PersistenceRecord::topLevelCommit - commit_state error\n";
	return FALSE;
    }
    
    return TRUE;
}

/*
 * topLevelPrepare attempts to save the object in the object
 * store by using the 'deactivate' function of the object. This is
 * an optimistic approach and assumes that actions normally commit.
 * Thus topLevelCommit doesn't have much to do, but topLevelAbort
 * must remove the state if the action aborts.
 */

PrepareOutcome PersistenceRecord::topLevelPrepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream <<"PersistenceRecord::topLevelPrepare() for " << order() << endl;
#endif

    StateManager *sm = getObjectAddr();
    
    if (sm && store && sm->deactivate(store, FALSE))
    {
	shadowMade = TRUE;
	return (PREP_OK);
    }
    else
	return (PREP_NOTOK);
}

void PersistenceRecord::setStore (char* tn)
{
    store = ObjectStore::create(tn);
}

Boolean PersistenceRecord::restore_state ( ObjectState& os, ObjectType ot)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream <<"PersistenceRecord::restore_state() for " << order() << endl;
#endif
    char *objStoreType = 0;

    os.unpack(objStoreType);

    setStore(objStoreType);

    delete objStoreType;
    return(RecoveryRecord::restore_state(os, ot));
}

Boolean PersistenceRecord::doSave () const
{
    return TRUE;
}

Boolean PersistenceRecord::packStore (ObjectState& os) const
{
    if (store)
	return os.pack(store->type());
    else
	return os.pack((char*) 0);
}
	
Boolean PersistenceRecord::save_state ( ObjectState& os, ObjectType ot)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream <<"PersistenceRecord::save_state() for " << order() << endl;
#endif
    Boolean res = TRUE;

	res = packStore(os);
    
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
