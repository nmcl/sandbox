/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AbstractR.cc,v 1.8 1993/12/16 12:04:27 nmcl Exp $
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
 * Many functions are declared pure virtual to force a definition to occur
 * in any derived class. These are currently all functions dealing with 
 * atomic action coordination as well as the following list management 
 * functions:
 *     typeIs:   returns the record type of the instance. This is one of
 *               the values of the enumerated type Record_type
 *     value:    Some arbitrary value associated with the record instance
 *     merge:    Used when two records need to merge togethor. Currently
 *               this is only used by CadaverRecords to merge information
 *               from PersistenceRecords
 *     shouldAdd:
 *               returns TRUE is the record should be added to the list
 *               FALSE if it should be discarded
 *     shouldMerge:
 *               returns TRUE is the two records should be merged into a
 *               single record, FALSE if it should be discarded
 *     shouldReplace:
 *               returns TRUE if the record should replace an existing
 *               one, FALSE otherwise.
 */


#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#if defined(DEBUG ) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef ABSTRACTR_H_
#  include <Arjuna/AbstractR.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef ABSRECLIST_H_
#  include "AbsRecList.h"
#endif

static const char RCSid[] = "$Id: AbstractR.cc,v 1.8 1993/12/16 12:04:27 nmcl Exp $";


AbstractRecordList* AbstractRecord::headOfList = 0;


/*
 * Public destructor
 */

AbstractRecord::~AbstractRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PROTECTED;
    debug_stream << "AbstractRecord::~AbstractRecord() for " << order() << endl;
#endif
}

/*
 * Dynamic linked-list of abstract records - typically used for crash recovery
 * purposes. All static methods.
 */

void AbstractRecord::addToList (AbstractRecord* (*func1)(), void (*func2)(AbstractRecord*&), const TypeName tn)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "void AbstractRecord::addToList ( " << tn << " )" << endl;
#endif

    if (!headOfList)
	headOfList = new AbstractRecordList(func1, func2, tn);
    else
    {
	Boolean found = FALSE;
	AbstractRecordList *marker = headOfList;

	while ((!found) && (marker))
	{
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
	    debug_stream << "AbstractRecord::addToList - Comparing " << tn << " with " << marker->type() << endl;
#endif
	    if (::strcmp(tn, marker->type()) != 0)
		marker = marker->next;
	    else
		found = TRUE;
	}

	if (!found)
	{
	    marker = new AbstractRecordList(func1, func2, tn);
	    marker->next = headOfList;
	    headOfList = marker;
	}
    }
}

void AbstractRecord::remove (AbstractRecord*& toDelete)
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "void AbstractRecord::remove ( " << toDelete->type() << " )" << endl;
#endif

    if (headOfList)
    {
      AbstractRecordList* marker = headOfList;

      while (marker)
      {
	if (::strcmp(marker->type(), toDelete->type()) == 0)
	{
	    marker->remove(toDelete);
	    toDelete = 0;
	    return;
	}
	else
	    marker = marker->next;
      }
    }
}

AbstractRecord* AbstractRecord::create (const TypeName tn)
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "AbstractRecord* AbstractRecord::create ( " << tn << " )" << endl;
#endif

    if (headOfList)
    {    
	AbstractRecordList* marker = headOfList;
    
	while (marker)
	{
	    if (::strcmp(marker->type(), tn) == 0)
		return marker->create();
	    else
		marker = marker->next;
	}
    }
    
    error_stream << FATAL << "Cannot find abstract record " << tn << endl;
    return 0;
}

/*
 * Public non-virtual member functions and operators
 */

const Uid& AbstractRecord::order () const
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
 * Determine if records are discarded on action abort or must be
 * propagated to parents
 */

Boolean AbstractRecord::propagateOnAbort () const
{
    return FALSE;
}

/*
 * Operators for comparing and sequencing instances of classes derived
 * from AbstractRecords.
 * Records are ordered primarily based upon the value of 'order',
 * followed by 'typeIs'.
 */

/*
 * Determine if two records are equal in that both are the same type
 * and have the same order value (determined via 'order()').
 */

Boolean AbstractRecord::operator== ( const AbstractRecord *ar ) const
{
    return ((order() == ar->order()) &&
	    (typeIs() == ar->typeIs()));
}

Boolean AbstractRecord::operator< ( const AbstractRecord *ar ) const
{
    return ((order() < ar->order()) ||
	     ((order() == ar->order()) &&
	      (typeIs() < ar->typeIs())));
}

Boolean AbstractRecord::operator> ( const AbstractRecord *ar ) const
{
    return ((order() > ar->order()) ||
	     ((order() == ar->order()) &&
	      (typeIs() > ar->typeIs())));
}

/*
 * Public virtual functions
 */

Boolean AbstractRecord::doSave () const
{
    return FALSE;
}

/*
 * Re-Implementation of virtual functions inherited from base class
 */

const TypeName AbstractRecord::type () const
{
    return ("/StateManager/AbstractRecord");
}

ostream& AbstractRecord::print ( ostream& strm ) const
{
    strm << "Uid of Managed Object:" << uidOfObject << endl;
    strm << "Type of Managed Object:" << typeOfObject << endl;
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
	<< ", " << otype << ")" << endl;
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
    debug_stream << "AbstractRecord::AbstractRecord(" << storeUid << ")" << endl;
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
    debug_stream << "- crash recovery constructor" << endl;
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
    return (Boolean) (uidOfObject.unpack(os) && os.unpack(typeOfObject));
}

Boolean AbstractRecord::save_state ( ObjectState& os, ObjectType)
{
    return (Boolean) (uidOfObject.pack(os) && os.pack(typeOfObject));
}

