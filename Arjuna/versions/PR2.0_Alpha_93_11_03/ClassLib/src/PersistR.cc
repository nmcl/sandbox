/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PersistR.cc,v 1.1 1993/11/03 12:30:04 nmcl Exp $
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

static const char RCSid[] = "$Id: PersistR.cc,v 1.1 1993/11/03 12:30:04 nmcl Exp $";

/*
 * PersistenceRecords manage old states of persistent objects. They are
 * created when a persistent object is first modified inside the scope of
 * an atomic action
 */

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
                                     : RecoveryRecord(os,sm) ,
                                       shadowMade(FALSE),
				       store(objStore)
{
#ifdef DEBUG
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "PersistenceRecord::PersistenceRecord("
		 << (void *)os << ", " << (void *)sm << ")\n" << flush;
#endif
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
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "PersistenceRecord::PersistenceRecord()"
		 << "- crash recovery constructor\n" << flush;
#endif
}




PersistenceRecord::~PersistenceRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "PersistenceRecord::~PersistenceRecord()\n" << flush;
#endif
}

/*
 * Redefintions of public virtual functions inherited from RecoveryRecord
 */

RecordType PersistenceRecord::type_is () const
{
    return PERSISTENCE;
}

/*
 * top_level_abort must remove the persistent state that was written
 * into the object store during the processing of top_level_prepare.
 * It then does the standard abort processing.
 */

void PersistenceRecord::top_level_abort ()
{

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream <<"PersistenceRecord::top_level_abort()\n" << flush;
#endif

    if (shadowMade)
	store->remove_uncommitted(state->stateUid(), state->type());
    nested_abort();
}

/*
 * commit the state saved during the prepare phase.
 */

void PersistenceRecord::top_level_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "PersistenceRecord::top_level_commit()\n" << flush;
#endif

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "About to commit state, uid = "
                 << getUidOfObject()
                 << ", ObjType = " << getTypeOfObject()
                 << ", store = " << store
                 << "(" << store->type() << ")"
                 << endl;
#endif

    if (store->commit_state(getUidOfObject(),
			    getTypeOfObject()) == FALSE)
	error_stream << WARNING << 
	    "PersistenceRecord::top_level_commit - commit_state error\n";
}

/*
 * top_level_prepare attempts to save the object in the object
 * store by using the 'deactivate' function of the object. This is
 * an optimistic approach and assumes that actions normally commit.
 * Thus top_level_commit doesn't have much to do, but top_level_abort
 * must remove the state if the action aborts.
 */

PrepareOutcome PersistenceRecord::top_level_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream <<"PersistenceRecord::top_level_prepare()\n" << flush;
#endif

    if (objectAddr->deactivate(store, FALSE))
    {
	shadowMade = TRUE;
	return (PREP_OK);
    }
    else
	return (PREP_NOTOK);
}

Boolean PersistenceRecord::restore_state ( ObjectState& os, ObjectType ot)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream <<"PersistenceRecord::restore_state\n" << flush;
#endif
    char *objStoreType;

    os.unpack(objStoreType);
    store = ObjectStore::create(objStoreType);
    delete objStoreType;
    return(RecoveryRecord::restore_state(os, ot));
}

Boolean PersistenceRecord::save_state ( ObjectState& os, ObjectType ot)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream <<"PersistenceRecord::save_state\n" << flush;
#endif
    Boolean res = TRUE;

    res = os.pack(store->type());
    return(res && RecoveryRecord::save_state(os, ot));
}

ostream& PersistenceRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);	/* bypass RecoveryRecord */

    strm << "PersistenceRecord with state:\n" << *state << "\n";
    return strm;
}

const TypeName PersistenceRecord::type () const
{
    return ("/StateManager/AbstractRecord/RecoveryRecord/PersistenceRecord");
}
