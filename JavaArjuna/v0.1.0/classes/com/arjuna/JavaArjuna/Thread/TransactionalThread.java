/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TransactionalThread.java,v 1.1 1998/07/31 15:18:15 nmcl Exp $
 */

package com.arjuna.JavaArjuna.Thread;

import com.arjuna.JavaArjuna.ClassLib.*;
import java.lang.Thread;

import java.lang.IllegalArgumentException;

public class TransactionalThread extends Thread
{

public void finalize ()
    {
	BasicAction currentAction = BasicAction.Current();

	if (currentAction != null)
	{
	    currentAction.removeChildThread(this);
	    currentAction = null;
	}
    }

public static Object create (Object thread) throws IllegalArgumentException
    {
	if (thread instanceof Thread)
	{
	    /*
	     * New thread should not be running yet, so this
	     * should work!
	     */
	
	    BasicAction currentAction = BasicAction.Current();

	    if (currentAction != null)
	    {
		currentAction.addChildThread((Thread) thread);
		currentAction = null;
	    }

	    return thread;
	}
	else
	    throw new IllegalArgumentException();
    }

public static void destroy (Object thread) throws IllegalArgumentException
    {
	if (thread instanceof Thread)
	{
	    BasicAction currentAction = BasicAction.Current();

	    if (currentAction != null)
	    {
		currentAction.removeChildThread((Thread) thread);
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
	
	BasicAction currentAction = BasicAction.Current();

	if (currentAction != null)
	{
	    currentAction.addChildThread(this);
	    currentAction = null;
	}
    }
    
};
