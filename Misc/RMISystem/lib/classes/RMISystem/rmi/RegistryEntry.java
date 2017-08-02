/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RegistryEntry.java,v 1.2 1997/05/31 13:32:00 nmcl Exp $
 */

package RMISystem.rmi;

import java.rmi.Remote;

class RegistryEntry
{

public RegistryEntry (Remote obj, String n)
    {
	_object = obj;
	_name = n;
	_refCount = 1;

	_next = null;
	_prev = null;
    }

public void finalize ()
    {
	if (_refCount != 0)
	    System.err.println("Error - object "+_name+" being removed with ref count "+_refCount);
	
	_object = null;
	_name = null;
    }

public Remote object ()
    {
	return _object;
    }

public void object (Remote obj)
    {
	_object = obj;
    }
    
public String name ()
    {
	return _name;
    }

public void name (String n)
    {
	_name = n;
    }
    
public synchronized void ref ()
    {
	_refCount++;
    }

public synchronized void unref ()
    {
	_refCount--;
    }
    
public synchronized int refCount ()
    {
	return _refCount;
    }

public RegistryEntry next ()
    {
	return _next;
    }

public void next (RegistryEntry n)
    {
	_next = n;
    }

public RegistryEntry prev ()
    {
	return _prev;
    }

public void prev (RegistryEntry p)
    {
	_prev = p;
    }
    
private Remote _object;
private String _name;
private int _refCount;

private RegistryEntry _next;
private RegistryEntry _prev;
    
};
