/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockR.cc,v 1.9 1995/10/02 15:12:20 ngdp Exp $
 */

/*
 * 
 * Lock Record Class Implementation
 *
 */ 

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef LOCKR_H_
#  include "LockR.h"
#endif

static const char RCSid[] = "$Id: LockR.cc,v 1.9 1995/10/02 15:12:20 ngdp Exp $";


Boolean LockSetUp::setUp = FALSE;


LockSetUp::LockSetUp ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "LockSetUp::LockSetUp ()" << endl;
    debug_stream.unlock();
    
#endif
    
    if (!setUp)
    {
	AbstractRecord::addToList(LockSetUp::create, LockSetUp::remove, "LockRecord");
	setUp = TRUE;
    }
}

LockSetUp::~LockSetUp () {}

AbstractRecord* LockSetUp::create ()
{
    return new LockRecord;
}

void LockSetUp::remove (AbstractRecord*& toDelete)
{
    delete toDelete;
    toDelete = 0;
}


/*
 * Public constructors and destructors 
 */

LockRecord::LockRecord ( LockManager *lm,
			 Boolean rdOnly )
                       : AbstractRecord(lm->get_uid(), lm->type(), ANDPERSISTENT),
			 managerAddress(lm),
			 readOnly(rdOnly)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS
		 << (FacilityCode)(FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
		  << VIS_PUBLIC;
    debug_stream << "LockRecord::LockRecord(" << lm->get_uid() << ", "
	         << (readOnly ? "READONLY" : "WRITEABLE") << ")" << endl;
    debug_stream.unlock();
    
#endif
}

LockRecord::LockRecord ()
                        : AbstractRecord(),
			  managerAddress(0),
			  readOnly(FALSE)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS
		 << (FacilityCode)(FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
		  << VIS_PROTECTED;
    debug_stream << "LockRecord::LockRecord ()" << endl;
    debug_stream.unlock();
    
#endif
}

LockRecord::~LockRecord()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS
		 << (FacilityCode)(FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
		  << VIS_PUBLIC;
    debug_stream << "LockRecord::~LockRecord() for " << order() << endl;
    debug_stream.unlock();
    
#endif
}

RecordType LockRecord::typeIs () const
{
    return LOCK;
}

AnyType LockRecord::value () const
{
    return (AnyType)managerAddress;
}

void LockRecord::setValue (AnyType)
{
    addError(new Error(FAC_ABSTRACT_REC, AR_NOT_ALLOWED,
		       "LockRecord::set_value() called illegally\n"));

}

Boolean LockRecord::nestedAbort ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS
		 << (FacilityCode)(FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
		  << VIS_PUBLIC;
    debug_stream << "LockRecord::nestedAbort() for " << order() << endl;
    debug_stream.unlock();
    
#endif

    AtomicAction *currAct = AtomicAction::Current();

    /* default constructor problem. */

    if (managerAddress == 0)
	return FALSE;

    if (currAct)
    {
	if (!managerAddress->releaseAll(currAct->get_uid()))
	{
	    char buff[512];
	    ostrstream temp(buff,512);
	    
	    temp << "Lock release failed for action "
		 << currAct->get_uid() << ends;

	    addError(new Error(FAC_ABSTRACT_REC, LM_RELEASE_FAILED, buff));
	    
	    return FALSE;
	}
    } else
	addError(new Error(FAC_ABSTRACT_REC, AA_NESTING_ERROR,
			   "LockManager::nestedAbort - no current action\n"));
    
    return TRUE;
}

Boolean LockRecord::nestedCommit ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS
		 << (FacilityCode)(FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
		  << VIS_PUBLIC;
    debug_stream << "LockRecord::nestedCommit() for " << order() << endl;
    debug_stream.unlock();
    
#endif

    AtomicAction *currAct = AtomicAction::Current();

    /* default constructor problem. */

    if (managerAddress == 0)
	return FALSE;
    
    if (currAct)
    {
	return managerAddress->propagate(currAct->get_uid(),
					 currAct->parent()->get_uid());
    } else
	addError(new Error(FAC_ABSTRACT_REC, AA_NESTING_ERROR,
			   "LockManager::nestedCommit - no current action\n"));
    return TRUE;
}

PrepareOutcome LockRecord::nestedPrepare ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS
		 << (FacilityCode)(FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
		  << VIS_PUBLIC;
    debug_stream << "LockRecord::nestedPrepare() for " << order() << endl;
    debug_stream.unlock();
    
#endif

    return PREP_OK;
}

Boolean LockRecord::topLevelAbort ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS
		 << (FacilityCode)(FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
		  << VIS_PUBLIC;
    debug_stream << "LockRecord::topLevelAbort() for " << order() << endl;
    debug_stream.unlock();
    
#endif
    
    return nestedAbort();
}

Boolean LockRecord::topLevelCommit ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS
		 << (FacilityCode)(FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
		  << VIS_PUBLIC;
    debug_stream << "LockRecord::topLevelCommit() for " << order() << endl;
    debug_stream.unlock();
    
#endif
    
    AtomicAction *currAct = AtomicAction::Current();

    /* default constructor problem. */

    if (managerAddress == 0)
	return FALSE;
    
    if (currAct)
    {
	if (!managerAddress->releaseAll(currAct->get_uid()))
	{
	    char buff[512];
	    ostrstream temp(buff,512);
	    
	    temp << "Lock release failed for action "
		 << currAct->get_uid() << endl << ends;

	    addError(new Error(FAC_ABSTRACT_REC, LM_RELEASE_FAILED, buff));

	    return FALSE;
	}
    } else
	addError(new Error(FAC_ABSTRACT_REC, AA_NESTING_ERROR,
			   "LockManager::topLevelCommit - no current action\n"));

    return TRUE;    
}

PrepareOutcome LockRecord::topLevelPrepare ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS
		 << (FacilityCode)(FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
		  << VIS_PUBLIC;
    debug_stream << "LockRecord::topLevelPrepare() for " << order() << endl;
    debug_stream.unlock();
    
#endif

    if (readOnly)
    {
	if (topLevelCommit())
	    return READONLY;
	else
	    return PREP_NOTOK;
    }
    
    return PREP_OK;
}

ostream& LockRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);

    strm << "Lock Record\n";
    return strm;
}

/*
 * restroe_state and save_state for LockRecords doesn't generally
 * apply due to object pointers.
 */

Boolean LockRecord::restore_state ( ObjectState&, ObjectType )
{
   char buff[1024];
   ostrstream temp(buff,1024);
   
   temp << "Invocation of LockRecord::restore_state for "
	<< type() << "inappropriate - ignored for " << order() << endl << ends;
   addError(new Error(FAC_ABSTRACT_REC, AR_OP_IGNORED, buff));
	     
   return FALSE;
}

Boolean LockRecord::save_state ( ObjectState&, ObjectType )
{
    return TRUE;
}

const TypeName LockRecord::type () const
{
    return("/StateManager/AbstractRecord/LockRecord");
}


void LockRecord::merge ( const AbstractRecord * )
{
}

void LockRecord::alter (AbstractRecord *)
{
}

Boolean LockRecord::shouldAdd ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean LockRecord::shouldAlter (const AbstractRecord*) const
{
    return FALSE;
}

Boolean LockRecord::shouldMerge ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean LockRecord::shouldReplace ( const AbstractRecord *ar ) const
{
    if ((order() == ar->order()) &&
	typeIs() == ar->typeIs())
    {
	if (((const LockRecord *)ar)->isReadOnly() && !readOnly)
	    return TRUE;
    }
    return FALSE;
}

#ifdef NO_INLINES
#  define LOCKR_CC_
#  include "LockR.n"
#  undef LOCKR_CC_
#endif
