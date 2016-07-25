/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ActiveR.cc,v 1.8 1995/10/13 09:02:30 ngdp Exp $
 */

/*
 *
 * ActivationRecord Class
 *
 */

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#if defined(DEBUG ) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef ACTIVER_H_
#  include "ActiveR.h"
#endif

static const char RCSid[] = "$Id: ActiveR.cc,v 1.8 1995/10/13 09:02:30 ngdp Exp $";


Boolean ActiveSetUp::setUp = FALSE;


/*
 * ActivationRecords manage activation states of objects. They are created when
 * an object is first activated inside the scope of an atomic action
 */


ActiveSetUp::ActiveSetUp ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActiveSetUp::ActiveSetUp ()" << endl;
    debug_stream.unlock();
    
#endif
    
    if (!setUp)
    {
        ActivationRecord tr;
	AbstractRecord::addToList(ActiveSetUp::create, ActiveSetUp::remove, tr.type());
	setUp = TRUE;
    }
}

ActiveSetUp::~ActiveSetUp () {}

AbstractRecord* ActiveSetUp::create ()
{
    return new ActivationRecord;
}

void ActiveSetUp::remove (AbstractRecord*& toDelete)
{
    delete toDelete;
    toDelete = 0;
}
 


/*
 * Public constructors and destructors
 */

/*
 * This constructor is used to create a new instance of an
 * ActivationRecord.
 */

ActivationRecord::ActivationRecord ( ObjectStatus st,
				     StateManager *sm )
                                   : AbstractRecord(sm->get_uid(), sm->type(), ANDPERSISTENT),
				     objectAddr(sm),
				     state(st)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::ActivationRecord(" << state
                 << ", " << sm->get_uid() << ")" << endl;
    debug_stream.unlock();
    
#endif
}

ActivationRecord::ActivationRecord ()
                                    : AbstractRecord(),
				      objectAddr(0),
				      state(PASSIVE)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PROTECTED;
    debug_stream << "ActivationRecord::ActivationRecord ()" << endl;
    debug_stream.unlock();
    
#endif
}

ActivationRecord::~ActivationRecord ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::~ActivationRecord() for " << order() << endl;
    debug_stream.unlock();
    
#endif

}

/*
 * Public virtual functions and operators
 */

RecordType ActivationRecord::typeIs () const
{
    return ACTIVATION;
}

AnyType ActivationRecord::value () const
{
    return (AnyType)state;
}

void ActivationRecord::setValue (AnyType)
{
    addError(new Error(FAC_ABSTRACT_REC, AR_NOT_ALLOWED,
		       "ActivationRecord::set_value() called illegally\n"));
}

/*
 * nestedAbort causes the reset_state function of the object to be invoked
 * passing it the saved ObjectStatus
 */

Boolean ActivationRecord::nestedAbort ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::nestedAbort() for " << order() << endl;
    debug_stream.unlock();
    
#endif

    if (objectAddr)
	return objectAddr->resetState(state);

    return TRUE;
}

/*
 * nestedCommit does nothing since the passing of the state up to
 * the parent action is handled by the record list merging system.
 * In fact since nested_prepare returns READONLY this function should
 * never actually be called
 */

Boolean ActivationRecord::nestedCommit ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::nestedCommit() for " << order() << endl;
    debug_stream.unlock();
    
#endif

    return TRUE;
}

PrepareOutcome ActivationRecord::nestedPrepare ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::nestedPrepare() for " << order() << endl;
    debug_stream.unlock();
    
#endif

    return (READONLY);
}

/*
 * topLevelAbort for Activation records is exactly like a nested
 * abort.
 */

Boolean ActivationRecord::topLevelAbort ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::topLevelAbort() for " << order() << endl;
    debug_stream.unlock();
    
#endif

    return nestedAbort();		/* ie same as nested case */
}

/*
 * topLevelCommit has little to do for ActivationRecords other
 * than to ensure the object is made PASSIVE
 */

Boolean ActivationRecord::topLevelCommit ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::topLevelCommit() for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    switch (state)
    {
    case PASSIVE:
    case PASSIVE_NEW:
	if (objectAddr)
	    return objectAddr->resetState(PASSIVE);
	break;
    default:
	break;
    }
    return TRUE;
    
}

PrepareOutcome ActivationRecord::topLevelPrepare ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ActivationRecord::topLevelPrepare() for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    /* No object -> READONLY */
    
    if (objectAddr == 0)
	return READONLY;
    else
    {
	ObjectStatus newState = objectAddr->status();

	/*
	 * if state has not changed OR has only gone to ACTIVE
	 * then the object was only read
	 */
	
	if (newState == state)
	    return READONLY;
	else
	    if (newState == ACTIVE)
	    {
		objectAddr->resetState(state);
		return READONLY;
	    }
    }

    return (PREP_OK);
}

/*
 * Saving of ActivationRecords is only undertaken during the Prepare
 * phase of the top level 2PC.
 */

Boolean ActivationRecord::restore_state ( ObjectState&, ObjectType )
{
    char buff[1024];
    ostrstream temp(buff,1024);
    
    temp << "Invocation of ActivationRecord::restore_state for "
	 << type() << "inappropriate - ignored for " << order() << endl << ends;
    addError(new Error(FAC_ABSTRACT_REC, AR_OP_IGNORED, buff));
	     
    return FALSE;
}

Boolean ActivationRecord::save_state ( ObjectState&, ObjectType )
{
    return (TRUE);
}

ostream& ActivationRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);

    strm << "ActivationRecord with state:\n" << state << "\n";
    return strm;
}

const TypeName ActivationRecord::type () const
{
    return ("/StateManager/AbstractRecord/ActivationRecord");
}

void ActivationRecord::merge ( const AbstractRecord * )
{
}

void ActivationRecord::alter (AbstractRecord *)
{
}

/*
 * should_merge and should_replace are invoked by the record list manager
 * to determine if two records should be merged togethor or if the
 * 'newer' should replace the older.
 * shouldAdd determines if the new record should be added in addition
 * to the existing record and is currently only invoked if both of
 * should_merge and should_replace return FALSE
 * Default implementations here always return FALSE - ie new records
 * do not override old
 */

Boolean ActivationRecord::shouldAdd ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean ActivationRecord::shouldAlter (const AbstractRecord*) const
{
    return FALSE;
}

Boolean ActivationRecord::shouldMerge ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean ActivationRecord::shouldReplace ( const AbstractRecord * ) const
{
    return FALSE;
}
