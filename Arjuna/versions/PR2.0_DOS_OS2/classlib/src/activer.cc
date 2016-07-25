/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ActiveR.cc,v 1.4 1993/12/14 11:30:59 nmcl Exp $
 */

/*
 *
 * ActivationRecord Class
 *
 */

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#if defined(DEBUG ) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef ACTIVER_H_
#  include "ActiveR.h"
#endif



static const char RCSid[] = "$Id: ActiveR.cc,v 1.4 1993/12/14 11:30:59 nmcl Exp $";


/*
 * Public constructors and destructors
 */

/*
 * This constructor is used to create a new instance of an
 * ActivationRecord.
 */

ActivationRecord::ActivationRecord ( ObjectStatus st,
				     StateManager *sm )
                                   : AbstractRecord(sm->get_uid(), sm->type(), ANDPERSISTENT),
				     objectAddr(sm),
				     state(st)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::ActivationRecord(" << state
                 << ", " << sm->get_uid() << ")" << endl;
#endif
}

ActivationRecord::ActivationRecord ()
                                    : AbstractRecord(),
				      objectAddr(0),
				      state(PASSIVE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PROTECTED;
    debug_stream << "ActivationRecord::ActivationRecord ()" << endl;
#endif
}

ActivationRecord::~ActivationRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::~ActivationRecord() for " << order() << endl;
#endif

}

/*
 * Public virtual functions and operators
 */

RecordType ActivationRecord::typeIs () const
{
    return ACTIVATION;
}

AnyType ActivationRecord::value () const
{
    return (AnyType)state;
}

void ActivationRecord::setValue (AnyType)
{
    error_stream << WARNING << "ActivationRecord::set_value() : called\n";
}

/*
 * nestedAbort causes the reset_state function of the object to be invoked
 * passing it the saved ObjectStatus
 */

Boolean ActivationRecord::nestedAbort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::nestedAbort() for " << order() << endl;
#endif

    if (objectAddr)
	return objectAddr->resetState(state);

    return TRUE;
}

/*
 * nestedCommit does nothing since the passing of the state up to
 * the parent action is handled by the record list merging system.
 * In fact since nested_prepare returns READONLY this function should
 * never actually be called
 */

Boolean ActivationRecord::nestedCommit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::nestedCommit() for " << order() << endl;
#endif

    return TRUE;
}

PrepareOutcome ActivationRecord::nestedPrepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::nestedPrepare() for " << order() << endl;
#endif

    return (READONLY);
}

/*
 * topLevelAbort for Activation records is exactly like a nested
 * abort.
 */

Boolean ActivationRecord::topLevelAbort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::topLevelAbort() for " << order() << endl;
#endif

    return nestedAbort();		/* ie same as nested case */
}

/*
 * topLevelCommit has little to do for ActivationRecords other
 * than to ensure the object is made PASSIVE
 */

Boolean ActivationRecord::topLevelCommit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::topLevelCommit() for " 
		 << order() << endl;
#endif

    switch (state)
    {
    case PASSIVE:
    case PASSIVE_NEW:
	if (objectAddr)
	    return objectAddr->resetState(PASSIVE);
	break;
    default:
	break;
    }
    return TRUE;
    
}

PrepareOutcome ActivationRecord::topLevelPrepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::topLevelPrepare() for " 
		 << order() << endl;
#endif

    if ((objectAddr == 0) || (objectAddr->status() == state))
	return READONLY;
    else
	return (PREP_OK);
}

/*
 * Saving of ActivationRecords is only undertaken during the Prepare
 * phase of the top level 2PC.
 */

Boolean ActivationRecord::restore_state ( ObjectState&, ObjectType )
{
    error_stream << WARNING << " Invocation of restore_state for "
	         << type() << "inappropriate - ignored for " << order() << endl;
    return FALSE;
}

Boolean ActivationRecord::save_state ( ObjectState&, ObjectType )
{
    return (TRUE);
}

ostream& ActivationRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);

    strm << "ActivationRecord with state:\n" << state << "\n";
    return strm;
}

const TypeName ActivationRecord::type () const
{
    return ("/StateManager/AbstractRecord/ActivationRecord");
}

void ActivationRecord::merge ( const AbstractRecord * )
{
}

void ActivationRecord::alter (AbstractRecord *)
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

Boolean ActivationRecord::shouldAdd ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean ActivationRecord::shouldAlter (const AbstractRecord*) const
{
    return FALSE;
}

Boolean ActivationRecord::shouldMerge ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean ActivationRecord::shouldReplace ( const AbstractRecord * ) const
{
    return FALSE;
}
