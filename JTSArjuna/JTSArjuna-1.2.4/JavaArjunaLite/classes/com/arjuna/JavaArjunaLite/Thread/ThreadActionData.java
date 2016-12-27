/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadActionData.java,v 1.8.6.1 1999/04/16 13:46:19 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Thread;

import com.arjuna.JavaArjunaLite.Atomic.BasicAction;
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
	ThreadData d = null;
	
	if (_threadList == null)
	    d = new ThreadData(t, a);
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

	    d = new ThreadData(t, a, _threadList);
	}
	
	a.addChildThread(t);
	_threadList = d;
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
