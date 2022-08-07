/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: TransactionalThread.java,v 1.1.4.1.2.1.2.1.2.1 2000/12/21 11:21:12 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Thread;

import com.arjuna.ArjunaCore.Atomic.*;
import java.util.Hashtable;
import java.lang.Thread;

import java.lang.IllegalArgumentException;

/**
 * A transactional thread is automatically registered with the transaction
 * that is in scope when it is created (i.e., the transaction that is
 * associated with the creating thread). The transactional thread is also
 * automatically unregistered from the transaction when the thread
 * terminates.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: TransactionalThread.java,v 1.1.4.1.2.1.2.1.2.1 2000/12/21 11:21:12 nmcl Exp $
 * @since JTS 1.0.
 */

public class TransactionalThread extends Thread
{

public void finalize ()
    {
	if (action != null)
	{
	    action.removeChildThread(this.getName());
	    action = null;
	}
    }

public static void create (Thread thread) throws IllegalArgumentException
    {
	/*
	 * New thread should not be running yet, so this
	 * should work!
	 */

	if (actions.get(thread.getName()) == null)
	{
	    BasicAction currentAction = BasicAction.Current();

	    if (currentAction != null)
	    {
		currentAction.addChildThread(thread);
		actions.put(thread.getName(), currentAction);
		
		currentAction = null;
	    }
	}
	else
	    throw new IllegalArgumentException();
    }

public static void destroy (Thread thread) throws IllegalArgumentException
    {
	BasicAction currentAction = (BasicAction) actions.remove(thread.getName());

	if (currentAction != null)
	{
	    if (currentAction != null)
	    {
		currentAction.removeChildThread(thread.getName());
		currentAction = null;
	    }
	}
	else
	    throw new IllegalArgumentException();
    }

protected TransactionalThread ()
    {
	/*
	 * New thread should not be running yet, so this
	 * should work!
	 */
	
	action = BasicAction.Current();

	if (action != null)
	{
	    action.addChildThread(this);
	}
    }

private BasicAction action = null;

private static Hashtable actions = new Hashtable();
    
}
