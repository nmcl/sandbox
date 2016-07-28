/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RajCallR.cc,v 1.12 1995/06/26 10:34:00 ngdp Exp $
 */

static const char RCSid[] = "$Id: RajCallR.cc,v 1.12 1995/06/26 10:34:00 ngdp Exp $";

/*
 * RajdootCallRecord Class
 *
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

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


Boolean RajCallSetUp::setUp = FALSE;


RajCallSetUp::RajCallSetUp ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajCallSetUp::RajCallSetUp ()" << endl;
    debug_stream.unlock();
    
#endif
    
    if (!setUp)
    {
        RajdootCallRecord tr;
	AbstractRecord::addToList(RajCallSetUp::create, RajCallSetUp::remove, tr.type());
	setUp = TRUE;
    }
}

RajCallSetUp::~RajCallSetUp () {}

AbstractRecord* RajCallSetUp::create ()
{
    return new RajdootCallRecord;
}

void RajCallSetUp::remove (AbstractRecord*& toDelete)
{
    delete toDelete;
    toDelete = 0;
}


/*
 * Public constructors and destructors
 */

RajdootCallRecord::RajdootCallRecord ( const Uid& obj_uid,
				       ClientRajdootAction_rpc* CRA_rpc)
                                     : AbstractRecord(obj_uid,0,ANDPERSISTENT),
				       rpc_interface(CRA_rpc)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::RajdootCallRecord (ClientRajdootAction_rpc [";
    debug_stream << CRA_rpc << "] ) for " << order() << endl;
    debug_stream.unlock();
    
#endif
}

RajdootCallRecord::RajdootCallRecord ()
                                      : AbstractRecord(),
                                        rpc_interface(0)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PROTECTED;
    debug_stream << "RajdootCallRecord::RajdootCallRecord ()" << endl;
    debug_stream.unlock();
    
#endif
}

RajdootCallRecord::~RajdootCallRecord ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::~RajdootCallRecord () for " 
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

RecordType RajdootCallRecord::typeIs () const
{
    return RPCCALL;
}

AnyType RajdootCallRecord::value () const
{
    return (AnyType) rpc_interface;
}

void RajdootCallRecord::setValue (AnyType value)
{
    rpc_interface = (ClientRajdootAction_rpc *) value;
}

Boolean RajdootCallRecord::nestedAbort ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::nestedAbort () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    if (rpc_interface)
        return rpc_interface->doRemoteNestedAbort();
    else
        return FALSE;
}

Boolean RajdootCallRecord::nestedCommit ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::nestedCommit () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    if (rpc_interface)
        return rpc_interface->doRemoteNestedCommit();
    else
        return FALSE;
}

PrepareOutcome RajdootCallRecord::nestedPrepare ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::nestedPrepare () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    if (rpc_interface)
        return rpc_interface->doRemoteNestedPrepare();
    else
        return PREP_NOTOK;
}

Boolean RajdootCallRecord::topLevelAbort ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::topLevelAbort () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    if (rpc_interface)
        return rpc_interface->doRemoteTopLevelAbort();
    else
        return FALSE;
}

Boolean RajdootCallRecord::topLevelCommit ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::topLevelCommit () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    if (rpc_interface)
        return rpc_interface->doRemoteTopLevelCommit();
    else
        return FALSE;
}

PrepareOutcome RajdootCallRecord::topLevelPrepare ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "RajdootCallRecord::topLevelPrepare () for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    if (rpc_interface)
        return rpc_interface->doRemoteTopLevelPrepare();
    else
        return PREP_NOTOK;
}

/*
 * Inherited functions from StateManager
 */

Boolean RajdootCallRecord::restore_state ( ObjectState& os, ObjectType)
{
    /*
     * Restore the state, but discard the results
     */

    Uid newUid(NIL_UID);
    TypeName newType;
    return ( newUid.unpack(os) && os.unpack(newType) );
}

Boolean RajdootCallRecord::doSave () const
{
    return TRUE;
}

Boolean RajdootCallRecord::save_state ( ObjectState& s, ObjectType)
{
    /*
     * Pack something here to ensure a state gets written
     * The contents are unimportant.
     */

    Uid aUid(get_uid());
    return (aUid.pack(s) && s.packMappedString(type()));
}

ostream& RajdootCallRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);

    if (rpc_interface)
        strm << "RajdootCallRecord - rpc_interface defined\n";
    else
        strm << "RajdootCallRecord - ** No rpc_interface **\n";

    return strm;
}

const TypeName RajdootCallRecord::type () const
{
     return ("/StateManager/AbstractRecord/RajdootCallRecord");
}

void RajdootCallRecord::merge ( const AbstractRecord * )
{
}

void RajdootCallRecord::alter ( AbstractRecord * )
{
}

Boolean RajdootCallRecord::shouldAdd ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean RajdootCallRecord::shouldAlter ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean RajdootCallRecord::shouldMerge ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean RajdootCallRecord::shouldReplace ( const AbstractRecord * ) const
{
    return FALSE;
}
