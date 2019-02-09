/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: KeyedList.java,v 1.2 1998/07/06 13:31:06 nmcl Exp $
 */

package com.arjuna.JavaArjuna.Template;

public class KeyedList
{

public KeyedList ()
    {
	_head = null;
    }

public void finalise ()
    {
	KeyedElement ptr = _head;

	while (ptr != null)
	{
	    _head = ptr.next();
	    ptr = null;
	    ptr = _head;
	}
    }

public synchronized void add (Object obj, String id)
    {
	KeyedElement ptr = new KeyedElement(obj, id);

	if (_head != null)
	    _head.prev(ptr);
	    
	ptr.next(_head);
	_head = ptr;
    }
    
public synchronized Object get (String id)
    {
	KeyedElement ptr = present(id);

	if (ptr != null)
	{
	    /*
	     * Now increment reference count.
	     */
	
	    ptr.ref();
	    
	    return ptr.object();
	}
	else
	    return null;
    }

    /*
     * Will delete if ref count == 0
     */

public synchronized void unref (String id)
    {
	KeyedElement ptr = present(id);

	if (ptr != null)
	{
	    if (ptr.unref())
	    {
		if (ptr == _head)
		{
		    _head = ptr.next();

		    if (_head != null)
			_head.prev(null);
		}
		else
		{
		    ptr.prev().next(ptr.next());

		    if (ptr.next() != null)
			ptr.next().prev(ptr.prev());
		}

		ptr = null;
	    }
	}
    }

    /*
     * Only called from within mutex protected methods.
     */

private KeyedElement present (String id)
    {
	KeyedElement ptr = _head;
	
	while (ptr != null)
	{
	    if (ptr.key().compareTo(id) == 0)
		return ptr;
	    else
		ptr = ptr.next();
	}

	return null;
    }

private KeyedElement _head;
    
}

class KeyedElement
{

public KeyedElement (Object obj, String key)
    {
	_obj = obj;
	_key = key;
	_refCount = 1;
	_next = _prev = null;
    }

public void finalize ()
    {
	if (_refCount != 0)
	    System.err.println("KeyedElement deleting object "+_key+" with ref count "+_refCount);

	_obj = null;
    }

public KeyedElement next ()
    {
	return _next;
    }

public void next (KeyedElement n)
    {
	_next = n;
    }

public KeyedElement prev ()
    {
	return _prev;
    }

public void prev (KeyedElement p)
    {
	_prev = p;
    }

public int refCount ()
    {
	return _refCount;
    }

public Object object ()
    {
	return _obj;
    }

public String key ()
    {
	return _key;
    }

public void ref ()
    {
	_refCount++;
    }

public boolean unref ()
    {
	return (boolean) (--_refCount == 0);
    }

private Object _obj;
private String _key;
private int _refCount;
private KeyedElement _next;
private KeyedElement _prev;

};
