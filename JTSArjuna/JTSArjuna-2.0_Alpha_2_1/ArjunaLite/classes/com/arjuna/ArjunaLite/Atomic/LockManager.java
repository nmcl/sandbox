/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: LockManager.java,v 1.6 2000/04/18 13:28:30 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

import com.arjuna.ArjunaLite.ArjunaLiteNames;
import com.arjuna.ArjunaLite.Interface.LockStore;
import com.arjuna.ArjunaLite.Interface.Semaphore;
import com.arjuna.ArjunaLite.Common.ArjunaLiteEnvironment;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Template.*;
import java.io.PrintStream;
import java.net.InetAddress;
import java.util.*;

import com.arjuna.ArjunaLite.Common.LockStoreException;
import java.lang.InterruptedException;
import java.io.IOException;

public abstract class LockManager extends StateManager
{

    /*
     * Cleanup. Note we grab the semaphore before destroying the
     * the lock store to ensure the store is deleted cleanly.
     */

public void finalize () throws Throwable
    {
	boolean doSignal = false;
	
	cleanUp();

	if (mutex != null)
	{
	    if (mutex.lock() == Semaphore.SM_LOCKED)
		doSignal = true;
	}

	lmAttributes = null;
	locksHeld = null;
	lockStore = null;

	if (doSignal)  // mutex must be set
	    mutex.unlock();
	
	mutex = null;

	super.finalize();
    }
    
    /*
     * propagate: Change lock ownership as nested action commits. All
     * locks owned by the committing action have their owners changed to be
     * the parent of the committing action. BasicAction ensures this is only
     * called at nested commit. This function works by copying the old LockList
     * pointer and then creating a new held lock list. Locks are then moved 
     * from the old to the new, propagating en route.
     */

public final synchronized boolean propagate (Uid from, Uid to)
    {
	boolean result = false;
	int retryCount = 10;

	do
	{
	    if (loadState())
	    {
		LockList oldlist = locksHeld;
		Lock current = null;
		boolean found = false;
		
		locksHeld = new LockList();	/* create a new one */
	    
		if (locksHeld != null)
		{
		    /*
		     * scan through old list of held locks and propagate to
		     * parent.
		     */
		    
		    while ((current = oldlist.pop()) != null)
		    {
			if (current.getCurrentOwner().equals(from))
			{
			    found = true;
			    current.propagate();
			}
		    
			if (!locksHeld.insert(current))
			{
			    current = null;
			}
		    }

		    oldlist = null;		/* get rid of old lock list */
		    result = true;
		}
		else
		{
		    /* Cannot create new locklist - abort and try again */
		
		    freeState();

		    try
		    {
			Thread.sleep(LockManager.DOZE_TIME);
		    }
		    catch (InterruptedException e)
		    {
		    }
		}
	    }
	    else
	    {    
		freeState();

		try
		{
		    Thread.sleep(LockManager.DOZE_TIME);
		}
		catch (InterruptedException e)
		{
		}
	    }
	    
	} while ((!result) && (--retryCount > 0));

	if (result)
	{
	    result = unloadState();
	}
	else
	{
	    System.err.println("LockManager: lock propagation failed");
	    freeState();
	}

	return result;
    }

    /*
     * releaseAll: Clear out all locks for a given action. Should be
     * triggered automatically at top-level commit but is also user
     * callable so is potentially dangerous.
     */

public final synchronized boolean releaseAll (Uid actionUid)
    {
	return doRelease(actionUid, true);
    }

    /*
     * releaselock: Release a SINGLE LOCK lock that has the given uid. Breaks
     * two-phase locking rules so watch out!
     */

public final synchronized boolean releaselock (Uid lockUid)
    {
	return doRelease(lockUid, false);
    }

    /*
     * setlock: This is the main user visible operation. Attempts to set
     * the given lock on the current object. If lock cannot be set, then
     * the lock attempt is retried retry times before giving up and
     * returning an error. This gives a simple handle on deadlock.
     */

public final synchronized int setlock (Lock toSet)
    {
	return setlock(toSet, LockManager.defaultRetry, LockManager.defaultSleepTime);
    }

public final synchronized int setlock (Lock toSet, int retry)
    {
	return setlock(toSet, retry, LockManager.defaultSleepTime);
    }
    
public final synchronized int setlock (Lock toSet, int retry, int sleepTime)
    {
	int conflict = ConflictType.CONFLICT;
	int returnStatus = LockResult.REFUSED;
	LockRecord newLockR = null;
	boolean modifyRequired = false;
	BasicAction currAct = null;

	if (toSet == null)
	{
	    System.err.println("LockManager::setlock() no lock!");

	    return LockResult.REFUSED;
	}
    
	currAct = BasicAction.Current();

	if (currAct != null)
	{
	    ActionHierarchy ah = currAct.getHierarchy();

	    if (ah != null)
		toSet.changeHierarchy(ah);
	    else
	    {
		System.err.println("LockManager::setlock() cannot find action hierarchy");

		toSet = null;
		
		return LockResult.REFUSED;
	    }
	}
	
	if (super.loadObjectState())
	    super.setupStore();
	
	do
	{
	    conflict = ConflictType.CONFLICT;
	
	    if (loadState())
	    {
		conflict = lockConflict(toSet);
	    }
	    else
	    {
		System.err.println("LockManager::setlock() cannot load existing lock states");
	    }

	    if (conflict == ConflictType.CONFLICT)
	    {
		retry--;
		if (retry > 0)
		{
		    freeState();

		    try
		    {
			/* hope things happen in time */
			
			Thread.sleep(sleepTime);
		    }
		    catch (InterruptedException e)
		    {
		    }
		}
	    }
	    
	} while ((conflict == ConflictType.CONFLICT) && (retry > 0));

	/* When here the conflict was resolved or the retry limit expired */

	if (conflict != ConflictType.CONFLICT)
	{
	    /* no conflict so set lock */

	    modifyRequired = toSet.modifiesObject();

	    /* trigger object load from store */

	    if (super.activate())
	    {
		returnStatus = LockResult.GRANTED;

		if (conflict == ConflictType.COMPATIBLE)
		{
		    int lrStatus = AddOutcome.AR_ADDED;

		    if (currAct != null)
		    {
			/* add new lock record to action list */
		    
			newLockR = new LockRecord(this, (modifyRequired ? false : true));

			if ((lrStatus = currAct.add(newLockR)) != AddOutcome.AR_ADDED)
			{
			    newLockR = null;
			    if (lrStatus == AddOutcome.AR_REJECTED)
				returnStatus = LockResult.REFUSED;
			}
		    }
		    
		    if (returnStatus == LockResult.GRANTED)
		    {
			locksHeld.insert(toSet); /* add to local lock list */
		    }
		}
	    }
	    else
	    {
		/* activate failed - refuse request */

		System.err.println("LockManager::setlock() cannot activate object");
	    
		returnStatus = LockResult.REFUSED;
	    }
	}

	/* 
	 * Unload internal state into lock store only if lock list was modified
	 * if this fails claim the setlock failed.
	 * If we are using the lock daemon we can arbitrarily throw the lock
	 * away as the daemon has it.
	 */

	if ((returnStatus == LockResult.GRANTED) && (conflict == ConflictType.COMPATIBLE))
	{
	    if (!unloadState())
	    {
		System.err.println("LockManager::setlock() cannot save new lock states");
		returnStatus = LockResult.REFUSED;
	    }
	}
	else
	{
	    toSet = null;

	    freeState();
	}

	/*
	 * Postpone call on modified to here so that semaphore will have been
	 * released. This means when modified invokes save_state that routine
	 * may set another lock without blocking
	 */
    
	if (returnStatus == LockResult.GRANTED)
	{
	    if (modifyRequired)
		super.modified();
	
	    hasBeenLocked = true;
	}

	return returnStatus;
    }

public void print (PrintStream strm)
    {
	LockListIterator next = new LockListIterator(locksHeld);
	Lock current;

	strm.println("LocalLockManager for object "+get_uid());

	if (!stateLoaded)
	    strm.println("No loaded state");
	else
	    if (locksHeld != null)
	    {
		strm.println("\tCurrently holding : "+locksHeld.entryCount()+" locks");
		
		while ((current = next.iterate()) != null)
		    current.print(strm);
	    }
	    else
		strm.println("Currently holding : 0 locks");
    }

    /*
     * Load state into object prior to doing the printing.
     */

public synchronized void printState (PrintStream strm)
    {
	boolean iDeleteState = false;

	if (!stateLoaded)
	{
	    loadState();
	    iDeleteState = true;
	}

	print(strm);

	if (iDeleteState)
	    freeState();
    }

public String type ()
    {
	return "StateManager/LockManager";
    }

public Object attributes ()
    {
	return lmAttributes;
    }
    
    /*
     * Constructor: Pass on some args to StateManager and initialise
     * internal state.
     * The lock store and semaphore are set up lazily since they depend
     * upon the result of the type() operation which if run in the 
     * constructor always give the same answer!
     */

protected LockManager (Uid storeUid)
    {
	this(storeUid, ObjectType.ANDPERSISTENT, null);
    }

protected LockManager (Uid storeUid, ObjectName attr)
    {
	this(storeUid, ObjectType.ANDPERSISTENT, attr);
    }    
    
protected LockManager (Uid storeUid, int ot)
    {
	this(storeUid, ot, null);
    }

protected LockManager (Uid storeUid, int ot, ObjectName attr)
    {
	super(storeUid, ot, attr);

	parseObjectName();
	
	systemKey = null;
	locksHeld = new LockList();
	lockStore = null;
	mutex = null;
	stateLoaded = false;
	hasBeenLocked = false;
	objectLocked = false;
    }    

    /*
     * Constructor: Same restrictions apply as documented above.
     */

protected LockManager ()
    {
	this(ObjectType.RECOVERABLE, null);
    }

protected LockManager (int ot)
    {
	this(ot, null);
    }

protected LockManager (int ot, ObjectName attr)
    {
	super(ot, attr);

	parseObjectName();	
	
	systemKey = null;
	locksHeld = new LockList();
	lockStore = null;
	mutex = null;
	stateLoaded = false;
	hasBeenLocked = false;
	objectLocked = false;
    }
    
protected void terminate ()
    {
	cleanUp();
	super.terminate();
    }

private final synchronized void cleanUp ()
    {
	if (hasBeenLocked)
	{
	    if ((super.smAttributes.objectModel == ObjectModel.MULTIPLE) &&
		(systemKey == null))
	    {
		initialise();
	    }
	    
	    /*
	     * Unlike in the original version of Arjuna, we don't check
	     * to see if the invoking thread is within a transaction. We
	     * look at whether this object has been used within a transaction,
	     * and then act accordingly.
	     */

	    synchronized (super.usingActions)
		{
		    if (super.usingActions != null)
		    {
			Enumeration e = super.usingActions.keys();
		
			while (e.hasMoreElements())
			{
			    BasicAction action = (BasicAction) e.nextElement();
		
			    while (action != null)
			    {
				/*
				 * Pop actions off using list.
				 * 
				 * Don't check if action is running below so that cadavers can
				 * be created in commit protocol too.
				 */

				/*
				 * We need to create a cadaver lock record to maintain the
				 * locks because this object is being deleted.
				 */
		
				AbstractRecord A = new CadaverLockRecord(lockStore, this);
				
				if (action.add(A) != AddOutcome.AR_ADDED)
				    A = null;
			    }
			}
		    }
		}

	    hasBeenLocked = false;	    
	}
    }

    /*
     * doRelease: Does all the hard work of lock release. Either releases
     * all locks for a given uid, or simply one lock with a given uid as
     * appropriate.
     *
     * Does not require 'synchronized' as it can only be called from
     * other synchronized methods.
     */

private final boolean doRelease (Uid u, boolean all)
    {
	Lock previous = null;
	Lock current = null;
	boolean deleted = false;
	boolean result = false;
	int retryCount = 10;

	do
	{
	    if (loadState())
	    {
		/*
		 * Must declare iterator after loadstate or it sees an
		 * empty list!
		 */
	    
		LockListIterator next = new LockListIterator(locksHeld);

		/*
		 * Now scan through held lock list to find which locks to
		 * release u is either the unique id of the lock owner
		 * (oneOrAll = ALL_LOCKS) or the uid of the actual lock
		 * itself (oneOrAll = SINGLE_LOCK).
		 */
	    
		previous = null;
	    
		while ((current = next.iterate()) != null)
		{
		    Uid checkUid = null;

		    if (all)
			checkUid = current.getCurrentOwner();
		    else
			checkUid = current.get_uid();

		    if (u.equals(checkUid))
		    {
			locksHeld.forgetNext(previous);
			current = null;
			deleted = true;
			    
			if (!all)
			{
			    break;
			}
		    }
		    else
			previous = current;
		}
		
		result = true;
	    }
	    else
	    {
		freeState();

		try
		{
		    Thread.sleep(LockManager.DOZE_TIME);
		}
		catch (InterruptedException e)
		{
		}
	    }
	    
	} while ((!result) && (--retryCount > 0));

	if (!stateLoaded)
	{
	    System.err.println("Lockmanager::releaselock() could not load old lock states");
	
	    freeState();
	    return false;
	}

	retryCount = 10;

	do
	{
	    if (!unloadState())
	    {
		System.err.println("Lockmanager::releaselock() could not unload new lock states");
	    }
	    else
		return true;
	    
	} while (--retryCount > 0);
    
	return false;
    }

    /*
     * Simply free up the semaphore. We do this if we detect conflict.
     * Since the list has not been modified it can simply be discarded.
     * This will change with the addition of multi-threading (THANKFULLY)
     *
     * Does not need 'synchronized' as can only be called from synchronized
     * methods.
     */

private final void freeState ()
    {
	if (mutex != null)
	{
	    Lock current = null;

	    /* clear out the existing list */
	
	    while ((current = locksHeld.pop()) != null)
		current = null;
    
	    stateLoaded = false;

	    if (objectLocked)
	    {
		objectLocked = false;

		mutex.unlock();
	    }
	}
	else
	{
	    stateLoaded = false;
	    objectLocked = false;
	}
    }
    
    /*
     * Don't need to protect with a synchronization as this routine can only
     * be called from within other protected methods.
     *
     * Only called if multiple object model is used.
     */
    
private final boolean initialise ()
    {
	boolean result = false;

	if (systemKey == null)
	{
	    systemKey = type();

	    if (mutex == null)
	    {
		mutex = new Semaphore(systemKey);
	    }

	    if (mutex != null)
	    {
		if (mutex.lock() == Semaphore.SM_LOCKED)
		{	    
		    if (lockStore == null)
		    {
			Object[] param = new Object[3];
		
			param[0] = lmAttributes.lockStoreType;
			param[1] = new Integer(ObjectModel.MULTIPLE);
			param[2] = systemKey;
		    
			lockStore = new LockStore(param);

			param = null;
		    }
		}
		
		mutex.unlock();
	    }
	}
	
	result = (lockStore != null);
	
	return result;
    }

private final boolean isAncestorOf (Lock heldLock)
    {
	BasicAction action = BasicAction.Current();

	if (action == null)
	    return false;			/* no action no ancestry! */

	return action.isAncestor(heldLock.getCurrentOwner());
    }

    /*
     * Lock and load the concurrency control state. First we grab the
     * semaphore to ensure exclusive access and then we build the held
     * lock list by retreiving the locks from the lock repository.
     * If there is only one server we do not bother doing this since all
     * the locks can stay in the server's memory. This is yet another
     * consequence of not having multi-threaded servers.
     *
     * Does not require synchronized since it can only be called from
     * other synchronized methods.
     */

private final boolean loadState ()
    {
	/*
	 * First do licence checking.
	 */

	if (!LockManager.ticilDone)
	{
	    String li = PropertyManager.getProperty("JAVA_AIT_LICENCE");
	
	    LockManager.ticilDone = true;
	
	    if (li != null)
	    {
		boolean ok = true;
		int index1 = li.indexOf('-');
		int index2 = 0;
		String hString = null;
		String nmString = null;
		String dString = null;
		String cString = null;
		
		if (index1 != -1)
		    hString = li.substring(0, index1);
		else
		    ok = false;

		if (ok)
		{
		    index2 = li.indexOf('-', index1 +1);
		    
		    if (index2 != -1)
		    {
			nmString = li.substring(index1 +1, index2);
			index1 = index2;
		    }
		    else
			ok = false;
		}

		if (ok)
		{
		    index2 = li.indexOf('-', index1 +1);

		    if (index2 != -1)
		    {
			dString = li.substring(index1 +1, index2);
			index1 = index2;
		    }
		    else
			ok = false;
		}

		if (ok)
		{
		    cString = li.substring(index1 +1);
		}

		if (ok)
		{
		    int hostAddr = 0;
		    int initTime = (int) (System.currentTimeMillis()/1000);

		    try
		    {
			hostAddr = Utility.hostInetAddr();
		    }
		    catch (Exception e)
		    {
			e.printStackTrace();
			System.exit(0);
		    }

		    /*
		     * We have to use hexStringToLong rather than
		     * hexStringToInt because Java throws an exception for
		     * large (valid!) integer strings such as d0a15428.
		     */
		    
		    int suppliedHost = (int) Utility.hexStringToLong(hString);
		    int suppliedNetmask = (int) Utility.hexStringToLong(nmString);
		    int suppliedDate = (int) Utility.hexStringToLong(dString);
		    int suppliedCheck = (int) Utility.hexStringToLong(cString);

		    int originalNetmask = suppliedNetmask ^ 0xa4db2e5f;
		    int localHost = hostAddr ^ 0xbd427ca2;
		    int network = localHost & originalNetmask;
		    int suppliedNetwork = suppliedHost & originalNetmask;

		    int calc = 0xa6a6a6a6;
		    int a = suppliedDate;
		    int b = network;
		    int c = suppliedNetmask;

		    for (int i = 0; i < 17; i++)
		    {
			a = (37 * (a ^ calc)) + 41;
			b = (27 * (b ^ calc)) + 31;
			c = (43 * (c ^ calc)) + 29;
			calc = a ^ b ^ c;
		    }

		    suppliedDate = suppliedDate ^ 0x8a021bd6;
		    
		    /*
		     * Check for 'infinite' timeout period.
		     */
		    
		    if ((calc != suppliedCheck) || (suppliedNetwork != network) || ((suppliedDate <= initTime) && (suppliedDate != 0xffffffff)))
		    {
			System.err.println("Java AIT licence information is invalid. Please apply for a new licence.");
			System.exit(1);
		    }
		}
		else
		{
		    System.err.println("Java AIT licence environment variable has invalid format.");
		    System.exit(1);
		}
	    }
	    else
	    {
		System.err.println("Java AIT licence environment variable JAVA_AIT_LICENCE not found.");
		System.exit(1);
	    }
	}

	if (super.smAttributes.objectModel == ObjectModel.SINGLE)
	{
	    stateLoaded = true;

	    return true;
	}
	else
	{
	    InputObjectState S = null;
	
	    if ((systemKey == null) && !initialise())
	    {
		return false;			/* init failed */
	    }

	    if ((mutex == null) || (mutex.lock() != Semaphore.SM_LOCKED))
	    {
		return false;
	    }

	    stateLoaded = false;
	    objectLocked = true;

	    /*
	     * An exception indicates some form of error and *NOT* that the
	     * state cannot be found, which is indicated by S being null.
	     */

	    try
	    {
		S = lockStore.read_state(get_uid(), type());

		/* Pick returned state apart again */

		if (S != null)
		{
		    Uid u = new Uid(Uid.nullUid());		/* avoid system calls in Uid creation */
		    Lock current = null;
		    int count = 0;

		    try
		    {
			count = S.unpackInt();
			
			boolean cleanLoad = true;

			/*
			 * Carefully rebuild the internal state - if we fail throw
			 * it away and return.
			 */
		
			for (int i = 0; (i < count) && cleanLoad; i++)
			{
			    try
			    {
				u.unpack(S);
				current = new Lock(u);

				if (current != null)
				{
				    if (current.restore_state(S, ObjectType.ANDPERSISTENT))
				    {
					locksHeld.push(current);
				    }
				    else
				    {
					current = null;
					cleanLoad = false;
				    }
				}
				else
				    cleanLoad = false;
			    }
			    catch (IOException e)
			    {
				cleanLoad = false;
			    }
			}

			if (cleanLoad)
			    stateLoaded = true;
			else
			{
			    while ((current = locksHeld.pop()) != null)
				current = null;
			}
		    }
		    catch (IOException e)
		    {
		    }
	    
		    S = null;
		}
		else
		    stateLoaded = true;
	    }
	    catch (LockStoreException e)
	    {
		System.err.println(e.getMessage());
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

private final int lockConflict (Lock otherLock)
    {
	boolean matching = false;
	Lock heldLock = null;
	LockListIterator next = new LockListIterator(locksHeld);

	while ((heldLock = next.iterate()) != null)
	{
	    if (heldLock.conflictsWith(otherLock))
	    {
		if (!isAncestorOf(heldLock)) /* not quite Moss's rules */
		{
		    return ConflictType.CONFLICT;
		}
	    }
	    else
	    {
		if (heldLock.equals(otherLock))
		    matching = true;
	    }
	}

	return (matching ? ConflictType.PRESENT : ConflictType.COMPATIBLE);
    }

    /*
     * Unload the state by writing all the locks to the repository
     * and then freeing the semaphore.
     */
    
private final boolean unloadState ()
    {
	/*
	 * Single object model means we don't need a lock
	 * store at all.
	 */
	
	if (super.smAttributes.objectModel == ObjectModel.SINGLE)
	{
	    stateLoaded = false;

	    return true;
	}
	else
	{
	    boolean unloadOk = false;
	    Lock current = null;
	    String otype = type();
	    Uid u = get_uid();
	    OutputObjectState S = new OutputObjectState(u, otype);
	    int lockCount = locksHeld.entryCount();

	    /* destroy old state from lock store */

	    if (lockCount == 0)
	    {
		if (lockStore.remove_state(u, otype))
		{
		    unloadOk = true;
		}
		else
		{
		    System.err.println("LockManager::unloadState() failed to remove empty lock state for object "+u+" of type "+otype);
		}
	    }
	    else
	    {
		try
		{
		    /* generate new state */
	    
		    S.packInt(lockCount);

		    while ((current = locksHeld.pop()) != null)
		    {
			current.get_uid().pack(S);
			current.save_state(S, ObjectType.ANDPERSISTENT);
			current = null;
		    }

		    /* load image into store */

		    if (S.valid() && lockStore.write_committed(u, otype, S))
		    {
			unloadOk = true;
		    }
		    else
		    {
			System.err.print("LockManager::unloadState() failed to write ");
			System.err.println("new state for object "+u+" of type "+otype);
		    }
		}
		catch (IOException e)
		{
		    unloadOk = false;
		
		    System.err.print("LockManager::unloadState() failed to pack up ");
		    System.err.println("new state for object "+u+" of type "+otype);
		}
	    }
    
	    stateLoaded = false;
	
	    if (objectLocked)
	    {
		objectLocked = false;
		
		if (mutex != null)   // means object model != SINGLE
		    mutex.unlock();  // and exit mutual exclusion
	    }

	    return unloadOk;
	}
    }

private void parseObjectName ()
    {
	lmAttributes = new LockManagerAttribute();
	
	if (super.objectName != null)
	{
	    try
	    {
		lmAttributes.remoteLockStore = ((super.objectName.getLongAttribute(ArjunaLiteNames.LockManager_remoteManager()) == 0) ? false : true);
	    }
	    catch (Exception e)
	    {
		// assume not present.
	    }

	    try
	    {
		/*
		 * Use same attribute name as environment.
		 */
		
		lmAttributes.lockStoreType = super.objectName.getClassNameAttribute(ArjunaLiteEnvironment.LOCKSTORE_TYPE);
	    }
	    catch (Exception e)
	    {
		// assume not present.
	    }

	    // if present should now look for locations as with StateManager
	}
    }

public static final int defaultRetry = 100;
public static final int defaultSleepTime = 250000;

protected LockManagerAttribute lmAttributes;

private String systemKey;	/* used in accessing system resources */
private LockList locksHeld;	/* the actual list of locks set */
private LockStore lockStore;	/* locks held in shared memory */
private boolean stateLoaded;
private boolean hasBeenLocked;	/* Locked at least once */
private boolean objectLocked;	/* Semaphore grabbed */
private Semaphore mutex;        /* Controls access to the lock store */

private static final int DOZE_TIME = 1000000;
private static boolean ticilDone = false;

};

