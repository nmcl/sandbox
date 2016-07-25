/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RajCallR.cc,v 1.10 1993/03/19 15:46:35 ngdp Exp $
 */

static const char RCSid[] = "$Id: RajCallR.cc,v 1.10 1993/03/19 15:46:35 ngdp Exp $";

/*
 * RajdootCallRecord Class
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

#ifndef RAJCALLR_H_
#  include "RajCallR.h"
#endif

/*
 * Public constructors and destructors
 */

RajdootCallRecord::RajdootCallRecord ( const Uid& obj_uid,
				       ClientRajdootAction_rpc* CRA_rpc)
                                     : AbstractRecord(obj_uid,0,ANDPERSISTENT),
				       rpc_interface(CRA_rpc)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::RajdootCallRecord (ClientRajdootAction_rpc [";
    debug_stream << CRA_rpc << "] ) on " << get_uid() << "\n" << flush;
#endif
}

RajdootCallRecord::~RajdootCallRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::~RajdootCallRecord () on " << get_uid() << "\n" << flush;
#endif
}

/*
 * Public virtual functions and operators
 */

/*
 * Inherited functions from AbstractRecord
 */

void RajdootCallRecord::merge ( const AbstractRecord *mergewith )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::merge ( const AbstractRecord [" << mergewith->get_uid(); 
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif
}

void RajdootCallRecord::alter (AbstractRecord *absRec)
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::alter( AbstractRecord [" << absRec->get_uid();
    debug_stream<< "] ) on " << get_uid() << "\n" << flush;
#endif
}

Boolean RajdootCallRecord::should_add ( const AbstractRecord * absRec) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::should_add ( const AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean RajdootCallRecord::should_merge ( const AbstractRecord * absRec) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::should_merge ( const AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean RajdootCallRecord::should_replace ( const AbstractRecord * absRec) const
{
#ifdef DEBUG 
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::should_replace ( const AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean RajdootCallRecord::should_alter (const AbstractRecord *absRec) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::should_alter( AbstractRecord [" << absRec->get_uid();
    debug_stream<< "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

RecordType RajdootCallRecord::type_is () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::type_is () on " << get_uid() << "\n" << flush;
#endif

    return RPCCALL;
}

AnyType RajdootCallRecord::value () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::value () on " << get_uid() << "\n" << flush;
#endif

    return (AnyType) rpc_interface;
}

void RajdootCallRecord::setValue (AnyType value)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::setValue( AnyType [" << value;
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    rpc_interface = (ClientRajdootAction_rpc *) value;
}

void RajdootCallRecord::nested_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::nested_abort () on " << get_uid() << "\n" << flush;
#endif

     rpc_interface->DoRemote_NestedAbort();
}

void RajdootCallRecord::nested_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::nested_commit () on " << get_uid() << "\n" << flush;
#endif

    rpc_interface->DoRemote_NestedCommit();
}

PrepareOutcome RajdootCallRecord::nested_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::nested_prepare () on " << get_uid() << "\n" << flush;
#endif

    return rpc_interface->DoRemote_NestedPrepare();
}

void RajdootCallRecord::top_level_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::top_level_abort () on " << get_uid() << "\n" << flush;
#endif

    rpc_interface->DoRemote_TopLevelAbort();
}

void RajdootCallRecord::top_level_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::top_level_commit () on " << get_uid() << "\n" << flush;
#endif

    rpc_interface->DoRemote_TopLevelCommit();
}

PrepareOutcome RajdootCallRecord::top_level_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::top_level_prepare () on " << get_uid() << "\n" << flush;
#endif

    return rpc_interface->DoRemote_TopLevelPrepare();
}

/*
 * Inherited functions from StateManager
 */

Boolean RajdootCallRecord::restore_state ( ObjectState&, ObjectType)
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::restore_state ( ObjectState& s, ObjectType t ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean RajdootCallRecord::save_state ( ObjectState& s, ObjectType)
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::save_state ( ObjectState& s, ObjectType t ) on " << get_uid() << "\n" << flush;
#endif

    Uid aUid(get_uid());
    
    return (aUid.pack(s) && s.pack(type()));
}

ostream& RajdootCallRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);

//    strm << "RajdootCallRecord  " << rpc_interface << "\n";
    return strm;
}

const TypeName RajdootCallRecord::type () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::type () on " << get_uid() << "\n" << flush;
#endif

     return ("/StateManager/AbstractRecord/RajdootCallRecord");
}
