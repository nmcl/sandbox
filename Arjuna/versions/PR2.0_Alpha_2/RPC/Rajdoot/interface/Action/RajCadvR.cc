/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RajCadvR.cc,v 1.1 1993/11/03 14:40:06 nmcl Exp $
 */

static const char RCSid[] = "$Id: RajCadvR.cc,v 1.1 1993/11/03 14:40:06 nmcl Exp $";

/*
 * RajdootCadaverRecord Class
 *
 */

#ifdef DEBUG
#  ifndef DEBUG_H_
#     include <Common/Debug.h>
#  endif
#endif

#ifndef RAJCADVR_H_
#  include "RajCadvR.h"
#endif
/*
 * Public constructors and destructors
 */

RajdootCadaverRecord::RajdootCadaverRecord ( const Uid& obj_uid,
					     ClientRajdootAction_rpc* CRA_rpc)
                                            : AbstractRecord(obj_uid, 0, ANDPERSISTENT),
					      rpc_interface(new ClientRajdootAction_rpc(CRA_rpc))
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::RajdootCadaverRecord (ClientRajdootAction_rpc [";
    debug_stream << CRA_rpc << "] ) on " << get_uid() << "\n" << flush;
#endif
}
RajdootCadaverRecord::~RajdootCadaverRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::~RajdootCadaverRecord () on " << get_uid() << "\n" << flush;
#endif

    if (rpc_interface != NULL)
	delete rpc_interface;
}

/*
 * Public virtual functions and operators
 */

/*
 * Inherited functions from AbstractRecord
 */

void RajdootCadaverRecord::merge ( const AbstractRecord *mergewith )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::merge (const AbstractRecord [" << mergewith->get_uid(); 
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif
}

void RajdootCadaverRecord::alter (AbstractRecord *absRec)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::alter( AbstractRecord [" << absRec->get_uid();
    debug_stream<< "] ) on " << get_uid() << "\n" << flush;
#endif

    absRec->setValue((AnyType) rpc_interface);
}

Boolean RajdootCadaverRecord::should_add ( const AbstractRecord * absRec ) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::should_add ( const AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean RajdootCadaverRecord::should_merge ( const AbstractRecord * absRec ) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::should_merge ( const AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean RajdootCadaverRecord::should_replace ( const AbstractRecord * absRec ) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::should_replace ( const AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return  FALSE;
}

Boolean RajdootCadaverRecord::should_alter (const AbstractRecord *absRec) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::should_alter( AbstractRecord [" << absRec->get_uid();
    debug_stream<< "] ) on " << get_uid() << "\n" << flush;
#endif

    return  (((absRec->type_is() == RPCINITIATE) ||
		    (absRec->type_is() == RPCCALL) ||
		    (absRec->type_is() == RPCTERMINATE)) &&
		   (absRec->ordering() == ordering()));
}

RecordType RajdootCadaverRecord::type_is () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::type_is () on " << get_uid() << "\n" << flush;
#endif

    return RPCCADAVER;
}

AnyType RajdootCadaverRecord::value () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::value () on " << get_uid() << "\n" << flush;
#endif

    return (AnyType) rpc_interface;
}

void RajdootCadaverRecord::setValue (AnyType value)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::setValue( AnyType [" << value;
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    rpc_interface = (ClientRajdootAction_rpc *) value;
}

void RajdootCadaverRecord::nested_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::nested_abort () on " << get_uid() << "\n" << flush;
#endif
}

void RajdootCadaverRecord::nested_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::nested_commit () on " << get_uid() << "\n" << flush;
#endif
}

PrepareOutcome RajdootCadaverRecord::nested_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::nested_prepare () on " << get_uid() << "\n" << flush;
#endif

    return READONLY;
}

void RajdootCadaverRecord::top_level_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::top_level_abort () on " << get_uid() << "\n" << flush;
#endif
}

void RajdootCadaverRecord::top_level_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::top_level_commit () on " << get_uid() << "\n" << flush;
#endif
}

PrepareOutcome RajdootCadaverRecord::top_level_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::top_level_prepare () on " << get_uid() << "\n" << flush;
#endif

    return PREP_OK;
}

/*
 * Inherited functions from StateManager
 */

Boolean RajdootCadaverRecord::restore_state ( ObjectState&, ObjectType )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::restore_state ( ObjectState& s, ObjectType t ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean RajdootCadaverRecord::save_state ( ObjectState&, ObjectType )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::save_state ( ObjectState& s, ObjectType t ) on " << get_uid() << "\n" << flush;
#endif

    return TRUE;
}

ostream& RajdootCadaverRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);

//    strm << "RajdootCadaverRecord " << rpc_interface << "\n";
    return strm;
}

const TypeName RajdootCadaverRecord::type () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::type () on " << get_uid() << "\n" << flush;
#endif

    return ("/StateManager/AbstractRecord/RajdootCadaverRecord");
}

