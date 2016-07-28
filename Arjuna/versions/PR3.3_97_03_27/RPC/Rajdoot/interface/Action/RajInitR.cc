/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RajInitR.cc,v 1.9 1995/06/26 10:34:01 ngdp Exp $
 */

static const char RCSid[] = "$Id: RajInitR.cc,v 1.9 1995/06/26 10:34:01 ngdp Exp $";

/*
 * RajdootInitiateRecord Class
 *
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifdef DEBUG
#  ifndef DEBUG_H_
#    include <Common/Debug.h>
#  endif
#endif

#ifndef RAJINITR_H_
#  include "RajInitR.h"
#endif


Boolean RajInitSetUp::setUp = FALSE;


RajInitSetUp::RajInitSetUp ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajInitSetUp::RajInitSetUp ()" << endl;
    debug_stream.unlock();
    
#endif
    
    if (!setUp)
    {
        RajdootInitiateRecord tr;
	AbstractRecord::addToList(RajInitSetUp::create, RajInitSetUp::remove, tr.type());
	setUp = TRUE;
    }
}

RajInitSetUp::~RajInitSetUp () {}

AbstractRecord* RajInitSetUp::create ()
{
    return new RajdootInitiateRecord;
}

void RajInitSetUp::remove (AbstractRecord*& toDelete)
{
    delete toDelete;
    toDelete = 0;
}


/*
 * Public constructors and destructors
 */

RajdootInitiateRecord::RajdootInitiateRecord (const Uid& obj_uid,
					      ClientRajdootAction_rpc* CRA_rpc)
                                             : AbstractRecord(obj_uid, 0, ANDPERSISTENT),
					       rpc_interface(CRA_rpc)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::RajdootInitiateRecord (ClientRajdoot_rpc [";
    debug_stream << CRA_rpc << "] ) for " << order() <<  endl;
    debug_stream.unlock();
    
#endif
}

RajdootInitiateRecord::RajdootInitiateRecord ()
                                              : AbstractRecord(),
                                                rpc_interface(0)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PROTECTED;
    debug_stream << "RajdootInitiateRecord::RajdootInitiateRecord ()" << endl;
    debug_stream.unlock();
    
#endif
}

RajdootInitiateRecord::~RajdootInitiateRecord ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::~RajdootInitiateRecord () for " 
		 << order() <<  endl;
    debug_stream.unlock();
    
#endif
}

/*
 * Public virtual functions and operators
 */

/*
 * Inherited functions from AbstractRecord
 */

RecordType RajdootInitiateRecord::typeIs () const
{
    return RPCINITIATE;
}

AnyType RajdootInitiateRecord::value () const
{
    return (AnyType) rpc_interface;
}

void RajdootInitiateRecord::setValue (AnyType value)
{
    rpc_interface = (ClientRajdootAction_rpc *) value;
}

Boolean RajdootInitiateRecord::nestedAbort ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::nestedAbort () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    if (rpc_interface)
	return (rpc_interface->ClientRajdoot_rpc::terminate() == OPER_DONE);
    else
	return FALSE;
}

Boolean RajdootInitiateRecord::nestedCommit ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::nestedCommit () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif
    
    return TRUE;
}

PrepareOutcome RajdootInitiateRecord::nestedPrepare ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::nestedPrepare () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    return PREP_OK;
}

Boolean RajdootInitiateRecord::topLevelAbort ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::topLevelAbort () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    if (rpc_interface)
	return (rpc_interface->ClientRajdoot_rpc::terminate() == OPER_DONE);
    else
	return FALSE;
}

Boolean RajdootInitiateRecord::topLevelCommit ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::topLevelCommit () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    return TRUE;
}

PrepareOutcome RajdootInitiateRecord::topLevelPrepare ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootInitiateRecord::topLevelPrepare () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    return PREP_OK;
}

/*
 * Inherited functions from StateManager
 */

Boolean RajdootInitiateRecord::restore_state ( ObjectState&, ObjectType )
{
    return FALSE;
}

Boolean RajdootInitiateRecord::save_state ( ObjectState&, ObjectType )
{
    return TRUE;
}

ostream& RajdootInitiateRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);

//    strm << "RajdootInitiateRecord " <<  rpc_interface << endl;
    return strm;
}

const TypeName RajdootInitiateRecord::type () const
{
    return ("/StateManager/AbstractRecord/RajdootInitiateRecord");
}

void RajdootInitiateRecord::merge ( const AbstractRecord * )
{
}

void RajdootInitiateRecord::alter ( AbstractRecord * )
{
}

Boolean RajdootInitiateRecord::shouldAdd ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean RajdootInitiateRecord::shouldAlter ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean RajdootInitiateRecord::shouldMerge ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean RajdootInitiateRecord::shouldReplace ( const AbstractRecord * ) const
{
    return FALSE;
}
