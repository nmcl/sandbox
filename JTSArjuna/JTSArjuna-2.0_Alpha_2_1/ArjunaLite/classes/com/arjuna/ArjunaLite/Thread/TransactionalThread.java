/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: TransactionalThread.java,v 1.1 2000/02/25 14:04:21 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Thread;

import com.arjuna.ArjunaLite.Atomic.*;
import java.util.Hashtable;
import java.lang.Thread;

import java.lang.IllegalArgumentException;

public class TransactionalThread extends Thread
{

public void finalize ()
    {
	if (action != null)
	{
	    action.removeChildThread(this);
	    action = null;
	}
    }

public static void create (Thread thread) throws IllegalArgumentException
    {
	/*
	 * New thread should not be running yet, so this
	 * should work!
	 */

	if (actions.get(thread) == null)
	{
	    BasicAction currentAction = BasicAction.Current();

	    if (currentAction != null)
	    {
		currentAction.addChildThread(thread);
		actions.put(thread, currentAction);
		
		currentAction = null;
	    }
	}
	else
	    throw new IllegalArgumentException();
    }

public static void destroy (Thread thread) throws IllegalArgumentException
    {
	BasicAction currentAction = (BasicAction) actions.remove(thread);

	if (currentAction != null)
	{
	    if (currentAction != null)
	    {
		currentAction.removeChildThread(thread);
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
    
};
