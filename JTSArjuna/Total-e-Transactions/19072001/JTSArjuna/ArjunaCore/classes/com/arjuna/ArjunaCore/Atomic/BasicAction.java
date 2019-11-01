/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: BasicAction.java,v 1.1.4.2.2.5.2.4.2.5.4.9.2.1.4.2.4.1.14.1.2.2 2001/07/23 11:13:06 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCore.Common.*;
import com.arjuna.ArjunaCore.Thread.*;
import com.arjuna.ArjunaCore.Interface.RecoveryAbstractRecord;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Template.*;
import java.util.*;

import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import java.lang.NumberFormatException;
import java.lang.InterruptedException;
import java.lang.IndexOutOfBoundsException;
import java.io.IOException;

/**
 * BasicAction does most of the work of an atomic action,
 * but does not manage thread scoping. This is the responsibility
 * of any derived classes.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: BasicAction.java,v 1.1.4.2.2.5.2.4.2.5.4.9.2.1.4.2.4.1.14.1.2.2 2001/07/23 11:13:06 nmcl Exp $
 * @since JTS 1.0.
 */

public class BasicAction extends StateManager
{
    
public BasicAction ()
    {
	super(ObjectType.NEITHER);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::BasicAction()");
	}

	pendingList = null;
	preparedList = null;
	readonlyList = null;
	failedList = null;
	heuristicList = null;
	
	currentHierarchy = null;
	currentStore = null;
	savedIntentionList = false;
	
	actionStatus = ActionStatus.CREATED;
	actionType = ActionType.NESTED;
	
	parentAction = null;
	recordBeingHandled = null;

	heuristicDecision = PrepareOutcome.PREP_OK;
	_checkedAction = new CheckedAction();
	
	_childThreads = null;
	_childActions = null;
    }

    /**
     * BasicAction constructor with a Uid.
     * This constructor is for recreating an BasicAction, typically during
     * crash recovery.
     */

public BasicAction (Uid objUid)
    {
	super(objUid, ObjectType.NEITHER);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::BasicAction("+objUid+")");
	}

	pendingList = null;
	preparedList = null;
	readonlyList = null;
	failedList = null;
	heuristicList = null;

	currentHierarchy = null;
	currentStore = null;
	savedIntentionList = false;

	actionStatus = ActionStatus.CREATED;
	actionType = ActionType.NESTED;

	parentAction = null;
	recordBeingHandled = null;

	heuristicDecision = PrepareOutcome.PREP_OK;	
	_checkedAction = new CheckedAction();
	
	_childThreads = null;
	_childActions = null;
    }

    /**
     * BasicAction destructor. Under normal circumstances we do very little.
     * However there exists the possibility that this action is being deleted
     * while still running (user forgot to commit/abort) - in which case we
     * do an abort for him and mark all our parents as unable to commit.
     * Additionally due to scoping we may not be the current action - but
     * in that case the current action must be one of our nested actions
     * so by applying abort to it we should end up at ourselves!
     */

public void finalize ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::finalize()");
	}

	if ((actionStatus == ActionStatus.RUNNING) || (actionStatus == ActionStatus.ABORT_ONLY))
	{
	    /* If current action is one of my children there's an error */

	    BasicAction currentAct = BasicAction.Current();
	
	    if ((currentAct != null) && (currentAct != this))
	    {
		/*
		 * Is the current action a child of this action? If so,
		 * abort until we get to the current action. This works
		 * even in a multi-threaded environment where each thread
		 * may have a different notion of current, since Current
		 * returns the thread specific current.
		 */
	    
		if (currentAct.isAncestor(get_uid()))
		{
		    ErrorStream.warning().println("Action nesting error - deletion of action id "+get_uid()+" invoked while child actions active");
	
		    while ((currentAct != this) && (currentAct != null))
		    {
			ErrorStream.warning().println("Aborting child "+currentAct.get_uid());

			currentAct.Abort();

			currentAct = BasicAction.Current();
		    }
		}
	    }
    
	    BasicAction parentAct = parent();

	    /* prevent commit of parents (safety) */

	    while (parentAct != null)
	    {
		parentAct.preventCommit();
		parentAct = parentAct.parent();
	    }
	
	    ErrorStream.warning().println("Destructor of still running action id "+get_uid()+" invoked - Aborting");

	    /* This will also kill any children */
	
	    Abort();
	}
	else
	{
	    if (actionStatus == ActionStatus.PREPARED)
		Thread.yield();
	}

	pendingList = null;
	preparedList = null;
	readonlyList = null;
	failedList = null;
	heuristicList = null;
	
	currentStore = null;
	currentHierarchy = null;

	_checkedAction = null;
	
	if (_childThreads != null)
	{
	    _childThreads.clear();
	    _childThreads = null;
	}
	
	if (_childActions != null)
	{
	    _childActions.clear();
	    _childActions = null;
	}
    }

    /**
     * Return the action hierarchy for this transaction.
     */

public final synchronized ActionHierarchy getHierarchy ()
    {
	return currentHierarchy;
    }

    /**
     * Force the only outcome for the transaction to be to
     * rollback. Only possible if this transaction has not
     * (or is not) terminated.
     *
     * @return <code>true</code> if successful, <code>false</code> otherwise.
     */

public final boolean preventCommit ()
    {
	boolean res = false;

	if (lockMutex())
	{
	    if (actionStatus == ActionStatus.RUNNING)
		actionStatus = ActionStatus.ABORT_ONLY;

	    res = (actionStatus == ActionStatus.ABORT_ONLY);

	    unlockMutex();
	}

	return res;
    }

    /**
     * @return the number of threads associated with this transaction.
     */

public final int activeThreads ()
    {
	if (_childThreads != null)
	    return _childThreads.size();
	else
	    return 0;
    }

    /**
     * Add a record to the atomic action.
     * This function returns AR_ADDED if the record is added. AR_REJECTED
     * if the record cannot be added because the action is past the
     * prepare phase, and IGNORED otherwise.
     *
     * @return <code>AddOutcome</code> indicating outcome.
     */

public final synchronized int add (AbstractRecord A)
    {
	int result = AddOutcome.AR_REJECTED;

	criticalStart();
	
	if ((actionStatus <= ActionStatus.ABORTING) &&
	    ((recordBeingHandled == null) ||
	     !(recordBeingHandled.equals(A))))
	{
	    if (pendingList == null)
		pendingList = new RecordList();
	    else
	    {
		/*
		 * Check one-phase commit restriction. If we already have
		 * a record list then we should have a single resource
		 * registered by now!
		 */

		LicenceError le = null;
		
		if (!BasicAction._ticilDone)
		{
		    try
		    {
			InitAction.onePhaseRestriction = !com.arjuna.ArjunaCommon.Core.Lector.confirm(com.arjuna.ArjunaCommon.Core.LicenceType.COMMIT_TWO_PHASE);

			/*
			 * This restriction comes into play as soon as you
			 * register a single resource, i.e., once you really
			 * begin to use transactions.
			 */

			InitAction.restrictedNumberOfTwoPhaseTransactions = !com.arjuna.ArjunaCommon.Core.Lector.confirm(com.arjuna.ArjunaCommon.Core.LicenceType.UNLIMITED_TWO_PHASE_TRANSACTIONS);

			InitAction.restrictedNumberOfOnePhaseTransactions = !com.arjuna.ArjunaCommon.Core.Lector.confirm(com.arjuna.ArjunaCommon.Core.LicenceType.UNLIMITED_ONE_PHASE_TRANSACTIONS);
			
			BasicAction._ticilDone = true;
		    }
		    catch (LicenceError e)
		    {
			le = new LicenceError(e.toString());
		    }
		}

		if (le != null)
		    throw le;
		
		if (InitAction.onePhaseRestriction && (pendingList.size() == 1))
		{
		    ErrorStream.fatal().println("Arjuna licence only allows a single resource to be registered. Please apply for a new licence.");

		    if (!preventCommit())  // just in case!
			throw new com.arjuna.ArjunaCommon.Common.LicenceError("Arjuna licence only allows a single resource to be registered. Please apply for a new licence.");
		}
	    }
	    
	    result = (pendingList.insert(A) ? AddOutcome.AR_ADDED : AddOutcome.AR_DUPLICATE);
	}

	criticalEnd();
	
	return result;
    }

    /**
     * @return the depth of the current transaction hierarchy.
     */

public final synchronized int hierarchyDepth ()
    {
	if (currentHierarchy != null)
	    return currentHierarchy.depth();
	else
	    return 0;			/* should never happen */
    }

    /**
     * boolean function that checks whether the Uid passed as an
     * argument is the Uid for an ancestor of the current
     * atomic action.
     *
     * @return <code>true</code> if the parameter represents an ancestor,
     * <code>false</code> otherwise.
     */

public final boolean isAncestor (Uid ancestor)
    {
	boolean res = false;

	if (get_uid().equals(ancestor))	/* actions are their own ancestors */
	    res = true;
	else
	{
	    if ((parentAction != null) && (actionType != ActionType.TOP_LEVEL))
		res = parentAction.isAncestor(ancestor);
	}
	
	return res;
    }

    /**
     * @return a reference to the parent BasicAction
     */

public final BasicAction parent ()
    {
	if (actionType == ActionType.NESTED)
	    return parentAction;
	else
	    return null;
    }

    /**
     * @return the status of the BasicAction
     */

public final int status ()
    {
	int s = ActionStatus.INVALID;
	
	if (tryLockMutex())
	{
	    s = actionStatus;
	    
	    unlockMutex();
	}
	
	return s;
    }

    /**
     * Set up an object store and assign it to the currentStore variable.
     *
     * @return the object store implementation to use.
     * @see com.arjuna.ArjunaCore.Interface.ObjectStore
     */

public final synchronized ObjectStore store ()
    {
	if (currentStore == null)
	    currentStore = InitAction.getStore();

	return currentStore;
    }

    /**
     * The following function returns the Uid of the top-level atomic
     * action. If this is the top-level transaction then it is
     * equivalent to calling get_uid().
     *
     * @return the top-level transaction's <code>Uid</code>.
     */

public final Uid topLevelActionUid ()
    {
	BasicAction root = this;

	while (root.parent() != null)
	    root = root.parent();

	return root.get_uid();
    }

    /**
     * @return a reference to the top-level transaction. If this is
     * the top-level transaction then a reference to itself will
     * be returned.
     */

public final BasicAction topLevelAction ()
    {
	BasicAction root = this;

	while (root.parent() != null)
	    root = root.parent();

	return root;
    }

    /**
     * Overloaded version of activate -- sets up the store, performs read_state
     * followed by restore_state. The store root is <code>null</code>.
     *
     * @return <code>true</code> if successful, <code>false</code> otherwise.
     */

public synchronized boolean activate ()
    {
	return activate(null);
    }

    /**
     * Overloaded version of activate -- sets up the store, performs read_state
     * followed by restore_state. The root of the object store to use is
     * specified in the <code>root</code> parameter.
     *
     * @return <code>true</code> if successful, <code>false</code> otherwise.
     */
    
public synchronized boolean activate (String root)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::activate() for action-id "+get_uid());
	}
	
	boolean restored = false;
    
	// Set up store
	ObjectStore aaStore = store();

	if (aaStore == null)
	    return false;
	
	try
	{
	    // Read object state
	    InputObjectState oState = aaStore.read_committed(get_uid(), type());

	    if (oState != null)
	    {
		restored = restore_state(oState, ObjectType.ANDPERSISTENT);
		oState = null;
	    }
	    else
	    {
		ErrorStream.warning().println("Activate of atomic action with id = "+get_uid()+" and type '"+type()+"' unexpectedly failed");

		restored = false;
	    }
	
	    return restored;
	}
	catch (ObjectStoreException e)
	{
	    ErrorStream.warning().println(e.getMessage());

	    return false;
	}
    }

    /**
     * Add the current thread to the list of threads associated
     * with this transaction.
     *
     * @return <code>true</code> if successful, <code>false</code> otherwise.
     */

public final synchronized boolean addChildThread () // current thread
    {
	return addChildThread(Thread.currentThread());
    }

    /**
     * Add the specified thread to the list of threads associated
     * with this transaction.
     *
     * @return <code>true</code> if successful, <code>false</code> otherwise.
     */

public final synchronized boolean addChildThread (Thread t)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::addChildThread () action "+get_uid()+" adding "+t);
	}

	if (t == null)
	    return false;
	
	boolean result = false;

	criticalStart();
	
	if (actionStatus <= ActionStatus.ABORTING)
	{
	    if (_childThreads == null)
		_childThreads = new Hashtable();

	    _childThreads.put(t.getName(), t); // makes sure so we don't get duplicates
	    
	    result = true;
	}

	criticalEnd();

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::addChildThread () action "+get_uid()+" adding "+t+" result = "+result);
	}

	return result;
    }

    /*
     * Can be done at any time (Is this correct?)
     */

    /**
     * Remove a child thread. The current thread is removed.
     *
     * @return <code>true</code> if successful, <code>false</code> otherwise.
     */

public final synchronized boolean removeChildThread () // current thread
    {
	return removeChildThread(Thread.currentThread().getName());
    }

    /**
     * Remove the specified thread from the transaction.
     *
     * @return <code>true</code> if successful, <code>false</code> otherwise.
     */

public final synchronized boolean removeChildThread (String threadId)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::removeChildThread () action "+get_uid()+" removing "+threadId);
	}

	if (threadId == null)
	    return false;
	
	boolean result = false;

	criticalStart();

	if (_childThreads != null)
	{
	    _childThreads.remove(threadId);
	    result = true;
	}

	criticalEnd();

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::removeChildThread () action "+get_uid()+" removing "+threadId+" result = "+result);
	}

	return result;
    }
    
    /**
     * Add a new child action to the atomic action.
     *
     * @return <code>true</code> if successful, <code>false</code> otherwise.
     */

public final synchronized boolean addChildAction (BasicAction act)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::addChildAction () action "+get_uid()+" adding "+((act != null) ? act.get_uid() : Uid.nullUid()));
	}

	if (act == null)
	    return false;
	
	boolean result = false;

	criticalStart();

	/*
	 * Must be <= as we sometimes need to do processing
	 * during commit phase.
	 */
	
	if (actionStatus <= ActionStatus.ABORTING)
	{
	    if (_childActions == null)
		_childActions = new Hashtable();

	    _childActions.put(act, act);
	    result = true;
	}

	criticalEnd();

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::addChildAction () action "+get_uid()+" adding "+act.get_uid()+" result = "+result);
	}

	return result;
    }

    /*
     * Can be done at any time (Is this correct?)
     */

    /**
     * Remove a child action.
     *
     * @return <code>true</code> if successful, <code>false</code> otherwise.
     */

public final synchronized boolean removeChildAction (BasicAction act)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::removeChildAction () action "+get_uid()+" removing "+((act != null) ? act.get_uid() : Uid.nullUid()));
	}

	if (act == null)
	    return false;
	
	boolean result = false;

	criticalStart();
	
	if (_childActions != null)
	{
	    _childActions.remove(act);
	    result = true;
	}

	criticalEnd();

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::removeChildAction () action "+get_uid()+" removing "+act.get_uid()+" result = "+result);
	}

	return result;
    }

    /**
     * Add the specified CheckedAction object to this transaction.
     *
     * @return the previous <code>CheckedAction</code>.
     * @see com.arjuna.ArjunaCore.Atomic.CheckedAction
     */
    
public final synchronized CheckedAction setCheckedAction (CheckedAction c)
    {
	criticalStart();
	
	CheckedAction toReturn = _checkedAction;

	_checkedAction = c;

	criticalEnd();
	
	return toReturn;
    }

    /**
     * @return the Uid that the transaction's intentions list will be
     * saved under.
     */

public Uid getSavingUid ()
    {
	return get_uid();
    }

    /**
     * Overloads Object.toString()
     */

public String toString ()
    {
	return new String("BasicAction: "+get_uid()+" status: "+actionStatus);
    }
    
    /**
     * Redefined version of save_state and restore_state from StateManager.
     *
     * Normal operation (no crashes):
     *
     * BasicAction.save_state is called after a successful prepare. This
     * causes and BasicAction object to be saved in the object store. This
     * object contains primarily the "intentions list" of the BasicAction.
     * After successfully completing phase 2 of the commit protocol, the
     * BasicAction object is deleted from the store.
     *
     * Failure cases:
     *
     * If a server crashes after successfully preparing, then upon recovery the
     * action must be resolved (either committed or aborted) depending upon 
     * whether the co-ordinating atomic action committed or aborted.
     * Upon server recovery, the crash recovery mechanism detects
     * ServerBasicAction objects in the object store and attempts to activate
     * the BasicAction object of the co-ordinating action. If this is
     * successful then the SAA is committed else aborted.
     *
     * If, when processing phase 2 of the commit protocol, the co-ordinator
     * experiences a failure to commit from one of the records then the
     * BasicAction object is NOT deleted. It is rewritten when a new state
     * which contains a list of the records that failed during phase 2 commit.
     * This list is called the "failedList".
     *
     * The crash recovery manager will detect local BasicAction objects in
     * addition to SAA objects in the objectstore. An attempt will be made
     * to commit these actions. If the action contained a call to a now dead
     * server, this action can never be resolved and the AA object can never
     * be removed. However, if the action is purely local then after the 
     * processing is complete the removed by crash recovery.
     *
     * @return <code>true</code> if successful, <code>false</code> otherwise.
     */

public boolean save_state (OutputObjectState os, int ot)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::save_state ()");
	}

	try
	{
	    packHeader(os, getSavingUid(), Utility.getProcessUid());
	}
	catch (IOException e)
	{
	    return false;
	}

	/*
	 * This routine is called:
	 * a) After a successful prepare - to save the intentions list
	 * b) After a failure during phase 2 of commit - to overwrite the
	 *    intentions list by the failedList
	 */

	RecordList listToSave = null;
	boolean res = true;

	/*
	 * If we have a failedList then we are re-writing a BasicAction
	 * object after a failure during phase 2 commit
	 */

	if ((failedList != null) && (failedList.size() > 0))
	{
	    listToSave = failedList;
	}
	else
	{
	    listToSave = preparedList;
	}

	AbstractRecord first = ((listToSave != null) ? listToSave.getFront() : null);
	AbstractRecord temp = first;
	boolean havePacked = ((listToSave == null) ? false : true);

	while ((res) && (temp != null))
	{
	    listToSave.putRear(temp);

	    /*
	     * First check to see if we need to call save_state. If we do then
	     * we must first save the record type (and enum) and then save the
	     * unique identity of the record (a string). The former is needed
	     * to determine what type of record we are restoring, while the
	     * latter is required to re-create the actual record.
	     */

	    /*
	     * First check to see if we need to call save_state. If we
	     * do then we must first save the record type. This is
	     * used to determine which type of record to create when
	     * restoring.
	     */

	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_ATOMIC_ACTION, 
						     "BasicAction::save_state - next record to pack is a\n"+
						     temp.typeIs()+" record ("+temp.type()+") should save it? = "
						     +temp.doSave());
	    }

	    if (temp.doSave())
	    {
		res = true;

		try
		{
		    if (DebugController.enabled())
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							     FacilityCode.FAC_ATOMIC_ACTION, 
							     "Packing a "+temp.typeIs()+" record");

		    os.packInt(temp.typeIs());
		    res = temp.save_state(os, ot);
		}
		catch (IOException e)
		{
		    res = false;
		}
	    }

	    temp = listToSave.getFront();

	    if (temp == first)
	    {
		listToSave.putFront(temp);
		temp = null;
	    }
	}

	/*
	 * If we only ever had a heuristic list (e.g., one-phase commit)
	 * then pack a record delimiter.
	 */

	if (res && (os.notempty() || !havePacked))
	{
	    try
	    {
		if (DebugController.enabled())
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 FacilityCode.FAC_ATOMIC_ACTION,
							 "Packing a NONE_RECORD");

		os.packInt(RecordType.NONE_RECORD);
	    }
	    catch (IOException e)
	    {
		res = false;
	    }
	}

	if (res)
	{
	    //Now deal with anything on the heuristic list!
	    
	    int hSize = ((heuristicList == null) ? 0 : heuristicList.size());

	    try
	    {
		os.packInt(hSize);
	    }
	    catch (IOException e)
	    {
		res = false;
	    }

	    if (res && (hSize > 0))
	    {
		first = heuristicList.getFront();
		temp = first;

		while (res && (temp != null))
		{
		    heuristicList.putRear(temp);

		    if (temp.doSave())
		    {
			res = true;
		    
			try
			{
			    if (DebugController.enabled())
				DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
								     FacilityCode.FAC_ATOMIC_ACTION, 
								     "HeuristicList - packing a "+temp.typeIs()+" record");

			    os.packInt(temp.typeIs());
			    res = temp.save_state(os, ot);
			}
			catch (IOException e)
			{
			    res = false;
			}
		    }

		    temp = heuristicList.getFront();

		    if (temp == first)
		    {
			heuristicList.putFront(temp);
			temp = null;
		    }
		}

		if (res && os.notempty())
		{
		    try
		    {
			if (DebugController.enabled())
			    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
								 FacilityCode.FAC_ATOMIC_ACTION, 
								 "HeuristicList - packing a NONE_RECORD");

			os.packInt(RecordType.NONE_RECORD);
		    }
		    catch (IOException e)
		    {
			res = false;
		    }
		}
	    }
	}

	if (res && os.notempty())
	{
	    try
	    {
		if (DebugController.enabled())
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 FacilityCode.FAC_ATOMIC_ACTION,
							 "Packing action status of "+ActionStatus.printString(actionStatus));

		os.packInt(actionStatus);
		os.packInt(actionType);
		os.packInt(heuristicDecision);
	    }
	    catch (IOException e)
	    {
		res = false;
	    }
	}

	return res;
    }

    /**
     * This assumes the various lists are zero length when it is called.
     *
     * @return <code>true</code> if successful, <code>false</code> otherwise.
     */

public boolean restore_state (InputObjectState os, int ot)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::restore_state ()");
	}

	createPreparedLists();
	
	boolean res = true;
	int record_type = RecordType.NONE_RECORD;
	int tempActionStatus = ActionStatus.INVALID;
	int tempActionType = ActionType.TOP_LEVEL;
	int tempHeuristicDecision = PrepareOutcome.PREP_OK;

	/*
	 * Unpack the prepared list.
	 * Note: This may either be a full intentions list or just the
	 * failedList, either way, restore it as the prepared list.
	 */

	try
	{
	    Uid txId = new Uid(Uid.nullUid());
	    Uid processUid = new Uid(Uid.nullUid());
		
	    unpackHeader(os, txId, processUid);
	}
	catch (IOException e)
	{
	    return false;
	}
	
	try
	{
	    record_type = os.unpackInt();

	    if (DebugController.enabled())
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_ATOMIC_ACTION,
						     "Unpacked a "+record_type+" record");
	}
	catch (IOException e)
	{
	    res = false;
	}
    
	while ((res) && (record_type != RecordType.NONE_RECORD))
	{
	    RecoveryAbstractRecord recRecord = new RecoveryAbstractRecord(RecordType.typeToClassName(record_type));
	    AbstractRecord record = recRecord.record();

	    if (record == null)
	    {
		ErrorStream.warning().println("BasicAction.restore_state - could not recover "+record_type);
		
		res = false;
	    }
	    else
		res = (record.restore_state(os, ot) && preparedList.insert(record));

	    if (res)
	    {
		try
		{
		    record_type = os.unpackInt();

		    if (DebugController.enabled())
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							     FacilityCode.FAC_ATOMIC_ACTION, 
							     "Unpacked a "+record_type+" record");
		}
		catch (IOException e)
		{
		    res = false;
		}
	    }
	}

	 // Now deal with the heuristic list!

	int hSize = 0;

	if (res)
	{
	    try
	    {
		hSize = os.unpackInt();

		if (DebugController.enabled())
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 FacilityCode.FAC_ATOMIC_ACTION,
							 "HeuristicList - Unpacked heuristic list size of "+hSize);

	    }
	    catch (IOException e)
	    {
		res = false;
	    }
	}

	if (hSize > 0)
	{
	    try
	    {
		record_type = os.unpackInt();

		if (DebugController.enabled())
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							 FacilityCode.FAC_ATOMIC_ACTION,
							 "HeuristicList - Unpacked a "+record_type+" record");
	    }
	    catch (IOException e)
	    {
		res = false;
	    }

	    while ((res) && (record_type != RecordType.NONE_RECORD))
	    {
		RecoveryAbstractRecord record = new RecoveryAbstractRecord(RecordType.typeToClassName(record_type));

		res = (record.restore_state(os, ot) && heuristicList.insert(record));

		try
		{
		    record_type = os.unpackInt();

		    if (DebugController.enabled())
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							     FacilityCode.FAC_ATOMIC_ACTION,
							     "HeuristicList - Unpacked a "+record_type+" record");
		}
		catch (IOException e)
		{
		    res = false;
		}
	    }
	}

	if (res)
	{
	    try
	    {
		tempActionStatus = os.unpackInt();
		tempActionType = os.unpackInt();
		tempHeuristicDecision = os.unpackInt();
	    }
	    catch (IOException e)
	    {
		ErrorStream.warning().println("BasicAction.restore_state - error unpacking action status.");

		res = false;
	    }
	}

	if (res)
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_ATOMIC_ACTION, 
						     "Restored action status of "
						     +ActionStatus.printString(tempActionStatus)
						     +" "+tempActionStatus);

		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_ATOMIC_ACTION, "Restored action type "+
						     ((tempActionType == ActionType.NESTED)?"Nested":"Top-level")
						     +" "+tempActionType);

		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						     FacilityCode.FAC_ATOMIC_ACTION, 
						     "Restored heuristic decision of "
						     +PrepareOutcome.printString(tempHeuristicDecision)
						     +" "+tempHeuristicDecision);
	    }

	    actionStatus = tempActionStatus;
	    actionType = tempActionType;
	    heuristicDecision = tempHeuristicDecision;
	    savedIntentionList = true;
	}

	return res;
    }

    /**
     * Overloads StateManager.type()
     */

public String type ()
    {
	return "/StateManager/BasicAction";
    }

    /**
     * @return the thread's notion of the current transaction.
     */

public static BasicAction Current ()
    {
	return ThreadActionData.currentAction();
    }

    /**
     * If heuristic outcomes are returned, by default we will not save
     * the state once the forget method has been called on them (which
     * happens as soon as we have received all outcomes from registered
     * resources). By specifying otherwise, we will always maintain the
     * heuristic information, which may prove useful for logging and
     * off-line resolution.
     *
     * @return <code>true</code> if the transaction should save its
     * heuristic information, <code>false</code> otherwise.
     */

public static boolean maintainHeuristics ()
    {
	return InitAction.maintainHeuristics;
    }

    /**
     * Overloads <code>StateManager.destroy</code> to prevent destroy
     * being called on a BasicAction. Could be a *very* bad idea!!
     *
     * @return <code>false</code>.
     * @see com.arjuna.ArjunaCore.Atomic.StateManager
     */

public synchronized boolean destroy ()
    {
	//	ErrorStream.warning().println("BasicAction.destroy called on "+get_uid());
	
	return true;
    }

    /**
     * @return the list of child transactions. Currently only their ids are
     * given.
     * @since JTS 2.2.
     */

public final Object[] childTransactions ()
    {
	int size = _childActions.size();
	
	if (size > 0)
	{
	    Collection c = _childActions.values();

	    return c.toArray();
	}
	
	return null;
    }

    /**
     * Forget any heuristics we may have received, and tell the resources
     * which generated them to forget too.
     *
     * @return <code>true</code> if heuristic information (if any) was
     * successfully forgotten, <code>false</code> otherwise.
     */

protected boolean forgetHeuristics ()
    {
	if ((heuristicList != null) && (heuristicList.size() > 0))
	{
	    doForget(heuristicList);
	    updateState();

	    if (heuristicList.size() == 0)
		return true;
	    else
		return false;
	}
	else
	    return true;
    }

    /**
     * Atomic action Begin operation. Does not change the calling
     * thread's notion of the current transaction.
     *
     * @return <code>ActionStatus</code> indicating outcome.
     */

protected synchronized int Begin (BasicAction parentAct)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::Begin() for action-id "+get_uid());
	}

	if (actionStatus != ActionStatus.CREATED)
	{
	    ErrorStream.warning().println("BasicAction.Begin of action "+get_uid()+" ignored - incorrect invocation sequence");
	}
	else
	{
	    actionInitialise(parentAct);
	    actionStatus = ActionStatus.RUNNING;
	    
	    if ((actionType != ActionType.TOP_LEVEL) &&
		((parentAct == null) || (parentAct.status() > ActionStatus.RUNNING)))
	    {
		actionStatus = ActionStatus.ABORT_ONLY;

		if (parentAct == null)
		{
		    ErrorStream.warning().println("BasicAction.Begin of action "+get_uid()+" ignored - no parent and set as nested action!");
		}
		else
		{
		    ErrorStream.warning().println("BasicAction.Begin of action "+get_uid()+" ignored - parent action "+parentAct.get_uid()+" is not running: "+parentAct.status());
		}
	    }
	}

	BasicAction.allActions.put(get_uid(), this);

	if (InitAction.enableStatistics)
	{
	    TxStats.incrementTransactions();

	    if (parentAct != null)
		TxStats.incrementNestedTransactions();
	}
	
	return actionStatus;
    }

    /**
     * End the atomic action by committing it. This invokes the prepare()
     * operation. If this succeeds then the pendingList should be empty 
     * and the records that were formally on it will have been distributed
     * between the preparedList and the readonlyList, also if the action is
     * topLevel then the intention list will have been written to the 
     * object store. Then invoke phase2Commit and clean up the object store
     * if necessary
     *
     * If prepare() fails invoke phase2Abort. In this case the pendingList
     * may still contain records but phase2Abort takes care of these. Also
     * in this case no intention list has been written.
     *
     * Does not change the calling thread's notion of the current transaction.
     *
     * Any heuristic outcomes will only be reported if the parameter is
     * <code>true</code>.
     *
     * @return <code>ActionStatus</code> indicating outcome.
     */

protected synchronized int End (boolean reportHeuristics)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::End() for action-id "+get_uid());
	}

	if (InitAction.restrictedNumberOfTwoPhaseTransactions ||
	    InitAction.restrictedNumberOfOnePhaseTransactions)
	{
	    boolean problem = true;
	    
	    if (InitAction.onePhaseRestriction &&
		!InitAction.restrictedNumberOfOnePhaseTransactions)
	    {
		if (pendingList.size() == 1)
		    problem = false;
	    }

	    if (problem && (InitAction.numberOfTransactions-- < 0))
	    {
		actionStatus = ActionStatus.ABORT_ONLY;
		InitAction.numberOfTransactions = 0;
		    
		ErrorStream.fatal().println("The Arjuna licence does not permit any further transactions to be committed!");
	    }
	}

	/* Check for superfluous invocation */

	if ((actionStatus != ActionStatus.RUNNING) && (actionStatus != ActionStatus.ABORT_ONLY))
	{
	    switch (actionStatus)
	    {
	    case ActionStatus.CREATED:
		ErrorStream.warning().print("End called on non-running atomic action ");
		break;
	    case ActionStatus.COMMITTED:
		ErrorStream.warning().print("End called on already committed atomic action ");
		break;
	    default:
		ErrorStream.warning().print("End called illegally on atomic action ");
		break;
	    }

	    ErrorStream.stream().println(get_uid());

	    return actionStatus;
	}

	/*
	 * Check we are the current action. Abort parents if not true.
	 * Check we have not children (threads or actions).	 
	 */
	
	if (!checkIsCurrent() || checkChildren(true) || (actionStatus == ActionStatus.ABORT_ONLY))
	{
	    return Abort();
	}

	if (pendingList != null)
	{
	    /*
	     * If we only have a single item on the prepare list then
	     * we can try to commit in a single phase.
	     */
	
	    if (doOnePhase())
	    {
		onePhaseCommit(reportHeuristics);
	    }
	    else
	    {
		if (prepare(reportHeuristics) == PrepareOutcome.PREP_NOTOK)
		{
		    ErrorStream.warning().println("BasicAction.End() - prepare phase of action-id "+get_uid()+" failed. ");

		    if (heuristicDecision != PrepareOutcome.PREP_OK)
		    {
			ErrorStream.warning().print("Received heuristic: ");
			PrepareOutcome.print(ErrorStream.stream(), heuristicDecision);
			ErrorStream.stream().print(". ");
		    }
		
		    ErrorStream.warning().println("Action aborting.");

		    if (!reportHeuristics && InitAction.asyncCommit && (parentAction == null))
		    {
			AsyncCommit.create(this, false);
		    }
		    else
			phase2Abort(reportHeuristics);	/* first phase failed */
		}
		else
		{
		    if (!reportHeuristics && InitAction.asyncCommit && (parentAction == null))
		    {
			AsyncCommit.create(this, true);
		    }
		    else
			phase2Commit(reportHeuristics);		/* first phase succeeded */
		}
	    }
	}
	else
	    actionStatus = ActionStatus.COMMITTED;

	BasicAction.allActions.remove(get_uid());
	
	boolean returnCurrentStatus = false;
	
	if (reportHeuristics || (!reportHeuristics && !InitAction.asyncCommit))
	    returnCurrentStatus = true;

	if (InitAction.enableStatistics)
	    TxStats.incrementCommittedTransactions();
	
	if (returnCurrentStatus)
	{
	    if (reportHeuristics)
	    {
		switch (heuristicDecision)
		{
		case PrepareOutcome.PREP_OK:
		case PrepareOutcome.FINISH_OK:
		    break;
		case PrepareOutcome.HEURISTIC_ROLLBACK:
		    return ActionStatus.H_ROLLBACK;
		case PrepareOutcome.HEURISTIC_COMMIT:
		    return ActionStatus.H_COMMIT;
		case PrepareOutcome.HEURISTIC_MIXED:
		    return ActionStatus.H_MIXED;
		case PrepareOutcome.HEURISTIC_HAZARD:
		default:
		    return ActionStatus.H_HAZARD;
		}
	    }

	    return actionStatus;
	}
	else
	    return ActionStatus.COMMITTING;  // if asynchronous then fake it.
    }
    
    /**
     * This is the user callable abort operation. It is invoked prior to the
     * start of two-phase commit and hence only processes records in the
     * pendingList (the other lists should be empty).
     *
     * Does not change the calling thread's notion of the current transaction.
     *
     * @return <code>ActionStatus</code> indicating outcome.
     */

protected synchronized int Abort ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::Abort() for action-id "+get_uid());
	}

	/* Check for superfluous invocation */

	if ((actionStatus != ActionStatus.RUNNING) && (actionStatus != ActionStatus.ABORT_ONLY))
	{
	    switch (actionStatus)
	    {
	    case ActionStatus.CREATED:
		ErrorStream.warning().print("Abort called on non-running atomic action ");
		break;
	    case ActionStatus.ABORTED:
		ErrorStream.warning().print("Abort called on already aborted atomic action ");
		break;
	    default:
		ErrorStream.warning().print("Abort called illegally on atomic action ");
		break;
	    }

	    ErrorStream.stream().println(get_uid());

	    return actionStatus;
	}
	
	/*
	 * Check we are the current action. Abort parents if not true.
	 */

	checkIsCurrent();
	
	/*
	 * Check we have no children (threads or actions).	 
	 */
    	
	checkChildren(false);

	if (pendingList != null)
	{
	    actionStatus = ActionStatus.ABORTING;

	    while (pendingList.size() > 0)
		doAbort(pendingList, false);  // turn off heuristics reporting

	    /*
	     * In case we get here because an End has failed. In this
	     * case we still need to tell the heuristic resources to
	     * forget their decision.
	     */
    
	    forgetHeuristics();
	}

	BasicAction.allActions.remove(get_uid());
    
	actionStatus = ActionStatus.ABORTED;

	if (InitAction.enableStatistics)
	    TxStats.incrementAbortedTransactions();

	return actionStatus;
    }

    /**
     * Create a transaction of the specified type.
     */
    
protected BasicAction (int at)
    {
	super(ObjectType.NEITHER);

	pendingList = null;
	preparedList = null;
	readonlyList = null;
	failedList = null;
	heuristicList = null;

	currentHierarchy = null;
	currentStore = null;
	savedIntentionList = false;

	actionStatus = ActionStatus.CREATED;
	actionType = at;
	parentAction = null;
	recordBeingHandled = null;

	heuristicDecision = PrepareOutcome.PREP_OK;	
	_checkedAction = new CheckedAction();
	
	_childThreads = null;
	_childActions = null;
    }

    /**
     * Recreate the specified transaction. Used for crash recovery
     * purposes.
     */

protected BasicAction (Uid u, int at)
    {
	super(u, ObjectType.NEITHER);

	pendingList = null;
	preparedList = null;
	readonlyList = null;
	failedList = null;
	heuristicList = null;

	currentHierarchy = null;
	currentStore = null;
	savedIntentionList = false;

	actionStatus = ActionStatus.CREATED;
	actionType = at;
	parentAction = null;
	recordBeingHandled = null;

	heuristicDecision = PrepareOutcome.PREP_OK;	
	_checkedAction = new CheckedAction();
	
	_childThreads = null;
	_childActions = null;
    }
    
    /**
     * Defines the start of a critical region
     * by setting the critical flag. If the signal handler is
     * called the class variable abortAndExit is set. The value of
     * this variable is checked in the corresponding operation to
     * end the critical region.
     */

protected final void criticalStart ()
    {
    }

    /**
     * Defines the end of a critical region
     * by resetting the critical flag. If the signal handler is
     * called the class variable abortAndExit is set. The value of
     * this variable is checked when ending the critical region.
     */

protected final void criticalEnd ()
    {
    }
    
    /**
     * Cleanup phase for actions.
     * If an action is in the PREPARED state when a terminate signal is
     * delivered (ie the coordinator node has crashed) then we need to
     * cleanup. This is essentially the same as phase2Abort but we call
     * cleanup ops rather than abort ops and let the records take care
     * of appropriate cleanup.
     *
     * The pendingList is processed because it may not be empty - since
     * prepare() stops processing the list at the first PREP_NOTOK result.
     *
     * The read_only list is processed to ensure that actions are aborted
     * immediately and any servers killed at that point since they need not
     * hang around. This contrasts with commit where readonlyList entries 
     * are simply merged with the parent list or discarded
     */

protected final synchronized void phase2Cleanup ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::phase2Cleanup() for action-id "+get_uid());
	}

	criticalStart();

	actionStatus = ActionStatus.CLEANUP;
	
	while ((preparedList != null) && (preparedList.size() > 0))
	    doCleanup(preparedList);

	while ((readonlyList != null) && (readonlyList.size() > 0))
	    doCleanup(readonlyList);

	while ((pendingList != null) && (pendingList.size() > 0))
	    doCleanup(pendingList);

	criticalEnd();
    }

    /**
     * Second phase of the two-phase commit protocol for committing actions.
     * This operation first invokes the doCommit operation on the
     * preparedList. This ensures that the appropriate commit operation
     * is performed on each entry which is then either deleted (top_level)
     * or merged into the parent's pendingList.
     *
     * Processing of the readonlyList is different in that if the action
     * is top_level then all records in the readonlyList are deleted without
     * further processing. If nested the records must be merged. This is
     * an optimisation to avoid unnecessary processing.
     *
     * Note that at this point the pendingList SHOULD be empty due to the
     * prior invocation of prepare().
     */

protected synchronized final void phase2Commit (boolean reportHeuristics)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::phase2Commit() for action-id "+get_uid());
	}

	if ((pendingList == null) || (pendingList.size() > 0))
	    ErrorStream.warning().println("BasicAction - non-empty pendingList at phase2Commit");

	criticalStart();

	actionStatus = ActionStatus.COMMITTING;

	/*
	 * If we get a heuristic during commit then we continue to commit since
	 * we may have already told some records to commit. We could optimise
	 * this if the first record raises the heuristic by aborting (or going
	 * with the heuristic decision).
	 */
    
	doCommit(preparedList, reportHeuristics);/* process the preparedList */

	/*
	 * Now check any heuristic decision. If we received one then we
	 * may have to raise HEURISTIC_MIXED since we will have committed
	 * some resources, whereas others may have aborted.
	 */
    
	if (heuristicDecision != PrepareOutcome.PREP_OK)
	{
	    /*
	     * Heuristic decision matched the actual outcome!
	     */
	
	    if (heuristicDecision == PrepareOutcome.HEURISTIC_COMMIT)
		heuristicDecision = PrepareOutcome.FINISH_OK;
	}
    
	/* The readonlyList requires special attention */

	if (readonlyList.size() > 0)
	{
	    while (((recordBeingHandled = readonlyList.getFront()) != null))
	    {
		if ((actionType == ActionType.NESTED) && (recordBeingHandled.propagateOnCommit()))
		{
		    merge(recordBeingHandled);
		}
		else
		{
		    recordBeingHandled = null;
		}
	    }
	}

	forgetHeuristics();
	
	actionStatus = ActionStatus.COMMITTED;
	
	updateState();
    
	criticalEnd();
    }

    /**
     * Second phase of the two phase commit protocol for aborting actions.
     * Actions are aborted by invoking the doAbort operation on the
     * preparedList, the readonlyList, and the pendingList.
     *
     * The pendingList is processed because it may not be empty - since
     * prepare() stops processing the list at the first PREP_NOTOK result.
     *
     * By default, records that responsed PREP_READONLY will not be contacted
     * during second-phase abort, just as they are not during second-phase
     * commit. This can be overridden at runtime using the
     * READONLY_OPTIMISATION variable.
     */

protected synchronized final void phase2Abort (boolean reportHeuristics)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::phase2Abort() for action-id "+get_uid());
	}

	criticalStart();

	actionStatus = ActionStatus.ABORTING;

	while ((preparedList != null) && (preparedList.size() > 0))
	    doAbort(preparedList, reportHeuristics);

	if (!InitAction.readonlyOptimisation)
	{
	    while ((readonlyList != null) && (readonlyList.size() > 0))
		doAbort(readonlyList, reportHeuristics);
	}

	while ((pendingList != null) && (pendingList.size() > 0))
	    doAbort(pendingList, reportHeuristics);

	/*
	 * Check heuristic decision, and try to make it match
	 * outcome.
	 */

	if (heuristicDecision != PrepareOutcome.PREP_OK)
	{
	    if (heuristicDecision == PrepareOutcome.HEURISTIC_ROLLBACK)
		heuristicDecision = PrepareOutcome.FINISH_OK;
	}

	forgetHeuristics();
    
	actionStatus = ActionStatus.ABORTED;
	
	updateState();  // we may end up saving more than the heuristic list here!
	
	criticalEnd();
    }

    /**
     * Phase one of a two phase commit protocol.
     * This function returns the ouctome of the prepare operation.
     * If all goes well it will be PREP_OK, if not PREP_NOTOK.
     * The value PREP_READONLY may also be returned if all the records indicate
     * that they are readonly records. Such records do not take part in the
     * second phase commit processing.
     *
     * @return <code>PrepareOutcome</code> indicating outcome.
     */

protected synchronized final int prepare (boolean reportHeuristics)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::prepare () for action-id "+get_uid());
	}

	boolean commitAllowed = (actionStatus != ActionStatus.ABORT_ONLY);

	actionStatus = ActionStatus.PREPARING;
	
	/* If we cannot commit - say the prepare failed */

	if (!commitAllowed)
	{
	    ErrorStream.warning().println("Transaction "+get_uid()+" marked as rollback only. Will abort.");
	    
	    actionStatus = ActionStatus.PREPARED;

	    return PrepareOutcome.PREP_NOTOK;
	}
	
	/*
	 * Make sure the object store is set up for a top-level atomic action.
	 */

	if (actionType == ActionType.TOP_LEVEL)
	{
	    if (store() == null)
	    {
		actionStatus = ActionStatus.ABORT_ONLY;

		return PrepareOutcome.PREP_NOTOK;
	    }
	}

	criticalStart();

	createPreparedLists();
	
	/*
	 * Here is the start of the hard work. Walk down the pendingList
	 * invoking the appropriate prepare operation. If it succeeds 
	 * put the record on either the preparedList or the read_only
	 * list and continue until the pendingList is exhausted.
	 *
	 * If prepare fails on any record stop processing immediately
	 * and put the offending record back on the pendingList
	 */

	int p = PrepareOutcome.PREP_OK;

	/*
	 * If asynchronous prepare, then spawn a separate thread to
	 * handle each entry in the intentions list. Could have some
	 * configurable option to allow more limited number of threads
	 * to divide up the intentions list.
	 */
	
	if ((actionType == ActionType.TOP_LEVEL) && (InitAction.asyncPrepare))
	{
	    int numberOfThreads = ((pendingList != null) ? pendingList.size() : 0);
	    Thread[] threads = new Thread[numberOfThreads];
	    int i;

	    /*
	     * First create them in a suspended way, so that we can purge the
	     * list before it is added to (in the event of failures!)
	     */
	    
	    for (i = 0; i < numberOfThreads; i++)
	    {
		threads[i] = AsyncPrepare.create(this, reportHeuristics, pendingList.getFront());
		
	    }

	    /*
	     * Now start the threads running.
	     */
	    
	    for (i = 0; i < numberOfThreads; i++)
	    {
		threads[i].start();
		Thread.yield();
	    }

	    /*
	     * If one of these threads fails (PREP_NOTOK) do we
	     * terminate the others or simply let them finish?
	     * Currently we wait and let them all terminate regardless.
	     */

	    /*
	     * Now synchronise with the threads.
	     */
	    
	    for (int j = 0; j < numberOfThreads; j++)
	    {
		while (threads[j].isAlive())
		{
		    try
		    {
			threads[j].join();
		    }
		    catch (Exception e)
		    {
			ErrorStream.warning().println("BasicAction.prepare - "+e);
			p = PrepareOutcome.PREP_NOTOK;
		    }
		}

		/*
		 * Only set the outcome if the current value is PREP_OK.
		 */
		
		if (p == PrepareOutcome.PREP_OK)
		    p = ((AsyncPrepare)threads[j]).outcome();

		threads[j] = null;
	    }
	}
	else
	{
	    boolean ok = true;
	    
	    do
	    {
		try
		{
		    p = doPrepare(reportHeuristics);

		    /*
		     * Stop as soon as we get a failure return code.
		     */

		    if ((p != PrepareOutcome.PREP_OK) && (p != PrepareOutcome.PREP_READONLY))
			ok = false;
		}
		catch (IndexOutOfBoundsException e)
		{
		    // end of list!

		    ok = false;
		}

	    } while (ok);
	}

	if ((p != PrepareOutcome.PREP_OK) && (p != PrepareOutcome.PREP_READONLY))
	{
	    if ((actionType == ActionType.NESTED) && ((preparedList.size() > 0) && (p == PrepareOutcome.ONE_PHASE_ERROR)))
	    {
		/*
		 * For the OTS we must merge those records told to commit with the
		 * parent, as the rollback invocation must come from that since
		 * they have already been told this transaction has committed!
		 */

		AbstractRecord tmpRec = preparedList.getFront();

		while (tmpRec != null)
		{
		    merge(tmpRec);
		    tmpRec = preparedList.getFront();
		}

		if (parentAction != null)
		    parentAction.preventCommit();
		else
		{
		    ErrorStream.warning().println("Cannot force parent to rollback - no handle!");
		}
	    }

	    criticalEnd();

	    return PrepareOutcome.PREP_NOTOK;
	}

	/*
	 * Now work out whether there is any state to save. Since we should be
	 * single threaded once again, there is no need to protect the lists
	 * with a synchronization.
	 */

	/*
	 * Could do this as we traverse the lists above, but would need
	 * some compound class for return values.
	 */
	
	boolean stateToSave = false;
	RecordListIterator iter = new RecordListIterator(preparedList);

	/*
	 * First check the prepared list.
	 */
	
	while (((recordBeingHandled = iter.iterate()) != null))
	{
	    if (!stateToSave)
		stateToSave = recordBeingHandled.doSave();

	    if (stateToSave)
		break;
	}

	iter = null;

	if (!stateToSave)
	{
	    iter = new RecordListIterator(heuristicList);
	    
	    /*
	     * Now check the heuristic list.
	     */

	    while (((recordBeingHandled = heuristicList.getFront()) != null))
	    {
		if (!stateToSave)
		    stateToSave = recordBeingHandled.doSave();

		if (stateToSave)
		    break;
	    }

	    iter = null;
	}

	/*
	 * The actual state we want to write depends upon whether
	 * or not we are in charge of the transaction outcome:
	 *
	 * (i) if we are a root transaction, or an interposed transaction which
	 * received a commit_one_phase call, then we have complete control
	 * over what the transaction outcome will be. So, we will always
	 * try to commit, and can set the state to committing.
	 *
	 * (ii) if we are an interpoosed transaction and it receives
	 * a complete two-phase protocol, then the root is in control.
	 * So, we set the state to prepared.
	 *
	 * (iii) nested transactions never write state, so the state
	 * is set to prepared anyway.
	 */

	if (actionType == ActionType.TOP_LEVEL)
	    actionStatus = preparedStatus();
	else
	    actionStatus = ActionStatus.PREPARED;

	/* 
	 * If we are here then everything went okay so save the intention list
	 * in the ObjectStore in case of a node crash providing that its not
	 * empty
	 */

	if ((actionType == ActionType.TOP_LEVEL) && (stateToSave) &&
	    ((preparedList.size() > 0) || (heuristicList.size() > 0)))
	{
	    /* Only do this if we have some records worth saving! */
	
	    Uid u = getSavingUid();
	    String tn = type();
	    OutputObjectState state = new OutputObjectState(u, tn);

	    if (!save_state(state, ObjectType.ANDPERSISTENT))
	    {
		ErrorStream.warning().println("BasicAction::prepare - creating intentions list failed for "+get_uid());

		criticalEnd();
		
		return PrepareOutcome.PREP_NOTOK;
	    }

	    if (state.notempty())
	    {
		try
		{
		    if (!currentStore.write_committed(u, tn, state))
		    {
			ErrorStream.warning().println("BasicAction::prepare - intentions list write failed for "+get_uid());
			
			criticalEnd();
			
			return PrepareOutcome.PREP_NOTOK;
		    }
		    else
			savedIntentionList = true;
		}
		catch (ObjectStoreException e)
		{
		    criticalEnd();
		    
		    return PrepareOutcome.PREP_NOTOK;
		}
	    }
	}

	criticalEnd();

	if ((preparedList.size() == 0) && (readonlyList.size() >= 0))
	    return PrepareOutcome.PREP_READONLY;
	else
	    return PrepareOutcome.PREP_OK;
    }

    /**
     * There is only one record on the intentions list.
     * Only called from synchronized methods.
     * Don't bother about creating separate threads here!
     */

protected void onePhaseCommit (boolean reportHeuristics)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::onePhaseCommit() for action-id "+get_uid());
	}

	/* Are we forced to abort? */

	if (actionStatus == ActionStatus.ABORT_ONLY)
	{
	    ErrorStream.warning().println("Transaction "+get_uid()+" marked as rollback only. Will abort.");

	    Abort();
	
	    return;
	}

	actionStatus = ActionStatus.COMMITTING;
    
	criticalStart();

	if ((heuristicList == null) && reportHeuristics)
	    heuristicList = new RecordList();  // only one we'll need.

	/*
	 * Since it is one-phase, the outcome from the record
	 * is the outcome of the transaction. Therefore, we don't
	 * need to save much intermediary transaction state - only
	 * heuristics in the case of interposition.
	 */
	
	boolean stateToSave = false;
    
	recordBeingHandled = pendingList.getFront();

	int p = ((actionType == ActionType.TOP_LEVEL)
		 ? recordBeingHandled.topLevelOnePhaseCommit()
		 : recordBeingHandled.nestedOnePhaseCommit());

	if ((p == PrepareOutcome.FINISH_OK) ||
	    (p == PrepareOutcome.PREP_READONLY))
	{
	    if ((actionType == ActionType.NESTED) && recordBeingHandled.propagateOnCommit())
	    {
		merge(recordBeingHandled);
	    }
	    else
	    {
		recordBeingHandled = null;
	    }
	
	    actionStatus = ActionStatus.COMMITTED;
	}
	else
	{
	    // aborted or heuristic which we aren't interested in
	    
	    if (p == PrepareOutcome.FINISH_ERROR)
	    {
		/*
		 * Don't bother about the failedList - we are aborting, and
		 * there is only one record!
		 */
		
		recordBeingHandled = null;

		actionStatus = ActionStatus.ABORTED;
	    }
	    else
	    {
		/*
		 * Heuristic decision!!
		 */

		//		if (reportHeuristics)
		{
		    ErrorStream.warning().print("One-phase commit of action "+get_uid()+" received heuristic decision: ");
		    PrepareOutcome.print(ErrorStream.stream(), p);
		    ErrorStream.stream().println();		    
		}

		if (reportHeuristics)
		{
		    updateHeuristic(p, true);

		    if (!heuristicList.insert(recordBeingHandled))
			recordBeingHandled = null;
		    else
		    {
			if (!stateToSave)
			    stateToSave = recordBeingHandled.doSave();
		    }
		}

		if (heuristicDecision == PrepareOutcome.HEURISTIC_ROLLBACK)
		{
		    /*
		     * Signal that the action outcome is the same as
		     * the heuristic decision.
		     */

		    heuristicDecision = PrepareOutcome.PREP_OK;  // means no heuristic was raised.

		    actionStatus = ActionStatus.ABORTED;
		}
		else
		    actionStatus = ActionStatus.COMMITTED;  // can't really say (could have aborted)
	    }
	}
    
	if (actionType == ActionType.TOP_LEVEL)
	{
	    if (stateToSave && (heuristicList.size() > 0))
	    {
		if (store() == null)
		{
		    ErrorStream.stream(ErrorStream.FATAL).println("BasicAction.onePhaseCommit failed - no object store for atomic action state!");

		    throw new com.arjuna.ArjunaCommon.Common.FatalError("No object store for: "+get_uid());
		}

		updateState();
	    }
	}

	forgetHeuristics();

	criticalEnd();
    }

    /**
     * @return the current heuristic decision. Each time a heuristic
     * outcome is received, we need to merge it with any previous
     * outcome to determine what the overall heuristic decision is
     * (e.g., a heuristic rollback followed by a heuristic commit means
     * the overall decision is heuristic mixed.)
     */

protected final synchronized int getHeuristicDecision ()
    {
	return heuristicDecision;
    }

    /**
     * Add the specified abstract record to the transaction. Does
     * not do any of the runtime checking of BasicAction.add, so
     * should be used with care. Currently used by crash recovery.
     */

protected final synchronized void addRecord (AbstractRecord A)
    {
	preparedList.insert(A);
    }

    /**
     * @returns the transaction's prepared status.
     *
     * @since JTS 2.0.
     */

protected synchronized int preparedStatus ()
    {
	return ActionStatus.COMMITTING;
    }

    /*
     * Package visibility.
     */

    /*
     * The single-threaded version of doPrepare.
     * If we do not use asynchronous prepare, then we don't need to
     * lock the RecordLists - only one thread can access them anyway!
     */

    /*
     * If we do not use asynchronous prepare, then we don't need to
     * lock the RecordLists - only one thread can access them anyway!
     * However, if asynchronous prepare is being used then we need to
     * synchronize on them. The 'lock' parameter (default false) indicates
     * whether we should try to synchronize the RecordList or not.
     */
    
final int doPrepare (boolean reportHeuristics) throws IndexOutOfBoundsException
    {
	/*
	 * Here is the start of the hard work. Walk down the pendingList
	 * invoking the appropriate prepare operation. If it succeeds 
	 * put the record on either the preparedList or the read_only
	 * list and continue until the pendingList is exhausted.
	 *
	 * If prepare fails on any record stop processing immediately
	 * and put the offending record back on the pendingList.
	 */

	int p = PrepareOutcome.PREP_NOTOK;

	do
	{
	    /*
	     * March down the pendingList and pass the head of the list
	     * to the main work routine until either we run out of
	     * elements, or one of them fails.
	     */
	    
	    AbstractRecord rec = ((pendingList != null) ? pendingList.getFront() : null);
	    
	    p = doPrepare(reportHeuristics, rec);
	    
	} while ((p == PrepareOutcome.PREP_OK) || (p == PrepareOutcome.PREP_READONLY));

	return p;
    }

    /*
     * The multi-threaded version of doPrepare. Each thread was given the
     * record it should process when it was created so that if a failure
     * occurs we can put it back onto the pendingList at the right place.
     * It also cuts down on the amount of synchronisation we must do.
     */
    
final int doPrepare (boolean reportHeuristics, AbstractRecord theRecord) throws IndexOutOfBoundsException
    {
	/*
	 * Here is the start of the hard work. Walk down the pendingList
	 * invoking the appropriate prepare operation. If it succeeds 
	 * put the record on either the preparedList or the read_only
	 * list and continue until the pendingList is exhausted.
	 *
	 * If prepare fails on any record stop processing immediately
	 * and put the offending record back on the pendingList.
	 */

	int p = PrepareOutcome.PREP_NOTOK;
	AbstractRecord record = theRecord;
	boolean lock = InitAction.asyncPrepare;
	
	if (record != null)
	{
	    p = ((actionType == ActionType.TOP_LEVEL)
		 ? record.topLevelPrepare()
		 : record.nestedPrepare());

	    if (p == PrepareOutcome.PREP_OK)
	    {
		if (lock)
		{
		    synchronized (preparedList)
		    {
			if (!preparedList.insert(record))
			    record = null;
		    }
		}
		else
		{
		    if (!preparedList.insert(record))
			record = null;
		}
	    }
	    else
	    {
		if (p == PrepareOutcome.PREP_READONLY)
		{
		    if (lock)
		    {
			synchronized (readonlyList)
		        {
			    if (!readonlyList.insert(record))
				record = null;
			}
		    }
		    else
		    {
			if (!readonlyList.insert(record))
			    record = null;
		    }
		}
		else
		{
		    /*
		     * If we are a subtransaction and this is an OTS resource
		     * then we may be in trouble: we may have already told
		     * other records to commit.
		     */
		    
		    if ((p == PrepareOutcome.PREP_NOTOK) || (p == PrepareOutcome.ONE_PHASE_ERROR) || (!reportHeuristics))
		    {
			if (actionType == ActionType.NESTED)
			{
			    if ((preparedList.size() > 0) && (p == PrepareOutcome.ONE_PHASE_ERROR))
			    {
				ErrorStream.warning().println("Prepare phase of nested action "+get_uid()+" received inconsistent outcomes.");
				
				/*
				 * Force parent to rollback. If this is not the
				 * desired result then we may need to check
				 * some environment variable (either here or
				 * in the OTS) and act accordingly. If we
				 * check in the OTS then we need to return
				 * something other than PREP_NOTOK.
				 */

				/*
				 * For the OTS we must merge those records
				 * told to commit with the
				 * parent, as the rollback invocation must
				 * come from that since
				 * they have already been told this
				 * transaction has committed!
				 *
				 * However, since we may be multi-threaded
				 * (asynchronous prepare)
				 * we don't do the merging yet. Wait until all
				 * threads have terminated
				 * and then do it.
				 *
				 * Therefore, can't force parent to rollback
				 * state at present, or merge will fail.
				 */
			    }
			}

			/*
			 * Prepare on this record failed - we are in trouble.
			 * Add the record back onto the pendingList and return.
			 */

			if (lock)
			{
			    synchronized (pendingList)
			    {
				/*
				 * Use insert to put back at the right place.
				 */
				
				pendingList.insert(record);
			    }
			}
			else
			{
			    /*
			     * In a single-threaded environment, the front of
			     * the list is the right place!
			     */
			    
			    pendingList.putFront(record);
			}
			
			record = null;
		
			actionStatus = ActionStatus.PREPARED;
		    
			return p;
		    }
		    else
		    {
			/*
			 * Heuristic decision!!
			 */

			/*
			 * Only report if request to do so.
			 */

			//			if (reportHeuristics)
			{
			    ErrorStream.warning().print("Prepare phase of action "+get_uid()+" received heuristic decision: ");
			    PrepareOutcome.print(ErrorStream.stream(), p);
			    ErrorStream.stream().println();

			    if (reportHeuristics)
				updateHeuristic(p, false);
			}

			/*
			 * Don't add to the prepared list. We process
			 * heuristics separately during phase 2. The processing
			 * of records will not be in the same order as during
			 * phase 1, but does this matter for heuristic
			 * decisions? If so, then we need to modify RecordList
			 * so that records can appear on multiple lists at the
			 * same time.
			 */

			if (lock)
			{
			    synchronized (heuristicList)
			    {
				if (!heuristicList.insert(record))
				    record = null;
			    }
			}
			else
			{
			    if (!heuristicList.insert(record))
				record = null;
			}

			/*
			 * If we have had a heuristic decision, then attempt to
			 * make the action outcome the same. If we have a
			 * conflict, then we will abort.
			 */
			
			if (heuristicDecision != PrepareOutcome.HEURISTIC_COMMIT)
			{
			    actionStatus = ActionStatus.PREPARED;
		    
			    return PrepareOutcome.PREP_NOTOK;
			}
			else
			{
			    /*
			     * Heuristic commit, which is ok since we want to
			     * commit anyway! So, ignore it (but remember the
			     * resource so we can tell it to forget later.)
			     */
			}
		    }
		}
	    }
	}
	else  // no more entries in pendingList
	    throw new IndexOutOfBoundsException();

	return p;
    }

    /*
     * This is only meant as an instance cut of the children, so
     * don't lock the entire transaction. Thus, the list may change
     * before we return.
     */

private final void createPreparedLists ()
    {
	if (preparedList == null)
 	    preparedList = new RecordList();
 
 	if (readonlyList == null)
 	    readonlyList = new RecordList();	
 
 	if (failedList == null)
 	    failedList = new RecordList();
 
 	if (heuristicList == null)
 	    heuristicList = new RecordList();

	if (pendingList == null)
	    pendingList = new RecordList();
    }
    
    /*
     * If we get a single heuristic then we will always rollback during
     * prepare.
     *
     * Getting a heuristic during commit is slightly different, since some
     * resources may have already committed, changing the type of heuristic
     * we may need to throw. However, once we get to commit we know that it
     * will be the final outcome. So, as soon as a single resource commits
     * successfully, we can take it as a HEURISTIC_COMMIT. We will forget a
     * HEURISTIC_COMMIT outcome at the end anyway.
     */
    
private final synchronized void updateHeuristic (int p, boolean commit)
    {
	/*
	 * Some resource has prepared/committed ok, so we need to remember
	 * this in case we get a future heuristic.
	 */

	if (p == PrepareOutcome.FINISH_OK)
	{
	    if (commit)
	    {
		if (heuristicDecision == PrepareOutcome.PREP_OK)
		    p = PrepareOutcome.HEURISTIC_COMMIT;
		    
		if (heuristicDecision == PrepareOutcome.HEURISTIC_ROLLBACK)
		    heuristicDecision = PrepareOutcome.HEURISTIC_MIXED;
	    }
	    else
	    {
		if (heuristicDecision == PrepareOutcome.PREP_OK)
		    p = PrepareOutcome.HEURISTIC_ROLLBACK;
		
		if (heuristicDecision == PrepareOutcome.HEURISTIC_COMMIT)
		    heuristicDecision = PrepareOutcome.HEURISTIC_MIXED;
	    }
		
	    // leave HAZARD and MIXED alone
	}
	
	/*
	 * Is this the first heuristic? Always give HEURISTIC_MIXED
	 * priority, but if we have no heuristic and we get a HEURISTIC_HAZARD
	 * the go with that until something better comes along!
	 */

	/*
	 * Have we already been given a conflicting heuristic? If so,
	 * raise the decision to the next heuristic level.
	 */

	switch (heuristicDecision)
	{
	case PrepareOutcome.PREP_OK:
	    if ((p != PrepareOutcome.PREP_OK) && (p != PrepareOutcome.FINISH_OK))   // first heuristic outcome.
		heuristicDecision = p;
	    break;
	case PrepareOutcome.HEURISTIC_COMMIT:
	    if ((p == PrepareOutcome.HEURISTIC_ROLLBACK) || (p == PrepareOutcome.HEURISTIC_MIXED))
		heuristicDecision = PrepareOutcome.HEURISTIC_MIXED;
	    else
	    {
		if (p == PrepareOutcome.HEURISTIC_HAZARD)
		    heuristicDecision = PrepareOutcome.HEURISTIC_HAZARD;
	    }
	    break;
	case PrepareOutcome.HEURISTIC_ROLLBACK:
	    if ((p == PrepareOutcome.HEURISTIC_COMMIT) || (p == PrepareOutcome.HEURISTIC_MIXED))
		heuristicDecision = PrepareOutcome.HEURISTIC_MIXED;
	    else
	    {
		if (p == PrepareOutcome.HEURISTIC_HAZARD)
		    heuristicDecision = PrepareOutcome.HEURISTIC_HAZARD;
	    }
	    break;
	case PrepareOutcome.HEURISTIC_HAZARD:
	    if (p == PrepareOutcome.HEURISTIC_MIXED)
		heuristicDecision = PrepareOutcome.HEURISTIC_MIXED;
	    break;
	case PrepareOutcome.HEURISTIC_MIXED:
	    break;
	default:
	    heuristicDecision = p; // anything!
	    break;
	}

	if (InitAction.enableStatistics)
	    TxStats.incrementHeuristics();
    }    
    
private final boolean checkIsCurrent ()
    {
	boolean isCurrent = true;
	BasicAction currentAct = BasicAction.Current();
	
	/* Ensure I am the currently active action */
	
	if ((currentAct != null) && (currentAct != this))
	{
	    ErrorStream.warning().println("BasicAction.checkIsCurrent "+currentAct.get_uid()+" - terminating non-current transaction: "+get_uid());
	    
	    isCurrent = false;
	
	    if (currentAct.isAncestor(get_uid()))
	    {
		/* current action is one of my children */

		BasicAction parentAct = parent();
	    
		/* prevent commit of my parents (ensures safety) */

		while (parentAct != null)
		{
		    parentAct.preventCommit();
		    parentAct = parentAct.parent();
		}
	    }
	}

	currentAct = null;
	
	return isCurrent;
    }
    
private final boolean checkChildren (boolean isCommit)
    {
	boolean problem = false;
	
	/*
	 * If we have child threads then by default we just print a warning
	 * and continue. The other threads will eventually find out the
	 * outcome.
	 */

	if ((_childThreads != null) && (_childThreads.size() > 0))
	{
	    if ((_childThreads.size() != 1) ||
		((_childThreads.size() == 1) && (!_childThreads.contains(Thread.currentThread()))))
	    {
		/* More than one thread or the one thread is not the current thread */

		if (isCommit)
		    ErrorStream.warning().print("Commit of action id ");
		else
		    ErrorStream.warning().print("Abort of action id ");

		ErrorStream.stream().println(get_uid() + " invoked while multiple threads active within it.");
		    
		if (_checkedAction != null)
		    _checkedAction.check(isCommit, get_uid(), _childThreads);

		removeAllChildThreads();
	    }
	}

	/* Ensure I have no child actions */
	
	if ((_childActions != null) && (_childActions.size() > 0))
	{
	    problem = true;

	    Enumeration iter = _childActions.elements();
	    BasicAction child = null;
	    boolean printError = true;
	    boolean havePrinted = false;

	    /*
	     * We may have already aborted our children, e.g., because of
	     * an out-of-sequence commit, so we check here to reduce the
	     * number of error messages!
	     *
	     * We can't just remove the children when we are finished with
	     * them because BasicAction is not responsible for action
	     * tracking.
	     */
	
	    while (iter.hasMoreElements())
	    {
		child = (BasicAction) iter.nextElement();
		
		if (child.status() != ActionStatus.ABORTED)
		{
		    if (printError)
		    {
			if (isCommit)
			    ErrorStream.warning().print("Commit of action id ");
			else
			    ErrorStream.warning().print("Abort of action id ");
	
			ErrorStream.stream().println(get_uid()+" invoked while child actions active");

			printError = false;
		    }

		    ErrorStream.warning().println("\tAborting child: "+child.get_uid());

		    havePrinted = true;

		    child.Abort();
		    child = null;
		}
	    }

	    iter = null;

	    if (isCommit)
	    {
		ErrorStream.warning().println("\tNow aborting self: "+get_uid());
		havePrinted = true;
	    }

	    if (havePrinted)
	    {
		ErrorStream.stream().println();
	    }
	}

	return problem;
    }
    
/*
 * Just in case we are deleted/terminated with threads still
 * registered. We must make sure those threads don't
 * try to remove themselves from this action later.
 * So we unregister them ourselves now.
 *
 * This is only called by End/Abort and so all child actions
 * will have been previously terminated as well.
 */

private final void removeAllChildThreads ()
    {
	/*
	 * Do not remove the current thread as it is
	 * committing/aborting!
	 */
	
	criticalStart();
    
	if ((_childThreads != null) && (_childThreads.size() != 0))
	{
	    Thread currentThread = Thread.currentThread();
	    
	    /*
	     * Iterate through all registered threads and
	     * tell them to ignore the action pointer, i.e.,
	     * they are now no longer within this action.
	     */

	    Enumeration iter = _childThreads.elements();
	    Thread t = null;
	    
	    while (iter.hasMoreElements())
	    {
		t = (Thread) iter.nextElement();
		
		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
							 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::removeAllChildThreads () action "+get_uid()+" removing "+t);
		}

		if (t != currentThread)
		    ThreadActionData.purgeAction(this, t);
	    }
	}

	criticalEnd();
    }
    
private final void updateState ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::updateState() for action-id "+get_uid());
	}

	/*
	 * If the action is topLevel then prepare() will have written the
	 * intention_list to the object_store.
	 * If any of the phase2Commit processing failed then records will
	 * exist on the failedList. If this is the case then we need to
	 * re-write the BasicAction record in the object store. If the
	 * failed list is empty we can simply delete the BasicAction
	 * record.
	 */

	if (actionType == ActionType.TOP_LEVEL)
	{
	    /*
	     * make sure the object store is set up for a top-level atomic
	     * action.
	     */

	    store();

	    if (((failedList != null) && (failedList.size() > 0)) ||
		((heuristicList != null) && (heuristicList.size() > 0)))
	    {
		/*
		 * Re-write the BasicAction record with the failed list
		 */

		Uid u = getSavingUid();
		String tn = type();
		OutputObjectState state = new OutputObjectState(u, tn);
	    
		if (!save_state(state, ObjectType.ANDPERSISTENT))
		{
		    ErrorStream.warning().println("BasicAction.updateState - Could not create ObjectState for failedList");

		    // what else?
		}

		if (state.notempty())
		{
		    try
		    {
			if (!currentStore.write_committed(u, tn, state))
			{
			    ErrorStream.warning().println("BasicAction.End - Could not write failed list");
			}
		    }
		    catch (ObjectStoreException e)
		    {
			ErrorStream.warning().println(e.getMessage());
		    }
		}
	    }
	    else
	    {
		try
		{
		    if (savedIntentionList)
			currentStore.remove_committed(getSavingUid(), type());
		}
		catch (ObjectStoreException e)
		{
		}
	    }
	}
    }

    /* 
     * action_initialise determines whether the BasicAction is a nested,
     * top-level, or a top-level nested atomic action
     */

private final void actionInitialise (BasicAction parent)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::actionInitialise() for action-id "+get_uid());
	}

	criticalStart();

	if (parent != null)		/* ie not top_level */
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						     FacilityCode.FAC_ATOMIC_ACTION, "Action "+get_uid()+" with parent status "+parent.actionStatus);
	    }

	    currentHierarchy = new ActionHierarchy(parent.getHierarchy());
	}
	else
	{
	    currentHierarchy = new ActionHierarchy(ActionHierarchy.DEFAULT_HIERARCHY_DEPTH);
	    
	    /* this is a top-level atomic action so set the signal handler */
	    /* block a number of signals */
	}
    
	currentHierarchy.add(get_uid(), actionType);

	switch(actionType)
	{
	case ActionType.TOP_LEVEL:
	    if (parent != null)
	    {
		/*
		 * do not want to print warning all the time as this is what
		 * nested top-level actions are used for.
		 */

		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
							 FacilityCode.FAC_ATOMIC_ACTION, "Running Top Level Action ("+get_uid()+") from within nested action ("+parent.get_uid()+")");
		}
	    }
	    break;
	case ActionType.NESTED :
	    if (parent == null)
		actionType = ActionType.TOP_LEVEL;
	    break;
	}

	parentAction = parent;

	criticalEnd();
    }

    /*
     * Walk down a record list extracting records and calling the appropriate
     * abort function. Discard records when done.
     */

private final void doAbort (RecordList list_toprocess, boolean reportHeuristics)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::doAbort ("+list_toprocess+")");
	}

	if (list_toprocess.size() > 0)
	{
	    int ok = PrepareOutcome.FINISH_OK;
	
	    while (((recordBeingHandled = list_toprocess.getFront()) != null))
	    {   
		if (actionType == ActionType.TOP_LEVEL)
		    ok = recordBeingHandled.topLevelAbort();
		else
		    ok = recordBeingHandled.nestedAbort();

		if ((actionType != ActionType.TOP_LEVEL) && (recordBeingHandled.propagateOnAbort()))
		{
		    merge(recordBeingHandled);
		}
		else
		{
		    if (ok == PrepareOutcome.FINISH_OK)
		    {
			updateHeuristic(PrepareOutcome.FINISH_OK, false);  // remember that something aborted ok
		    }
		    else
		    {
			if ((reportHeuristics) && ((ok == PrepareOutcome.HEURISTIC_ROLLBACK) ||
						   (ok == PrepareOutcome.HEURISTIC_COMMIT) ||
						   (ok == PrepareOutcome.HEURISTIC_MIXED) ||
						   (ok == PrepareOutcome.HEURISTIC_HAZARD)))
			{
			    if (actionType == ActionType.TOP_LEVEL)
				ErrorStream.warning().print("Top-level ");
			    else
				ErrorStream.warning().print("Nested ");

			    ErrorStream.stream().print("abort of action "+get_uid()+" received heuristic decision: ");
			    PrepareOutcome.print(ErrorStream.stream(), ok);
			    ErrorStream.stream().println();
			    
			    updateHeuristic(ok, false);
			    heuristicList.insert(recordBeingHandled);
			}
			else
			{
			    if (ok != PrepareOutcome.FINISH_OK)
			    {
				if (actionType == ActionType.TOP_LEVEL)
				    ErrorStream.warning().print("Top-level ");
				else
				    ErrorStream.warning().print("Nested ");

				ErrorStream.print("abort of action "+get_uid()+" received ");
				PrepareOutcome.print(ErrorStream.stream(), ok);
				ErrorStream.println(" from "+RecordType.typeToClassName(recordBeingHandled.typeIs()));
			    }
			}
		    }

		    /*
		     * Don't need a canDelete as in the C++ version since
		     * Java's garbage collection will deal with things for us.
		     */
		 
		    recordBeingHandled = null;
		}
	    }
	}
    }

private final void doForget (RecordList list_toprocess)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::doForget ("+list_toprocess+")");
	}

	/*
	 * If the user has selected to maintain all heuristic information, then
	 * we never explicitly tell resources to forget. We assume that the
	 * user (or some management tool) will do this, and simply save as much
	 * information as we can into the action state to allow them to do so.
	 */
    
	if (!InitAction.maintainHeuristics)
	{
	    if (list_toprocess.size() > 0)
	    {
		RecordList tmpList = new RecordList();
		
		while (((recordBeingHandled = list_toprocess.getFront())) != null)
		{
		    /*
		     * Remember for later if we cannot tell it to forget.
		     */

		    if (recordBeingHandled.forgetHeuristic())
			recordBeingHandled = null;
		    else
			tmpList.putFront(recordBeingHandled);
		}

		/*
		 * Now put those resources we couldn't tell to forget
		 * back on the heuristic list.
		 */

		if (tmpList.size() > 0)
		{
		    while ((recordBeingHandled = tmpList.getFront()) != null)
			list_toprocess.putFront(recordBeingHandled);
		}
	    }
	}
    }
    
    /*
     * Walk down a record list extracting records and calling the appropriate
     * cleanup function. Discard records when done.
     * NOTE: We only need to do cleanup at top level since cleanup at nested
     * level would be subsumed when the parent action is forced to abort
     *
     * Ignore heuristics. Who can we report them to?
     *
     * This routine is called by phase2Cleanup, which gets called only in
     * exceptional circumstances. By default we leave cleaning up the various
     * lists until the action instance goes out of scope.
     */

private final void doCleanup (RecordList list_toprocess)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::doCleanup ("+list_toprocess+")");
	}

	if (list_toprocess.size() > 0)
	{
	    int ok = PrepareOutcome.FINISH_OK;
	
	    while (((recordBeingHandled = list_toprocess.getFront()) != null))
	    {
		if (actionType == ActionType.TOP_LEVEL)
		    ok = recordBeingHandled.topLevelCleanup();
		else
		    ok = recordBeingHandled.nestedCleanup();

		if ((actionType != ActionType.TOP_LEVEL) &&
		    (recordBeingHandled.propagateOnAbort()))
		{
		    merge(recordBeingHandled);
		}
		else
		{
		    if (ok != PrepareOutcome.FINISH_OK)
		    {
			/* Preserve error messages */
		    }

		    recordBeingHandled = null;
		}
	    }
	}
    }

private final synchronized boolean doOnePhase ()
    {
	if (InitAction.onePhase)
	    return (((pendingList == null) || (pendingList.size() == 1)) ? true : false);
	else
	    return false;
    }
    
    /*
     * Walk down a record list extracting records and calling the appropriate
     * commit function. Discard or merge records as appropriate
     */

private final void doCommit (RecordList rl, boolean reportHeuristics)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_ATOMIC_ACTION, "BasicAction::doCommit ("+rl+")");
	}

	/*
	 * To get heuristics right, as soon as we manage to commit the
	 * first record we set the heuristic to HEURISTIC_COMMIT. Then, if
	 * any other heuristics are raised we can manage the final outcome
	 * correctly.
	 */
	
	if (rl.size() > 0)
	{
	    int ok = PrepareOutcome.FINISH_ERROR;
	
	    while (((recordBeingHandled = rl.getFront()) != null))
	    {
		if (actionType == ActionType.TOP_LEVEL)
		{
		    if ((ok = recordBeingHandled.topLevelCommit()) == PrepareOutcome.FINISH_OK)
		    {
			/*
			 * Record successfully committed, we can delete it now.
			 */

			recordBeingHandled = null;

			updateHeuristic(PrepareOutcome.FINISH_OK, true);// must remember that something has committed
		    }
		    else
		    {
			if (DebugController.controller().debugAllowed(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
								      FacilityCode.FAC_ATOMIC_ACTION))
			{
			    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
							       FacilityCode.FAC_ATOMIC_ACTION, "BasicAction.doCommit for "+get_uid()+" received ");

			    PrepareOutcome.print(DebugController.controller().getStream(), ok);
			    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
							       FacilityCode.FAC_ATOMIC_ACTION, " from ");

			    RecordType.print(DebugController.controller().getStream(), recordBeingHandled.typeIs());

			    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
								 FacilityCode.FAC_ATOMIC_ACTION, "");
			}
			
			if ((reportHeuristics) && ((ok == PrepareOutcome.HEURISTIC_ROLLBACK) ||
						   (ok == PrepareOutcome.HEURISTIC_COMMIT) ||
						   (ok == PrepareOutcome.HEURISTIC_MIXED) ||
						   (ok == PrepareOutcome.HEURISTIC_HAZARD)))
			{
			    updateHeuristic(ok, true);
			    heuristicList.insert(recordBeingHandled);
			}
			else
			{
			    if (ok == PrepareOutcome.NOT_PREPARED)
			    {
				/*
				 * If this is the first resource then rollback,
				 * otherwise promote to HEURISTIC_HAZARD, but
				 * don't add to heuristicList.
				 */

				updateHeuristic(PrepareOutcome.HEURISTIC_HAZARD, true);
			    }
			    else
			    {
				/*
				 * The commit failed. Add this record to the
				 * failed list to indicate this.
				 */

				failedList.insert(recordBeingHandled);
			    }
			}
		    }
		}
		else
		{
		    /*
		     * Thankfully nested actions cannot raise heuristics!
		     */
		    
		    ok = recordBeingHandled.nestedCommit();

		    if (recordBeingHandled.propagateOnCommit())
		    {
			merge(recordBeingHandled);
		    }
		    else
		    {
			recordBeingHandled = null;
		    }
		}

		if (ok != PrepareOutcome.FINISH_OK)
		{
		    /* Preserve error messages */
		}
	    }
	}
    }

    /* 
     * Operation to merge a record into those held by the parent BasicAction.
     * This is accomplished by invoking the add operation of the parent
     * BasicAction. If the add operation does not return AR_ADDED, the record
     * is deleted
     */

private final synchronized void merge (AbstractRecord A)
    {
	int as;
    
	if ((as = parentAction.add(A)) != AddOutcome.AR_ADDED)
	{
	    A = null;
	    if (as == AddOutcome.AR_REJECTED)
		ErrorStream.warning().println("(Internal) BasicAction.merge - record rejected");
	}
    }

    /*
     * We would like this to be a list of created BasicActions, but
     * if so, they will never be garbage collected. Could use weak
     * references, but that is 1.2 specific. So, at present it is
     * a list of running BasicActions! Slightly different from that
     * in C++.
     */
    
public static Hashtable allActions = new Hashtable();

    /* These (genuine) lists hold the abstract records */

protected RecordList pendingList;
protected RecordList preparedList;
protected RecordList readonlyList;
protected RecordList failedList;
protected RecordList heuristicList;
    
private ActionHierarchy currentHierarchy;
private ObjectStore     currentStore;
private boolean         savedIntentionList;

    /* Atomic action status variables */

private int            actionStatus;
private int            actionType;

private BasicAction    parentAction;    
private AbstractRecord recordBeingHandled;
private int            oldMask;    // would be signal mask if such a thing existed in Java
private int            heuristicDecision;
    
private CheckedAction  _checkedAction;  // control what happens if threads active when terminating.
    
    /*
     * We need to keep track of the number of threads associated with
     * each action. Since we can't override the basic thread methods,
     * we have to provide an explicit means of registering threads with
     * an action.
     */

private Hashtable _childThreads;
private Hashtable _childActions;

private static boolean _ticilDone = false;
    
}
