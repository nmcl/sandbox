/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AbstractR.cc,v 1.1 1993/11/03 12:29:37 nmcl Exp $
 */

/*
 *
 * Abstract Record Class
 * This class provides an abstract template that defines the interface
 * that the atomic action system uses to notify objects that various
 * state transitions have occurred as the 2PC protocol executes.
 * Record types derived from this class manage certain properties
 * of objects such as recovery information, concurrency control
 * information etc, and all must redifine the operations defined
 * here as abstract to take appropriate action.
 *
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#if defined(DEBUG ) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef _ABSTRACTR_H_
#  include <Arjuna/AbstractR.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

static const char RCSid[] = "$Id: AbstractR.cc,v 1.1 1993/11/03 12:29:37 nmcl Exp $";

/*
 * Public destructor
 */

AbstractRecord::~AbstractRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PROTECTED;
    debug_stream << "AbstractRecord::~AbstractRecord()\n" << flush;
#endif
}

/*
 * Public non-virtual member functions and operators
 */

const Uid& AbstractRecord::ordering () const
{
    return uidOfObject;
}

const Uid& AbstractRecord::getUidOfObject () const
{
    return uidOfObject;
}

const TypeName AbstractRecord::getTypeOfObject () const
{
    return typeOfObject;
}

#ifdef __GNUG__

/*
 * These routines are used by RecoveryRecord::restore_state to overcome
 * a bug in g++.
 */
void AbstractRecord::setUidOfObject (const Uid& newUid)
{
    uidOfObject = newUid;
}

void AbstractRecord::setTypeOfObject (const TypeName newType)
{
    typeOfObject = newType;
}
#endif

/*
 * Operators for comparing and sequencing instances of classes derived
 * from AbstractRecords.
 * Records are ordered primarily based upon the value of 'ordering',
 * followed by 'type_is'.
 */

/*
 * Determine if two records are equal in that both are the same type
 * and have the same order value (determined via 'ordering()').
 */

Boolean AbstractRecord::operator== ( const AbstractRecord *ar ) const
{
    return ((ordering() == ar->ordering()) &&
	    (type_is() == ar->type_is()));
}

Boolean AbstractRecord::operator< ( const AbstractRecord *ar ) const
{
    return ((ordering() < ar->ordering()) ||
	     ((ordering() == ar->ordering()) &&
	      (type_is() < ar->type_is())));
}

Boolean AbstractRecord::operator> ( const AbstractRecord *ar ) const
{
    return ((ordering() > ar->ordering()) ||
	     ((ordering() == ar->ordering()) &&
	      (type_is() > ar->type_is())));
}

/*
 * Public virtual functions
 */

/*
 * Many functions are declared pure virtual to force a definition to occur
 * in any derived class. These functions are:
 *     type_is:  returns the record type of the instance. This is one of
 *               the values of the enumerated type Record_type
 *     value:    Some arbitrary value associated with the record instance
 *     merge:    Used when two records need to merge togethor. Currently
 *               this is only used by CadaverRecords to merge information
 *               from PersistenceRecords
 *     should_add:
 *               returns TRUE is the record should be added to the list
 *               FALSE if it should be discarded
 *     should_merge:
 *               returns TRUE is the two records should be merged into a
 *               single record, FALSE if it should be discarded
 *     should_replace:
 *               returns TRUE if the record should replace an existing
 *               one, FALSE otherwise.
 */

/*
 * Atomic action two-phase commit functions. These functions are invoked
 * automatically as actions commit or abort. All of these functions are
 * pure virtual and must be redefined in derived classes.
 */

/*
 * Re-Implementation of virtual functions inherited from base class
 */

const TypeName AbstractRecord::type () const
{
    return ("/StateManager/AbstractRecord");
}

ostream& AbstractRecord::print ( ostream& strm ) const
{
    return LocalStateManager::print(strm);
}

/* 
 * Protected constructors
 */

AbstractRecord::AbstractRecord ( const Uid& storeUid, 
				 const TypeName objType,
				 ObjectType otype ) 
                               : LocalStateManager(otype),
				 next(0),
				 previous(0),
				 uidOfObject(storeUid),
				 typeOfObject(objType)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PROTECTED;
    debug_stream << "AbstractRecord::AbstractRecord(" << storeUid
	<< ", " << otype << ")\n" << flush;
#endif
}

AbstractRecord::AbstractRecord ( const Uid& storeUid ) 
                               : LocalStateManager(storeUid),
				 next(0),
				 previous(0),
				 uidOfObject(NIL_UID),
				 typeOfObject(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PROTECTED;
    debug_stream << "AbstractRecord::AbstractRecord(" << storeUid << ")\n" << flush;
#endif
}

/*
 * Creates a 'blank' abstract record. This is used during crash recovery
 * when recreating the prepared list of a server atomic action.
 */

AbstractRecord::AbstractRecord () 
                               : LocalStateManager(NIL_UID),
				 next(0),
				 previous(0),
				 uidOfObject(NIL_UID),
				 typeOfObject(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PROTECTED;
    debug_stream << "AbstractRecord::AbstractRecord()";
    debug_stream << "- crash recovery constructor\n" << flush;
#endif
}

/*
 * These few functions are link manipulation primitives used by the
 * RecordList processing software to chain instances togethor
 */

AbstractRecord *AbstractRecord::getPrevious () const
{
    return previous;
}

AbstractRecord *AbstractRecord::getNext () const
{
    return next;
}

void AbstractRecord::setPrevious ( AbstractRecord *ar )
{
    previous = ar;
}

void AbstractRecord::setNext ( AbstractRecord *ar )
{
    next = ar;
}

/*
 * save_state and restore_state here are used by the corresponding operations
 * of persistence records for crash recovery purposes.
 */

Boolean AbstractRecord::restore_state ( ObjectState& os, ObjectType)
{
    return ( (uidOfObject.unpack(os) && os.unpack(typeOfObject)) ? TRUE
                                                                 : FALSE );
}

Boolean AbstractRecord::save_state ( ObjectState& os, ObjectType)
{
    return ( (uidOfObject.pack(os) && os.pack(typeOfObject)) ? TRUE
                                                             : FALSE );
}

