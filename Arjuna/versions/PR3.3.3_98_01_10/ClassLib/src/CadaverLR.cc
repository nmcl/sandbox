/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CadaverLR.cc,v 1.9 1995/06/26 13:16:29 ngdp Exp $
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

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

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

static const char RCSid[] = "$Id: CadaverLR.cc,v 1.9 1995/06/26 13:16:29 ngdp Exp $";


Boolean CadaverLSetUp::setUp = FALSE;


CadaverLSetUp::CadaverLSetUp ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "CadaverLSetUp::CadaverLSetUp ()" << endl;
    debug_stream.unlock();
    
#endif
    
    if (!setUp)
    {
        CadaverLockRecord tr;
	AbstractRecord::addToList(CadaverLSetUp::create, CadaverLSetUp::remove, tr.type());
	setUp = TRUE;
    }
}

CadaverLSetUp::~CadaverLSetUp () {}

AbstractRecord* CadaverLSetUp::create ()
{
    return new CadaverLockRecord;
}

void CadaverLSetUp::remove (AbstractRecord*& toDelete)
{
    delete toDelete;
    toDelete = 0;
}


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
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS
	         << (FacilityCode) (FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
	         << VIS_PUBLIC;
    debug_stream << "CadaverLockRecord::CadaverLockRecord(" << storeKey
		 << ", " << lm->get_uid() << ")" << endl;
    debug_stream.unlock();
    
#endif
}

CadaverLockRecord::CadaverLockRecord ()
                                      : LockRecord(),
					cadaverLockStore(0),
					objectTypeName(0)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS
	         << (FacilityCode) (FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
	         << VIS_PROTECTED;
    debug_stream << "CadaverLockRecord::CadaverLockRecord ()" << endl;
    debug_stream.unlock();
    
#endif
}

CadaverLockRecord::~CadaverLockRecord()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS
	         << (FacilityCode) (FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
	         << VIS_PUBLIC;
    debug_stream << "CadaverLockRecord::~CadaverLockRecord() for " << order() << endl;
    debug_stream.unlock();
    
#endif
    
    if (cadaverLockStore)
	delete cadaverLockStore;
    
}

/*
 * Public virtual functions. These are all re-implementations of inherited
 * functions 
 */

Boolean CadaverLockRecord::propagateOnAbort () const
{
    return TRUE;
}

/*
 * Atomic action controlled functions. These functions create an instance
 * of CadaverLockManager to handle the lock manipulation that is needed and
 * then throw it away when done.
 */

Boolean CadaverLockRecord::nestedAbort ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS
	         << (FacilityCode) (FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
	         << VIS_PUBLIC;
    debug_stream << "CadaverLockRecord::nestedAbort() for " << order() << endl;
    debug_stream.unlock();
    
#endif

    CadaverLockManager manager(order(),objectTypeName);

    return (manager.releaseAll(AtomicAction::Current()->get_uid()));
}

Boolean CadaverLockRecord::nestedCommit ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS
	         << (FacilityCode) (FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
	         << VIS_PUBLIC;
    debug_stream << "CadaverLockRecord::nestedCommit() for " << order() << endl;
    debug_stream.unlock();
    
#endif

    /*
     * Need to change the owner of the locks from the current
     * committing action to its parent. Since no genuine LockManager
     * exists at this time create one to take care of this.
     */
    
    AtomicAction *curr = AtomicAction::Current();
    
    CadaverLockManager manager(order(),objectTypeName);

    return (manager.propagate(curr->get_uid(), curr->parent()->get_uid()));
}

Boolean CadaverLockRecord::topLevelAbort ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS
	         << (FacilityCode) (FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
	         << VIS_PUBLIC;
    debug_stream << "CadaverLockRecord::topLevelAbort() for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    CadaverLockManager manager(order(),objectTypeName);

    return (manager.releaseAll(AtomicAction::Current()->get_uid()));
}

Boolean CadaverLockRecord::topLevelCommit ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS
	         << (FacilityCode) (FAC_CONCURRENCY_CONTROL | FAC_ABSTRACT_REC)
	         << VIS_PUBLIC;
    debug_stream << "CadaverLockRecord::topLevelCommit() for " 
		 << order() << endl;
    debug_stream.unlock();
    
#endif

    CadaverLockManager manager(order(),objectTypeName);

    return (manager.releaseAll(AtomicAction::Current()->get_uid()));
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


Boolean CadaverLockRecord::shouldReplace ( const AbstractRecord *ar ) const
{
    return (((order() == ar->order()) &&
	     ar->typeIs() == LOCK ) ? TRUE : FALSE);
}


