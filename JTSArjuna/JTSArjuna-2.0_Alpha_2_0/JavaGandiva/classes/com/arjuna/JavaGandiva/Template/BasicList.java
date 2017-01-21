/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BasicList.java,v 1.6.8.1 1999/05/21 14:46:19 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Template;

/*
 * Currently implemented as a stack, but will change.
 */

public class BasicList
{

public BasicList ()
    {
	_head = null;
	_tail = null;
	_size = 0;
    }

    /**
     * Add to the front of the list.
     */
    
public synchronized void add (Object t)
    {
	ObjectElement ptr = new ObjectElement(t);

	ptr.next(_head);

	_head = ptr;

	if (_tail == null)
	    _tail = _head;

	_size++;
    }

    /*
     * Insert is like add except that it only adds once. i.e., if the entry
     * already exists it returns FALSE;
     */

public synchronized boolean insert (Object t)
    {
	ObjectElement ptr = _head;

	while (ptr != null)
	{
	    if (ptr.value().equals(t))
		return false;
	    else
		ptr = ptr.next();
	}

	/*
	 * Must be a new element.
	 */

	add(t);

	return true;
    }
    
public synchronized Object orderedPop ()
    {
	if (_head == null)
	    return null;
	else
	{
	    Object t = _head.value();
	    _head = _head.next();

	    _size--;
	    
	    return t;
	}
    }

public synchronized boolean remove (Object o)
    {
	if (_head == null)
	    return false;
	else
	{
	    ObjectElement ptr = _head;
	    ObjectElement trail = null;
	    
	    do
	    {
		if (ptr.value().equals(o))
		{
		    if (ptr == _tail)
			_tail = trail;
		    
		    if (trail != null)
			trail.next(ptr.next());
		    else
			_head = ptr.next();

		    _size--;
		    ptr = null;
			
		    return true;
		}
		else
		{
		    trail = ptr;
		    ptr = ptr.next();
		}
		
	    } while (ptr != null);

	    return false;
	}
    }
    
public synchronized Object inspect ()
    {
	if (_head == null)
	    return null;
	else
	    return _head.value();
    }

public synchronized boolean contains (Object t)
    {
	if (_size == 0)
	    return false;

	ObjectElement o = _head;

	while (o != null)
	{
	    if (o.value().equals(t))
		return true;
	    else
		o = o.next();
	}

	return false;
    }
	
public synchronized int size ()
    {
	return _size;
    }

public synchronized boolean isEmpty ()
    {
	return (boolean) (_size == 0);
    }

public synchronized void empty ()
    {
	Object t;

	while ((t = orderedPop()) != null)
	{
	    t = null;
	    t = orderedPop();
	}
    }
    
protected ObjectElement _head;
protected ObjectElement _tail;
protected int _size;
    
};

