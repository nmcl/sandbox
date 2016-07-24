/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RajInitR.cc,v 1.1 1993/11/03 14:40:09 nmcl Exp $
 */

static const char RCSid[] = "$Id: RajInitR.cc,v 1.1 1993/11/03 14:40:09 nmcl Exp $";

/*
 * RajdootInitiateRecord Class
 *
 */

#ifdef DEBUG
#  ifndef DEBUG_H_
#    include <Common/Debug.h>
#  endif
#endif

#ifndef RAJINITR_H_
#  include "RajInitR.h"
#endif

/*
 * Public constructors and destructors
 */

RajdootInitiateRecord::RajdootInitiateRecord (const Uid& obj_uid,
					      ClientRajdootAction_rpc* CRA_rpc)
                                             : AbstractRecord(obj_uid, 0, ANDPERSISTENT),
					       rpc_interface(CRA_rpc)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::RajdootInitiateRecord (ClientRajdoot_rpc [";
    debug_stream << CRA_rpc << "] ) on " << get_uid() << "\n" << flush;
#endif
}

RajdootInitiateRecord::~RajdootInitiateRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::~RajdootInitiateRecord () on " << get_uid() << "\n" << flush;
#endif
}

/*
 * Public virtual functions and operators
 */

/*
 * Inherited functions from AbstractRecord
 */

void RajdootInitiateRecord::merge ( const AbstractRecord * mergewith)
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::merge ( const AbstractRecord [" << mergewith->get_uid(); 
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif
}

void RajdootInitiateRecord::alter (AbstractRecord *absRec)
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::alter( AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif
}

Boolean RajdootInitiateRecord::should_add ( const AbstractRecord * absRec) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::should_add (AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean RajdootInitiateRecord::should_merge ( const AbstractRecord * absRec ) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::should_merge (AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean RajdootInitiateRecord::should_replace ( const AbstractRecord * absRec ) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::should_replace (AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean RajdootInitiateRecord::should_alter (const AbstractRecord *absRec) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::should_alter( AbstractRecord [" << absRec->get_uid();
    debug_stream<< "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

RecordType RajdootInitiateRecord::type_is () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::type_is () on " << get_uid() << "\n" << flush;
#endif

    return RPCINITIATE;
}

AnyType RajdootInitiateRecord::value () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::value () on " << get_uid() << "\n" << flush;
#endif

    return (AnyType) rpc_interface;
}

void RajdootInitiateRecord::setValue (AnyType value)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::setValue( AnyType [" << value;
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    rpc_interface = (ClientRajdootAction_rpc *) value;
}

void RajdootInitiateRecord::nested_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::nested_abort () on " << get_uid() << "\n" << flush;
#endif

     rpc_interface->ClientRajdoot_rpc::Terminate();
}

void RajdootInitiateRecord::nested_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::nested_commit () on " << get_uid() << "\n" << flush;
#endif
}

PrepareOutcome RajdootInitiateRecord::nested_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::nested_prepare () on " << get_uid() << "\n" << flush;
#endif

    return PREP_OK;
}

void RajdootInitiateRecord::top_level_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::top_level_abort () on " << get_uid() << "\n" << flush;
#endif

     rpc_interface->ClientRajdoot_rpc::Terminate();
}

void RajdootInitiateRecord::top_level_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::top_level_commit () on " << get_uid() << "\n" << flush;
#endif
}

PrepareOutcome RajdootInitiateRecord::top_level_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::top_level_prepare () on " << get_uid() << "\n" << flush;
#endif

    return PREP_OK;
}

/*
 * Inherited functions from StateManager
 */

Boolean RajdootInitiateRecord::restore_state ( ObjectState&, ObjectType )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::restore_state ( ObjectState& s, ObjectType t ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean RajdootInitiateRecord::save_state ( ObjectState&, ObjectType )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::save_state ( ObjectState& s, ObjectType t )\n" << flush;
#endif

    return TRUE;
}

ostream& RajdootInitiateRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);

//    strm << "RajdootInitiateRecord " <<  rpc_interface << "\n" << flush;
    return strm;
}

const TypeName RajdootInitiateRecord::type () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::type () on " << get_uid() << "\n" << flush;
#endif

    return ("/StateManager/AbstractRecord/RajdootInitiateRecord");
}
