/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LocalLockDaemon.cc,v 1.5 1995/10/09 08:30:10 ngdp Exp $
 */

/*
 *
 * Lock concurrency controller
 *
 */ 

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ACTIONH_H_
#  include <Arjuna/ActionH.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

#ifndef SEMAPHORE_H_
#  include <Common/Semaphore.h>
#endif

#ifndef LOCKLIST_H_
#  include "LockList.h"
#endif

#ifndef LOCKLISTI_H_
#  include "LockListI.h"
#endif

#ifndef LOCKTABLE_H_
#  include "LockTable.h"
#endif

#ifndef LOCKDAEMONIF_STUB_H_
#  include "LockDaemonIf_stub.h"
#endif

static const char RCSid[] = "$Id: LocalLockDaemon.cc,v 1.5 1995/10/09 08:30:10 ngdp Exp $";

/*
 * Public non-virtual functions 
 */

LocalLockDaemonIf::LocalLockDaemonIf( Boolean & result )
				    : locksHeld(0),
				      mutex(0)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PROTECTED;
    debug_stream << "LocalLockDaemonIf::LocalLockDaemonIf()" << endl;
    debug_stream.unlock();
    
#endif

    result = TRUE;
}

LocalLockDaemonIf::~LocalLockDaemonIf()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_CONCURRENCY_CONTROL << VIS_PROTECTED;
    debug_stream << "LocalLockDaemonIf::~LocalLockDaemonIf()" << endl;
    debug_stream.unlock();
    
#endif    

    Boolean doSignal = FALSE;
    
    if (mutex)
    {
	if (mutex->wait() == Semaphore::SEM_LOCKED)
	    doSignal = TRUE;
    }
    
    if (locksHeld)
	delete locksHeld;
    
    if (mutex)
    {
	if (doSignal)
	    mutex->signal();
	
	delete mutex;
    }
}

/*
 * propagate: Change lock ownership as nested action commits. All
 * locks owned by the committing action have their owners changed to be
 * the parent of the committing action. AtomicAction ensures this is only
 * called at nested commit.
 * We pop things off one list onto another because insert might decide
 * that 2 locks are replicas of each other so we can discard one.
 */

void LocalLockDaemonIf::propagate ( const Uid& fromAction,
				    const Uid& objUid,
				    const TypeName type,
				    Boolean &result )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LocalLockDaemonIf::propagate(" << fromAction << ", " 
		 << objUid << ", " << type <<  ")" << endl;
    debug_stream.unlock();
    
#endif

    result = FALSE;
    
    if ((mutex == 0) && !initialise ())
        return;				/* init failed */

    if (mutex->wait() == Semaphore::SEM_LOCKED)
    {
	LockTableEntry *theList = locksHeld->lookFor(objUid, type);

	if (theList)
	{
	    Lock *current = 0;
	    LockList *newList = new LockList;
	    LockList *oldList = theList->getLockList();
	
	    /* scan through old list of held locks and propagate to parent */
	
	    while ((current = oldList->pop()) != 0)
	    {
		if (current->getCurrentOwner() == fromAction)
		{
		    current->propagate();
		}
		
		if (!newList->insert(current))
		{
		    delete current;
		}
	    }
	
	    theList->replaceList(newList);	/* deletes old as well */
	}	

	result = TRUE;
	mutex->signal();
    }
}

/*
 * releaseAll: Clear out all locks for a given action. Should be
 * triggered automatically at top-level commit but is also user
 * callable so is potentially dangerous.
 */

void LocalLockDaemonIf::releaseAll ( const Uid& actionUid,
				     const Uid& objUid,
				     const TypeName type,
				     Boolean &result )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LocalLockDaemonIf::releaseAll(" << actionUid << ")" << endl;
    debug_stream.unlock();
    
#endif

    result = FALSE;
    
    if ((mutex == 0) && !initialise ())
        return;				/* init failed */
  
    result = doRelease(actionUid, objUid, type, TRUE);
}

/*
 * releaselock: Release a SINGLE LOCK lock that has the given uid. Can break
 * two-phase locking rules so watch out!
 */

void LocalLockDaemonIf::releaselock ( const Uid& lockUid,
				      const Uid& objUid,
				      const TypeName type,
				      Boolean& result )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LocalLockDaemonIf::releaselock(" << lockUid << ")" << endl;
    debug_stream.unlock();
    
#endif

    result = FALSE;
    
    if ((mutex == 0) && !initialise ())
        return ;                   /* init failed */
  
    result = doRelease(lockUid, objUid, type, FALSE);
}

/*
 * setlock: This is the main user visible operation. Attempts to set
 * the given lock on the current object. If lock cannot be set, then
 * the lock attempt is retried retry times before giving up and
 * returning an error. This gives a simple handle on deadlock.
 */

void LocalLockDaemonIf::setlock ( Lock *toSet,
				  const Uid& objUid,
				  const TypeName type,
				  LockResult &returnStatus )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LocalLockDaemonIf::setlock(" << toSet << ", "
		 << objUid << ", " << type << ")" << endl;
    debug_stream.unlock();
    
#endif
    
    returnStatus = REFUSED;
    
    if ((mutex == 0) && !initialise ())
    {
	delete toSet;
        return;				/* init failed */
    }
      
    if (mutex->wait() == Semaphore::SEM_LOCKED)
    {
	if (lockConflict(toSet, objUid, type) !=  CONFLICT)
	{
	    returnStatus = GRANTED;
	}
	else 
	{
	    returnStatus = REFUSED;
	}

	mutex->signal();
    }
    
    if (returnStatus == REFUSED)
    {
	delete toSet;	
    }
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PUBLIC;
    debug_stream << "LocalLockDaemonIf::setlock(" << toSet << ", "
		 << objUid << ", " << type << ") = "
		 << (returnStatus == REFUSED ? "REFUSED " : "GRANTED") 
		 << endl;
    debug_stream.unlock();
    
#endif
 }

/*
 * Private non-virtual functions
 */

/*
 * doRelease: Does all the hard work of lock release. Either releases
 * all locks for a given uid, or simply one lock with a given uid as
 * appropriate.
 */

Boolean LocalLockDaemonIf::doRelease ( const Uid& u, const Uid& obj,
				       const TypeName type, Boolean all )
{ 
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LocalLockDaemonIf::doRelease(" << u << ", "
		 << obj << ", " << type << ", "
		 << all << ")" << endl;
    debug_stream.unlock();
    
#endif

    Boolean deleted = FALSE;
    
    if (mutex->wait() == Semaphore::SEM_LOCKED)
    {
	LockTableEntry *theList = locksHeld->lookFor(obj, type);
	LockList *oldList = (theList ? theList->getLockList() : new LockList());

	if (oldList)
	{
	    LockList *newList = new LockList();
	
	    Lock *current;


	    /*
	     * Now scan through held lock list to find which locks to release
	     * u is either the unique id of the lock owner (all = TRUE)
	     *   or the uid of the actual lock itself (all = FALSE)
	     */

	    while ((current = oldList->pop()) != 0)
	    {
		Uid checkUid(NIL_UID);

		if (all)
		    checkUid = current->getCurrentOwner();
		else
		    checkUid = current->get_uid();

		if (u == checkUid)
		{
		    delete current;
		    
		    deleted = TRUE;
		} else
		    newList->push(current);
	    }

	    theList->replaceList(newList);
	}
    } else
    {
	return FALSE;
    }

#ifdef DEBUG
    debug_stream.lock();
    
    if (!deleted)
    {
	debug_stream << ERROR_MESSAGES << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
	debug_stream << "LocalLockDaemonIf::doRelease(" << u << ", "
		     << obj << ", " << type << ", "
		     << all << ")";
	debug_stream << "*** CANNOT locate locks ***" << endl;
    }
    debug_stream.unlock();
    
#endif

    mutex->signal();

    return TRUE;
}

Boolean LocalLockDaemonIf::initialise ()
{
    mutex = new Semaphore(typeToKey("LocalLockDaemonIf"));

    if ((mutex != 0) && (mutex->wait() == Semaphore::SEM_LOCKED))
    {
        locksHeld = new LockTable;

        mutex->signal();
    }

    if ((mutex == 0) || (locksHeld == 0))
        return FALSE;

    return TRUE;
}

/*
 * Check if holder of heldLock is an ancestor of toSet. Called while locked
 */

Boolean LocalLockDaemonIf::isAncestorOf ( const Lock& heldLock,
					  const Lock& toSet )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LocalLockDaemonIf::isAncestorOf(" << heldLock.get_uid() << ")" << endl;
    debug_stream.unlock();
    
#endif

    return toSet.getAllOwners().isAncestor(heldLock.getCurrentOwner());
    
}

/*
 * lockconflict: Here we attempt to determine if the provided lock is
 * in conflict with any of the existing locks. If it is we use nested
 * locking rules to allow children to lock objects already locked by
 * their ancestors.
 * Called while locked
 */

ConflictType LocalLockDaemonIf::lockConflict ( Lock *toSet,
					       const Uid& objUid,
					       const TypeName type )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_CONCURRENCY_CONTROL << VIS_PRIVATE;
    debug_stream << "LocalLockDaemonIf::lockConflict(" << toSet->get_uid() << ")"
		 << endl;
    debug_stream.unlock();
    
#endif

    Boolean matching = FALSE;
    LockTableEntry *theList = locksHeld->lookFor(objUid, type);
    LockList *locks = (theList ? theList->getLockList() : new LockList);

    Lock *heldLock;
    LockListI next(*locks);
    
    while ((heldLock = next()) != 0)
    {
	if (heldLock->conflictsWith(*toSet))
	{
	    if ((heldLock->getCurrentStatus() == LOCKHELD) ||
		(!isAncestorOf(*heldLock, *toSet)))
	    {
		return (CONFLICT);
	    }
	}
	else 
	    if (*heldLock == *toSet)
		matching = TRUE;
    }

    if (!matching)
    {
	locks->insert(toSet); /* add to local lock list */
	return COMPATIBLE;
    }
    else 
	return PRESENT;
}
