/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RegistryList.java,v 1.2 1997/05/31 13:32:00 nmcl Exp $
 */

package RMISystem.rmi;

import java.rmi.Remote;

class RegistryList
{

public RegistryList ()
    {
	_list = null;
	_size = 0;
    }

public void finalize ()
    {
	if (_list != null)
	    System.err.println("Error - removing RegistryList with objects still attached.");
	
	_list = null;
    }

public synchronized Remote find (String name)  // if present does a ref for us
    {
	RegistryEntry ptr = _list;

	while (ptr != null)
	{
	    System.out.println("comparing "+ptr.name()+" and "+name);
	    
	    if (ptr.name().compareTo(name) == 0)
	    {
		ptr.ref();
		return ptr.object();
	    }
	    else
		ptr = ptr.next();
	}

	return null;
    }

public synchronized String[] list ()
    {
	if (_size == 0)
	    return null;
	
	String[] s = new String[_size];
	RegistryEntry ptr = _list;
	int index = 0;

	while (ptr != null)
	{
	    s[index] = ptr.name();
	    index++;
	    ptr = ptr.next();
	}

	return s;
    }
    
public synchronized void add (String name, Remote obj)
    {
	RegistryEntry ptr = new RegistryEntry(obj, name);

	ptr.next(_list);

	if (_list != null)
	    _list.prev(ptr);

	_list = ptr;
	_size++;
    }    

    /*
     * If not present then simply add, otherwise replace.
     */
    
public synchronized void replace (String name, Remote obj)
    {
	RegistryEntry ptr = _list;

	while (ptr != null)
	{
	    if (ptr.name().compareTo(name) == 0)
	    {
		ptr.object(obj);
		ptr.name(name);

		return;
	    }
	    else
		ptr = ptr.next();
	}

	add(name, obj);
    }

public synchronized void unref (String name)
    {
	RegistryEntry ptr = _list;

	while (ptr != null)
	{
	    if (ptr.name().compareTo(name) == 0)
	    {
		ptr.unref();

		if (ptr.refCount() == 0)
		    remove(ptr);

		return;
	    }
	    else
		ptr = ptr.next();
	}

	System.err.println("Error - attempt to unref unknown object: "+name);
    }

private void remove (RegistryEntry ptr)
    {
	if (ptr == _list)
	{
	    _list = ptr.next();
	    _list.prev(null);
	}
	else
	{
	    _list.prev().next(_list.next());

	    if (_list.next() != null)
		_list.next().prev(_list.prev());
	}

	_size--;
    }
    
private RegistryEntry _list;
private int _size;
    
}
