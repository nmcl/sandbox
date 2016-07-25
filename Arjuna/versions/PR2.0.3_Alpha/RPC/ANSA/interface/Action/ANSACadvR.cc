/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ANSACadvR.cc,v 1.3 1993/05/05 13:21:23 n048z Exp $
 */

static const char RCSid[] = "$Id: ANSACadvR.cc,v 1.3 1993/05/05 13:21:23 n048z Exp $";

/*
 * ANSACadaverRecord Class
 *
 */

#ifdef DEBUG
#  ifndef DEBUG_H_
#     include <Common/Debug.h>
#  endif
#endif

#ifndef ANSACADVR_H_
#  include "ANSACadvR.h"
#endif
/*
 * Public constructors and destructors
 */

ANSACadaverRecord::ANSACadaverRecord ( const Uid& obj_uid,
					     ClientANSAAction_rpc* CRA_rpc)
                                            : AbstractRecord(obj_uid, 0, ANDPERSISTENT),
					      rpc_interface(new ClientANSAAction_rpc(CRA_rpc))
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::ANSACadaverRecord (ClientANSAAction_rpc [";
    debug_stream << CRA_rpc << "] ) on " << get_uid() << "\n" << flush;
#endif
}
ANSACadaverRecord::~ANSACadaverRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::~ANSACadaverRecord () on " << get_uid() << "\n" << flush;
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

void ANSACadaverRecord::merge ( const AbstractRecord *mergewith )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::merge (const AbstractRecord [" << mergewith->get_uid(); 
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif
}

void ANSACadaverRecord::alter (AbstractRecord *absRec)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::alter( AbstractRecord [" << absRec->get_uid();
    debug_stream<< "] ) on " << get_uid() << "\n" << flush;
#endif

    absRec->setValue((AnyType) rpc_interface);
}

Boolean ANSACadaverRecord::should_add ( const AbstractRecord * absRec ) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::should_add ( const AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean ANSACadaverRecord::should_merge ( const AbstractRecord * absRec ) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::should_merge ( const AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean ANSACadaverRecord::should_replace ( const AbstractRecord * absRec ) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::should_replace ( const AbstractRecord [" << absRec->get_uid();
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    return  FALSE;
}

Boolean ANSACadaverRecord::should_alter (const AbstractRecord *absRec) const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::should_alter( AbstractRecord [" << absRec->get_uid();
    debug_stream<< "] ) on " << get_uid() << "\n" << flush;
#endif

    return  (((absRec->type_is() == RPCINITIATE) ||
		    (absRec->type_is() == RPCCALL) ||
		    (absRec->type_is() == RPCTERMINATE)) &&
		   (absRec->ordering() == ordering()));
}

RecordType ANSACadaverRecord::type_is () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::type_is () on " << get_uid() << "\n" << flush;
#endif

    return RPCCADAVER;
}

AnyType ANSACadaverRecord::value () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::value () on " << get_uid() << "\n" << flush;
#endif

    return (AnyType) rpc_interface;
}

void ANSACadaverRecord::setValue (AnyType value)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::setValue( AnyType [" << value;
    debug_stream << "] ) on " << get_uid() << "\n" << flush;
#endif

    rpc_interface = (ClientANSAAction_rpc *) value;
}

void ANSACadaverRecord::nested_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::nested_abort () on " << get_uid() << "\n" << flush;
#endif
}

void ANSACadaverRecord::nested_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::nested_commit () on " << get_uid() << "\n" << flush;
#endif
}

PrepareOutcome ANSACadaverRecord::nested_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::nested_prepare () on " << get_uid() << "\n" << flush;
#endif

    return READONLY;
}

void ANSACadaverRecord::top_level_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::top_level_abort () on " << get_uid() << "\n" << flush;
#endif
}

void ANSACadaverRecord::top_level_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::top_level_commit () on " << get_uid() << "\n" << flush;
#endif
}

PrepareOutcome ANSACadaverRecord::top_level_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::top_level_prepare () on " << get_uid() << "\n" << flush;
#endif

    return PREP_OK;
}

/*
 * Inherited functions from StateManager
 */

Boolean ANSACadaverRecord::restore_state ( ObjectState&, ObjectType )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::restore_state ( ObjectState& s, ObjectType t ) on " << get_uid() << "\n" << flush;
#endif

    return FALSE;
}

Boolean ANSACadaverRecord::save_state ( ObjectState&, ObjectType )
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::save_state ( ObjectState& s, ObjectType t ) on " << get_uid() << "\n" << flush;
#endif

    return TRUE;
}

ostream& ANSACadaverRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);

//    strm << "ANSACadaverRecord " << rpc_interface << "\n";
    return strm;
}

const TypeName ANSACadaverRecord::type () const
{
#ifdef DEBUG
    debug_stream << TRIVIAL_FUNCS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ANSACadaverRecord::type () on " << get_uid() << "\n" << flush;
#endif

    return ("/StateManager/AbstractRecord/ANSACadaverRecord");
}

