/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ANSACallR.cc,v 1.1 1993/11/03 14:38:29 nmcl Exp $
 */

static const char RCSid[] = "$Id: ANSACallR.cc,v 1.1 1993/11/03 14:38:29 nmcl Exp $";

/*
 * ANSACallRecord Class
 *
 */

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifdef DEBUG
#  ifndef DEBUG_H_
#     include <Common/Debug.h>
#  endif
#endif

#ifndef ANSACALLR_H_
#  include "ANSACallR.h"
#endif

/*
 * Public constructors and destructors
 */

ANSACallRecord::ANSACallRecord ( const Uid& obj_uid,
				       ClientANSAAction_rpc* CRA_rpc)
                                     : AbstractRecord(obj_uid,0,ANDPERSISTENT),
				       rpc_interface(CRA_rpc)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::ANSACallRecord (ClientANSAAction_rpc [";
    debug_stream << CRA_rpc << "] ) on " << get_uid() << "\n" << flush;
#endif
}

ANSACallRecord::~ANSACallRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::~ANSACallRecord () on " << get_uid() << "\n" << flush;
#endif
}

/*
 * Public virtual functions and operators
 */

/*
 * Inherited functions from AbstractRecord
 */

void ANSACallRecord::merge ( const AbstractRecord *mergewith )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::merge ( const AbstractRecord [" << mergewith->get_uid(); 
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif
}

void ANSACallRecord::alter (AbstractRecord *absRec)
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::alter( AbstractRecord [" << absRec->get_uid();
    debug_stream<< "] ) on " << get_uid() << "\n" << flush;
#endif
}

Boolean ANSACallRecord::should_add ( const AbstractRecord * absRec) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::should_add ( const AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean ANSACallRecord::should_merge ( const AbstractRecord * absRec) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::should_merge ( const AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean ANSACallRecord::should_replace ( const AbstractRecord * absRec) const
{
#ifdef DEBUG 
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::should_replace ( const AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean ANSACallRecord::should_alter (const AbstractRecord *absRec) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::should_alter( AbstractRecord [" << absRec->get_uid();
    debug_stream<< "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

RecordType ANSACallRecord::type_is () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::type_is () on " << get_uid() << "\n" << flush;
#endif

    return RPCCALL;
}

AnyType ANSACallRecord::value () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::value () on " << get_uid() << "\n" << flush;
#endif

    return (AnyType) rpc_interface;
}

void ANSACallRecord::setValue (AnyType value)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::setValue( AnyType [" << value;
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    rpc_interface = (ClientANSAAction_rpc *) value;
}

void ANSACallRecord::nested_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::nested_abort () on " << get_uid() << "\n" << flush;
#endif

     rpc_interface->DoRemote_NestedAbort();
}

void ANSACallRecord::nested_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::nested_commit () on " << get_uid() << "\n" << flush;
#endif

    rpc_interface->DoRemote_NestedCommit();
}

PrepareOutcome ANSACallRecord::nested_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::nested_prepare () on " << get_uid() << "\n" << flush;
#endif

    return rpc_interface->DoRemote_NestedPrepare();
}

void ANSACallRecord::top_level_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::top_level_abort () on " << get_uid() << "\n" << flush;
#endif

    rpc_interface->DoRemote_TopLevelAbort();
}

void ANSACallRecord::top_level_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::top_level_commit () on " << get_uid() << "\n" << flush;
#endif

    rpc_interface->DoRemote_TopLevelCommit();
}

PrepareOutcome ANSACallRecord::top_level_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::top_level_prepare () on " << get_uid() << "\n" << flush;
#endif

    return rpc_interface->DoRemote_TopLevelPrepare();
}

/*
 * Inherited functions from StateManager
 */

Boolean ANSACallRecord::restore_state ( ObjectState&, ObjectType)
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::restore_state ( ObjectState& s, ObjectType t ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean ANSACallRecord::save_state ( ObjectState& s, ObjectType)
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::save_state ( ObjectState& s, ObjectType t ) on " << get_uid() << "\n" << flush;
#endif

    return (get_uid().pack(s) && s.pack(type()));
}

ostream& ANSACallRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);

//    strm << "ANSACallRecord  " << rpc_interface << "\n";
    return strm;
}

const TypeName ANSACallRecord::type () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACallRecord::type () on " << get_uid() << "\n" << flush;
#endif

     return ("/StateManager/AbstractRecord/ANSACallRecord");
}
