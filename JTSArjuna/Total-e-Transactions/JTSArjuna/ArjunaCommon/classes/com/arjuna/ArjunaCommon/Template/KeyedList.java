/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: KeyedList.java,v 1.1.8.2.26.1 2000/12/21 11:20:55 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Template;

import com.arjuna.ArjunaCommon.Common.ErrorStream;
import java.util.Hashtable;

/*
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: KeyedList.java,v 1.1.8.2.26.1 2000/12/21 11:20:55 nmcl Exp $
 * @since JTS 1.0.
 */

public class KeyedList
{

public KeyedList ()
    {
	_theTable = new Hashtable();
    }

public void finalise ()
    {
	_theTable.clear();
	_theTable = null;
    }

public synchronized void add (Object obj, String id)
    {
	_theTable.put(id, new KeyedElement(obj, id));
    }
    
public synchronized Object get (String id)
    {
	KeyedElement ptr = (KeyedElement) _theTable.get(id);

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
     * Will delete if ref count == 0.
     */

public synchronized void unref (String id)
    {
	KeyedElement ptr = (KeyedElement) _theTable.get(id);

	if (ptr != null)
	{
	    if (ptr.unref())
	    {
		_theTable.remove(id);
	    }
	}
    }
    
private Hashtable _theTable;
    
}

class KeyedElement
{

public KeyedElement (Object obj, String key)
    {
	_obj = obj;
	_key = key;
	_refCount = 1;
    }

public void finalize ()
    {
	if (_refCount != 0)
	    ErrorStream.stream(ErrorStream.WARNING).println("KeyedElement deleting object "+_key+" with ref count "+_refCount);

	_obj = null;
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

};
