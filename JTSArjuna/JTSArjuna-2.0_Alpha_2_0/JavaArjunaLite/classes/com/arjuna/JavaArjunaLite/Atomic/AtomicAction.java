/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AtomicAction.java,v 1.13 1998/08/13 14:31:33 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Atomic;

import com.arjuna.JavaArjunaLite.Interface.ObjectStore;
import com.arjuna.JavaArjunaLite.Thread.*;
import com.arjuna.JavaGandiva.Interface.Inventory;
import com.arjuna.JavaGandiva.Common.*;

import com.arjuna.JavaArjunaLite.Common.ObjectStoreException;
import java.lang.NumberFormatException;
import java.lang.InterruptedException;
import java.io.IOException;

/*
 * AtomicAction which takes care of thread-to-action scoping.
 */

public class AtomicAction extends BasicAction
{
    
public AtomicAction ()
    {
	super();

	parentAction = null;
    }

    /*
     * AtomicAction constructor with a Uid.
     * This constructor is for recreating an AtomicAction, typically during
     * crash recovery.
     */

public AtomicAction (Uid objUid)
    {
	super(objUid);

	parentAction = null;	
    }

    /*
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
	super.finalize();
    }

public synchronized int begin ()
    {
	int status = ActionStatus.INVALID;
	
	parentAction = BasicAction.Current();

	if (parentAction != null)
	    parentAction.addChildAction(this);
	
	status = super.Begin(parentAction);

	/*
	 * Now do thread/action tracking.
	 */

	ThreadActionData.pushAction(this);
	
	return status;
    }

public synchronized int commit ()
    {
	int status = ActionStatus.INVALID;
	
	if (parentAction != null)
	    parentAction.removeChildAction(this);
	
	status = super.End(false);

	/*
	 * Now remove this thread from the action state.
	 */

	ThreadActionData.popAction();
	
	return status;
    }
    
public synchronized int abort ()
    {
	int status = ActionStatus.INVALID;
	
	if (parentAction != null)
	    parentAction.removeChildAction(this);
	
	status = super.Abort();

	/*
	 * Now remove this thread from the action state.
	 */

	ThreadActionData.popAction();

	return status;
    }

public String type ()
    {
	return "/StateManager/BasicAction/AtomicAction";
    }

public synchronized boolean addThread ()
    {
	return addThread(Thread.currentThread());
    }

public synchronized boolean addThread (Thread t)
    {
	if (t != null)
	{
	    ThreadActionData.pushAction(this);
	    return true;
	}

	return false;
    }

public synchronized boolean removeThread ()
    {
	return removeThread(Thread.currentThread());
    }

public synchronized boolean removeThread (Thread t)
    {
	if (t != null)
	{
	    ThreadActionData.purgeAction(this);
	    return true;
	}

	return false;
    }
    
protected AtomicAction (int at)
    {
	super(at);

	parentAction = null;	
    }

private BasicAction parentAction;
    
}
