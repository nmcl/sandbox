/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ThreadActionData.java,v 1.2.4.1.4.1 2000/05/22 10:21:40 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Thread;

import com.arjuna.ArjunaLite.Atomic.BasicAction;
import java.lang.Thread;
import java.util.Hashtable;
import java.util.Stack;

import java.util.NoSuchElementException;
import java.util.EmptyStackException;

public class ThreadActionData
{

public static synchronized BasicAction currentAction ()
    {
	Stack txs = (Stack) _threadList.get(Thread.currentThread());

	if (txs != null)
	{
	    try
	    {
		return (BasicAction) txs.peek();
	    }
	    catch (EmptyStackException e)
	    {
	    }
	}
	
	return null;
    }
    
public static synchronized void pushAction (BasicAction a)
    {
	pushAction(a, true);
    }

    /*
     * By setting the register flag accordingly, information about
     * the thread may not be propagated to the action, 
     * i.e., only the thread's notion of current changes.
     */
    
public static synchronized void pushAction (BasicAction a, boolean register)
    {
	Thread t = Thread.currentThread();
	Stack txs = (Stack) _threadList.get(t);
	
	if (txs == null)
	{
	    txs = new Stack();
	    txs.push(a);
	    
	    _threadList.put(t, txs);
	}
	else
	    txs.push(a);

	if (register)
	    a.addChildThread(t);
    }

public static synchronized BasicAction popAction () throws EmptyStackException
    {
	return popAction(true);
    }

    /*
     * By setting the unregister flag accordingly, information
     * about the thread is not removed from the action.
     */
    
public static synchronized BasicAction popAction (boolean unregister) throws EmptyStackException
    {
	Thread t = Thread.currentThread();
	Stack txs = (Stack) _threadList.get(t);
	
	if (txs != null)
	{
	    BasicAction a = (BasicAction) txs.pop();
	    
	    if ((a != null) && (unregister))
	    {
		a.removeChildThread(t);
	    }

	    return a;
	}

	return null;
    }

public static synchronized void purgeAction (BasicAction act) throws NoSuchElementException
    {
	ThreadActionData.purgeAction(act, Thread.currentThread(), true);
    }

public static synchronized void purgeAction (BasicAction act, Thread t) throws NoSuchElementException
    {
	ThreadActionData.purgeAction(act, t, true);
    }
    
public static synchronized void purgeAction (BasicAction act, Thread t,
					     boolean unregister) throws NoSuchElementException
    {
	if ((act != null) && (unregister))
	    act.removeChildThread(t);

	Stack txs = (Stack) _threadList.get(t);
	
	if (txs != null)
	    txs.removeElement(act);
    }

private static Hashtable _threadList = new Hashtable();

};
