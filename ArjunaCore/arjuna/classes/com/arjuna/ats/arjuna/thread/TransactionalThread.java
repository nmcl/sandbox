/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: TransactionalThread.java,v 1.1 2003/06/19 10:50:26 nmcl Exp $
 */

package com.arjuna.ats.arjuna.thread;

import com.arjuna.ats.arjuna.coordinator.*;
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
 * @version $Id: TransactionalThread.java,v 1.1 2003/06/19 10:50:26 nmcl Exp $
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
