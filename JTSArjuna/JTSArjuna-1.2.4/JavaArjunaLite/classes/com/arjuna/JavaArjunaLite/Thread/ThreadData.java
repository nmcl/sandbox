/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadData.java,v 1.5.6.1 1999/04/16 13:46:20 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Thread;

import com.arjuna.JavaArjunaLite.Atomic.BasicAction;
import com.arjuna.JavaGandiva.Template.BasicList;
import java.lang.Thread;

/*
 * Default visibility.
 */

class ThreadData
{

public ThreadData (Thread t, BasicAction a)
    {
	_thread = t;
	_actionHierarchy = new ActionStack(a);	
	_next = null;
    }

public ThreadData (Thread t, BasicAction a, ThreadData n)
    {
	_thread = t;
	_actionHierarchy = new ActionStack(a);
	_next = n;
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
	return _actionHierarchy.current();
    }

public final void push (BasicAction a)
    {
	_actionHierarchy.push(a);
    }

public final BasicAction pop ()
    {
	return _actionHierarchy.pop();
    }

public final void purge (BasicAction a)
    {
	_actionHierarchy.purge(a);
    }
    
private Thread      _thread;
private ActionStack _actionHierarchy;
private ThreadData  _next;
    
};


class ActionStack
{
    
public ActionStack ()
    {
	_head = new BasicList();
    }

public ActionStack (BasicAction a)
    {
	_head = new BasicList();
	_head.add(a);
    }    

public void finalize ()
    {
	_head = null;
    }

public final synchronized BasicAction current ()
    {
	return (BasicAction) _head.inspect();
    }

public final synchronized void push (BasicAction a)
    {
	_head.add(a);
    }

public final synchronized BasicAction pop ()
    {
	return (BasicAction) _head.orderedPop();
    }

public final synchronized void purge (BasicAction a)
    {
	_head.remove(a);
    }

private BasicList _head;
    
};

