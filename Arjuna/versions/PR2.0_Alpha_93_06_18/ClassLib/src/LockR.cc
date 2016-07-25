/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockR.cc,v 1.13 1993/11/02 16:05:23 nsmw Exp $
 */

/*
 * 
 * Lock Record Class Implementation
 *
 */ 

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
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

static const char RCSid[] = "$Id: LockR.cc,v 1.13 1993/11/02 16:05:23 nsmw Exp $";

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
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << CONSTRUCTORS
		 << (FacilityCode)(FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
		  << VIS_PUBLIC;
    debug_stream << "LockRecord::LockRecord(" << (void *)lm << ", "
	         << readOnly << ")\n" << flush;
#endif
}

LockRecord::~LockRecord()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS
		 << (FacilityCode)(FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
		  << VIS_PUBLIC;
    debug_stream << "LockRecord::~LockRecord()\n" << flush;
#endif
}

void LockRecord::merge ( const AbstractRecord * )
{
}

void LockRecord::alter (AbstractRecord *)
{
}

Boolean LockRecord::should_add ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean LockRecord::should_merge ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean LockRecord::should_replace ( const AbstractRecord *ar ) const
{
#ifdef DEBUG
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "LockRecord::should_replace(" << (void *)ar << ")\n" << flush;
#endif

    if ((ordering() == ar->ordering()) &&
	type_is() == ar->type_is())
    {
	if (((const LockRecord *)ar)->isReadOnly() && !readOnly)
	    return TRUE;
    }
    return FALSE;
}

Boolean LockRecord::should_alter (const AbstractRecord*) const
{
    return FALSE;
}

RecordType LockRecord::type_is () const
{
    return LOCK;
}

AnyType LockRecord::value () const
{
    return (AnyType)managerAddress;
}

void LockRecord::setValue (AnyType)
{
    error_stream << WARNING << "LockRecord::setValue() : called\n";
}

void LockRecord::nested_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS
		 << (FacilityCode)(FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
		  << VIS_PUBLIC;
    debug_stream << "LockRecord::nested_abort()\n" << flush;
#endif

    AtomicAction *currAct = AtomicAction::Current();
    
    if (currAct)
	managerAddress->releaseAll(currAct->get_uid());
}

void LockRecord::nested_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS
		 << (FacilityCode)(FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
		  << VIS_PUBLIC;
    debug_stream << "LockRecord::nested_commit()\n" << flush;
#endif

    AtomicAction *currAct = AtomicAction::Current();
    
    if (currAct)
	managerAddress->propagate(currAct->get_uid(),
				  currAct->parent()->get_uid());
}

PrepareOutcome LockRecord::nested_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS
		 << (FacilityCode)(FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
		  << VIS_PUBLIC;
    debug_stream << "LockRecord::nested_prepare()\n" << flush;
#endif

    return PREP_OK;
}

void LockRecord::top_level_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS
		 << (FacilityCode)(FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
		  << VIS_PUBLIC;
    debug_stream << "LockRecord::top_level_abort()\n" << flush;
#endif
    
    AtomicAction *currAct = AtomicAction::Current();
    
    if (currAct)
	managerAddress->releaseAll(currAct->get_uid());
}

void LockRecord::top_level_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS
		 << (FacilityCode)(FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
		  << VIS_PUBLIC;
    debug_stream << "LockRecord::top_level_commit()\n" << flush;
#endif
    
    AtomicAction *currAct = AtomicAction::Current();
    
    if (currAct)
	managerAddress->releaseAll(currAct->get_uid());
}

PrepareOutcome LockRecord::top_level_prepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS
		 << (FacilityCode)(FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
		  << VIS_PUBLIC;
    debug_stream << "LockRecord::top_level_prepare()\n" << flush;
#endif

    if (readOnly)
    {
	AtomicAction *currAct = AtomicAction::Current();
    
	if (currAct)
	    managerAddress->releaseAll(currAct->get_uid());

	return READONLY;
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
    error_stream << WARNING << " Invocation of restore_state for LockRecords "
           	 << "inappropriate - ignored\n";
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


#ifdef NO_INLINES
#  define LOCKR_CC_
#  include "LockR.n"
#  undef LOCKR_CC_
#endif
