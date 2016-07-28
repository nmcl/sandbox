/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RajCadvR.cc,v 1.7 1995/06/26 10:33:58 ngdp Exp $
 */

static const char RCSid[] = "$Id: RajCadvR.cc,v 1.7 1995/06/26 10:33:58 ngdp Exp $";

/*
 * RajdootCadaverRecord Class
 *
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifdef DEBUG
#  ifndef DEBUG_H_
#     include <Common/Debug.h>
#  endif
#endif

#ifndef RAJCADVR_H_
#  include "RajCadvR.h"
#endif


Boolean RajCadvSetUp::setUp = FALSE;


RajCadvSetUp::RajCadvSetUp ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajCadvSetUp::RajCadvSetUp ()" << endl;
    debug_stream.unlock();
    
#endif
    
    if (!setUp)
    {
        RajdootCadaverRecord tr;
	AbstractRecord::addToList(RajCadvSetUp::create, RajCadvSetUp::remove, tr.type());
	setUp = TRUE;
    }
}

RajCadvSetUp::~RajCadvSetUp () {}

AbstractRecord* RajCadvSetUp::create ()
{
    return new RajdootCadaverRecord;
}

void RajCadvSetUp::remove (AbstractRecord*& toDelete)
{
    delete toDelete;
    toDelete = 0;
}


/*
 * Public constructors and destructors
 */

RajdootCadaverRecord::RajdootCadaverRecord ( const Uid& obj_uid,
					     ClientRajdootAction_rpc* CRA_rpc)
                                            : AbstractRecord(obj_uid, 0, ANDPERSISTENT),
					      rpc_interface(new ClientRajdootAction_rpc(CRA_rpc))
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::RajdootCadaverRecord (ClientRajdootAction_rpc [";
    debug_stream << CRA_rpc << "] ) for " << order() << endl;
    debug_stream.unlock();
    
#endif
}

RajdootCadaverRecord::RajdootCadaverRecord ()
                                            : AbstractRecord(),
                                              rpc_interface(0)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PROTECTED;
    debug_stream << "RajdootCadaverRecord::RajdootCadaverRecord ()" << endl;
    debug_stream.unlock();
    
#endif
}

RajdootCadaverRecord::~RajdootCadaverRecord ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::~RajdootCadaverRecord () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    if (rpc_interface != NULL)
	delete rpc_interface;
}

/*
 * Public virtual functions and operators
 */

Boolean RajdootCadaverRecord::propagateOnAbort () const
{
    return TRUE;
}

/*
 * Inherited functions from AbstractRecord
 */

RecordType RajdootCadaverRecord::typeIs () const
{
    return RPCCADAVER;
}

AnyType RajdootCadaverRecord::value () const
{
    return (AnyType) rpc_interface;
}

void RajdootCadaverRecord::setValue (AnyType value)
{
    rpc_interface = (ClientRajdootAction_rpc *) value;
}

Boolean RajdootCadaverRecord::nestedAbort ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::nestedAbort () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif
    
    return TRUE;
}

Boolean RajdootCadaverRecord::nestedCommit ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::nestedCommit () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif
    
    return TRUE;
}

PrepareOutcome RajdootCadaverRecord::nestedPrepare ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::nestedPrepare () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    return READONLY;
}

Boolean RajdootCadaverRecord::topLevelAbort ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::topLevelAbort () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif
    
    return TRUE;
}

Boolean RajdootCadaverRecord::topLevelCommit ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::topLevelCommit () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif
    
    return TRUE;
}

PrepareOutcome RajdootCadaverRecord::topLevelPrepare ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCadaverRecord::topLevelPrepare () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    return PREP_OK;
}

/*
 * Inherited functions from StateManager
 */

Boolean RajdootCadaverRecord::restore_state ( ObjectState&, ObjectType )
{
    return FALSE;
}

Boolean RajdootCadaverRecord::save_state ( ObjectState&, ObjectType )
{
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
    return ("/StateManager/AbstractRecord/RajdootCadaverRecord");
}


void RajdootCadaverRecord::merge ( const AbstractRecord * )
{
}

void RajdootCadaverRecord::alter ( AbstractRecord *absRec )
{
    absRec->setValue((AnyType) rpc_interface);
}

Boolean RajdootCadaverRecord::shouldAdd ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean RajdootCadaverRecord::shouldAlter ( const AbstractRecord *absRec ) const
{
    return  (((absRec->typeIs() == RPCINITIATE) ||
		    (absRec->typeIs() == RPCCALL) ||
		    (absRec->typeIs() == RPCTERMINATE)) &&
		   (absRec->order() == order()));
}

Boolean RajdootCadaverRecord::shouldMerge ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean RajdootCadaverRecord::shouldReplace ( const AbstractRecord * ) const
{
    return  FALSE;
}
