/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ActiveR.cc,v 1.11 1993/03/22 09:19:27 ngdp Exp $
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

static const char RCSid[] = "$Id: ActiveR.cc,v 1.11 1993/03/22 09:19:27 ngdp Exp $";

/*
 * ActivationRecords manage activation states of objects. They are created when
 * an object is first activated inside the scope of an atomic action
 */

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
                 << ", " << objectAddr << ")\n" << flush;
#endif
}

ActivationRecord::~ActivationRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::~ActivationRecord()\n" << flush;
#endif

}

/*
 * Public virtual functions and operators
 */

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
 * should_add determines if the new record should be added in addition
 * to the existing record and is currently only invoked if both of
 * should_merge and should_replace return FALSE
 * Default implementations here always return FALSE - ie new records
 * do not override old
 */

Boolean ActivationRecord::should_add ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean ActivationRecord::should_merge ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean ActivationRecord::should_replace ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean ActivationRecord::should_alter (const AbstractRecord*) const
{
    return FALSE;
}

RecordType ActivationRecord::type_is () const
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
 * nested_abort causes the reset_state function of the object to be invoked
 * passing it the saved ObjectStatus
 */

void ActivationRecord::nested_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::nested_abort()\n" << flush;
#endif

    objectAddr->resetState(state);
}

/*
 * nested_commit does nothing since the passing of the state up to
 * the parent action is handled by the record list merging system.
 * In fact since nested_prepare returns READONLY this function should
 * never actually be called
 */

void ActivationRecord::nested_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::nested_commit()\n" << flush;
#endif

}

PrepareOutcome ActivationRecord::nested_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::nested_prepare()\n" << flush;
#endif

    return (READONLY);
}

/*
 * top_level_abort for Activation records is exactly like a nested
 * abort.
 */

void ActivationRecord::top_level_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::top_level_abort()\n" << flush;
#endif

    nested_abort();			/* ie same as nested case */
}

/*
 * top_level_commit has little to do for ActivationRecords other
 * than to ensure the object is made PASSIVE
 */

void ActivationRecord::top_level_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::top_level_commit()\n" << flush;
#endif

    switch (state)
    {
    case PASSIVE:
    case PASSIVE_NEW:
	objectAddr->resetState(PASSIVE);
	break;
    default:
	break;
    }
}

PrepareOutcome ActivationRecord::top_level_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::top_level_prepare()\n" << flush;
#endif

    return (PREP_OK);
}

/*
 * Saving of ActivationRecords is only undertaken during the Prepare
 * phase of the top level 2PC.
 */

Boolean ActivationRecord::restore_state ( ObjectState&, ObjectType )
{
    error_stream << WARNING << " Invocation of restore_state for "
	         << type() << "inappropriate - ignored\n" << flush;
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
