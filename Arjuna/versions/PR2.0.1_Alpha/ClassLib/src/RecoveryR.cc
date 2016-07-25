/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecoveryR.cc,v 1.1 1993/11/03 12:30:10 nmcl Exp $
 */

/*
 *
 * RecoveryRecord Class
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

#ifndef RECOVERYR_H_
#  include "RecoveryR.h"
#endif

static const char RCSid[] = "$Id: RecoveryR.cc,v 1.1 1993/11/03 12:30:10 nmcl Exp $";

/*
 * RecoveryRecords manage old states of objects. They are created when
 * an object is first midified inside the scope of an atomic action
 */

/*
 * Public constructors and destructors
 */

/*
 * This constructor is used to create a new instance of an
 * RecoveryRecord.
 */

RecoveryRecord::RecoveryRecord ( ObjectState *os,
				 StateManager *sm )
                               : AbstractRecord(sm->get_uid(), sm->type(), ANDPERSISTENT),
				 objectAddr(sm),
				 state(os)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RecoveryRecord::RecoveryRecord(" 
                 << objectAddr << ")\n" << flush;
#endif
}


/*
 * Creates a 'blank' recovery record. This is used during crash recovery
 * when recreating the prepared list of a server atomic action.
 */

RecoveryRecord::RecoveryRecord ()
                               : AbstractRecord(),
				 objectAddr(0),
				 state(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RecoveryRecord::RecoveryRecord()" 
                 << "- crash recovery constructor\n" << flush;
#endif
}



RecoveryRecord::~RecoveryRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RecoveryRecord::~RecoveryRecord()\n" << flush;
#endif

    if (state)
        delete state;
}

/*
 * Public virtual functions and operators
 */

void RecoveryRecord::merge ( const AbstractRecord * )
{
}

void RecoveryRecord::alter ( AbstractRecord * )
{
}

/*
 * should_merge and should_replace are invoked by the record list manager
 * to determine if two records should be merged togethor or if the
 * 'newer' should replace the older.
 * should_add determines if the new record should be added in addition
 * to the existing record and is currently only invoked if both of
 * should_merge and should_replace return FALSE
 * Default implementations here always return FALSE - ie new records
 * do not override old
 */

Boolean RecoveryRecord::should_add ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean RecoveryRecord::should_merge ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean RecoveryRecord::should_replace ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean RecoveryRecord::should_alter (const AbstractRecord*) const
{
    return FALSE;
}

RecordType RecoveryRecord::type_is () const
{
    return RECOVERY;
}

AnyType RecoveryRecord::value () const
{
    return (AnyType)state;
}

void RecoveryRecord::setValue (AnyType)
{
    error_stream << WARNING << "RecoveryRecord::setValue() : called\n";
}

/*
 * nested_abort causes the restore_state function of the object to be invoked
 * passing it the saved ObjectState
 */

void RecoveryRecord::nested_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RecoveryRecord::nested_abort()\n" << flush;
#endif

    /* 
     * First check that we have a state. We won't have for records
     * created by crash recovery.
     */

    if (state)
        if (state->notempty())		/* anything to restore ? */
	    objectAddr->restore_state(*state, RECOVERABLE);
}

/*
 * nested_commit does nothing since the passing of the state up to
 * the parent action is handled by the record list merging system.
 * In fact since nested_prepare returns READONLY this function should
 * never actaully be called
 */

void RecoveryRecord::nested_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RecoveryRecord::nested_commit()\n" << flush;
#endif
}

PrepareOutcome RecoveryRecord::nested_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RecoveryRecord::nested_prepare()\n" << flush;
#endif

    return (READONLY);
}

/*
 * top_level_abort for Recovery records implies the object state
 * should be restored to the saved state exactly like a nested
 * abort.
 */

void RecoveryRecord::top_level_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RecoveryRecord::top_level_abort()\n" << flush;
#endif

    nested_abort();			/* ie same as nested case */
}

/*
 * top_level_commit has nothing to do for RecoveryRecords as no changes
 * have been made in the object store. In fact since top_level_prepare
 * returns READONLY this function should never actually be called
 */

void RecoveryRecord::top_level_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RecoveryRecord::top_level_commit()\n" << flush;
#endif

}

/*
 * top_level_prepare can return READONLY to avoid top_level_commit
 * being called in the action commit case
 */

PrepareOutcome RecoveryRecord::top_level_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RecoveryRecord::top_level_prepare()\n" << flush;
#endif

    return (READONLY);
}

/*
 * Saving of RecoveryRecords is only undertaken during the Prepare
 * phase of the top level 2PC. Since the managed objects are only
 * recoverable (not persistent) there is no need to save any
 * information (or restore any either).
 * However, persistence records (derived from recovery records) need
 * to be saved for crash recovery purposes.
 */

/*
 * There are two versions of save_state and restore_state here. The
 * default versions calling AbstractRecord save_state and restore_state
 * cause problems with g++. The alternative versions use the public methods
 * setUidOfObject and setTypeOfObject of AbstractRecord. These routines
 * are only available when building with g++.
 * We shouldn't need to do this !
 */

Boolean RecoveryRecord::restore_state ( ObjectState& os, ObjectType ot)
{
#ifdef __GNUG__
    Uid newUid(NIL_UID);
    TypeName newType;
    Boolean res = TRUE;

    res = ( newUid.unpack(os) && os.unpack(newType) );

    if (res)
    {
        setUidOfObject(newUid);
        setTypeOfObject(newType);
    }

    return(res);

#else
    return (AbstractRecord::restore_state(os, ot));
#endif
}

Boolean RecoveryRecord::save_state ( ObjectState& os, ObjectType ot)
{
#ifdef __GNUG__
    return ( (getUidOfObject().pack(os)
             && os.pack(getTypeOfObject())) ? TRUE
                                            : FALSE );
#else
    return (AbstractRecord::save_state(os, ot));
#endif
}

ostream& RecoveryRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);

    strm << "RecoveryRecord with state:\n" << *state << "\n";
    return strm;
}

const TypeName RecoveryRecord::type () const
{
    return ("/StateManager/AbstractRecord/RecoveryRecord");
}
