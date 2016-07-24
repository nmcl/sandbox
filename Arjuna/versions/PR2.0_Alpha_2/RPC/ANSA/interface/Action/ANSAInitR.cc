/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: ANSAInitR.cc,v 1.1 1993/11/03 14:38:30 nmcl Exp $
 */

static const char RCSid[] = "$Id: ANSAInitR.cc,v 1.1 1993/11/03 14:38:30 nmcl Exp $";

/*
 * ANSAInitiateRecord Class
 *
 */

#ifdef DEBUG
#  ifndef DEBUG_H_
#    include <Common/Debug.h>
#  endif
#endif

#ifndef ANSAINITR_H_
#  include "ANSAInitR.h"
#endif

/*
 * Public constructors and destructors
 */

ANSAInitiateRecord::ANSAInitiateRecord (const Uid& obj_uid,
					      ClientANSAAction_rpc* CRA_rpc)
                                             : AbstractRecord(obj_uid, 0, ANDPERSISTENT),
					       rpc_interface(CRA_rpc)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::ANSAInitiateRecord (ClientANSA_rpc [";
    debug_stream << CRA_rpc << "] ) on " << get_uid() << "\n" << flush;
#endif
}

ANSAInitiateRecord::~ANSAInitiateRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::~ANSAInitiateRecord () on " << get_uid() << "\n" << flush;
#endif
}

/*
 * Public virtual functions and operators
 */

/*
 * Inherited functions from AbstractRecord
 */

void ANSAInitiateRecord::merge ( const AbstractRecord * mergewith)
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::merge ( const AbstractRecord [" << mergewith->get_uid(); 
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif
}

void ANSAInitiateRecord::alter (AbstractRecord *absRec)
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::alter( AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif
}

Boolean ANSAInitiateRecord::should_add ( const AbstractRecord * absRec) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::should_add (AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean ANSAInitiateRecord::should_merge ( const AbstractRecord * absRec ) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::should_merge (AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean ANSAInitiateRecord::should_replace ( const AbstractRecord * absRec ) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::should_replace (AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean ANSAInitiateRecord::should_alter (const AbstractRecord *absRec) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::should_alter( AbstractRecord [" << absRec->get_uid();
    debug_stream<< "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

RecordType ANSAInitiateRecord::type_is () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::type_is () on " << get_uid() << "\n" << flush;
#endif

    return RPCINITIATE;
}

AnyType ANSAInitiateRecord::value () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::value () on " << get_uid() << "\n" << flush;
#endif

    return (AnyType) rpc_interface;
}

void ANSAInitiateRecord::setValue (AnyType value)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::setValue( AnyType [" << value;
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    rpc_interface = (ClientANSAAction_rpc *) value;
}

void ANSAInitiateRecord::nested_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::nested_abort () on " << get_uid() << "\n" << flush;
#endif

     rpc_interface->ClientANSA_rpc::Terminate();
}

void ANSAInitiateRecord::nested_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::nested_commit () on " << get_uid() << "\n" << flush;
#endif
}

PrepareOutcome ANSAInitiateRecord::nested_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::nested_prepare () on " << get_uid() << "\n" << flush;
#endif

    return PREP_OK;
}

void ANSAInitiateRecord::top_level_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::top_level_abort () on " << get_uid() << "\n" << flush;
#endif

     rpc_interface->ClientANSA_rpc::Terminate();
}

void ANSAInitiateRecord::top_level_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::top_level_commit () on " << get_uid() << "\n" << flush;
#endif
}

PrepareOutcome ANSAInitiateRecord::top_level_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::top_level_prepare () on " << get_uid() << "\n" << flush;
#endif

    return PREP_OK;
}

/*
 * Inherited functions from StateManager
 */

Boolean ANSAInitiateRecord::restore_state ( ObjectState&, ObjectType )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::restore_state ( ObjectState& s, ObjectType t ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean ANSAInitiateRecord::save_state ( ObjectState&, ObjectType )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::save_state ( ObjectState& s, ObjectType t )\n" << flush;
#endif

    return TRUE;
}

ostream& ANSAInitiateRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);

//    strm << "ANSAInitiateRecord " <<  rpc_interface << "\n" << flush;
    return strm;
}

const TypeName ANSAInitiateRecord::type () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSAInitiateRecord::type () on " << get_uid() << "\n" << flush;
#endif

    return ("/StateManager/AbstractRecord/ANSAInitiateRecord");
}
