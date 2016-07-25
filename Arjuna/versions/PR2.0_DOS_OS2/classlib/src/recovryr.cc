/*
 * Copyright (C) 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecoveryR.cc,v 1.7 1994/01/20 12:51:27 ngdp Exp $
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

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
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
#  include "RecovryR.h"
#endif


static const char RCSid[] = "$Id: RecoveryR.cc,v 1.7 1994/01/20 12:51:27 ngdp Exp $";


/*
 * Public constructors and destructors
 */

/*
 * This constructor is used to create a new instance of an
 * RecoveryRecord.
 */


RecoveryRecord::RecoveryRecord ( StateManager* sm )
                               : AbstractRecord(sm->get_uid(), sm->type(), ANDPERSISTENT),
				 objectAddr(sm),
				 state(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RecoveryRecord::RecoveryRecord(" 
		 << (void *) os << ", "
                 << sm->get_uid() << ")" << endl;
#endif
}

void RecoveryRecord::setState (ObjectState* os)
{
    state = os;
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
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PROTECTED;
    debug_stream << "RecoveryRecord::RecoveryRecord()" 
                 << "- crash recovery constructor" << endl;
#endif
}



RecoveryRecord::~RecoveryRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RecoveryRecord::~RecoveryRecord() for " << order() << endl;
#endif

    if (state)
        delete state;
}

/*
 * Public virtual functions and operators
 */

RecordType RecoveryRecord::typeIs () const
{
    return RECOVERY;
}

AnyType RecoveryRecord::value () const
{
    return (AnyType)state;
}

void RecoveryRecord::setValue ( AnyType newState )
{
    if (state)
		delete state;
    
    state = (ObjectState *)newState;
}

/*
 * nestedAbort causes the restore_state function of the object to be invoked
 * passing it the saved ObjectState
 */

Boolean RecoveryRecord::nestedAbort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RecoveryRecord::nestedAbort() for " << order() << endl;
#endif

    /* 
     * First check that we have a state. We won't have for records
     * created by crash recovery.
     */

    if (state)
        if (state->notempty())		/* anything to restore ? */
			return objectAddr->restore_state(*state, RECOVERABLE);

    return TRUE;
}

/*
 * nestedCommit does nothing since the passing of the state up to
 * the parent action is handled by the record list merging system.
 * In fact since nestedPrepare returns READONLY this function should
 * never actaully be called
 */

Boolean RecoveryRecord::nestedCommit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RecoveryRecord::nestedCommit() for " << order() << endl;
#endif

    return TRUE;
}

PrepareOutcome RecoveryRecord::nestedPrepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RecoveryRecord::nestedPrepare() for " << order() << endl;
#endif

    return (READONLY);
}

/*
 * topLevelAbort for Recovery records implies the object state
 * should be restored to the saved state exactly like a nested
 * abort.
 */

Boolean RecoveryRecord::topLevelAbort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RecoveryRecord::topLevelAbort() for " << order() << endl;
#endif

    return nestedAbort();		/* ie same as nested case */
}

/*
 * topLevelCommit has nothing to do for RecoveryRecords as no changes
 * have been made in the object store. In fact since topLevelPrepare
 * returns READONLY this function should never actually be called
 */

Boolean RecoveryRecord::topLevelCommit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RecoveryRecord::topLevelCommit() for " << order() << endl;
#endif

    return TRUE;
}

/*
 * topLevelPrepare can return READONLY to avoid topLevelCommit
 * being called in the action commit case
 */

PrepareOutcome RecoveryRecord::topLevelPrepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RecoveryRecord::topLevelPrepare() for " << order() << endl;
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

Boolean RecoveryRecord::doSave () const
{
    return TRUE;
}

Boolean RecoveryRecord::save_state ( ObjectState& os, ObjectType ot)
{
#ifdef __GNUG__
    return ( (order().pack(os)
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

/*
 * Private operations
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
 * shouldAdd determines if the new record should be added in addition
 * to the existing record and is currently only invoked if both of
 * should_merge and should_replace return FALSE
 * Default implementations here always return FALSE - ie new records
 * do not override old
 */

Boolean RecoveryRecord::shouldAdd ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean RecoveryRecord::shouldAlter (const AbstractRecord*) const
{
    return FALSE;
}

Boolean RecoveryRecord::shouldMerge ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean RecoveryRecord::shouldReplace ( const AbstractRecord * ) const
{
    return FALSE;
}

#ifdef NO_INLINES
#  define RECOVERYR_CC_
#  include "RecovryR.n"
#  undef RECOVERYR_CC_
#endif
