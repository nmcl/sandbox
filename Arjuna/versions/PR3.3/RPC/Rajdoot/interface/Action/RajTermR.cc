/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RajTermR.cc,v 1.10 1995/09/25 15:05:04 nmcl Exp $
 */

static const char RCSid[] = "$Id: RajTermR.cc,v 1.10 1995/09/25 15:05:04 nmcl Exp $";

/*
 * RajdootTerminateRecord Class
 *
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifdef DEBUG
#  ifndef RDEBUG_H_
#    include <Common/Debug.h>
#  endif
#endif

#ifndef RAJTERMR_H_
#  include "RajTermR.h"
#endif


Boolean RajTermSetUp::setUp = FALSE;


RajTermSetUp::RajTermSetUp ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajTermSetUp::RajTermSetUp ()" << endl;
    debug_stream.unlock();
    
#endif
    
    if (!setUp)
    {
        RajdootTerminateRecord tr;
	AbstractRecord::addToList(RajTermSetUp::create, RajTermSetUp::remove, tr.type());
	setUp = TRUE;
    }
}

RajTermSetUp::~RajTermSetUp () {}

AbstractRecord* RajTermSetUp::create ()
{
    return new RajdootTerminateRecord;
}

void RajTermSetUp::remove (AbstractRecord*& toDelete)
{
    delete toDelete;
    toDelete = 0;
}


/*
 * Public constructors and destructors
 */

RajdootTerminateRecord::RajdootTerminateRecord (const Uid& obj_uid,
						ClientRajdootAction_rpc* CRA_rpc)
					       : AbstractRecord(obj_uid, 0, ANDPERSISTENT),
						 rpc_interface(CRA_rpc)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::RajdootTerminateRecord (ClientRajdootAction_rpc [";
    debug_stream << CRA_rpc << "] ) for " << order() << endl;
    debug_stream.unlock();
    
#endif
}

RajdootTerminateRecord::RajdootTerminateRecord ()
                                                : AbstractRecord(),
                                                  rpc_interface(0)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PROTECTED;
    debug_stream << "RajdootTerminateRecord::RajdootTerminateRecord ()" << endl;
    debug_stream.unlock();
    
#endif
}

RajdootTerminateRecord::~RajdootTerminateRecord ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::~RajdootTerminateRecord () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif
}

/*
 * Public virtual functions and operators
 */

/*
 * Inherited functions from AbstractRecord
 */

RecordType RajdootTerminateRecord::typeIs () const
{
    return RPCTERMINATE;
}

AnyType RajdootTerminateRecord::value () const
{
    return (AnyType) rpc_interface;
}

void RajdootTerminateRecord::setValue (AnyType value)
{
    rpc_interface = (ClientRajdootAction_rpc *) value;
}

Boolean RajdootTerminateRecord::nestedAbort ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::nestedAbort () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    return TRUE;
}

Boolean RajdootTerminateRecord::nestedCommit ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::nestedCommit () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    return TRUE;
}

PrepareOutcome RajdootTerminateRecord::nestedPrepare ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::nestedPrepare () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    return READONLY;
}

Boolean RajdootTerminateRecord::topLevelAbort ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::topLevelAbort () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    if (rpc_interface)
	return (rpc_interface->ClientRajdoot_rpc::terminate() == OPER_DONE);
    else
	return FALSE;
}

Boolean RajdootTerminateRecord::topLevelCommit ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::topLevelCommit () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    if (rpc_interface)
	return (rpc_interface->ClientRajdoot_rpc::terminate() == OPER_DONE);
    else
	return FALSE;
}

PrepareOutcome RajdootTerminateRecord::topLevelPrepare ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootTerminateRecord::topLevelPrepare () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    return PREP_OK;
}

/*
 * Inherited functions from StateManager
 */

Boolean RajdootTerminateRecord::restore_state ( ObjectState&, ObjectType )
{
    return FALSE;
}

Boolean RajdootTerminateRecord::save_state ( ObjectState&, ObjectType )
{
    return TRUE;
}

ostream& RajdootTerminateRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);

//    strm << "RajdootTerminateRecord  " << rpc_interface << endl;
    return strm;
}

const TypeName RajdootTerminateRecord::type () const
{
    return ("/StateManager/AbstractRecord/RajdootTerminateRecord");
}

void RajdootTerminateRecord::merge ( const AbstractRecord * )
{
}

void RajdootTerminateRecord::alter ( AbstractRecord * )
{
}

Boolean RajdootTerminateRecord::shouldAdd ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean RajdootTerminateRecord::shouldAlter ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean RajdootTerminateRecord::shouldMerge ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean RajdootTerminateRecord::shouldReplace ( const AbstractRecord * absRec) const
{
    return ((absRec->typeIs() == RPCINITIATE) && (absRec->order() == order()) && (absRec->value() == value()));
}
