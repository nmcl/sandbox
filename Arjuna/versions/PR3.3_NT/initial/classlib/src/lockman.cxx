/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockMan.cc,v 1.23 1995/10/13 09:02:36 ngdp Exp $
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

#ifndef WIN32

#ifndef SYS_FILE_H_
#  include <System/sys/file.h>
#endif

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
#  include <Common/Utility.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

#ifndef ACTIONH_H_
#  include <Arjuna/ActionH.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef SEMAPHORE_H_
#  include <Common/Semaphore.h>
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

#if defined(LOCKDAEMON_H_) && !defined(RESTRICTED_ARJUNA)
#  include <ArjServers/LockDaemon.h>
#endif

static const char RCSid[] = "$Id: LockMan.cc,v 1.23 1995/10/13 09:02:36 ngdp Exp $";
static const unsigned int DOZE_TIME = 1000000;

/* 
 * Implementation of the Lock Management class. Named LocalLockManager
 * because the stub generation system processes the header file to produce
 * RemoteLockManager, ServerLockManager, and LocalLockManager classes
 * togethor with code for RemoteLockManager and ServerLockManager.
 */

/*
 * If we are using the lock daemon. Most of the functionality normally
 * implemented by LockManager is forwarded to the daemon
 */

/*
 * Public non-virtual functions 
 */

/*
 * propagate: Change lock ownership as nested action commits. All
 * locks owned by the committing action have their owners changed to be
 * the parent of the committing action. AtomicAction ensures this is only
 * called at nested commit. This function works by copying the old LockList
 * pointer and then creating a new held lock list. Locks are then moved 
 * from the old to the new, propagating en route.
 */

Boolean LocalLockManager::propagate ( const Uid& from, const Uid& to )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LocalLockManager::propagate(" << from << ", " 
		 << to << ")" << endl;
    debug_stream.unlock();
    
#endif

    Boolean result = FALSE;
    int retryCount = 10;

    do
    {
		if (loadState())
		{
	#ifndef RESTRICTED_ARJUNA
			if (useDaemon)
			{
				if (initialise() && lockDaemon)
					result = lockDaemon->propagate(from, get_uid(), type());
			}
			else
	#endif	
			{
				LockList *oldlist = locksHeld;
				Lock *current = 0;
				Boolean found = FALSE;
				
				locksHeld = new LockList;	/* create a new one */
				
				if (locksHeld)
				{
					/* scan through old list of held locks and propagate to parent */
					
					while ((current = oldlist->pop()) != 0)
					{
					if (current->getCurrentOwner() == from)
					{
						found = TRUE;
						current->propagate();
					}
					
					if (!locksHeld->insert(current))
					{
						delete current;
					}
					}
					delete oldlist;		/* get rid of old lock list */
					result = TRUE;
				}
				else
				{
					/* Cannot create new locklist - abort and try again */
					
					freeState();
					usleep(DOZE_TIME);
				}
			}
		}
		else
		{    
			freeState();
			usleep(DOZE_TIME);
		}

    } while ((!result) && (--retryCount > 0));

    if (result)
		result = unloadState();
    else
    {
		Error *error = 0;
		
		error = new Error(FAC_CONCURRENCY_CONTROL, LM_PROPAGATE_FAILED,
				  "LockManager: lock propagation failed");
		addError(error);

		freeState();
    }

    return result;
}

/*
 * releaseAll: Clear out all locks for a given action. Should be
 * triggered automatically at top-level commit but is also user
 * callable so is potentially dangerous.
 */

Boolean LocalLockManager::releaseAll ( const Uid& actionUid )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LocalLockManager::releaseAll(" << actionUid << ")" << endl;
    debug_stream.unlock();
    
#endif

    Boolean result = FALSE;

#ifndef RESTRICTED_ARJUNA
    if (useDaemon)
    {
		if (initialise() && lockDaemon)
			result = lockDaemon->releaseAll(actionUid, get_uid(), type());
    }
    else
    {
		result = doRelease(actionUid, TRUE);
    }
#else
	result = doRelease(actionUid, TRUE);
#endif
    
    return result;
    
}

/*
 * releaselock: Release a SINGLE LOCK lock that has the given uid. Breaks
 * two-phase locking rules so watch out!
 */

Boolean LocalLockManager::releaselock ( const Uid& lockUid )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LocalLockManager::releaselock(" << lockUid << ")" << endl;
    debug_stream.unlock();
    
#endif

    Boolean result = FALSE;
 
#ifndef RESTRICTED_ARJUNA
    if (useDaemon)
    {
		if (initialise() && lockDaemon)
			result = lockDaemon->releaselock(lockUid, get_uid(), type());
    }
    else
    {
		result = doRelease(lockUid, FALSE);
    }
#else
	result = doRelease(lockUid, FALSE);
#endif
    
    return result;
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
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LocalLockManager::setlock(" << toSet << ", " << retry << ")" << endl;
    debug_stream.unlock();
    
#endif

    ConflictType conflict = CONFLICT;
    LockResult returnStatus = REFUSED;
    LockRecord *newLockR = 0;
    Boolean modifyRequired = FALSE;
    AtomicAction *currAct = 0;
    Error *error = 0;

    clearErrorList();

    if (toSet == 0)
    {
		error = new Error(FAC_CONCURRENCY_CONTROL, LM_BAD_PARAMS,
				  "LockManager::setlock() no lock!");

		addError(error);
		return REFUSED;
    }
    
    currAct = AtomicAction::Current();
    
    if (currAct != 0)
    {
		ActionHierarchy *ah = currAct->getHierarchy();

		if (ah)
			toSet->changeHierarchy(*ah);
		else
		{
			error = new Error(FAC_CONCURRENCY_CONTROL, LM_BAD_HIERARCHY,
					  "LockManager::setlock() cannot find action hierarchy");

			addError(error);
			delete toSet;
			return REFUSED;
		}
    }

    do
    {
		conflict = CONFLICT;
		
		if (loadState())
		{
#ifndef RESTRICTED_ARJUNA
			if (useDaemon)
			{
				if (initialise() && (lockDaemon != 0))
				{
					returnStatus = lockDaemon->setlock(toSet, get_uid(), type());
					conflict = (returnStatus == GRANTED ? COMPATIBLE : CONFLICT);
				}
				else
					break;
			}
			else 
			{
				conflict = lockConflict(*toSet);
			}
#else
			conflict = lockConflict(*toSet);
#endif
		}
		else
		{
			error = new Error(FAC_CONCURRENCY_CONTROL, LM_LOCKLOAD_FAILED,
					  "LockManager::setlock() cannot load existing lock states");

			addError(error);
		}
		
		if (conflict == CONFLICT)
		{	
			retry--;
			if (retry > 0)
			{
				freeState();
				usleep(sleepTime);	/* hope things happen in time */
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
			returnStatus = GRANTED;

			if (conflict == COMPATIBLE)
			{
				AtomicAction::addOutcome lrStatus = AtomicAction::ADDED;

				if (currAct != 0)
				{				 
					/* add new lock record to action list */
					
					newLockR = new LockRecord(this,
								  (modifyRequired ? FALSE : TRUE));

					if ((lrStatus = currAct->add(newLockR)) != AtomicAction::ADDED)
					{
						delete newLockR;
						if (lrStatus == AtomicAction::REJECTED)
							returnStatus = REFUSED;
					}
					
				}

#ifndef RESTRICTED_ARJUNA
				if ((returnStatus == GRANTED) && !useDaemon)
#else
				if (returnStatus == GRANTED)
#endif
					locksHeld->insert(toSet); /* add to local lock list */
			}
		} else
		{
			/* activate failed - refuse request */
			
			error = new Error(FAC_CONCURRENCY_CONTROL, LM_ACTIVATE_FAILED,
					  "LockManager::setlock() cannot activate object");
			
			addError(error);
			
			returnStatus = REFUSED;

#ifndef RESTRICTED_ARJUNA
			if (useDaemon)
			{
				/* throw away lock we have already set */
				
				lockDaemon->releaselock(toSet->get_uid(), get_uid(), type());
			}
#endif	
		}
    }

    /* 
     * Unload internal state into lock store only if lock list was modified 
     * if this fails claim the setlock failed.
     * If we are using the lock daemon we can arbitrarily throw the lock away
     * as the daemon has it.
     */

#ifndef RESTRICTED_ARJUNA
    if ((returnStatus == GRANTED) && (conflict == COMPATIBLE) && !useDaemon)
#else
    if ((returnStatus == GRANTED) && (conflict == COMPATIBLE))
#endif
    {
		if (!unloadState())
		{
			error = new Error(FAC_CONCURRENCY_CONTROL, LM_LOCKUNLOAD_FAILED,
					  "LockManager::setlock() cannot save new lock states");

			addError(error);

			returnStatus = REFUSED;
		}
    }
    else
    {
		delete toSet;
		freeState();
    }

    /*
     * Postpone call on modified to here so that semaphore will have been
     * released. This means when modified invokes save_state that routine
     * may set another lock without blocking
     */
    
    if (returnStatus == GRANTED)
    {
		if (modifyRequired)
			LocalStateManager::modified();
		
		hasBeenLocked = TRUE;
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
 * the same space. This key is calculated from the type of
 * the object by typeToKey() which must be called BEFORE the semaphore
 * and lock store are set up. This mess will go away once we have
 * multi-threaded servers.
 * The lock store and semaphore are set up lazily since they depend
 * upon the result of the type() operation which if run in the 
 * constructor always give the same answer!
 */

LocalLockManager::LocalLockManager ( const Uid& storeUid,
				     ObjectType ot,
				     Boolean useLockDaemon )
                                   : LocalStateManager(storeUid, ot),
				     systemKey(0),
				     locksHeld(new LockList),
				     lockStore(0),
				     mutex(0),
				     stateLoaded(FALSE),
				     hasBeenLocked(FALSE),
#ifndef RESTRICTED_ARJUNA
				     useDaemon(useLockDaemon),
				     objectLocked(FALSE),
				     lockDaemon(0)
#else
					 useDaemon(FALSE),
					 objectLocked(FALSE)
#endif
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PROTECTED;
    debug_stream << "LocalLockManager::LocalLockManager(" << storeUid << ")" << endl;
    debug_stream.unlock();
    
#endif
}

/*
 * Constructor: Same restrictions apply as documented above
 */

LocalLockManager::LocalLockManager ( ObjectType ot,
				     Boolean isShareable,
				     Boolean useLockDaemon )
                                   : LocalStateManager(ot, isShareable),
				     systemKey(0),
				     locksHeld(new LockList),
				     lockStore(0),
				     mutex(0),
				     stateLoaded(FALSE),
				     hasBeenLocked(FALSE),
#ifndef RESTRICTED_ARJUNA
				     useDaemon(useLockDaemon),
				     objectLocked(FALSE),
				     lockDaemon(0)
#else
					 useDaemon(FALSE),
					 objectLocked(FALSE)
#endif
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PROTECTED;
    debug_stream << "LocalLockManager::LocalLockManager(" << ot << ")" << endl;
    debug_stream.unlock();
    
#endif
}

/*
 * Cleanup. Note we grab the semaphore before destroying the
 * the lock store to ensure the store is deleted cleanly
 */

LocalLockManager::~LocalLockManager()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PROTECTED;
    debug_stream << "LocalLockManager::~LocalLockManager()" << endl;
    debug_stream.unlock();
    
#endif    

    Boolean doSignal = TRUE;
    Semaphore::Status semStat = Semaphore::SEM_ERROR;
    
    cleanUp();
    
    if (mutex)
    {
		semStat = mutex->wait();
		doSignal = (semStat == Semaphore::SEM_LOCKED);
    }
    
    if (locksHeld != 0)
		delete locksHeld;

    if (lockStore)
		delete lockStore;

#ifndef WIN32
    if (lockDaemon)
		delete lockDaemon;
#endif
    
    if (mutex)
    {
		if (doSignal)
			mutex->signal();
		
		delete mutex;
    }
}

void LocalLockManager::terminate ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_STATE_MAN << VIS_PROTECTED;
    debug_stream << "LocalLockManager::terminate() for object-id"
	         << get_uid() << "" << endl;
    debug_stream.unlock();
    
#endif

    cleanUp();
    LocalStateManager::terminate();
    
}

/*
 * Turn on use of lock daemon. Can be done any time up to first call of setlock
 * (really initialise) after which use or otherwise will already have been decided
 */

Boolean LocalLockManager::useLockDaemon ()
{
#ifndef RESTRICTED_ARJUNA
    if (lockStore == 0)
		useDaemon = TRUE;
#endif

    return useDaemon;
}

void LocalLockManager::cleanUp ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_STATE_MAN << VIS_PROTECTED;
    debug_stream << "LocalLockManager::cleanUp() for object-id"
	         << get_uid() << "" << endl;
    debug_stream.unlock();
    
#endif
    
    if (hasBeenLocked)
    {
		if (mutex == 0)
			initialise();

		AtomicAction *action = AtomicAction::Current();	
		if (action != 0)
		{
			AbstractRecord *A = new CadaverLockRecord(systemKey ,this);
			
			if (action->add(A) != AtomicAction::ADDED)
			delete A;
		}
    }

    hasBeenLocked = FALSE;
    
}

/*
 * Private non-virtual functions
 */

/*
 * doRelease: Does all the hard work of lock release. Either releases
 * all locks for a given uid, or simply one lock with a given uid as
 * appropriate. ONLY invoked if useDaemon is FALSE.
 */

Boolean LocalLockManager::doRelease ( const Uid& u, Boolean all )
{ 
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LocalLockManager::doRelease(" << u << ", " << all << ")" << endl;
    debug_stream.unlock();
    
#endif

    Lock *previous, *current;
    Boolean deleted = FALSE;
    Boolean result = FALSE;
    int retryCount = 10;
    
    clearErrorList();

    do
    {
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
		
		if (all) 
		    checkUid = current->getCurrentOwner();
		else
		    checkUid = current->get_uid();
		
		if (u == checkUid)
		{
		    locksHeld->forgetNext(previous);
		    delete current;
		    deleted = TRUE;
		    if (!all)
		    {
			break;
		    }
		}
		else
		    previous = current;
	    }

	    result = TRUE;
	}
	else
	{    
	    freeState();
	    usleep(DOZE_TIME);
	}
    } while ((!result) && (--retryCount > 0));

    if (!stateLoaded)
    {
	addError(new Error(FAC_CONCURRENCY_CONTROL, LM_LOCKLOAD_FAILED,
			   "Lockmanager::releaselock() could not load old lock states"));
	
	freeState();
	return FALSE;
    }
    

#ifdef DEBUG
    debug_stream.lock();
    
    if (!deleted)
    {
	debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
	debug_stream << "*** CANNOT locate locks ***" << endl;
    }
    debug_stream.unlock();
    
#endif

    retryCount = 10;

    do
    {
	if (!unloadState())
	{
	    addError(new Error(FAC_CONCURRENCY_CONTROL, LM_LOCKLOAD_FAILED,
			       "Lockmanager::releaselock() could not unload new lock states"));
	}
	else
	    return TRUE;
    }
    while (--retryCount > 0);
    
    return FALSE;
}

/*
 *  Simply free up the semaphore. We do this if we detect conflict.
 *  Since the list has not been modified it can simply be discarded.
 *  This will change with the addition of multi-threading (THANKFULLY)
 */

void LocalLockManager::freeState ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LocalLockManager::freeState()" << endl;
    debug_stream.unlock();
    
#endif

    if (!useDaemon)
    {
		Lock *current;
		
		while ((current = locksHeld->pop())) /* clear out the existing */
							 /* list */
			delete current;
		
    }
    
    stateLoaded = FALSE;

    if (objectLocked)
    {
		objectLocked = FALSE;
		mutex->signal();
    }
}

Boolean LocalLockManager::initialise ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LocalLockManager::initialise()" << endl;
    debug_stream.unlock();
    
#endif
    
    Boolean result = FALSE;
    
    if (mutex == 0)
    {
		systemKey = typeToKey(type());
		mutex = new Semaphore(systemKey);
    }

#ifndef RESTRICTED_ARJUNA
    if ((lockStore == 0) && (lockDaemon == 0))
#else
	if (lockStore == 0)
#endif
    {
		if (mutex != 0 )
		{
			Semaphore::Status semStat = mutex->wait();

			if (semStat == Semaphore::SEM_LOCKED)
			{
#ifndef RESTRICTED_ARJUNA
				if (useDaemon)
				{
					if (lockDaemon == 0)
						lockDaemon = new LockDaemon();
					
					result = lockDaemon != 0;
				}
				else
#endif
				{    
					if (lockStore == 0)
						lockStore = new LockStore(systemKey);
					result = lockStore != 0;
				}
			
				mutex->signal();
			}
		}
    }
    else
		result = TRUE;

    return result;
}

Boolean LocalLockManager::isAncestorOf ( const Lock& heldLock )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LocalLockManager::isAncestorOf(" << heldLock.getCurrentOwner() << ")" << endl;
    debug_stream.unlock();
    
#endif

    AtomicAction *action = AtomicAction::Current();

    if (action == 0)
		return FALSE;			/* no action no ancestry! */

    return (action->isAncestor(heldLock.getCurrentOwner()));
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
    debug_stream.lock();
    
    /*
     * Adding the following line stops Cfront 3.0.1 falling over
     * with an internal error!
     */
    debug_stream << flush;
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LocalLockManager::loadState()" << endl;
    debug_stream.unlock();
    
#endif

    ObjectState *S = 0;
    Semaphore::Status semStat = Semaphore::SEM_UNLOCKED;
	
    stateLoaded = FALSE;

    if ((mutex == 0) && !initialise ())
		return FALSE;			/* init failed */

    if ((semStat = mutex->wait()) != Semaphore::SEM_LOCKED)
    {
		return FALSE;
    }
    
    objectLocked = TRUE;
  
#ifndef RESTRICTED_ARJUNA
    if (useDaemon)
    {
		stateLoaded = TRUE;
		return TRUE;
    }
#endif
    
    if (lockStore->read_state(get_uid(), LocalLockManager::type(), S))
    {
	/* Pick returned state apart again */

	if (S != 0)
	{
	    Uid u(NIL_UID);		/* avoid system calls in Uid creation */
	    Lock *current = 0;
	    int count = 0;

	    if (S->unpack(count))
	    {
		Boolean cleanLoad = TRUE;
		
#ifdef DEBUG
		debug_stream.lock();
	    
		debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
		debug_stream << "LocalLockManager::loadState() loading "
			     << count << " lock(s)" << endl;
		debug_stream.unlock();
	    
#endif

		/* Carefully rebuild the internal state - if we fail throw it away and return */
		
		for (int i = 0; (i < count) && cleanLoad; i++)
		{
		    if (u.unpack(*S))
		    {
			current = new Lock(u);

			if (current)
			{
			    if (current->restore_state(*S, ANDPERSISTENT))
				locksHeld->push(current);
			    else
			    {
				delete current;
				cleanLoad = FALSE;
			    }
			}
			else
			    cleanLoad = FALSE;
		    }
		    else
		    {
			cleanLoad = FALSE;
		    }
		    
		}

		if (cleanLoad)
		    stateLoaded = TRUE;
		else
		{
		    while (current = locksHeld->pop())
			delete current;
		}
	    }
	    
	    delete S;
	}
	else
	    stateLoaded = TRUE;
    }
    else
    {
	ErrorList& eList = lockStore->getErrorList();
	Error *error = 0;

	while ((error = eList.pop()))
	{
	    addError(error);
	}

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
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LocalLockManager::lockConflict(" << otherLock.get_uid() << ")" << endl;
    debug_stream.unlock();
    
#endif

    Boolean matching = FALSE;
    Lock *heldLock;
    LockListI next(*locksHeld);
    
    while ((heldLock = next()) != 0)
    {
		if (heldLock->conflictsWith(otherLock))
		{
			if ((heldLock->getCurrentStatus() == LOCKHELD) || !(isAncestorOf(*heldLock)))
			{
				return (CONFLICT);
			}
		}
		else
			if (*heldLock == otherLock)
				matching = TRUE;
    }

    return (matching ? PRESENT : COMPATIBLE);
}

/*
 *   Unload the state by writing all the locks to the repository
 *   and then freeing the semaphore.
 */
 
Boolean LocalLockManager::unloadState ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LocalLockManager::unloadState()" << endl;
    debug_stream.unlock();
    
#endif

    Boolean unloadOk = useDaemon;

    if (!useDaemon)
    {
		Lock *current = 0;
		TypeName otype = LocalLockManager::type();
		Uid u = get_uid();
		ObjectState S(u, otype);
		int lockCount = locksHeld->entryCount();

		/* destroy old state from lock store */

	#ifdef DEBUG
		debug_stream.lock();
		
		debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
		debug_stream << "LocalLockManager::unloadState() unloading "
				 << lockCount << " lock(s)" << endl;
		debug_stream.unlock();
		
	#endif
		
		if (lockCount == 0)
		{
			if (lockStore->remove_state(u, otype))
			{
				unloadOk = TRUE;
			}
			else 
			{
				char buff[1024];
				ostrstream temp(buff,1024);
				
				temp << "LockManager::unloadState() failed to remove empty lock state for object "
					 << u << " of type " << otype << ends;
				addError(new Error(FAC_CONCURRENCY_CONTROL, OS_REMOVE_FAILED, buff));
			}
		}
		else
		{
			/* generate new state */
			
			S.pack(lockCount);

			while ((current = locksHeld->pop()))
			{
				current->get_uid().pack(S);
				current->save_state(S, ANDPERSISTENT);
				delete current;
			}
			
			/* load image into store */
			
			if ((S.readState() == BUFFER_GOOD) && (lockStore->write_committed(u, otype, S)))
			{
				unloadOk = TRUE;
			}
			else
			{
				char buff[1024];
				ostrstream temp(buff,1024);
				
				/*
				 * Propagate any error info in the lock store to this object
				 */
				
				ErrorList& eList = lockStore->getErrorList();
				Error *error = 0;
				
				while ((error = eList.pop()))
				{
					addError(error);
				}
				
				temp << "LockManager::unloadState() failed to ";
				
				if (S.readState() != BUFFER_GOOD)
				{
					temp << "pack up ";
				}
				else
				{
					temp << "write ";
				}
				
				temp << "new state for object " << u << " of type " << otype << ends;
				
				addError(new Error(FAC_CONCURRENCY_CONTROL, OS_WRITE_FAILED, buff));
			}
		}
    }
    
    stateLoaded = FALSE;
    if (objectLocked)
    {
		objectLocked = FALSE;
		mutex->signal();		/* and exit mutual exclusion */
    }

    return unloadOk;
}

