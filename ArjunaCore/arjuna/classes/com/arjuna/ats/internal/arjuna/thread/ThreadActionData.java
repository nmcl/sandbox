/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ThreadActionData.java,v 1.1 2003/06/19 10:50:40 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.thread;

import com.arjuna.ats.arjuna.coordinator.BasicAction;
import java.lang.Thread;
import java.util.Hashtable;
import java.util.Stack;
import java.util.Vector;

import java.util.NoSuchElementException;
import java.util.EmptyStackException;

/**
 * This class maintains a mapping between a thread and its notion of
 * the current transaction. Essentially this is a stack of transactions.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ThreadActionData.java,v 1.1 2003/06/19 10:50:40 nmcl Exp $
 * @since JTS 1.0.
 */

public class ThreadActionData
{

    public static synchronized BasicAction currentAction ()
    {
	ThreadActionData.setup();
	
	Stack txs = (Stack) _threadList.get(Thread.currentThread().getName());

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
    
    public static void pushAction (BasicAction a)
    {
	pushAction(a, true);
    }

    /**
     * By setting the register flag accordingly, information about
     * the thread may not be propagated to the action, 
     * i.e., only the thread's notion of current changes.
     */
    
    public static synchronized void pushAction (BasicAction a, boolean register)
    {
	Thread t = Thread.currentThread();
	Stack txs = (Stack) _threadList.get(t.getName());
	
	if (txs == null)
	{
	    txs = new Stack();
	    txs.push(a);
	    
	    _threadList.put(t.getName(), txs);
	}
	else
	    txs.push(a);

	if (register)
	    a.addChildThread(t);
    }

    /**
     * Put back the entire hierarchy, removing whatever is already there.
     */

    public static synchronized void restoreActions (BasicAction act)
    {
	purgeActions();

	if (act != null)
	{
	    /*
	     * First get the hierarchy from the bottom up.
	     */

	    java.util.Stack s = new java.util.Stack();
	    BasicAction nextLevel = act.parent();

	    s.push(act);
	    
	    while (nextLevel != null)
	    {
		s.push(nextLevel);
		
		nextLevel = nextLevel.parent();
	    }
	    
	    /*
	     * Now push the hierarchy onto the thread stack.
	     */

	    try
	    {
		while (!s.empty())
		{
		    pushAction((BasicAction) s.pop());
		}
	    }
	    catch (Exception ex)
	    {
	    }
	}
    }

    public static BasicAction popAction () throws EmptyStackException
    {
	return popAction(Thread.currentThread().getName(), true);
    }

    public static BasicAction popAction (boolean unregister) throws EmptyStackException
    {
	return popAction(Thread.currentThread().getName(), unregister);
    }

    public static BasicAction popAction (String threadId) throws EmptyStackException
    {
	return popAction(threadId, true);
    }

    /**
     * By setting the unregister flag accordingly, information
     * about the thread is not removed from the action.
     */
    
    public static synchronized BasicAction popAction (String threadId, boolean unregister) throws EmptyStackException
    {
	Stack txs = (Stack) _threadList.get(threadId);
	
	if (txs != null)
	{
	    BasicAction a = (BasicAction) txs.pop();
	    
	    if ((a != null) && (unregister))
	    {
		a.removeChildThread(threadId);
	    }

	    if (txs.size() == 0)
		_threadList.remove(threadId);
	    
	    return a;
	}

	return null;
    }

    public static void purgeAction (BasicAction act) throws NoSuchElementException
    {
	ThreadActionData.purgeAction(act, Thread.currentThread(), true);
    }

    public static void purgeAction (BasicAction act, Thread t) throws NoSuchElementException
    {
	ThreadActionData.purgeAction(act, t, true);
    }
    
    public static synchronized void purgeAction (BasicAction act, Thread t,
						 boolean unregister) throws NoSuchElementException
    {
	if ((act != null) && (unregister))
	    act.removeChildThread(t.getName());

	Stack txs = (Stack) _threadList.get(t.getName());
	
	if (txs != null)
	{
	    txs.removeElement(act);

	    if (txs.size() == 0)
		_threadList.remove(t.getName());
	}
    }

    public static void purgeActions ()
    {
	purgeActions(Thread.currentThread(), true);
    }
    
    public static void purgeActions (Thread t)
    {
	purgeActions(t, true);
    }
    
    public static synchronized void purgeActions (Thread t, boolean unregister)
    {
	Stack txs = (Stack) _threadList.remove(t.getName());
	
	if (txs != null)
	{
	    if (unregister)
	    {
		while (!txs.empty())
		{
		    BasicAction act = (BasicAction) txs.pop();

		    if (act != null)
			act.removeChildThread(t.getName());
		}
	    }
	}
    }

    public static void addSetup (ThreadSetup s)
    {
	synchronized (_threadSetups)
	    {
		_threadSetups.addElement(s);
	    }
    }
    
    public static boolean removeSetup (ThreadSetup s)
    {
	synchronized (_threadSetups)
	    {
		return _threadSetups.removeElement(s);
	    }
    }
    
    private static void setup ()
    {
	synchronized (_threadSetups)
	    {
		for (int i = 0; i < _threadSetups.size(); i++)
		{
		    ThreadSetup s = (ThreadSetup) _threadSetups.elementAt(i);
	    
		    if (s != null)
			s.setup();
		}
	    }
    }
    
    private static Hashtable _threadList = new Hashtable();
    private static Vector    _threadSetups = new Vector();
    
}
