/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RajTermR.cc,v 1.9 1993/03/19 15:46:41 ngdp Exp $
 */

static const char RCSid[] = "$Id: RajTermR.cc,v 1.9 1993/03/19 15:46:41 ngdp Exp $";

/*
 * RajdootTerminateRecord Class
 *
 */

#ifdef DEBUG
#  ifndef RDEBUG_H_
#    include <Common/Debug.h>
#  endif
#endif

#ifndef RAJTERMR_H_
#  include "RajTermR.h"
#endif

/*
 * Public constructors and destructors
 */

RajdootTerminateRecord::RajdootTerminateRecord (const Uid& obj_uid,
						ClientRajdootAction_rpc* CRA_rpc)
                                             : AbstractRecord(obj_uid, 0, ANDPERSISTENT),
					       rpc_interface(CRA_rpc)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::RajdootTerminateRecord (ClientRajdootAction_rpc [";
    debug_stream << CRA_rpc << "] ) on " << get_uid() << "\n" << flush;
#endif
}

RajdootTerminateRecord::~RajdootTerminateRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::~RajdootTerminateRecord () on " << get_uid() << "\n" << flush;
#endif
}

/*
 * Public virtual functions and operators
 */

/*
 * Inherited functions from AbstractRecord
 */

void RajdootTerminateRecord::merge ( const AbstractRecord *mergewith )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::merge (const AbstractRecord [" << mergewith->get_uid(); 
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif
}

void RajdootTerminateRecord::alter (AbstractRecord *absRec)
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::alter( AbstractRecord [" << absRec->get_uid();
    debug_stream<< "] ) on " << get_uid() << "\n" << flush;
#endif
}

Boolean RajdootTerminateRecord::should_add ( const AbstractRecord * absRec) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::should_add ( const AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean RajdootTerminateRecord::should_merge ( const AbstractRecord * absRec ) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::should_merge ( const AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean RajdootTerminateRecord::should_replace ( const AbstractRecord * absRec) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::should_replace ( const AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return ((absRec->type_is() == RPCINITIATE) && (absRec->ordering() == ordering()) && (absRec->value() == value()));
}

Boolean RajdootTerminateRecord::should_alter (const AbstractRecord *absRec) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::should_alter( AbstractRecord [" << absRec->get_uid();
    debug_stream<< "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

RecordType RajdootTerminateRecord::type_is () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::type_is () on " << get_uid() << "\n" << flush;
#endif

    return RPCTERMINATE;
}

AnyType RajdootTerminateRecord::value () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::value () on " << get_uid() << "\n" << flush;
#endif

    return (AnyType) rpc_interface;
}

void RajdootTerminateRecord::setValue (AnyType value)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::setValue( AnyType [" << value;
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    rpc_interface = (ClientRajdootAction_rpc *) value;
}

void RajdootTerminateRecord::nested_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::nested_abort () on " << get_uid() << "\n" << flush;
#endif
}

void RajdootTerminateRecord::nested_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::nested_commit () on " << get_uid() << "\n" << flush;
#endif
}

PrepareOutcome RajdootTerminateRecord::nested_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::nested_prepare () on " << get_uid() << "\n" << flush;
#endif

    return READONLY;
}

void RajdootTerminateRecord::top_level_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::top_level_abort () on " << get_uid() << "\n" << flush;
#endif

    rpc_interface->ClientRajdoot_rpc::Terminate();
}

void RajdootTerminateRecord::top_level_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::top_level_commit () on " << get_uid() << "\n" << flush;
#endif

    rpc_interface->ClientRajdoot_rpc::Terminate();
}

PrepareOutcome RajdootTerminateRecord::top_level_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::top_level_prepare () on " << get_uid() << "\n" << flush;
#endif

    return PREP_OK;
}

/*
 * Inherited functions from StateManager
 */

Boolean RajdootTerminateRecord::restore_state ( ObjectState&, ObjectType )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::restore_state ( ObjectState& s, ObjectType t ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean RajdootTerminateRecord::save_state ( ObjectState&, ObjectType )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::save_state ( ObjectState& s, ObjectType t ) on " << get_uid() << "\n" << flush;
#endif

    return TRUE;
}

ostream& RajdootTerminateRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);

//    strm << "RajdootTerminateRecord  " << rpc_interface << "\n" << flush;
    return strm;
}

const TypeName RajdootTerminateRecord::type () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::type () on " << get_uid() << "\n" << flush;
#endif

    return ("/StateManager/AbstractRecord/RajdootTerminateRecord");
}
