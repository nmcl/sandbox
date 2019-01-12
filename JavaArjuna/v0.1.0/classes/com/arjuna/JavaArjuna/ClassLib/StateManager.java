/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StateManager.java,v 1.14.2.1 1999/02/12 09:51:51 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ClassLib;

import com.arjuna.JavaArjuna.ObjectStore.ObjectStore;
import com.arjuna.JavaArjuna.ObjectStore.ObjectStoreType;
import com.arjuna.JavaArjuna.Common.*;
import java.io.PrintStream;

import com.arjuna.JavaArjuna.Common.ObjectStoreException;

/*
 * This operation activates an object. Activation of an object may
 * entail loading its passive state from the object store and unpacking
 * it into the memory resident form, or it may simply be a no-op.
 * Full activation is only necessary if the object is currently marked
 * as being PASSIVE (that is, the object was constructed as being of
 * type ANDPERSISTENT with an existing uid and has not already been
 * activated).
 * Objects that are not of type ANDPERSISTENT or are persistent but
 * have not yet been saved in an object store (so-called new persistent
 * objects) are unaffected by this function.
 * Returns false if PASSIVE object cannot be loaded from object store,
 * true otherwise.
 */

public abstract class StateManager
{

public abstract boolean restore_state (InputObjectState os, int ot);
public abstract boolean save_state (OutputObjectState os, int ot);
    
    /*
     * Destructor.
     */

    /*
     * This generates a deprecation warning these days but to remove it would
     * require too much re-architecture of the code and hence take it away from what
     * JavaArjuna looked like originally.
     */
    
public void finalize ()
    {
	BasicAction action = BasicAction.Current();
 
	if ((action != null) &&
	    (action.status() == ActionStatus.RUNNING) &&
	    (currentStatus == ObjectStatus.ACTIVE_NEW))
	{
	    System.err.println("StateManager::terminate() should be invoked in every destructor");
	    
	    cleanup(false);
	}

	smAttributes = null;
	
	objectStore = null;
	storeRoot = null;
    }

public synchronized boolean activate ()
    {
	return activate(null);
    }
    
public synchronized boolean activate (String rootName)
    {
	BasicAction action = null;
	int oldStatus = currentStatus;
	boolean result = true;			/* assume 'succeeds' */
	boolean forceAR = false;

	/*
	 * Originally we allowed an object to change whether or not it was
	 * replicated between top-level transactions. This was possible
	 * because the programmer could override replication methods and simply
	 * change the return value of hasRemoteState.
	 *
	 * if (currentRemoteState != hasRemoteState())
	 * {
	 * currentRemoteState = hasRemoteState();
	 *
	 * if (objectStore != null)
	 * objectStore = null;
	 * }
	 *
	 * With the move to attribute objects, rather than additional methods
	 * in the class signature, we don't allow that at present. (It also
	 * simplifies things a lot.) However, if we allow attributes to be
	 * provided to an object after construction, we could return to this
	 * model.
	 */
	
	/*
	 * Check if this action has logged its presence before. If not we force
	 * creation of an ActivationRecord so that each thread/action tree has
	 * an ActivationRecord in it. This allows us to passivate the object when
	 * the last thread has finished with it, i.e., when the last
	 * ActivationRecord is gone.
	 */

	action = BasicAction.Current();

	if ((action != null) && (action.status() == ActionStatus.RUNNING))
	{
	    /*
	     * Only check for top-level action. This is sufficient because
	     * activation records are propagated to the parent on nested
	     * transaction commit, and dropped when the (nested) action
	     * aborts. Thus, an object remains active as long as a single
	     * ActivationRecord is being used, and we don't need to create a
	     * new record for each transaction in the same hierarchy. Once
	     * activated, the object remains active until the action commits
	     * or aborts (at which time it may be passivated, and then
	     * reactivated later by the creation of a new ActivationRecord.)
	     */

	    if (usingActions.insert(action.topLevelAction()))
		forceAR = true;
	}

	if (forceAR || (currentStatus == ObjectStatus.PASSIVE) ||
	    (currentStatus == ObjectStatus.PASSIVE_NEW))
	{
	    /*
	     * If object is recoverable only, then no need to set up
	     * the object store.
	     */

	    if (loadObjectState())
	    {
		setupStore(rootName);
	    }

	    /* Only really activate if object is PASSIVE */
	    
	    if (currentStatus == ObjectStatus.PASSIVE)
	    {
		/*
		 * If the object is shared between different processes, then we
		 * must load the state each time a top-level action accesses it.
		 * Otherwise we can continue to use the last state as in Dharma/
		 * ArjunaII.
		 */

		if (loadObjectState())
		{
		    InputObjectState oldState = null;
		
		    try
		    {
			oldState = objectStore.read_committed(objectUid, type());
		    }
		    catch (ObjectStoreException e)
		    {
			oldState = null;
		    }

		    if (oldState != null)
		    {
			if ((result = restore_state(oldState, ObjectType.ANDPERSISTENT)))
			    currentStatus = ObjectStatus.ACTIVE;
			oldState = null;
		    }
		    else
		    {
			System.err.println("Activate of object with id = "+
					   objectUid+" and type '"+type()
					   +"' unexpectedly failed");
			result = false;
		    }
		}
		else
		{
		    currentStatus = ObjectStatus.ACTIVE;
		}
	    }

	    /*
	     * Create ActivationRecord if status changed Passive->Active or if
	     * object is a new persistent object.
	     */
	
	    if (forceAR ||
		((currentStatus == ObjectStatus.ACTIVE) || (currentStatus == ObjectStatus.PASSIVE_NEW)) &&
		(action != null))
	    {
		int arStatus = BasicAction.ADDED;
		ActivationRecord ar = new ActivationRecord(oldStatus, this, action.topLevelAction());

		if ((arStatus = action.add(ar)) != BasicAction.ADDED)
		{
		    ar = null;

		    if (forceAR)
			usingActions.remove(action.topLevelAction());
		    
		    if (arStatus == BasicAction.REJECTED)
			result = false;
		}
		else
		{
		    /*
		     * We never reset activated, so we can optimise state
		     * loading/unloading in the case of SINGLE object model
		     */
		    
		    currentlyActivated = activated = true;
		}
	    }
	}
	
	return result;
    }
    
    /*
     * This operation deactivates a persistent object.
     * It behaves in a similar manner to the activate operation, but has an
     * extra argument which defines whether the object's state should be
     * commited now or not.
     */

public synchronized boolean deactivate ()
    {
	return deactivate(null, true);
    }
    
public synchronized boolean deactivate (String rootName)
    {
	return deactivate(rootName, true);
    }

public synchronized boolean deactivate (String rootName, boolean commit)
    {
	boolean result = false;

	if (currentlyActivated || loadObjectState())
	{
	    setupStore(rootName);

	    if ((currentStatus == ObjectStatus.ACTIVE_NEW) || (currentStatus == ObjectStatus.ACTIVE))
	    {
		String tn = type();
		OutputObjectState newState = new OutputObjectState(objectUid, tn);

		if (save_state(newState, myType))
		{
		    try
		    {
			if (commit)
			    result = objectStore.write_committed(objectUid, tn, newState);
			else
			    result = objectStore.write_uncommitted(objectUid, tn, newState);
		    }
		    catch (ObjectStoreException e)
		    {
			System.err.println("StateManager::deactivate - object store error "+e);
			
			result = false;
		    }
		}
		else
		    System.err.println("StateManager::deactivate - save_state error");

		/*
		 * Not needed any more because activation record does this when
		 * all actions are forgotten.
		 *
		 *  if (result)
		 *      currentStatus = ObjectStatus.PASSIVE;
		 *
		 */
	    }

	    /*
	     * Not allowed anymore. See the comment in activate.
	     *
	     * if (currentRemoteState != hasRemoteState())
	     * {
	     * currentRemoteState = hasRemoteState();
	     * if (objectStore != null)
	     * objectStore = null;
	     * }
	     */
	}
	else
	{
	    System.err.println("StateManager::deactivate() called for non-persistent object - ignored");
	}
	
	return result;
    }

public synchronized int status ()
    {
	return currentStatus;
    }

public final Uid get_uid ()
    {
	return objectUid;
    }

public void destroy ()
    {
	BasicAction action = BasicAction.Current();

	if ((action != null) && (action.status() == ActionStatus.RUNNING))
	    System.err.println("Sorry destroy is not implemented at the moment\b");
	else
	{
	    if (objectStore != null)
	    {
		try
		{
		    objectStore.remove_committed(get_uid(), type());
		}
		catch (ObjectStoreException e)
		{
		    System.err.println(e);
		}
	    }
	}
    }

    /*
     * The following function disables recovery for an object by setting
     * the ObjectType to NEITHER (RECOVERABLE or ANDPERSISTENT).
     * The value of this variable is checked in the modified operation
     * so that no recovery information is created if myType is set to NEITHER.
     */

public void disable ()
    {
	myType = ObjectType.NEITHER;
    }

public void print (PrintStream strm)
    {
	strm.println("Uid: "+objectUid);
	strm.println("Type: "+type());
    }

public String type ()
    {
	return "/StateManager";
    }

public final String getStoreRoot ()
    {
	return storeRoot;
    }
    
    /*
     * The following function checks to see if the object is going out of
     * scope while an action is still running.
     */

protected void terminate ()
    {
	cleanup(true);
    }
    
    /*
     * Create object with specific uid. This constructor
     * is primarily used when recreating an existing object. The object type
     * is set to 'ANDPERSISTENT' this is equivalent to invoking
     * persist in the object constructor
     */

protected StateManager (Uid objUid)
    {
	this(objUid, ObjectType.ANDPERSISTENT, null);
    }

protected StateManager (Uid objUid, StateManagerAttribute attr)
    {
	this(objUid, ObjectType.ANDPERSISTENT, attr);
    }
    
protected StateManager (Uid objUid, int ot)
    {
	this(objUid, ot, new StateManagerAttribute());
    }

protected StateManager (Uid objUid, int ot, StateManagerAttribute sm)
    {
	smAttributes = sm;
	
	activated = false;
	currentlyActivated = false;
	currentStatus = ObjectStatus.PASSIVE;
	initialStatus = ObjectStatus.PASSIVE;
	myType = ot;
	objectStore = null;
	storeRoot = null;

	objectUid = new Uid(objUid);
	
	modifyingActions = new ActionList();
	usingActions = new ActionList();
    }

protected StateManager ()
    {
	this(ObjectType.RECOVERABLE, null);
    }
    
protected StateManager (int ot)
    {
	this(ot, new StateManagerAttribute());
    }

protected StateManager (int ot, StateManagerAttribute sm)
    {
	smAttributes = sm;
	
	activated = false;
	currentlyActivated = false;
	currentStatus = (((smAttributes.objectModel == ObjectModel.SINGLE) && (ot == ObjectType.RECOVERABLE)) ? ObjectStatus.ACTIVE : ObjectStatus.PASSIVE_NEW);
	initialStatus = currentStatus;
	myType = ot;
	objectStore = null;
	storeRoot = null;

	objectUid = new Uid();
	
	modifyingActions = new ActionList();
	usingActions = new ActionList();
    }
    
    /*
     * Protected non-virtual functions.
     */
    
protected synchronized final void modified ()
    {
	BasicAction action = BasicAction.Current();
	RecoveryRecord record = null;
	
	if (myType == ObjectType.NEITHER) /*  NEITHER => no recovery info */
	    return;
    
	if (currentStatus == ObjectStatus.PASSIVE)
	{
	    System.err.println("StateManager::modified() invocation on an object whose state has not been restored - activating object");
	    activate();
	}
	
	/*
	 * Need not have gone through active if new object.
	 */

	if (currentStatus == ObjectStatus.PASSIVE_NEW)
	    currentStatus = ObjectStatus.ACTIVE_NEW;
    
	if (action != null)
	{
	    /*
	     * Check if this is the first call to modified in this action.
	     * BasicList insert returns FALSE if the entry is already
	     * present.
	     */

	    if (!modifyingActions.insert(action))
		return;
	
	    /* If here then its a new action */
	
	    OutputObjectState state = new OutputObjectState(objectUid, type());
	    int rStatus = BasicAction.ADDED;
	
	    save_state(state, ObjectType.RECOVERABLE);

	    if ((myType == ObjectType.RECOVERABLE) && (smAttributes.objectModel == ObjectModel.SINGLE))
		record = new RecoveryRecord(state, this);
	    else
		record = new PersistenceRecord(state, objectStore, this);
	    
	    if ((rStatus = action.add(record)) != BasicAction.ADDED)
	    {
		modifyingActions.remove(action);  // remember to unregister with action
		record = null;
	    }
	}
    }

    /*
     * The persist function changes the type of the object from RECOVERABLE
     * to ANDPERSISTENT.
     * No changes are made unless the status of the object is ACTIVE, so it
     * is not possible to change the type of the object if it has been
     * modified.
     */

protected final synchronized void persist ()
    {
	if (currentStatus == ObjectStatus.ACTIVE)
	{
	    currentStatus = ObjectStatus.PASSIVE_NEW;
	    myType = ObjectType.ANDPERSISTENT;
	}
    }
    
    /*
     * Object cleanup.
     * Attempt sane cleanup when object is deleted. Handle perverse cases
     * where multiple actions are still active as object dies.
     */
    
protected final synchronized void cleanup (boolean fromTerminate)
    {
	if (myType == ObjectType.NEITHER)
	    return;

	BasicAction action = null;
	
	while ((action = usingActions.orderedPop()) != null)
	{
	    /*
	     * Pop actions off using list 
	     * 
	     * Don't check if action is running below so that cadavers can
	     * be created in commit protocol too
	     */

	    AbstractRecord record = null;
	    int rStatus = BasicAction.ADDED;

	    if (currentStatus == ObjectStatus.ACTIVE_NEW)
	    {
		OutputObjectState state = null;
	    
		/* If we get here via terminate its ok to do a save_state */
	    
		if (fromTerminate)
		{
		    state = new OutputObjectState(objectUid, type());
		
		    save_state(state, myType);
		}
		else
		{
		    /* otherwise force action abort */
		    
		    action.preventCommit();
		}
	    
		/* This should be unnecessary - but just in case */
	    
		setupStore(storeRoot);
	    
		record = new CadaverRecord(state, objectStore, this);

		if ((rStatus = action.add(record)) != BasicAction.ADDED)
		    record = null;
	    }

	    if (currentlyActivated)
	    {
		record = new CadaverActivationRecord(this);
		
		if ((rStatus = action.add(record)) == BasicAction.ADDED)
		{
		    currentStatus = ObjectStatus.PASSIVE;
		}
		else
		    record = null;
	    }
	}
	
	/*
	 * Here the object must be either RECOVERABLE or PERSISTENT.
	 * Whether or not an action exists we still need to reset the
	 * object status to avoid possible later confusion
	 * What it gets set to is not important really as long as it gets
	 * changed from ACTIVE_NEW which might cause any running action to
	 * abort.
	 */
    
	if (currentStatus == ObjectStatus.ACTIVE_NEW)
	{
	    if ((myType == ObjectType.RECOVERABLE) && (smAttributes.objectModel == ObjectModel.SINGLE))
		currentStatus = ObjectStatus.ACTIVE;
	    else
		currentStatus = ObjectStatus.PASSIVE;
	}

	currentlyActivated = false;
    }

protected final void setupStore ()
    {
	this.setupStore(null);
    }
    
protected synchronized void setupStore (String rootName)
    {
	Thread.dumpStack();
	
	/*
	 * Already setup?
	 * Assume type will not change once object is created.
	 */

	if (objectStore != null)
	    return;
	
	if (rootName == null)
	    rootName = "";

	/* Check if we have a store */

	if (storeRoot != null)
	{
	    /* Attempting to reuse it ? */

	    if ((rootName == null) || (rootName.compareTo("") == 0) ||
		(rootName.compareTo(storeRoot) == 0))
	    {
		return;
	    }

	    /* No - destroy old store and create new */

	    objectStore = null;
	}

	if (rootName == null)
	    rootName = "";
    
	/* Create store now */

	storeRoot = new String(rootName);

	if (smAttributes.remoteState)
	{
	    /*
	     *
	     * if (currentRemoteState == false)
	     * {
	     * if (objectStore != null)
	     * objectStore = null;
	     * 
	     * currentRemoteState = hasRemoteState();
	     * }
	     *
	     */

	    /*
	     * Use string as otherwise we setup dependency on replication
	     * library.
	     */

	    objectStore = new ObjectStore(ObjectStoreType.REPLICATED_STORE, storeRoot);  // https://github.com/nmcl/sandbox/issues/86
	    objectStore.storeLocation(smAttributes.storeLocations);
	    objectStore.useStoreLocation(smAttributes.useStoreLocation);

	    /* This is for replication abstract record debugging only. */

	    objectStore.setObjectData(get_uid(), type());
	}
	else
	{
	    if ((myType == ObjectType.ANDPERSISTENT) || (myType == ObjectType.NEITHER))
	    {
		/*
		 * Default object store type is obtained by the interface.
		 */
		
		objectStore = new ObjectStore(storeRoot);
	    }
	    else
	    {
		/*
		 * Currently we should never get here!
		 * However, since Arjuna supports a volatile (in memory)
		 * object store we will also eventually, probably through a set
		 * of native methods.
		 */

		System.err.println("Error - attempt to use volatile store.");
		System.exit(0);

		objectStore = new ObjectStore(ObjectStoreType.VOLATILE_STORE);
	    }
	}

	/*
	 * Do any work needed to initialise the object store.
	 * Really only makes sense for replicated object store where
	 * we attempt to do early binding to the replicas.
	 */

	objectStore.initialise(get_uid(), type());
    }
    
    /*
     * Private non-virtual functions
     */

    /* do we need to load the object's state? */
    
protected final boolean loadObjectState ()
    {
	boolean load = (smAttributes.objectModel != ObjectModel.SINGLE);

	/*
	 * MULTIPLE object model requires loading of state every
	 * time, even if we are RECOVERABLE - we use the volatile
	 * store.
	 */
	    
	if (!load)
	{
	    /*
	     * Must be SINGLE object model. So, is this the first
	     * time? If so, load state.
	     */
		
	    if ((myType != ObjectType.RECOVERABLE) && (!activated))
		load = true;
	}

	return load;
    }
    
    /*
     * Called ONLY by ActivationRecords!
     */

    /*
     * Remove action from list of using actions. If the action list empties
     * reset state to PASSIVE.
     * The second param tells why the action should be forgotten.
     * This aids in resetting the state correctly
     */
      
protected final synchronized boolean forgetAction (BasicAction action, boolean committed,
						   int recordType)
    {
	boolean result = false;

	if (recordType == RecordType.RECOVERY)
	{
	    modifyingActions.remove(action);
	}
	else
	{
	    usingActions.remove(action);
	
	    if (usingActions.size() == 0)
	    {
		if (committed)
		{
		    if ((myType == ObjectType.RECOVERABLE) && (smAttributes.objectModel == ObjectModel.SINGLE))
			initialStatus = currentStatus = ObjectStatus.ACTIVE;
		    else
			initialStatus = currentStatus = ObjectStatus.PASSIVE;
		}
		else
		    currentStatus = initialStatus;
	    }
	}

	result = true;

        return result;
    }
      
protected final synchronized boolean rememberAction (BasicAction action, int recordType)
    {
	boolean result = false;

	if (recordType == RecordType.RECOVERY)
	{
	}
	else
	{
	    if ((action != null) && (action.status() == ActionStatus.RUNNING))
		usingActions.insert(action);
	}

	result = true;

	return result;
    }

protected StateManagerAttribute smAttributes;

private boolean activated;
private boolean currentlyActivated;
private int currentStatus;
private int initialStatus;    
private int myType;
private ObjectStore objectStore;
private String storeRoot;
private Uid objectUid;
private ActionList modifyingActions;
private ActionList usingActions;
    
}
