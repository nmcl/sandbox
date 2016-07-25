/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: ANSATermR.cc,v 1.3 1993/05/05 13:21:35 n048z Exp $
 */

static const char RCSid[] = "$Id: ANSATermR.cc,v 1.3 1993/05/05 13:21:35 n048z Exp $";

/*
 * ANSATerminateRecord Class
 *
 */

#ifdef DEBUG
#  ifndef DEBUG_H_
#    include <Common/Debug.h>
#  endif
#endif

#ifndef ANSATERMR_H_
#  include "ANSATermR.h"
#endif

/*
 * Public constructors and destructors
 */

ANSATerminateRecord::ANSATerminateRecord (const Uid& obj_uid,
						ClientANSAAction_rpc* CAA_rpc)
                                             : AbstractRecord(obj_uid, 0, ANDPERSISTENT),
					       rpc_interface(CAA_rpc)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::ANSATerminateRecord (ClientANSAAction_rpc [";
    debug_stream << CAA_rpc << "] ) on " << get_uid() << "\n" << flush;
#endif
}

ANSATerminateRecord::~ANSATerminateRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::~ANSATerminateRecord () on " << get_uid() << "\n" << flush;
#endif
}

/*
 * Public virtual functions and operators
 */

/*
 * Inherited functions from AbstractRecord
 */

void ANSATerminateRecord::merge ( const AbstractRecord *mergewith )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::merge (const AbstractRecord [" << mergewith->get_uid(); 
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif
}

void ANSATerminateRecord::alter (AbstractRecord *absRec)
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::alter( AbstractRecord [" << absRec->get_uid();
    debug_stream<< "] ) on " << get_uid() << "\n" << flush;
#endif
}

Boolean ANSATerminateRecord::should_add ( const AbstractRecord * absRec) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::should_add ( const AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean ANSATerminateRecord::should_merge ( const AbstractRecord * absRec ) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::should_merge ( const AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean ANSATerminateRecord::should_replace ( const AbstractRecord * absRec) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::should_replace ( const AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return ((absRec->type_is() == RPCINITIATE) && (absRec->ordering() == ordering()) && (absRec->value() == value()));
}

Boolean ANSATerminateRecord::should_alter (const AbstractRecord *absRec) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::should_alter( AbstractRecord [" << absRec->get_uid();
    debug_stream<< "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

RecordType ANSATerminateRecord::type_is () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::type_is () on " << get_uid() << "\n" << flush;
#endif

    return RPCTERMINATE;
}

AnyType ANSATerminateRecord::value () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::value () on " << get_uid() << "\n" << flush;
#endif

    return (AnyType) rpc_interface;
}

void ANSATerminateRecord::setValue (AnyType value)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::setValue( AnyType [" << value;
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    rpc_interface = (ClientANSAAction_rpc *) value;
}

void ANSATerminateRecord::nested_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::nested_abort () on " << get_uid() << "\n" << flush;
#endif
}

void ANSATerminateRecord::nested_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::nested_commit () on " << get_uid() << "\n" << flush;
#endif
}

PrepareOutcome ANSATerminateRecord::nested_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::nested_prepare () on " << get_uid() << "\n" << flush;
#endif

    return READONLY;
}

void ANSATerminateRecord::top_level_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::top_level_abort () on " << get_uid() << "\n" << flush;
#endif

    rpc_interface->ClientANSA_rpc::Terminate();
}

void ANSATerminateRecord::top_level_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::top_level_commit () on " << get_uid() << "\n" << flush;
#endif

    rpc_interface->ClientANSA_rpc::Terminate();
}

PrepareOutcome ANSATerminateRecord::top_level_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::top_level_prepare () on " << get_uid() << "\n" << flush;
#endif

    return PREP_OK;
}

/*
 * Inherited functions from StateManager
 */

Boolean ANSATerminateRecord::restore_state ( ObjectState&, ObjectType )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::restore_state ( ObjectState& s, ObjectType t ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean ANSATerminateRecord::save_state ( ObjectState&, ObjectType )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::save_state ( ObjectState& s, ObjectType t ) on " << get_uid() << "\n" << flush;
#endif

    return TRUE;
}

ostream& ANSATerminateRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);

//    strm << "ANSATerminateRecord  " << rpc_interface << "\n" << flush;
    return strm;
}

const TypeName ANSATerminateRecord::type () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSATerminateRecord::type () on " << get_uid() << "\n" << flush;
#endif

    return ("/StateManager/AbstractRecord/ANSATerminateRecord");
}
