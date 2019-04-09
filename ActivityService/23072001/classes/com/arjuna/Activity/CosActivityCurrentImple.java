/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CosActivityCurrentImple.javatmpl,v 1.1 2001/07/23 13:44:50 nmcl Exp $
 */

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2.4.1.2.3.26.1.2.2.4.2 2001/02/02 11:51:52 nmcl Exp $
 */
































































package com.arjuna.Activity;

import com.arjuna.Activity.ActivityCoordinator.ActivityCoordinatorWrapper;
import com.arjuna.CosTransactions.OTS;
import com.arjuna.CosTransactions.OTS_Current;
import com.arjuna.CosTransactions.ArjunaOTS.ThreadAssociations;
import com.arjuna.Activity.ArjunaOTS.TxActivity;
import org.omg.CosTransactions.*;
import org.omg.CosActivity.*;
import org.omg.CORBA.*;
import java.util.*;

import java.util.EmptyStackException;
import java.util.NoSuchElementException;
import org.omg.CosActivity.InvalidState;
import org.omg.CosActivity.TimeoutOutOfRange;
import org.omg.CosActivity.NoActivity;
import org.omg.CosActivity.ActivityPending;
import org.omg.CosActivity.ChildContextPending;
import org.omg.CosActivity.SignalSetUnknown;
import org.omg.CosActivity.InvalidToken;
import org.omg.CosActivity.InvalidParentContext;
import org.omg.CosActivity.PropertyGroupUnknown;
import org.omg.CosActivity.AlreadyDestroyed;
import org.omg.CORBA.BAD_OPERATION;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.SystemException;

public class CosActivityCurrentImple extends  org.omg.CosActivity . CurrentPOA 
{

public void begin (int timeout) throws InvalidState, TimeoutOutOfRange, SystemException
    {
	if (timeout < -1)
	    throw new TimeoutOutOfRange();
	else
	{
	    ActivityCoordinatorWrapper currentActivity = currentActivity();
	    
	    checkStatus(currentActivity);

	    /*
	     * If we get here then the parent's status is ok to continue.
	     */

	    if (timeout == 0)  // get thread specific value
	    {
		Integer i = (Integer) _timeouts.get(Thread.currentThread());
		
		if (i == null)
		    timeout = Defaults.DEFAULT_TIMEOUT;
		else
		    timeout = i.intValue();
	    }
	    
	    ActivityCoordinatorWrapper act = new ActivityCoordinatorWrapper(currentActivity, timeout);
	    
	    pushActivity(act);
	}
    }
 
public Outcome complete () throws NoActivity, ActivityPending, ChildContextPending, SystemException
    {
	ActivityCoordinatorWrapper currentActivity = currentActivity();

	if (currentActivity == null)
	    throw new NoActivity();
	else
	{
	    Outcome theOutcome = null;
	    
	    try
	    {
		org.omg.CosActivity.CompletionStatus cs = currentActivity.get_completion_status();
		boolean completeOk = true;
		
		try
		{
		    theOutcome = currentActivity.complete();
		}
		catch (BAD_OPERATION ex1)  // should be INVALID_ACTIVITY
		{
		    popActivity();

		    try
		    {
			currentActivity.destroy();	
		    }
		    catch (AlreadyDestroyed ex)
		    {
			// could happen if multiple threads call complete?
		    }

		    throw ex1;
		}

		popActivity();
		
		if (currentActivity.get_completion_status() != cs)
		    completeOk = false;
		
		try
		{
		    currentActivity.destroy();	
	        }
		catch (AlreadyDestroyed ex)
		{
		    // could happen if multiple threads call complete?
		}

		if (!completeOk)
		    throw new BAD_OPERATION("Final Activity completion status different from required.");
		else
		    return theOutcome;
	    }
	    catch (SignalSetUnknown e1)
	    {
		throw new BAD_OPERATION(e1.toString());
	    }
	    catch (BAD_OPERATION e2)
	    {
		throw e2;
	    }
	    catch (SystemException e3)
	    {
		e3.printStackTrace();
		
		throw new UNKNOWN(e3.toString());
	    }
	}
    }
 
public Outcome complete_with_status (org.omg.CosActivity.CompletionStatus cs) throws NoActivity, ActivityPending, ChildContextPending, InvalidState, SystemException
    {
	ActivityCoordinatorWrapper currentActivity = currentActivity();
	
	if (currentActivity == null)
	    throw new NoActivity();
	else
	{
	    checkStatus(currentActivity);
	    
	    currentActivity.set_completion_status(cs);

	    Outcome theOutcome = null;
	    
	    try
	    {
		boolean completeOk = true;

		try
		{
		    theOutcome = currentActivity.complete();
		}
		catch (BAD_OPERATION ex1)  // should be INVALID_ACTIVITY
		{
		    popActivity();

		    try
		    {
			currentActivity.destroy();	
		    }
		    catch (AlreadyDestroyed ex)
		    {
			// could happen if multiple threads call complete?
		    }

		    throw ex1;
		}

		popActivity();

		if (currentActivity.get_completion_status() != cs)
		    completeOk = false;

		try
		{
		    currentActivity.destroy();	
	        }
		catch (AlreadyDestroyed ex)
		{
		    // could happen if multiple threads call complete?
		}

		if (!completeOk)
		    throw new BAD_OPERATION("Final Activity completion status different from required.");
		else
		    return theOutcome;
	    }
	    catch (SignalSetUnknown e1)
	    {
		throw new BAD_OPERATION(e1.toString());
	    }
	    catch (BAD_OPERATION e2)
	    {
		throw e2;
	    }
	    catch (SystemException e3)
	    {
		throw new UNKNOWN(e3.toString());
	    }
	}
    }

public void set_completion_status (org.omg.CosActivity.CompletionStatus cs) throws NoActivity, InvalidState, SystemException
    {
	ActivityCoordinatorWrapper act = currentActivity();
	
	if (act == null)
	    throw new NoActivity();
	else
	{
	    checkStatus(act);
	    
	    act.set_completion_status(cs);
	}
    }
 
public org.omg.CosActivity.CompletionStatus get_completion_status () throws NoActivity, SystemException
    {
	ActivityCoordinatorWrapper act = currentActivity();	

	if (act == null)
	    throw new NoActivity();
	else
	    return act.get_completion_status();
    }

public void set_completion_signal_set (String signal_set_name) throws NoActivity, SignalSetUnknown, InvalidState, SystemException
    {
	ActivityCoordinatorWrapper act = currentActivity();	

	if (act == null)
	    throw new NoActivity();
	else
	{
	    checkStatus(act);

	    act.set_completion_signal_set(signal_set_name);
	}
    }
 
public String get_completion_signal_set () throws NoActivity, SystemException
    {
	ActivityCoordinatorWrapper act = currentActivity();	

	if (act == null)
	    throw new NoActivity();
	else
	    return act.get_completion_signal_set();
    }

/*
 * Suspend only the current Activity/transaction and any enclosing contexts.
 * Since we cannot suspend specific parts of an OTS hierarchy, we cannot
 * allow suspend to we called if the Activity is nested within a
 * transaction.
 */
 
public ActivityToken suspend () throws InvalidParentContext, SystemException
    {
	ActivityCoordinatorWrapper act = currentActivity();
	
	if (act != null)
	{
	    /*
	     * If current activity is transactional and is nested within
	     * a transaction then we cannot just suspend the current
	     * activity's part of the transaction hierarchy.
	     */

	    if (act.isTransactional() && (act.getEnclosingTransaction() != null))
	    {
		throw new InvalidParentContext();
	    }
	}
	
	ActivityTokenImple token = getActivityToken(true);

	act = popActivity();
	    
	if (token != null)
	{
	    try
	    {
		/*
		 * Now suspend transactions.
		 */

		OTS.current().suspend();
	    }
	    catch (Exception e)
	    {
		throw new BAD_OPERATION(e.toString());  // what else?
	    }

 	    return token.getReference();
	}
	else
	    return null;
    }

/*
 * Resume context. Must have the same context active as was left after
 * previous suspend (i.e., parent must be the same).
 */
 
/*
 * Options are:
 *
 * (i) token is an activity only, so must check current context is same
 *     activity/transaction as for suspend.
 * (ii) token is an activity and transaction: as above.
 * (iii) token is a top-level transaction and activity/transaction: as above.
 * (iv) token is a nested transaction and activity/transaction: INVALID!!
 *      Can't do this since suspend will suspend all transactions on the
 *      thread.
 */

public void resume (ActivityToken at) throws InvalidToken, InvalidParentContext, SystemException
    {
	if (at == null)
	    throw new InvalidToken();

	ActivityTokenImple token = ActivityTokenImple.getImplementation(at);
	
	if (token == null)  // not a local ActivityToken!
	    throw new InvalidToken();
	else
	{
	    if (!token.fromSuspend())  // from suspend or suspend_all?
		throw new InvalidToken();
	    else
	    {
		/*
		 * Is the current context on the thread the right one
		 * to resume this context into?
		 */

		ActivityCoordinatorWrapper act = currentActivity();

		/*
		 * Check enclosing activity first.
		 *
		 * Is the current activity on this thread the parent of
		 * the activity we ar resuming?
		 */

		Control cont = null;

		try
		{
		    cont = OTS.current().get_control();
		}
		catch (Exception e)
		{
		    throw new BAD_OPERATION(e.toString());  // what else?
		}

		if (token.isSameParentActivity(act))
		{
		    // only need to check current transaction

		    /*
		     * Because of the way in which suspend works for the OTS,
		     * (it removes all transactions from the thread and
		     * leaves no association), we must be have an activity
		     * to have an enclosed transaction.
		     */

		    if (act != null)
		    {
			if (!token.isSameParentTransaction(cont))
			    throw new InvalidParentContext();
		    }
		    else
		    {
			if (!token.isSameTransaction(cont))
			    throw new InvalidParentContext();
		    }
		}
		else
		    throw new InvalidParentContext();
	    }

	    /*
	     * If we get here then the current context is the same as the
	     * one the token was suspended from.
	     */

	    Control currentTx = null;

	    for (int i = token.size()-1; i >= 0; i--)
	    {
		try
		{
		    ActivityContextElement e = token.getElement(i);

		    if (!e.isActivity())
			currentTx = (Control) e.getEntity();
		    else
			pushActivity((ActivityCoordinatorWrapper) e.getEntity());
		}
		catch (NoSuchElementException e1)
		{
		    throw new InvalidToken();
		}
		catch (Exception e2)
		{
		    throw new BAD_OPERATION(e2.toString());
		}
	    }

	    /*
	     * Now resume the current transaction.
	     */

	    if (currentTx != null)
	    {
		try
		{
		    OTS.current().resume(currentTx);
		}
		catch (Exception e)
		{
		    throw new BAD_OPERATION(e.toString());
		}
	    }
	}
    }

/*
 * Suspends all activities/transactions association with this
 * thread. No context must be active on the thread when we return.
 */

public ActivityToken suspend_all () throws SystemException
    {
	ActivityTokenImple token = getActivityToken(false);
	ActivityCoordinatorWrapper act = popActivity();
	
	if (act != null)
	{
	    ActivityCoordinatorWrapper parent = popActivity();
	    
	    while (parent != null)
	    {
		act = parent;
		parent = popActivity();
	    }
	}

	if (token != null)
	{
	    try
	    {
		/*
		 * Now suspend transactions.
		 */

		OTS.current().suspend();
	    }
	    catch (Exception e)
	    {
		throw new BAD_OPERATION(e.toString());  // what else?
	    }

	    return token.getReference();
	}
	else
	    return null;
    }

/*
 * Resume the entire hierarchy, which must have been obtained either from
 * a get_token or a suspend_all call. There must be no context active on
 * the thread when called.
 */
 
public void resume_all (ActivityToken at) throws InvalidToken, InvalidParentContext, SystemException
    {
	if (at == null)
	    throw new InvalidToken();

	ActivityTokenImple token = ActivityTokenImple.getImplementation(at);
	
	if (token == null)  // not a local ActivityToken!
	    throw new InvalidToken();
	else
	{
	    if (token.fromSuspend())  // from suspend or suspend_all?
		throw new InvalidToken();
	}

	/*
	 * There must be no activity or transaction currently associated
	 * with the thread.
	 */

	if (currentActivity() != null)
	    throw new InvalidParentContext();

	boolean problem = false;
	
	try
	{
	    if (OTS.current().get_status() == org.omg.CosTransactions.Status.StatusActive)
	    {
		// if here then there is a transaction!
		
		problem = true;
	    }
	}
	catch (Exception e)
	{
	    problem = true;
	}
	
	if (problem)
	    throw new InvalidParentContext();
	else
	{
	    /*
	     * resume_all differs from resume in that the token actually
	     * represents the entire hierarchy, i.e., it essentially points
	     * to the root activity (or transaction). From this we can
	     * just push every child activity until we reach the end.
	     */

	    Control currentTx = null;
	    
	    for (int i = token.size() -1; i >= 0; i--)
	    {
		try
		{
		    ActivityContextElement e = token.getElement(i);

		    if (!e.isActivity())
			currentTx = (Control) e.getEntity();
		    else
			pushActivity((ActivityCoordinatorWrapper) e.getEntity());
		}
		catch (NoSuchElementException e1)
		{
		    throw new InvalidToken();
		}
		catch (Exception e2)
		{
		    throw new BAD_OPERATION(e2.toString());
		}
	    }

	    /*
	     * Now resume the current transaction.
	     */

	    if (currentTx != null)
	    {
		try
		{
		    OTS.current().resume(currentTx);
		}
		catch (Exception e)
		{
		    throw new BAD_OPERATION(e.toString());
		}
	    }
	}
    }

public byte[] get_global_id () throws SystemException
    {
	ActivityCoordinatorWrapper aw = currentActivity();

	if (aw == null)
	    return null;
	else
	    return aw.get_global_id();
    }

public org.omg.CosActivity.Status get_status () throws SystemException
    {
	ActivityCoordinatorWrapper aw = currentActivity();
	
	if (aw == null)
	    return org.omg.CosActivity.Status.StatusNoActivity;
	else
	    return aw.get_status();
    }
 
public String get_activity_name () throws SystemException
    {
	ActivityCoordinatorWrapper act = currentActivity();

	if (act == null)
	    return null;
	else
	    return act.get_activity_name();
    }

public synchronized void set_timeout (int timeout) throws TimeoutOutOfRange, SystemException
    {
	if (timeout < -1)
	    throw new TimeoutOutOfRange();
	else
	{
	    Integer t = null;
	    
	    switch (timeout)
	    {
	    case -1:
		t = new Integer(-1);
		break;
	    case 0:
		t = new Integer(Defaults.DEFAULT_TIMEOUT);
		break;
	    default:
		t = new Integer(timeout);
		break;
	    }

	    _timeouts.put(Thread.currentThread(), t);
	}
    }
 
public synchronized int get_timeout () throws SystemException
    {
	Integer i = (Integer) _timeouts.get(Thread.currentThread());
	
	if (i == null)
	    return 0;
	else
	    return i.intValue();
    }

public ActivityContext get_context () throws SystemException
    {
	ActivityTokenImple token = getActivityToken(false);
	
	if (token != null)
	    return token.get_context();
	else
	    return null;
    }
 
public void recreate_context (ActivityContext ctx) throws InvalidContext, SystemException
    {
	if (ctx == null)
	    throw new BAD_PARAM();
	
	throw new BAD_OPERATION("Not implemented!");
    }

public ActivityCoordinator get_coordinator () throws SystemException
    {
	ActivityCoordinatorWrapper act = currentActivity();

	if (act == null)
	    return null;
	else
	    return act.get_coordinator();
    }
 
public ActivityCoordinator get_parent_coordinator () throws SystemException
    {
	ActivityCoordinatorWrapper act = currentActivity();

	if (act == null)
	    return null;
	else
	    return act.get_parent_coordinator();
    }

public ActivityIdentity get_identity () throws SystemException
    {
	ActivityCoordinatorWrapper act = currentActivity();

	if (act == null)
	{
	    /*
	     * No activity, but there may be a transaction.
	     */

	    try
	    {
		if (OTS.current().get_status() == org.omg.CosTransactions.Status.StatusActive)
		{
		    return ActivityHierarchy.getId(OTS.current().get_control());
		}
		else
		    return null;
	    }
	    catch (Exception e)
	    {
		throw new BAD_OPERATION(e.toString());
	    }
	}
	else
	    return ActivityHierarchy.getId(act);
    }

/*
 * Returns a token that can be used in a resume call.
 * Each invocation returns a different instance because it is not possible
 * to have a single instance for each activity since each instance needs
 * to know whether it can be used in a resume or resume_all call.
 *
 * In addition, the token represents the current Activity and any children
 * at the point of invocation only, i.e., a subsequent call to get_token
 * may return a different context from the 'current' one.
 */

public ActivityToken get_token () throws SystemException
    {
	ActivityTokenImple token = getActivityToken(true);
	
	if (token != null)
	    return token.getReference();
	else
	    return null;
    }

public PropertyGroup get_property_group (String name) throws PropertyGroupUnknown, NoActivity, SystemException
    {
	if (currentActivity() == null)
	    throw new NoActivity();

	throw new PropertyGroupUnknown();
    }

public ActivityCoordinatorWrapper getCoordinator () throws SystemException
    {
	return currentActivity();
    }

protected final ActivityCoordinatorWrapper currentActivity ()
    {
	Stack s = null;

	synchronized (_coordinators)
	    {
		s = (Stack) _coordinators.get(Thread.currentThread());
		
		if (s == null)
		    return null;
	    }
	
	try
	{
	    return (ActivityCoordinatorWrapper) s.peek();
	}
	catch (EmptyStackException e)
	{
	}

	return null;
    }

private final ActivityTokenImple getActivityToken (boolean resumeOnly) throws SystemException
    {
	ActivityCoordinatorWrapper act = currentActivity();
	ActivityTokenImple toReturn = null;
	
	if (act != null)
	{
	    if (!resumeOnly)
	    {
		/*
		 * Get the root of the hierarchy.
		 */

		/*
		ActivityCoordinatorWrapper parent = act.getParentCoordinator();
	    
		while (parent != null)
		{
		    act = parent;
		    parent = act.getParentCoordinator();
		}
		*/
	    }
	    
	    /*
	     * If there is an activity coordinator then it should have
	     * the necesary data for enclosed/enclosing transactions.
	     */

	    toReturn = new ActivityTokenImple(act, resumeOnly);
	}
	else
	{
	    /*
	     * No activity, but there may be a transaction.
	     */

	    Control cont = null;
	    
	    try
	    {
		if (OTS.current().get_status() == org.omg.CosTransactions.Status.StatusActive)
		{
		    cont = OTS.current().get_control();
		
		    toReturn = new ActivityTokenImple(cont, resumeOnly);
		}
	    }
	    catch (Exception e)
	    {
		throw new BAD_OPERATION(e.toString());  // what else?
	    }
	}

	return toReturn;
    }

private final void pushActivity (ActivityCoordinatorWrapper act)
    {
	Stack s = null;
	Thread ct = Thread.currentThread();

	synchronized (_coordinators)
	    {
		s = (Stack) _coordinators.get(ct);
		
		if (s == null)
		    s = new Stack();
		
		_coordinators.put(ct, s);
	    }
	
	/*
	 * No need to synchronize on the stack since it is thread specific.
	 */

	s.push(act);

	// Add thread information to the activity

	if ((act != null) && (act.isLocal()))
	    act.addThread(ct);
    }

private final ActivityCoordinatorWrapper popActivity ()
    {
	Stack s = null;
	Thread ct = Thread.currentThread();
	ActivityCoordinatorWrapper act = null;
	
	synchronized (_coordinators)
	    {
		s = (Stack) _coordinators.get(ct);
	    }
	
	if (s == null)
	    return null;
	
	/*
	 * No need to synchronize on the stack since it is thread specific.
	 */

	try
	{
	    act = (ActivityCoordinatorWrapper) s.pop();
	}
	catch (EmptyStackException e)
	{
	    act = null;
	}

	if ((act != null) && (act.isLocal()))
	    act.removeThread(ct);

	return act;
    }

private final void checkStatus (ActivityCoordinatorWrapper act) throws InvalidState, SystemException
    {
	if (act != null)
	{
	    org.omg.CosActivity.CompletionStatus cs = act.get_completion_status();
	    
	    if (cs == org.omg.CosActivity.CompletionStatus.CompletionStatusFailOnly)
		throw new InvalidState();
	    else
	    {
		try
		{
		    if (act.get_status() != org.omg.CosActivity.Status.StatusActive)
			throw new BAD_OPERATION();  // should be INVALID_ACTIVITY
		}
		catch (Exception e)
		{
		    throw new UNKNOWN();
		}
	    }
	}
    }
 
private static Hashtable _timeouts = new Hashtable();
private static Hashtable _coordinators = new Hashtable();

	static
	{
	    ThreadAssociations.addGlobal(new TxActivity());
        }
 
};
