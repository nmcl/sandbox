/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockMan.cc,v 1.4 1994/01/20 12:51:22 ngdp Exp $
 */

/*
 *
 * Lock concurrency controller
 *
 */ 


#ifndef STDDEF_H_
#  include <System/stddef.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef SYS_FILE_H_
#  include <System/sys/file.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifdef _AIX
#  ifndef SYS_TIME_H_
#    include <System/sys/time.h>
#  endif
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

#ifndef LOCKLIST_H_
#  include "LockList.h"
#endif

#ifndef LOCKLISTI_H_
#  include "LockLstI.h"
#endif


static const char RCSid[] = "$Id: LockMan.cc,v 1.4 1994/01/20 12:51:22 ngdp Exp $";


/*
 * Public non-virtual functions 
 */

/*
 * propagate: Change lock ownership as nested action commits. All
 * locks owned by the committing action have their owners changed to be
 * the parent of the committing action. AtomicAction ensures this is only
 * called at nested commit. This function works by copy the old LockList
 * pointer and then creating a new held lock list. Locks are then moved 
 * from the old to the new, propagating en route.
 */

Boolean LockManager::propagate ( const Uid& from, const Uid& to )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LockManager::propagate(" << from << ", " 
		 << to << ")" << endl;
#endif

	// removed most of the hard work for local MSDOS and OS/2

	return TRUE;
}

/*
 * releaseAll: Clear out all locks for a given action. Should be
 * triggered automatically at top-level commit but is also user
 * callable so is potentially dangerous.
 */

LockResult LockManager::releaseAll ( const Uid& actionUid )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LockManager::releaseAll(" << actionUid << ")" << endl;
#endif

    doRelease(actionUid, ALL_LOCKS);
    return(RELEASED);
}

/*
 * releaselock: Release a SINGLE LOCK lock that has the given uid. Breaks
 * two-phase locking rules so watch out!
 */

LockResult LockManager::releaselock ( const Uid& lockUid )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LockManager::releaselock(" << lockUid << ")" << endl;
#endif

	// removed for local MSDOS and OS/2

    return(RELEASED);
}

/*
 * setlock: This is the main user visible operation. Attempts to set
 * the given lock on the current object. If lock cannot be set, then
 * the lock attempt is retried retry times before giving up and
 * returning an error. This gives a simple handle on deadlock.
 */

LockResult LockManager::setlock ( Lock *toSet, 
				       int retry, 
				       unsigned int sleepTime )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LockManager::setlock(" << toSet << ", " << retry << ")" << endl;
#endif

    ConflictType conflict = CONFLICT;
    LockResult returnStatus = REFUSED;
    LockRecord *newLockR;
    Boolean modifyRequired = FALSE;
    AtomicAction *currAct = AtomicAction::Current();

    if (currAct != 0)
	toSet->setOwner(currAct->get_uid(), TRUE);

    // removed alot for local MSDOS and OS/2

    conflict = COMPATIBLE;

    /* When here the conflict was resolved or the retry limit expired */

    if (conflict != CONFLICT)
    {
	 /* no conflict so set lock */

	modifyRequired = toSet->modifiesObject();

	/* trigger object load from store */
	
	if (StateManager::activate())
	{
	    if (modifyRequired)
		StateManager::modified();

	    returnStatus = GRANTED;	/* assume lock granted successfully */

	    if (conflict == COMPATIBLE)
	    {
		AtomicAction *action = AtomicAction::Current();
		AtomicAction::addOutcome lrStatus = AtomicAction::ADDED;

		if (action != 0)
		{				 
		    /* add new lock record to action list */
		    
		    newLockR = new LockRecord(this,
					      (modifyRequired ? FALSE : TRUE));

		    if ((lrStatus = action->add(newLockR)) != AtomicAction::ADDED)
		    {
			delete newLockR;
			if (lrStatus == AtomicAction::REJECTED)
			    returnStatus = REFUSED;
		    }
		    
		}
		
		if (returnStatus == GRANTED)
		    locksHeld->insert(toSet); /* add to local lock list */
	    }
	} else
	{
	    /* activate failed - refuse request */

	    returnStatus = REFUSED;
	}
    }

    /* 
     * Unload internal state into lock store only if lock list was modified 
     * if this fails claim the setlock failed.
     */

    if ((returnStatus == GRANTED) && (conflict == COMPATIBLE))
    {
	if (!unloadState())
	{
	    error_stream << WARNING 
			 << "Lockmanager::setlock() cannot save new lock states" << endl;
	    returnStatus = REFUSED;
	}
    }
    else
    {
	delete toSet;
	freeState();
    }
    
    return (returnStatus);
}

/*
 * Public virtual functions - redefinitions of inherited functions
 */

ostream& LockManager::print ( ostream& strm ) const
{

    LockListI next(*locksHeld);
    Lock *current;

    strm << "LockManager for object " << get_uid() << "\n";

    if (!stateLoaded)
    {
	strm << "No loaded state\n";
    }
    else if (locksHeld != 0)
    {
	strm << "\tCurrently holding : " << locksHeld->entryCount() << " locks\n";
	while ((current = next()) != 0)
	{
	    strm << *current;
	}
    } else
	strm << "Currently holding : 0 locks\n";

    return strm;
}

/*
 * Load state into object prior to doing the printing
 */

ostream& LockManager::print ( ostream& strm )
{
    Boolean iDeleteState = FALSE;

    if (!stateLoaded)
    {
	loadState();
	iDeleteState = TRUE;
    }

    ((const LockManager * const)this)->print(strm);

    if (iDeleteState)
	freeState();
    
    return strm;
}

const TypeName LockManager::type () const
{
    return "StateManager/LockManager";
}

/*
 * Protected constructors and destructor
 */

/*
 * Constructor: Pass on some args to StateManager and initialise
 * internal state. Note that the current implementation uses 
 * SystemV shared memory and semaphores - these require some
 * key to identify them and allow disjoint processes to share
 * the same space. This key is calculated from the unique id of
 * the object by map_uid() which must be called BEFORE the semaphore
 * and lock store are set up. This mess will go away once we have
 * multi-threaded servers.
 * The lock store and semaphore are set up lazily since they depend
 * upon the result of the type() operation which if run in the 
 * constructor always give the same answer!
 * NOTE: Due to hideously low limits on semaphore creation we only ever
 * use 1 semaphore - this lowers concurrency but at least we don't run out!
 */

 // modified constructors for local MSDOS and OS/2

LockManager::LockManager ( const Uid& storeUid, ObjectType ot )
                                   : StateManager(storeUid, ot),
				     locksHeld(new LockList),
				     systemKey(0),
				     stateLoaded(FALSE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PROTECTED;
    debug_stream << "LockManager::LockManager(" << storeUid << ")" << endl;
#endif
}

/*
 * Constructor: Same restrictions apply as documented above
 */

LockManager::LockManager ( ObjectType ot, Boolean isShareable )
                                   : StateManager(ot, isShareable),
					 locksHeld(new LockList),
					 systemKey(0),
					 stateLoaded(FALSE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PROTECTED;
    debug_stream << "LockManager::LockManager(" << ot << ")" << endl;
#endif
}

/*
 * Cleanup. Note we grab the semaphore before destroying the
 * the lock store to ensure the store is deleted cleanly
 */

LockManager::~LockManager()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PROTECTED;
    debug_stream << "LockManager::~LockManager()" << endl;
#endif    

	// modified for local MSDOS and OS/2

    if (locksHeld != 0)
	delete locksHeld;
}

void LockManager::terminate ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_STATE_MAN << VIS_PROTECTED;
    debug_stream << "LockManager::terminate() for object-id"
	         << get_uid() << "" << endl;
#endif

	// modified alot for local MSDOS and OS/2

	StateManager::terminate();
}

/*
 * Private non-virtual functions
 */

/*
 * doRelease: Does all the hard work of lock release. Either releases
 * all locks for a given uid, or simply one lock with a given uid as
 * appropriate.
 */

void LockManager::doRelease ( const Uid& u, ReleaseType oneOrAll )
{ 
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LockManager::doRelease(" << u << ", " << oneOrAll << ")" << endl;
#endif

    Lock *previous, *current;
    Boolean deleted = FALSE;

    // modified for local MSDOS and OS/2

#ifdef DEBUG
    if (!deleted)
    {
	debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
	debug_stream << "*** CANNOT locate locks ***" << endl;
    }
#endif

    if (!unloadState())
	error_stream << WARNING 
		     << "Lockmanager::releaselock() could not save new lock states" << endl;
}

/*
 *  Simply free up the semaphore. We do this if we detect conflict.
 *  Since the list has not been modified it can simply be discarded.
 *  This will change with the addition of multi-threading (THANKFULLY)
 */

void LockManager::freeState ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LockManager::freeState()" << endl;
#endif

    Lock *current;

    while (current = locksHeld->pop())	/* clear out the existing */
					/* list */
	delete current;

    stateLoaded = FALSE;

    // modified for MSDOS and OS/2
}

Boolean LockManager::initialise ()
{
	// modified alot for MSDOS and OS/2

    return TRUE;
}

Boolean LockManager::isAncestorOf ( const Lock& heldLock )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LockManager::isAncestorOf(" << heldLock.get_uid() << ")" << endl;
#endif

    AtomicAction *action = AtomicAction::Current();

    if (action == 0)
	return FALSE;			/* no action no ancestry! */

    return (action->isAncestor(heldLock.getOwner()));
}

/*
 * Lock and load the concurrency control state. First we grab the
 * semaphore to ensure exclusive access and then we build the held
 * lock list by retreiving the locks from the lock repository.
 * If there is only one server we do not bother doing this since all
 * the locks can stay in the server's memory. This is yet another
 * consequence of not having multi-threaded servers.
 */

Boolean LockManager::loadState ()
{
#ifdef DEBUG
    /*
     * Adding the following line stops Cfront 3.0.1 falling over
     * with an internal error!
     */
    debug_stream << flush;
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LockManager::loadState()" << endl;
#endif

	// modified for MSDOS and OS/2

    return stateLoaded;
}

/*
 * lockconflict: Here we attempt to determine if the provided lock is
 * in conflict with any of the existing locks. If it is we use nested
 * locking rules to allow children to lock objects already locked by
 * their ancestors.
 */

ConflictType LockManager::lockConflict ( const Lock& otherLock )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LockManager::lockConflict(" << otherLock.get_uid() << ")" << endl;
#endif

    Boolean matching = FALSE;
    Lock *heldLock;
    LockListI next(*locksHeld);
    
    while ((heldLock = next()) != 0)
    {
	if (heldLock->conflictsWith(otherLock))
	{
	    if (!isAncestorOf(*heldLock))  /* not quite Moss's rules */
	    {
                return (CONFLICT);
	    }
	}
        else
           if (*heldLock == otherLock)
               matching = TRUE;
    }
    return(matching ? PRESENT : COMPATIBLE);
}

/*
 *   Unload the state by writing all the locks to the repository
 *   and then freeing the semaphore.
 */
 
Boolean LockManager::unloadState ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LockManager::unloadState()" << endl;
#endif

    Boolean unloadOk = FALSE;
    Lock *current;
    TypeName otype = LockManager::type();
    Uid u = get_uid();
    ObjectState S(u, otype);
    int lockCount = locksHeld->entryCount();

    /* destroy old state from lock store */
    
    if (lockCount == 0)
	{
    // modified for MSDOS and OS/2

	unloadOk = TRUE;
    }
    else
    {
	
	/* generate new state */
    
	S.pack(lockCount);
	while (current = locksHeld->pop())
	{
	    current->get_uid().pack(S);
	    current->save_state(S, ANDPERSISTENT);
	    delete current;
	}

    // modified for MSDOS and OS/2

	unloadOk = TRUE;
	}

    stateLoaded = FALSE;
    return unloadOk;
}

