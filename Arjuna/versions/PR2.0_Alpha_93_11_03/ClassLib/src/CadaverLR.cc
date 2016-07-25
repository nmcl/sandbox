/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CadaverLR.cc,v 1.1 1993/11/03 12:29:47 nmcl Exp $
 */

/*
 *
 * Cadaver Lock Record Class Implementation
 *
 * Instances of this record class are created by LockManager if the
 * object goes out of scope prior to the end of a manipulating action.
 * The intention is that the operations of this class will clean up
 * those locks that get left set as the object goes out of scope but
 * which must remain held until the action ends otherwise serialisability
 * is compromised
 *
 */ 

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef CADAVERLM_H_
#  include "CadaverLM.h"
#endif

#ifndef CADAVERLR_H_
#  include "CadaverLR.h"
#endif

#ifndef LOCKSTORE_H_
#  include "LockStore.h"
#endif

static const char RCSid[] = "$Id: CadaverLR.cc,v 1.1 1993/11/03 12:29:47 nmcl Exp $";

/*
 * Public constructors and destructor
 */

CadaverLockRecord::CadaverLockRecord ( const key_t storeKey,
				       LockManager *lm )
                                     : LockRecord(lm),
				       cadaverLockStore(new LockStore(storeKey)),
				       objectTypeName(lm->type())
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS
	         << (FacilityCode) (FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
	         << VIS_PUBLIC;
    debug_stream << "CadaverLockRecord::CadaverLockRecord(" << storeKey
		 << ", " << lm << ")\n" << flush;
#endif
}

CadaverLockRecord::~CadaverLockRecord()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS
	         << (FacilityCode) (FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
	         << VIS_PUBLIC;
    debug_stream << "CadaverLockRecord::~CadaverLockRecord()\n" << flush;
#endif
    
    if (cadaverLockStore)
	delete cadaverLockStore;
    
}

/*
 * Public virtual functions. These are all re-implementations of inherited
 * functions 
 */

Boolean CadaverLockRecord::should_replace ( const AbstractRecord *ar ) const
{
    return (((ordering() == ar->ordering()) &&
	     ar->type_is() == LOCK ) ? TRUE : FALSE);
}

/*
 * Atomic action controlled functions. These functions create an instance
 * of CadaverLockManager to handle the lock manipulation that is needed and
 * then throw it away when done.
 */

void CadaverLockRecord::nested_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS
	         << (FacilityCode) (FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
	         << VIS_PUBLIC;
    debug_stream << "CadaverLockRecord::nested_abort()\n" << flush;
#endif

    CadaverLockManager manager(ordering(),objectTypeName);

    manager.releaseAll(AtomicAction::Current()->get_uid());
}

void CadaverLockRecord::nested_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS
	         << (FacilityCode) (FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
	         << VIS_PUBLIC;
    debug_stream << "CadaverLockRecord::nested_commit()\n" << flush;
#endif

    /*
     * Need to change the owner of the locks from the current
     * committing action to its parent. Since no genuine LockManager
     * exists at this time create one to take care of this.
     */
    
    AtomicAction *curr = AtomicAction::Current();
    
    CadaverLockManager manager(ordering(),objectTypeName);

    manager.propagate(curr->get_uid(),
		      curr->parent()->get_uid());
}

void CadaverLockRecord::top_level_abort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS
	         << (FacilityCode) (FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
	         << VIS_PUBLIC;
    debug_stream << "CadaverLockRecord::top_level_abort()\n" << flush;
#endif

    CadaverLockManager manager(ordering(),objectTypeName);

    manager.releaseAll(AtomicAction::Current()->get_uid());
}

void CadaverLockRecord::top_level_commit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS
	         << (FacilityCode) (FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
	         << VIS_PUBLIC;
    debug_stream << "CadaverLockRecord::top_level_commit()\n" << flush;
#endif

    CadaverLockManager manager(ordering(),objectTypeName);

    manager.releaseAll(AtomicAction::Current()->get_uid());
}

ostream& CadaverLockRecord::print ( ostream& strm ) const
{
    strm << "CadaverLockRecord : ";
    strm << "\n";
    return LockRecord::print(strm);
}

const TypeName CadaverLockRecord::type () const
{
    return("/StateManager/AbstractRecord/LockRecord/CadaverLockRecord");
}



