/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: AtomicAction.java,v 1.1.4.2.2.1.4.1.56.1 2001/08/08 14:18:52 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCore.Thread.*;
import com.arjuna.ArjunaCommon.Interface.Inventory;
import com.arjuna.ArjunaCommon.Common.*;

import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import java.lang.NumberFormatException;
import java.lang.InterruptedException;
import java.io.IOException;

/**
 * This is a user-level transaction class, unlike BasicAction.
 * AtomicAction takes care of thread-to-action scoping.
 * This is a "one-shot" object, i.e., once terminated, the
 * instance cannot be re-used for another transaction.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: AtomicAction.java,v 1.1.4.2.2.1.4.1.56.1 2001/08/08 14:18:52 nmcl Exp $
 * @since JTS 1.0.
 */

public class AtomicAction extends BasicAction
{
    
    /**
     * Create a new transaction. If there is already a transaction
     * associated with the thread then this new transaction will
     * be automatically nested. The transaction is *not* running
     * at this point.
     */

public AtomicAction ()
    {
	super();

	parentAction = null;
    }

    /**
     * AtomicAction constructor with a Uid.
     * This constructor is for recreating an AtomicAction, typically during
     * crash recovery.
     */

public AtomicAction (Uid objUid)
    {
	super(objUid);

	parentAction = null;	
    }

    /**
     * AtomicAction destructor. Under normal circumstances we do very little.
     * However there exists the possibility that this action is being deleted
     * while still running (user forgot to commit/abort) - in which case we
     * do an abort for him and mark all our parents as unable to commit.
     * Additionally due to scoping we may not be the current action - but
     * in that case the current action must be one of our nested actions
     * so by applying abort to it we should end up at ourselves!
     */

public void finalize ()
    {
	ThreadActionData.purgeAction(this);

	parentAction = null;

	super.finalize();
    }

    /**
     * Start the transaction running.
     *
     * If the transaction is already running or has terminated, then
     * an error code will be returned.
     *
     * @return <code>ActionStatus</code> indicating outcome.
     */

public synchronized int begin ()
    {
	int status = ActionStatus.INVALID;
	
	parentAction = BasicAction.Current();

	if (parentAction != null)
	    parentAction.addChildAction(this);
	
	status = super.Begin(parentAction);

	if (status == ActionStatus.RUNNING)
	{
	    /*
	     * Now do thread/action tracking.
	     */

	    ThreadActionData.pushAction(this);
	}
	
	return status;
    }

    /**
     * Commit the transaction, and have heuristic reporting. Heuristic
     * reporting via the return code is enabled.
     *
     * @return <code>ActionStatus</code> indicating outcome.
     */

public synchronized int commit ()
    {
	return commit(true);
    }

    /**
     * Commit the transaction. The report_heuristics parameter
     * can be used to determine whether or not heuristic outcomes
     * are reported.
     *
     * If the transaction has already terminated, or has not been
     * begun, then an appropriate error code will be returned.
     *
     * @return <code>ActionStatus</code> indicating outcome.
     */

public synchronized int commit (boolean report_heuristics)
    {
	int status = ActionStatus.INVALID;
	
	if (parentAction != null)
	    parentAction.removeChildAction(this);
	
	status = super.End(report_heuristics);

	if ((status == ActionStatus.COMMITTED) ||
	    (status == ActionStatus.COMMITTING) ||
	    (status == ActionStatus.ABORTED))
	{
	    /*
	     * Now remove this thread from the action state.
	     */

	    ThreadActionData.popAction();
	}
	
	return status;
    }

    /**
     * Abort (rollback) the transaction.
     *
     * If the transaction has already terminated, or has not been
     * begun, then an appropriate error code will be returned.
     *
     * @return <code>ActionStatus</code> indicating outcome.
     */

public synchronized int abort ()
    {
	int status = ActionStatus.INVALID;
	
	if (parentAction != null)
	    parentAction.removeChildAction(this);
	
	status = super.Abort();

	if (status == ActionStatus.ABORTED)
	{
	    /*
	     * Now remove this thread from the action state.
	     */

	    ThreadActionData.popAction();
	}

	return status;
    }

    /**
     * Overloads BasicAction.type()
     */

public String type ()
    {
	return "/StateManager/BasicAction/AtomicAction";
    }

    /**
     * Register the current thread with the transaction.
     *
     * @return <code>true</code> if successful, <code>false</code> otherwise.
     */

public synchronized boolean addThread ()
    {
	return addThread(Thread.currentThread());
    }

    /**
     * Register the specified thread with the transaction.
     *
     * @return <code>true</code> if successful, <code>false</code> otherwise.
     */

public synchronized boolean addThread (Thread t)
    {
	if (t != null)
	{
	    ThreadActionData.pushAction(this);
	    return true;
	}

	return false;
    }

    /**
     * Unregister the current thread from the transaction.
     *
     * @return <code>true</code> if successful, <code>false</code> otherwise.
     */

public synchronized boolean removeThread ()
    {
	return removeThread(Thread.currentThread());
    }

    /**
     * Unregister the specified thread from the transaction.
     *
     * @return <code>true</code> if successful, <code>false</code> otherwise.
     */

public synchronized boolean removeThread (Thread t)
    {
	if (t != null)
	{
	    ThreadActionData.purgeAction(this);
	    return true;
	}

	return false;
    }

    /**
     * Create a new transaction of the specified type.
     */
    
protected AtomicAction (int at)
    {
	super(at);

	parentAction = null;	
    }

private BasicAction parentAction;
    
}
