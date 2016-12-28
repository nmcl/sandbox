/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ThreadData.java,v 1.1.6.1 2000/05/22 10:21:40 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Thread;

import com.arjuna.ArjunaLite.Atomic.BasicAction;
import com.arjuna.ArjunaCommon.Template.BasicList;
import java.lang.Thread;
import java.util.Stack;

/*
 * Default visibility.
 */

class ThreadData
{

public ThreadData (Thread t, BasicAction a)
    {
	_thread = t;
	_actionHierarchy = new Stack();
	_next = null;

	try
	{
	    _actionHierarchy.push(a);
	}
	catch (Exception e)
	{
	}
    }

public ThreadData (Thread t, BasicAction a, ThreadData n)
    {
	_thread = t;
	_actionHierarchy = new Stack();
	_next = n;

	try
	{
	    _actionStack.push(a);
	}
	catch (Exception e)
	{
	}
    }    

public final boolean equals (Thread t)
    {
	if (_thread == t)
	    return true;
	else
	    return false;
    }

public final ThreadData next ()
    {
	return _next;
    }

public final void next (ThreadData n)
    {
	_next = n;
    }

public final BasicAction action ()
    {
	return (BasicAction) _actionHierarchy.peek();
    }

public final void push (BasicAction a)
    {
	_actionHierarchy.push(a);
    }

public final BasicAction pop ()
    {
	try
	{
	    return (BasicAction) _actionHierarchy.pop();
	}
	catch (Exception e)
	{
	    return null;
	}
    }

public final void purge (BasicAction a)
    {
	try
	{
	    _actionHierarchy.removeElement(a);
	}
	catch (Exception e)
	{
	}
    }
    
private Thread      _thread;
private Stack       _actionHierarchy;
private ThreadData  _next;
    
};
