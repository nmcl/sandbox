/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockMan.cc,v 1.1 1993/11/03 12:29:56 nmcl Exp $
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

#ifndef UTILITY_H_
#include <Common/Utility.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef SEMAPHORE_H_
#  include "Semaphore.h"
#endif

#ifndef LOCKR_H_
#  include "LockR.h"
#endif

#ifndef CADAVERLR_H_
#  include "CadaverLR.h"
#endif

#ifndef LOCKLIST_H_
#  include "LockList.h"
#endif

#ifndef LOCKLISTI_H_
#  include "LockListI.h"
#endif

#ifndef LOCKSTORE_H_
#  include "LockStore.h"
#endif

static const char RCSid[] = "$Id: LockMan.cc,v 1.1 1993/11/03 12:29:56 nmcl Exp $";

/* 
 * Implementation of the Lock Management class. Named LocalLockManager
 * because the stub generation system processes the header file to produce
 * RemoteLockManager, ServerLockManager, and LocalLockManager classes
 * togethor with code for RemoteLockManager and ServerLockManager.
 */

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

void LocalLockManager::propagate ( const Uid& from, const Uid& to )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LocalLockManager::propagate(" << from << ", " 
		 << to << ")\n" << flush;
#endif
    
    LockList *oldlist;
    Lock *current;

    if (loadState())
    {
	oldlist = locksHeld;	/* copy pointer to old held list */
	locksHeld = new LockList;	/* create a new one */
	
	/* scan through old list of held locks and propagate to parent */
	
	while ((current = oldlist->pop()) != 0)
	{
	    if (current->getOwner() == from)
	    {
		current->setOwner(to, current->isAction());
		current->setStatus(LOCKRETAINED);
		if (!locksHeld->insert(current))
		{
		    delete current;
		}
	    } else
		locksHeld->push(current);
	}
	delete oldlist;			/* get rid of old lock list */
    
	unloadState();
    }
    else
    {
	error_stream << WARNING
		     << "LockManager::propagate() lock propagation failed (state load failed)\n" << flush;
	freeState();
    }
}

/*
 * releaseAll: Clear out all locks for a given action. Should be
 * triggered automatically at top-level commit but is also user
 * callable so is potentially dangerous.
 */

LockResult LocalLockManager::releaseAll ( const Uid& actionUid )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LocalLockManager::releaseAll(" << actionUid << ")\n" << flush;
#endif

    doRelease(actionUid, ALL_LOCKS);
    return(RELEASED);
}

/*
 * releaselock: Release a SINGLE LOCK lock that has the given uid. Breaks
 * two-phase locking rules so watch out!
 */

LockResult LocalLockManager::releaselock ( const Uid& lockUid )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LocalLockManager::releaselock(" << lockUid << ")\n" << flush;
#endif

    doRelease(lockUid, SINGLE_LOCK);
    return(RELEASED);
}

/*
 * setlock: This is the main user visible operation. Attempts to set
 * the given lock on the current object. If lock cannot be set, then
 * the lock attempt is retried retry times before giving up and
 * returning an error. This gives a simple handle on deadlock.
 */

LockResult LocalLockManager::setlock ( Lock *toSet, 
				       int retry, 
				       unsigned int sleepTime )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LocalLockManager::setlock(" << toSet << ", " << retry << ")\n" << flush;
#endif

    ConflictType conflict = CONFLICT;
    LockResult returnStatus = REFUSED;
    LockRecord *newLockR;
    Boolean modifyRequired = FALSE;
    AtomicAction *currAct = AtomicAction::Current();

    if (currAct != 0)
	toSet->setOwner(currAct->get_uid(), TRUE);

    do
    {
	if (!loadState())
	{
	    error_stream << WARNING
		<< "LockManager::setlock() cannot load existing lock states\n" << flush;
	    break;
	}
	
	if ((conflict = lockConflict(*toSet)) == CONFLICT)
	{	
	    retry--;
	    if (retry > 0)
	    {
		freeState();
		usleep(sleepTime);		/* hope things happen in time */
	    }
	}
    } while ((conflict == CONFLICT) && (retry > 0));

    /* When here the conflict was resolved or the retry limit expired */

    if (conflict != CONFLICT)
    {
	 /* no conflict so set lock */

	modifyRequired = toSet->modifiesObject();

	/* trigger object load from store */
	
	if (LocalStateManager::activate())
	{
	    if (modifyRequired)
		LocalStateManager::modified();

	    returnStatus = GRANTED;	/* lock granted successfully */

	    if (conflict == COMPATIBLE)
	    {
		AtomicAction *action = AtomicAction::Current();

		locksHeld->insert(toSet); /* add to local lock list */
		if (action != 0)
		{				 
		    /* add new lock record to action list */
		    
		    newLockR = new LockRecord(this,
					      (modifyRequired ? FALSE : TRUE));

		    if (action->add(newLockR) == FALSE)
			delete newLockR;
		}
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
			 << "Lockmanager::setlock() cannot save new lock states\n" << flush;
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

ostream& LocalLockManager::print ( ostream& strm ) const
{

    LockListI next(*locksHeld);
    Lock *current;

    strm << "LocalLockManager for object " << get_uid() << "\n";

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

ostream& LocalLockManager::print ( ostream& strm )
{
    Boolean iDeleteState = FALSE;

    if (!stateLoaded)
    {
	loadState();
	iDeleteState = TRUE;
    }

    ((const LocalLockManager * const)this)->print(strm);

    if (iDeleteState)
	freeState();
    
    return strm;
}

const TypeName LocalLockManager::type () const
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

LocalLockManager::LocalLockManager ( const Uid& storeUid, ObjectType ot )
                                   : LocalStateManager(storeUid, ot),
				     locksHeld(new LockList),
				     systemKey(0),
				     lockStore(0),
				     mutex(0),
				     stateLoaded(FALSE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PROTECTED;
    debug_stream << "LocalLockManager::LocalLockManager(" << storeUid << ")\n" << flush;
#endif
}

/*
 * Constructor: Same restrictions apply as documented above
 */

LocalLockManager::LocalLockManager ( ObjectType ot, Boolean isShareable )
                                   : LocalStateManager(ot, isShareable),
				     locksHeld(new LockList),
				     systemKey(0),
				     lockStore(0),
				     mutex(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PROTECTED;
    debug_stream << "LocalLockManager::LocalLockManager(" << ot << ")\n" << flush;
#endif
}

/*
 * Cleanup. Note we grab the semaphore before destroying the
 * the lock store to ensure the store is deleted cleanly
 */

LocalLockManager::~LocalLockManager()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PROTECTED;
    debug_stream << "LocalLockManager::~LocalLockManager()\n" << flush;
#endif    

    if (mutex)
	mutex->wait();

    if (locksHeld != 0)
	delete locksHeld;

    if (lockStore)
	delete lockStore;

    if (mutex)
    {
	mutex->signal();
	
	delete mutex;
    }
}

void LocalLockManager::terminate ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_STATE_MAN << VIS_PROTECTED;
    debug_stream << "LocalLockManager::terminate() for object-id"
	         << get_uid() << "\n" << flush;
#endif

    if (status() != PASSIVE)
    {
	AtomicAction *action = AtomicAction::Current();	
	if (action != 0)
	{
	    if (action->status() == RUNNING)
		action->add(new CadaverLockRecord(systemKey ,this));
	}
    }
    
    LocalStateManager::terminate();
    
}

/*
 * Private non-virtual functions
 */

/*
 * doRelease: Does all the hard work of lock release. Either releases
 * all locks for a given uid, or simply one lock with a given uid as
 * appropriate.
 */

void LocalLockManager::doRelease ( const Uid& u, ReleaseType oneOrAll )
{ 
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LocalLockManager::doRelease(" << u << ", " << oneOrAll << ")\n" << flush;
#endif

    Lock *previous, *current;
    Boolean deleted = FALSE;

    if (loadState())
    {
	/* Must declare iterator after loadstate or it sees an empty list! */

	LockListI next(*locksHeld);

	/*
	 * Now scan through held lock list to find which locks to release
         * u is either the unique id of the lock owner (oneOrAll = All_LOCKS)
         *   or the uid of the actual lock itself (oneOrAll = SINGLE_LOCK)
         */

	previous = 0;
	while ((current = next()) != 0)
	{
	    Uid checkUid(NIL_UID);

	    if (oneOrAll == SINGLE_LOCK) 
		checkUid = current->get_uid();
	    else
		checkUid = current->getOwner();

	    if (u == checkUid)
	    {
		locksHeld->forgetNext(previous);
		delete current;
		deleted = TRUE;
		if (oneOrAll == SINGLE_LOCK)
		{
		    break;
		}
	    } else
		previous = current;
	}
    } else
    {
	error_stream << WARNING 
		     << "Lockmanager::releaselock() could not load old lock states\n" 
		     << flush;
	freeState();
	return;
    }
    

#ifdef DEBUG
    if (!deleted)
    {
	debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
	debug_stream << "*** CANNOT locate locks ***\n" << flush;
    }
#endif

    if (!unloadState())
	error_stream << WARNING 
		     << "Lockmanager::releaselock() could not save new lock states\n" << flush;
}

/*
 *  Simply free up the semaphore. We do this if we detect conflict.
 *  Since the list has not been modified it can simply be discarded.
 *  This will change with the addition of multi-threading (THANKFULLY)
 */

void LocalLockManager::freeState ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LocalLockManager::freeState()\n" << flush;
#endif

    Lock *current;

    while (current = locksHeld->pop())	/* clear out the existing */
					/* list */
	delete current;

    stateLoaded = FALSE;
    mutex->signal();
}

Boolean LocalLockManager::initialise ()
{
    systemKey = typeToKey(type());
    mutex = new Semaphore(systemKey);

    if (mutex != 0)
    {
	mutex->wait();
	
	lockStore = new LockStore(systemKey);

	mutex->signal();
    }
    

    if ((mutex == 0) || (lockStore == 0))
	return FALSE;

    return TRUE;
}

Boolean LocalLockManager::isAncestorOf ( const Lock& heldLock )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LocalLockManager::isAncestorOf(" << heldLock.get_uid() << ")\n" << flush;
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

Boolean LocalLockManager::loadState ()
{
#ifdef DEBUG
    /*
     * Adding the following line stops Cfront 3.0.1 falling over
     * with an internal error!
     */
    debug_stream << flush;
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LocalLockManager::loadState()\n" << flush;
#endif

    ObjectState *S;

    stateLoaded = FALSE;

    if ((mutex == 0) && !initialise ())
	return FALSE;			/* init failed */

    mutex->wait();			/* grab semaphore */

    if (lockStore->read_state(get_uid(), LocalLockManager::type(), S))
    {
	/* Pick returned state apart again */

	if (S != 0)
	{
	    Uid u(NIL_UID);		/* avoid system calls in Uid creation */
	    Lock *current;
	    int count;

	    S->unpack(count);
	    for( int i = 0; i < count; i++)
	    {
		u.unpack(*S);
		current = new Lock(u);
		current->restore_state(*S, ANDPERSISTENT);
		locksHeld->push(current);
		
	    }
	    delete S;
	}
	stateLoaded = TRUE;
    }

    return stateLoaded;
}

/*
 * lockconflict: Here we attempt to determine if the provided lock is
 * in conflict with any of the existing locks. If it is we use nested
 * locking rules to allow children to lock objects already locked by
 * their ancestors.
 */

ConflictType LocalLockManager::lockConflict ( const Lock& otherLock )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LocalLockManager::lockConflict(" << otherLock.get_uid() << ")\n" << flush;
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
 
Boolean LocalLockManager::unloadState ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LocalLockManager::unloadState()\n" << flush;
#endif

    Boolean unloadOk = FALSE;
    Lock *current;
    TypeName otype = LocalLockManager::type();
    Uid u = get_uid();
    ObjectState S(u, otype);
    int lockCount = locksHeld->entryCount();

    /* destroy old state from lock store */
    
    if (lockCount == 0)
    {
	if (lockStore->remove_state(u, otype))
	{
	    unloadOk = TRUE;
	}
	else
	    error_stream << WARNING 
			 << "LockManager::unloadState() failed to remove state for object "
			 << u << " of type " << otype << endl << flush;
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

	/* load image into store */

	if (lockStore->write_committed(u, otype, S))
	{
	    unloadOk = TRUE;
	}
	else
	    error_stream << WARNING 
			 << "LockManager::unloadState() failed to write new state for object "
			 << u << " of type " << otype << endl << flush;
    }
    

    mutex->signal();			 /* and exit mutual exclusion */

    stateLoaded = FALSE;
    return unloadOk;
}

