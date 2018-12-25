/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BasicAction.java,v 1.14 1998/08/13 14:31:34 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ClassLib;

import com.arjuna.JavaArjuna.ObjectStore.ObjectStore;
import com.arjuna.JavaArjuna.Thread.*;
import com.arjuna.JavaArjuna.ClassLib.RecoveryAbstractRecord;
import com.arjuna.JavaArjuna.Common.*;
import com.arjuna.JavaArjuna.Template.*;

import com.arjuna.JavaArjuna.Common.ObjectStoreException;
import java.lang.NumberFormatException;
import java.lang.InterruptedException;
import java.io.IOException;

/*
 * BasicAction does most of the work of an atomic action,
 * but does not manage thread scoping. This is the responsibility
 * of any derived classes.
 */

public class BasicAction extends StateManager implements UidListElement
{

    /*
     * ActionType
     */
    
public static final int TOP_LEVEL = 0;
public static final int NESTED = 1;

    /*
     * addOutcome
     */

public static final int ADDED = 2;
public static final int REJECTED = 3;
public static final int DUPLICATE = 4;

public BasicAction ()
    {
	super(ObjectType.NEITHER);
	
	currentHierarchy = null;
	currentStore = null;
	savedIntentionList = false;
	
	pendingList = new RecordList();
	preparedList = new RecordList();
	readonlyList = new RecordList();	
	failedList = new RecordList();
	heuristicList = new RecordList();

	actionStatus = ActionStatus.CREATED;
	actionType = BasicAction.NESTED;
	
	parentAction = null;
	recordBeingHandled = null;

	heuristicDecision = PrepareOutcome.PREP_OK;
	_checkedAction = new CheckedAction();
	
	_childThreads = null;
	_childActions = new BasicList();

	BasicAction.allActions.add(this);
    }

    /*
     * BasicAction constructor with a Uid.
     * This constructor is for recreating an BasicAction, typically during
     * crash recovery.
     */

public BasicAction (Uid objUid)
    {
	super(objUid, ObjectType.NEITHER);

	currentHierarchy = null;
	currentStore = null;
	savedIntentionList = false;

	pendingList = new RecordList();
	preparedList = new RecordList();
	readonlyList = new RecordList();
	failedList = new RecordList();
	heuristicList = new RecordList();
	
	actionStatus = ActionStatus.CREATED;
	actionType = BasicAction.NESTED;

	parentAction = null;
	recordBeingHandled = null;

	heuristicDecision = PrepareOutcome.PREP_OK;	
	_checkedAction = new CheckedAction();
	
	_childThreads = null;
	_childActions = new BasicList();
	
	BasicAction.allActions.add(this);
    }

    /*
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
		    System.err.println("Action nesting error - deletion of action id "+get_uid()+" invoked while child actions active");
	
		    while ((currentAct != this) && (currentAct != null))
		    {
			System.err.println("Aborting child "+currentAct.get_uid());
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
	
	    System.err.println("Destructor of still running action id "+get_uid()+" invoked - Aborting");

	    
	    /* This will also kill any children */
	
	    Abort();
	}

	pendingList = null;
	preparedList = null;
	readonlyList = null;
	failedList = null;
	heuristicList = null;
	
	currentStore = null;
	currentHierarchy = null;

	_checkedAction = null;
	
	_childThreads = null;
	_childActions = null;

	BasicAction.allActions.remove(this);
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
	case PrepareOutcome.HEURISTIC_HAZARD:
	    if ((p != PrepareOutcome.PREP_OK) && (p != PrepareOutcome.FINISH_OK))   // first heuristic outcome.
		heuristicDecision = p;
	    break;
	case PrepareOutcome.HEURISTIC_COMMIT:
	    if ((p == PrepareOutcome.HEURISTIC_ROLLBACK) || (p == PrepareOutcome.HEURISTIC_MIXED))
		heuristicDecision = PrepareOutcome.HEURISTIC_MIXED;
	    break;
	case PrepareOutcome.HEURISTIC_ROLLBACK:
	    if ((p == PrepareOutcome.HEURISTIC_COMMIT) || (p == PrepareOutcome.HEURISTIC_MIXED))
		heuristicDecision = PrepareOutcome.HEURISTIC_MIXED;
	    break;
	case PrepareOutcome.HEURISTIC_MIXED:
	    break;
	default:
	    heuristicDecision = p; // anything!
	    break;
	}
    }    

public final synchronized ActionHierarchy getHierarchy ()
    {
	return currentHierarchy;
    }

public final synchronized boolean preventCommit ()
    {
	if (actionStatus == ActionStatus.RUNNING)
	    actionStatus = ActionStatus.ABORT_ONLY;

	return (actionStatus == ActionStatus.ABORT_ONLY);
    }

public final int activeThreads ()
    {
	if (_childThreads != null)
	    return _childThreads.size();
	else
	    return 0;
    }

    /*
     * Add a record to the atomic action.
     * This function returns ADDED if the record is added. REJECTED
     * if the record cannot be added because the action is past the
     * prepare phase, and IGNORED otherwise
     */

public final synchronized int add (AbstractRecord A)
    {
	int result = BasicAction.REJECTED;

	criticalStart();
	
	if ((actionStatus <= ActionStatus.ABORTING) &&
	    ((recordBeingHandled == null) || 
	     !(recordBeingHandled.equals(A))))
	{
	    result = (pendingList.insert(A) ? BasicAction.ADDED : BasicAction.DUPLICATE );
	}

	criticalEnd();
	
	return result;
    }

public final synchronized int hierarchyDepth ()
    {
	if (currentHierarchy != null)
	    return currentHierarchy.depth();
	else
	    return 0;			/* should never happen */
    }

    /* 
     * boolean function that checks whether the Uid passed as an
     * argument is the Uid for an ancestor of the current
     * atomic action.
     */

public final boolean isAncestor (Uid ancestor)
    {
	boolean res = false;

	if (get_uid().equals(ancestor))	/* actions are their own ancestors */
	    res = true;
	else
	{
	    if ((parentAction != null) && (actionType != BasicAction.TOP_LEVEL))
		res = parentAction.isAncestor(ancestor);
	}
	
	return res;
    }

    /*
     * Return a pointer to the parent BasicAction
     */

public final BasicAction parent ()
    {
	if (actionType == BasicAction.NESTED)
	    return parentAction;
	else
	    return null;
    }

    /*
     * Return the status of the BasicAction
     */

public final synchronized int status ()
    {
	return actionStatus;
    }

    /*
     * Set up an object store and assign it to the currentStore variable.
     */

public final synchronized ObjectStore store ()
    {
	if (currentStore == null)
	{
	    /*
	     * Check for action store once per application. The second
	     * parameter is the default value, which is returned if no
	     * other value is specified.
	     */
	    
	    if (actionStoreType == null)
		actionStoreType = new ClassName(System.getProperty(JavaArjunaLiteNames.BasicAction_actionStore(), JavaArjunaLiteNames.Implementation_ObjectStore_defaultActionStore().stringForm()));
	    
	    currentStore = new ObjectStore(actionStoreType);
	}

	return currentStore;
    }

    /*
     * The following function returns the Uid of the top-level atomic
     * action.
     */

public final Uid topLevelActionUid ()
    {
	BasicAction root = this;

	while (root.parent() != null)
	    root = root.parent();

	return root.get_uid();
    }

public final BasicAction topLevelAction ()
    {
	BasicAction root = this;

	while (root.parent() != null)
	    root = root.parent();

	return root;
    }

    /*
     * Overloaded version of activate -- sets up the store, performs read_state
     * followed by restore_state
     */

public synchronized boolean activate ()
    {
	return activate(null);
    }
    
public synchronized boolean activate (String root)
    {
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
		System.err.println("Activate of atomic action with id = "+get_uid()+" and type '"+type()+"' unexpectedly failed");
		restored = false;
	    }
	
	    return restored;
	}
	catch (ObjectStoreException e)
	{
	    System.err.println(e.getMessage());
	    return false;
	}
    }

public final synchronized boolean addChildThread () // current thread
    {
	return addChildThread(Thread.currentThread());
    }

public final synchronized boolean addChildThread (Thread t)
    {
	boolean result = false;

	criticalStart();
	
	if (actionStatus < ActionStatus.ABORTING)
	{
	    if (_childThreads == null)
		_childThreads = new BasicList();

	    _childThreads.insert(t); // use insert so we don't get duplicates
	    
	    result = true;
	}

	criticalEnd();

	return result;
    }

    /*
     * Remove a child thread. Can be done at any time (Is this correct?)
     */

public final synchronized boolean removeChildThread () // current thread
    {
	return removeChildThread(Thread.currentThread());
    }

public final synchronized boolean removeChildThread (Thread t)
    {
	boolean result = false;

	criticalStart();

	if (_childThreads != null)
	{
	    _childThreads.remove(t);
	    result = true;
	}

	criticalEnd();

	return result;
    }
    
    /*
     * Add a new child action to the atomic action.
     */

public final synchronized boolean addChildAction (BasicAction act)
    {
	boolean result = false;

	criticalStart();
	
	if (actionStatus < ActionStatus.ABORTING)
	{
	    _childActions.add(act);
	    result = true;
	}

	criticalEnd();

	return result;
    }

    /*
     * Remove a child action.
     */

public final synchronized boolean removeChildAction (BasicAction act)
    {
	boolean result = false;

	criticalStart();
	
	_childActions.remove(act);
	result = true;

	criticalEnd();

	return result;
    }
    
public final synchronized CheckedAction setCheckedAction (CheckedAction c)
    {
	criticalStart();
	
	CheckedAction toReturn = _checkedAction;

	_checkedAction = c;

	criticalEnd();
	
	return toReturn;
    }
    
public Uid getSavingUid ()
    {
	return get_uid();
    }

public String toString ()
    {
	return new String("BasicAction: "+get_uid()+" status: "+actionStatus);
    }
    
    /*
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
     * ServerAtomicAction objects in the object store and attempts to activate
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
     */

public boolean save_state (OutputObjectState os, int ot)
    {
	/*
	 * This routine is called:
	 * a) After a successful prepare - to save the intentions list
	 * b) After a failure during phase 2 of commit - to overwrite the
	 *    intentions list by the failedList
	 * c) By the save_state operation of ServerAtomicAction
	 */

	RecordList listToSave;
	boolean res = true;

	/*
	 * If we have a failedList then we are re-writing an BasicAction
	 * object after a failure during phase 2 commit
	 */

	if (failedList.size() > 0)
	{
	    listToSave = failedList;
	}
	else
	{
	    listToSave = preparedList;
	}

	AbstractRecord first = listToSave.getFront();
	AbstractRecord temp = first;

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
	     * PERSISTENCE records are saved so that the state modifications
	     * of objects in a server atomic action may be committed or aborted
	     * by the crash recovery mechanism.
	     *
	     * RPCCALL records are saved so that an BasicAction that only 
	     * manipulates remote objects will have a record in the object
	     * store that crash recovery can examine when deciding whether a
	     * ServerAtomicAction should be committed or aborted.
	     */

	    if (temp.doSave())
	    {
		res = true;

		try
		{
		    /*
		     * We pack the type and the classname. This
		     * may be too much information.
		     */

		    os.packInt(temp.typeIs());

		    temp.className().pack(os);
		    
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

	if (res && os.notempty())
	{
	    try
	    {
		os.packInt(RecordType.NONE_RECORD);
	    }
	    catch (IOException e)
	    {
		res = false;
	    }
	}
	
	if (res)
	{
	    /*
	     * Now deal with anything on the heuristic list!
	     */

	    try
	    {
		os.packInt(heuristicList.size());
	    }
	    catch (IOException e)
	    {
		res = false;
	    }

	    if (res && (heuristicList.size() > 0))
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
			    os.packInt(temp.typeIs());
			    temp.className().pack(os);
			    
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
	    }

	    if (res && os.notempty())
	    {
		try
		{
		    os.packInt(RecordType.NONE_RECORD);
		}
		catch (IOException e)
		{
		    res = false;
		}
	    }
	}

	if (res && os.notempty())
	{
	    try
	    {
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

public boolean restore_state (InputObjectState os, int ot)
    {
	boolean res = true;
	int record_type = RecordType.NONE_RECORD;
	int tempActionStatus = ActionStatus.INVALID;
	int tempActionType = BasicAction.TOP_LEVEL;
	int tempHeuristicDecision = PrepareOutcome.PREP_OK;

	/*
	 * This routine is called:
	 * a) by the AAStatus module of the crash recovery system for recovery
	 *    of local AAs
	 * b) from restore_state of ServerAtomicAction which also invoked
	 *    by the crash recovery system
	 */

	/*
	 * Now unpack the prepared list.
	 * Note: This may either be a full intentions list or just the
	 * failedList, either way, restore it as the prepared list.
	 */

	try
	{
	    record_type = os.unpackInt();
	}
	catch (IOException e)
	{
	    res = false;
	}
    
	while ((res) && (record_type != RecordType.NONE_RECORD))
	{
	    ClassName cName = new ClassName();

	    try
	    {
		cName.unpack(os);
	    }
	    catch (IOException e)
	    {
		System.err.println("BasicAction.restore_state - error unpacking "+record_type);
		res = false;
	    }
	    
	    if (res)
	    {
		RecoveryAbstractRecord record = new RecoveryAbstractRecord(cName);

		res = (record.restore_state(os, ot) &&
		       preparedList.insert(record));
	    }

	    /* Tidy up before we leave. */

	    cName = null;

	    if (res)
	    {
		try
		{
		    record_type = os.unpackInt();
		}
		catch (IOException e)
		{
		    res = false;
		}
	    }
	}

	/*
	 * Now deal with the heuristic list!
	 */

	int hSize = 0;

	if (res)
	{
	    try
	    {
		hSize = os.unpackInt();
	    }
	    catch (IOException e)
	    {
		res = false;
	    }
	}

	if (hSize > 0)
	{
	    while ((res) && (record_type != RecordType.NONE_RECORD))
	    {
		ClassName cName = new ClassName();

		try
		{
		    cName.unpack(os);
		}
		catch (IOException e)
		{
		    System.err.println("BasicAction.restore_state - error unpacking "+record_type);
		    res = false;
		}
	    
		if (res)
		{
		    RecoveryAbstractRecord record = new RecoveryAbstractRecord(cName);

		    res = (record.restore_state(os, ot) &&
			   heuristicList.insert(record));
		}

		/* Tidy up before we leave. */

		cName = null;

		if (res)
		{
		    try
		    {
			record_type = os.unpackInt();
		    }
		    catch (IOException e)
		    {
			res = false;
		    }
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
		System.err.println("BasicAction.restore_state - error unpacking action status.");
		res = false;
	    }
	}

	if (res)
	{
	    actionStatus = tempActionStatus;
	    actionType = tempActionType;
	    heuristicDecision = tempHeuristicDecision;
	    savedIntentionList = true;
	}

	return res;
    }

public String type ()
    {
	return "/StateManager/BasicAction";
    }

public static BasicAction Current ()
    {
	return ThreadActionData.currentAction();
    }

public static HashUidList allActions = new HashUidList(29);
    
    /*
     * Atomic action Begin operation
     */

protected synchronized int Begin (BasicAction parentAct)
    {
	if (actionStatus != ActionStatus.CREATED)
	{
	    System.err.println("BasicAction.Begin of action "+get_uid()+" ignored - incorrect invocation sequence");
	}
	else
	{
	    actionInitialise(parentAct);
	    actionStatus = ActionStatus.RUNNING;

	    if ((actionType != BasicAction.TOP_LEVEL) &&
		((parentAct == null) || (parentAct.status() > ActionStatus.RUNNING)))
	    {
		actionStatus = ActionStatus.ABORT_ONLY;

		if (parentAct == null)
		    System.err.println("BasicAction.Begin of action "+get_uid()+" ignored - no parent and set as nested action!");
		else
		    System.err.println("BasicAction.Begin of action "+get_uid()+" ignored - parent action "+parentAct.get_uid()+" is not running: "+parentAct.status());
	    }
	}
	
	return actionStatus;
    }

    /* 
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
     * in this case no intention list has been written
     */

protected synchronized int End (boolean reportHeuristics)
    {
	/* Check for superfluous invocation */

	if ((actionStatus != ActionStatus.RUNNING) && (actionStatus != ActionStatus.ABORT_ONLY))
	{
	    switch (actionStatus)
	    {
	    case ActionStatus.CREATED:
		System.err.print("End called on non-running atomic action ");
		break;
	    case ActionStatus.COMMITTED:
		System.err.print("End called on already committed atomic action ");
		break;
	    default:
		System.err.print("End called illegally on atomic action ");
		break;
	    }

	    System.err.println(get_uid());

	    return actionStatus;
	}

	/*
	 * Check we are the current action. Abort parents if not true.
	 * Check we have not children (threads or actions).	 
	 */
	
	if (!checkIsCurrent() || checkChildren(true))
	{
	    return Abort();
	}

	if (prepare(reportHeuristics) == PrepareOutcome.PREP_NOTOK)
	{
	    System.err.print("BasicAction.End() - prepare phase of action-id "+get_uid()+" failed. ");

	    if (heuristicDecision != PrepareOutcome.PREP_OK)
	    {
		System.err.print("Received heuristic: ");
		PrepareOutcome.print(System.err, heuristicDecision);
		System.err.print(". ");
	    }
	    
	    System.err.println("Action aborting.");

	    phase2Abort(reportHeuristics);		/* first phase failed */
	}
	else
	{
	    phase2Commit(reportHeuristics);		/* first phase succeeded */
	}

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
    
    /*
     * This is the user callable abort operation. It is invoked prior to the
     * start of two-phase commit and hence only processes records in the
     * pendingList (the other lists should be empty).
     */

protected synchronized int Abort ()
    {
	/* Check for superfluous invocation */

	if ((actionStatus != ActionStatus.RUNNING) && (actionStatus != ActionStatus.ABORT_ONLY))
	{
	    switch (actionStatus)
	    {
	    case ActionStatus.CREATED:
		System.err.print("Abort called on non-running atomic action ");
		break;
	    case ActionStatus.ABORTED:
		System.err.print("Abort called on already aborted atomic action ");
		break;
	    default:
		System.err.print("Abort called illegally on atomic action ");
		break;
	    }

	    System.err.println(get_uid());

	    return actionStatus;
	}
	
	/*
	 * Check we are the current action. Abort parents if not true.
	 */

	checkIsCurrent();
	
	/*
	 * Check we have not children (threads or actions).	 
	 */
    	
	checkChildren(false);

	actionStatus = ActionStatus.ABORTING;

	while (pendingList.size() > 0)
	    doAbort(pendingList, false);  // turn off heuristics reporting

	/*
	 * In case we get here because an End has failed. In this
	 * case we still need to tell the heuristic resources to
	 * forget their decision.
	 */
    
	if (heuristicList.size() > 0)
	    doForget(heuristicList);
    
	actionStatus = ActionStatus.ABORTED;

	return actionStatus;
    }
    
protected BasicAction (int at)
    {
	super(ObjectType.NEITHER);

	currentHierarchy = null;
	currentStore = null;
	savedIntentionList = false;

	pendingList = new RecordList();
	preparedList = new RecordList();
	readonlyList = new RecordList();
	failedList = new RecordList();
	heuristicList = new RecordList();	

	actionStatus = ActionStatus.CREATED;
	actionType = at;
	parentAction = null;
	recordBeingHandled = null;

	heuristicDecision = PrepareOutcome.PREP_OK;	
	_checkedAction = new CheckedAction();
	
	_childThreads = null;
	_childActions = new BasicList();

	BasicAction.allActions.add(this);	
    }

protected BasicAction (Uid u, int at)
    {
	super(u, ObjectType.NEITHER);

	currentHierarchy = null;
	currentStore = null;
	savedIntentionList = false;

	pendingList = new RecordList();
	preparedList = new RecordList();
	readonlyList = new RecordList();
	failedList = new RecordList();
	heuristicList = new RecordList();	

	actionStatus = ActionStatus.CREATED;
	actionType = at;
	parentAction = null;
	recordBeingHandled = null;

	heuristicDecision = PrepareOutcome.PREP_OK;	
	_checkedAction = new CheckedAction();
	
	_childThreads = null;
	_childActions = new BasicList();

	BasicAction.allActions.add(this);	
    }
    
protected final synchronized void criticalEnd ()
    {
    }

    /*
     * The following function defines the start of a critical region
     * by setting the critical flag. If the signal handler is
     * called the class variable abortAndExit is set. The value of
     * this variable is checked in the corresponding operation to
     * end the critical region.
     */

protected final synchronized void criticalStart ()
    {
    }

    /*
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

protected final void phase2Cleanup ()
    {
	criticalStart();

	actionStatus = ActionStatus.CLEANUP;
    
	while ((pendingList.size() > 0) ||
	       (readonlyList.size() > 0) ||
	       (preparedList.size() > 0))
	{
	    doCleanup(preparedList);
	    doCleanup(readonlyList);
	    doCleanup(pendingList);
	}

	criticalEnd();
    }

    /*
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

protected final void phase2Commit (boolean reportHeuristics)
    {
	if (pendingList.size() > 0)
	    System.err.println("BasicAction - non-empty pendingList at phase2Commit");

	criticalStart();

	actionStatus = ActionStatus.COMMITTING;

	/*
	 * If we get a heuristic during commit then we continue to commit since
	 * we may have already told some records to commit. We could optimise this
	 * if the first record raises the heuristic by aborting (or going with the
	 * heuristic decision).
	 */
    
	doCommit(preparedList, reportHeuristics);		/* process the preparedList */

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
		if ((actionType == BasicAction.NESTED) && (recordBeingHandled.propagateOnCommit()))
		{
		    merge(recordBeingHandled);
		}
		else
		{
		    recordBeingHandled = null;
		}
	    }
	}

	if (heuristicList.size() > 0)
	    doForget(heuristicList);
	
	actionStatus = ActionStatus.COMMITTED;
	
	updateState();
    
	criticalEnd();
    }

    /*
     * Second phase of the two phase commit protocol for aborting actions.
     * Actions are aborted by invoking the doAbort operation on the
     * preparedList, the readonlyList, and the pendingList.
     *
     * The pendingList is processed because it may not be empty - since
     * prepare() stops processing the list at the first PREP_NOTOK result.
     *
     * Then read_only list is processed to ensure that actions are aborted
     * immediately and any servers killed at that point since they need not
     * hang around. This contrasts with commit where readonlyList entries 
     * are simply merged with the parent list or discarded
     */

protected final void phase2Abort (boolean reportHeuristics)
    {
	criticalStart();

	actionStatus = ActionStatus.ABORTING;
    
	while ((pendingList.size() > 0) ||
	       (readonlyList.size() > 0) ||
	       (preparedList.size() > 0))
	{
	    doAbort(preparedList, reportHeuristics);
	    doAbort(readonlyList, reportHeuristics);
	    doAbort(pendingList, reportHeuristics);
	}

	/*
	 * Check heuristic decision, and try to make it match
	 * outcome.
	 */

	if (heuristicDecision != PrepareOutcome.PREP_OK)
	{
	    if (heuristicDecision == PrepareOutcome.HEURISTIC_ROLLBACK)
		heuristicDecision = PrepareOutcome.FINISH_OK;
	}

	if (heuristicList.size() > 0)
	    doForget(heuristicList);
    
	actionStatus = ActionStatus.ABORTED;

	criticalEnd();
    }

    /*
     * Phase one of a two phase commit protocol.
     * This function returns the ouctome of the prepare operation.
     * If all goes well it will be PREP_OK, if not PREP_NOTOK.
     * The value READONLY may also be returned if all the records indicate
     * that they are readonly records. Such records do not take part in the
     * second phase commit processing.
     */

protected final int prepare (boolean reportHeuristics)
    {
	boolean commitAllowed = (actionStatus != ActionStatus.ABORT_ONLY);

	actionStatus = ActionStatus.PREPARING;
	
	/* If we cannot commit - say the prepare failed */

	if (!commitAllowed)
	{
	    actionStatus = ActionStatus.PREPARED;
	    return PrepareOutcome.PREP_NOTOK;
	}
	
	/*
	 * Make sure the object store is set up for a top-level atomic action.
	 */

	if (actionType == BasicAction.TOP_LEVEL)
	{
	    if (store() == null)
		return PrepareOutcome.PREP_NOTOK;
	}

	criticalStart();

	/*
	 * Here is the start of the hard work. Walk down the pendingList
	 * invoking the appropriate prepare operation. If it succeeds 
	 * put the record on either the preparedList or the read_only
	 * list and continue until the pendingList is exhausted..
	 *
	 * If prepare fails on any record stop processing immediately
	 * and put the offending record back on the pendingList
	 */

	int p = PrepareOutcome.PREP_NOTOK;
	boolean stateToSave = false;
	
	while (((recordBeingHandled = pendingList.getFront()) != null))
	{
	    p = ((actionType == BasicAction.TOP_LEVEL)
		 ? recordBeingHandled.topLevelPrepare()
		 : recordBeingHandled.nestedPrepare());

	    if (p == PrepareOutcome.PREP_OK)
	    {
		if (!preparedList.insert(recordBeingHandled))
		    recordBeingHandled = null;
		else
		{
		    if (!stateToSave)
			stateToSave = recordBeingHandled.doSave();
		}
	    }
	    else
	    {
		if (p == PrepareOutcome.READONLY)
		{
		    if (!readonlyList.insert(recordBeingHandled))
			recordBeingHandled = null;
		}
		else
		{
		    if ((p == PrepareOutcome.PREP_NOTOK) || (!reportHeuristics))
		    {
			/*
			 * Prepare on this record failed - we are in trouble.
			 * Add the record back onto the pendingList and return
			 */

			pendingList.putFront(recordBeingHandled);
			recordBeingHandled = null;
		
			actionStatus = ActionStatus.PREPARED;
		    
			criticalEnd();

			return PrepareOutcome.PREP_NOTOK;
		    }
		    else
		    {
			/*
			 * Heuristic decision!!
			 */

			System.err.print("Prepare phase of action "+get_uid()+" received heuristic decision: ");
			PrepareOutcome.print(System.err, p);
			System.err.println();

			updateHeuristic(p, false);

			/*
			 * Don't add to the prepared list. We process heuristics separately during
			 * phase 2. The processing of records will not be in the same order as
			 * during phase 1, but does this matter for heuristic decisions? If so,
			 * then we need to modify RecordList so that records can appear on multiple
			 * lists at the same time.
			 */
		    
			if (!heuristicList.insert(recordBeingHandled))
			    recordBeingHandled = null;
			else
			{
			    if (!stateToSave)
				stateToSave = recordBeingHandled.doSave();
			}

			if (heuristicDecision != PrepareOutcome.HEURISTIC_COMMIT)
			{
			    actionStatus = ActionStatus.PREPARED;
		    
			    criticalEnd();
		    
			    return PrepareOutcome.PREP_NOTOK;
			}		
		    }
		}
	    }
	}

	actionStatus = ActionStatus.PREPARED;

	/* 
	 * If we are here then everything went okay so save the intention list
	 * in the ObjectStore in case of a node crash providing that its not
	 * empty
	 */

	if ((actionType == BasicAction.TOP_LEVEL) && (stateToSave) &&
	    ((preparedList.size() > 0) || (heuristicList.size() > 0)))
	{
	    /* Only do this if we have some records worth saving! */
	
	    Uid u = getSavingUid();
	    String tn = type();
	    OutputObjectState state = new OutputObjectState(u, tn);

	    if (!save_state(state, ObjectType.ANDPERSISTENT))
	    {
		criticalEnd();
		
		return PrepareOutcome.PREP_NOTOK;
	    }

	    if (state.notempty())
	    {
		try
		{
		    if (!currentStore.write_committed(u, tn, state))
		    {
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
	    return PrepareOutcome.READONLY;
	else
	    return PrepareOutcome.PREP_OK;
    }

protected synchronized final int getHeuristicDecision ()
    {
	return heuristicDecision;
    }
    
private final boolean checkIsCurrent ()
    {
	boolean isCurrent = true;
	BasicAction currentAct = BasicAction.Current();
	
	/* Ensure I am the currently active action */
	
	if ((currentAct != null) && (currentAct != this))
	{
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

	if (_childThreads != null)
	{
	    if (_childThreads.size() > 0)
	    {
		if ((_childThreads.size() != 1) ||
		    ((_childThreads.size() == 1) && (!_childThreads.contains(Thread.currentThread()))))
		{
		    /* More than one thread or the one thread is not the current thread */

		    if (isCommit)
			System.err.print("Commit of action id ");
		    else
			System.err.print("Abort of action id ");

		    System.err.println(get_uid() + " invoked while multiple threads active within it.");
		    
		    if (_checkedAction != null)
			_checkedAction.check(isCommit, get_uid(), _childThreads);

		    removeAllChildThreads();
		}
	    }
	}

	/* Ensure I have no child actions */
	
	if (_childActions.size() > 0)
	{
	    problem = true;

	    BasicListIterator iter = new BasicListIterator(_childActions);
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
	
	    while ((child = (BasicAction) iter.next()) != null)
	    {
		if (child.status() != ActionStatus.ABORTED)
		{
		    if (printError)
		    {
			if (isCommit)
			    System.err.print("Commit of action id ");
			else
			    System.err.print("Abort of action id ");
	
			System.err.println(get_uid()+" invoked while child actions active");

			printError = false;
		    }

		    System.err.println("\tAborting child: "+child.get_uid());

		    havePrinted = true;

		    child.Abort();
		}
	    }

	    if (isCommit)
	    {
		System.err.println("\tNow aborting self: "+get_uid());
		havePrinted = true;
	    }

	    if (havePrinted)
	    {
		System.err.println();
	    }
	}

	return problem;
    }
    
/*
 * Just in case we are deleted/terminated with threads still
 * registered. We must make sure those threads don't
 * try to remove themselves from this action later, since
 * that will involve traversing a dangling pointer!
 * So we unregister them ourselves now.
 *
 * This is only called by End/Abort and so all threads and
 * child actions will have been previously terminated as well.
 */

private final void removeAllChildThreads ()
    {
	/*
	 * Do not remove the current thread as it is
	 * committing/aborting!
	 */
	
	criticalStart();
    
	if (_childThreads.size() != 0)
	{
	    Thread currentThread = Thread.currentThread();
	    
	    /*
	     * Iterate through all registered threads and
	     * tell them to ignore the action pointer, i.e.,
	     * they are now no longer within this action.
	     */
	 
	    BasicListIterator iter = new BasicListIterator(_childThreads);
	    Thread t = null;

	    while ((t = (Thread) iter.next()) != null)
	    {
		if (t != currentThread)
		{
		    removeChildThread(t);
		
		    ThreadActionData.purgeAction(this, t);
		}
	    }
	}

	criticalEnd();
    }
    
private final void updateState ()
    {
	/*
	 * If the action is topLevel then prepare() will have written the
	 * intention_list to the object_store.
	 * If any of the phase2Commit processing failed then records will
	 * exist on the failedList. If this is the case then we need to
	 * re-write the BasicAction record in the object store. If the
	 * failed list is empty we can simply delete the BasicAction
	 * record.
	 */

	if (actionType == BasicAction.TOP_LEVEL)
	{
	    /*
	     * make sure the object store is set up for a top-level atomic
	     * action.
	     */

	    store();
	
	    if ((failedList.size() > 0) || (heuristicList.size() > 0))
	    {
		/*
		 * Re-write the BasicAction record with the failed list
		 */

		Uid u = getSavingUid();
		String tn = type();
		OutputObjectState state = new OutputObjectState(u, tn);
	    
		if (!save_state(state, ObjectType.ANDPERSISTENT))
		{
		    System.err.println("BasicAction.End - Could not create ObjectState for failedList");
		}

		if (state.notempty())
		{
		    try
		    {
			if (!currentStore.write_committed(u, tn, state))
			{
			    System.err.println("BasicAction.End - Could not write failed list");
			}
		    }
		    catch (ObjectStoreException e)
		    {
			System.err.println(e.getMessage());
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
	criticalStart();

	if (parent != null)		/* ie not top_level */
	{
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
	case BasicAction.TOP_LEVEL:
	    if (parent != null)
	    {
		/*
		 * do not want to print warning all the time as this is what
		 * nested top-level actions are used for.
		 */
	    }
	    break;
	case BasicAction.NESTED :
	    if (parent == null)
		actionType = BasicAction.TOP_LEVEL;
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
	if (list_toprocess.size() > 0)
	{
	    int ok = PrepareOutcome.FINISH_OK;
	
	    while (((recordBeingHandled = list_toprocess.getFront()) != null))
	    {   
		if (actionType == BasicAction.TOP_LEVEL)
		    ok = recordBeingHandled.topLevelAbort();
		else
		    ok = recordBeingHandled.nestedAbort();

		if ((actionType != BasicAction.TOP_LEVEL) && (recordBeingHandled.propagateOnAbort()))
		    merge(recordBeingHandled);
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
			    System.err.println("Abort of action "+get_uid()+" received heuristic decision: "+ok);
		    
			    updateHeuristic(ok, false);
			    heuristicList.insert(recordBeingHandled);
			}
			else
			{
			    if (ok != PrepareOutcome.FINISH_OK)
			    {
				/* Preserve error messages */			    
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
	if (list_toprocess.size() > 0)
	{
	    while (((recordBeingHandled = list_toprocess.getFront())) != null)
	    {
		recordBeingHandled.forgetHeuristic();
		recordBeingHandled = null;
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
     */

private final void doCleanup (RecordList list_toprocess)
    {
	if (list_toprocess.size() > 0)
	{
	    int ok = PrepareOutcome.FINISH_OK;
	
	    while (((recordBeingHandled = list_toprocess.getFront()) != null))
	    {
		if (actionType == BasicAction.TOP_LEVEL)
		    ok = recordBeingHandled.topLevelCleanup();
		else
		    ok = recordBeingHandled.nestedAbort();

		if ((actionType != BasicAction.TOP_LEVEL) &&
		    (recordBeingHandled.propagateOnAbort()))
		    merge(recordBeingHandled);
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

    /*
     * Walk down a record list extracting records and calling the appropriate
     * commit function. Discard or merge records as appropriate
     */

private final void doCommit (RecordList rl, boolean reportHeuristics)
    {
	/*
	 * To get heuristics right, as soon as we manage to commit the
	 * first record we set the heuristic to HEURISTIC_COMMIT. Then, if
	 * any other heuristics are raised we can manage the final outcome
	 * correctly.
	 */
	
	if (rl.size() > 0)
	{
	    int ok = PrepareOutcome.FINISH_OK;
	
	    while (((recordBeingHandled = rl.getFront()) != null))
	    {
		if (actionType == BasicAction.TOP_LEVEL)
		{
		    if ((ok = recordBeingHandled.topLevelCommit()) == PrepareOutcome.FINISH_OK)
		    {
			/*
			 * Record successfully committed, we can delete it now
			 */

			recordBeingHandled = null;

			updateHeuristic(PrepareOutcome.FINISH_OK, true);// must remember that something has committed
		    }
		    else
		    {
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
				 * If this is the first resource then rollback, otherwise
				 * promote to HEURISTIC_HAZARD, but don't add to heuristicList.
				 */

				updateHeuristic(PrepareOutcome.HEURISTIC_HAZARD, true);
			    }
			    else
			    {
				/*
				 * The commit failed. Add this record to the failed
				 * list to indicate this.
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
     * BasicAction. If the add operation does not return ADDED, the record is
     * deleted
     */

private final synchronized void merge (AbstractRecord A)
    {
	int as;
    
	if ((as = parentAction.add(A)) != BasicAction.ADDED)
	{
	    A = null;
	    if (as == BasicAction.REJECTED)
		System.err.println("(internal) BasicAction.merge - record rejected");
	}
    }
    
private ActionHierarchy currentHierarchy;
private ObjectStore currentStore;

private boolean savedIntentionList;

    /* These (genuine) lists hold the abstract records */

private RecordList pendingList;
private RecordList preparedList;
private RecordList readonlyList;
private RecordList failedList;
private RecordList heuristicList;

    /* Atomic action status variables */

private int actionStatus;
private int actionType;
private BasicAction parentAction;    
private AbstractRecord recordBeingHandled;
private int oldMask;    // would be signal mask
private int heuristicDecision;

private CheckedAction _checkedAction;  // control what happens if threads active when terminating.
    
    /*
     * We need to keep track of the number of threads associated with
     * each action. Since we can't override the basic thread methods,
     * we have to provide an explicit means of registering threads with
     * an action.
     */

private BasicList _childThreads;
private BasicList _childActions;

private static ClassName actionStoreType = null;

};
