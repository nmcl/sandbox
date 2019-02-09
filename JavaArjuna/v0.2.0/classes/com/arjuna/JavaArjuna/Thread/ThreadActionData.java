/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadActionData.java,v 1.8 1998/07/06 13:26:54 nmcl Exp $
 */

package com.arjuna.JavaArjuna.Thread;

import com.arjuna.JavaArjuna.ClassLib.BasicAction;
import java.lang.Thread;

import java.util.NoSuchElementException;

public class ThreadActionData
{

public static synchronized BasicAction currentAction () // throws NoSuchElementException
    {
	Thread t = Thread.currentThread();
	ThreadData ptr = _threadList;

	while (ptr != null)
	{
	    if (ptr.equals(t))
		return ptr.action();
	    else
		ptr = ptr.next();
	}

	return null;
    }
    
public static synchronized void pushAction (BasicAction a)
    {
	Thread t = Thread.currentThread();

	if (_threadList == null)
	    _threadList = new ThreadData(t, a);
	else
	{
	    ThreadData ptr = _threadList;

	    while (ptr != null)
	    {
		if (ptr.equals(t))
		{
		    ptr.push(a);
		    a.addChildThread(t);
		    
		    return;
		}
		else
		    ptr = ptr.next();
	    }

	    ptr = new ThreadData(t, a, _threadList);
	    a.addChildThread(t);
	    _threadList = ptr;
	}
    }

public static synchronized BasicAction popAction ()
    {
	Thread t = Thread.currentThread();
	ThreadData ptr = _threadList;

	while (ptr != null)
	{
	    if (ptr.equals(t))
	    {
		BasicAction a = ptr.pop();

		if (a != null)
		{
		    a.removeChildThread(t);
		}

		return a;
	    }
	    else
		ptr = ptr.next();
	}

	return null;
    }

public static synchronized void purgeAction (BasicAction act)
    {
	ThreadActionData.purgeAction(act, Thread.currentThread());
    }

public static synchronized void purgeAction (BasicAction act, Thread t)
    {
	ThreadData ptr = _threadList;

	if (act != null)
	    act.removeChildThread(t);

	while (ptr != null)
	{
	    if (ptr.equals(t))
	    {
		ptr.purge(act);
		return;
	    }
	    else
		ptr = ptr.next();
	}
    }

private static ThreadData _threadList = null;

};
