/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectStoreSetup.java,v 1.1.4.1 2000/10/16 16:00:30 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation.ObjectStore;

import com.arjuna.ArjunaCore.Implementation.ObjectStoreImple;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Base.InventoryElement;

/*
 * The base class for all object store setups. Make sure we only
 * ever create a singe instance of a given object store.
 */

/*
 * Not currently used!
 */

public abstract class ObjectStoreSetup implements InventoryElement
{

protected abstract ObjectStoreImple create (String location);
    
protected ObjectStoreSetup ()
    {
    }

protected synchronized ObjectStoreImple createStore (String location)
    {
	StoreList slptr = null;
	boolean found = false, endOfList = false;

	if (location == null)
	    location = "";
    
	slptr = _headOfList;

	while ((slptr != null) && (!found) && (!endOfList))
	{
	    if (!_alwaysCreate)
	    {
		String sName = slptr.instance.getStoreName();
	    
		if (sName == null)
		    sName = "";
		
		if (sName.compareTo(location) == 0)
		{
		    found = true;
		    break;
		}
	    }
	
	    if (slptr.next != null)
		slptr = slptr.next;
	    else
		endOfList = true;
	}
    
	if (!found)
	{
	    StoreList toAdd = new StoreList();
	    toAdd.instance = create(location);

	    if (_headOfList == null)
		_headOfList = toAdd;
	    else
		slptr.next = toAdd;

	    slptr = toAdd;
	}
    
	slptr.useCount++;
    
	return slptr.instance;
    }

private synchronized boolean referenceStore (ObjectStoreImple toRef)
    {
	StoreList slptr = null;
	boolean found = false, endOfList = false;

	slptr = _headOfList;
	while ((slptr != null) && (!found) && (!endOfList))
	{
	    if (slptr.instance == toRef)
		found = true;
	    else
	    {
		if (slptr.next != null)
		    slptr = slptr.next;
		else
		    endOfList = true;
	    }
	}
    
	if (!found)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("ObjectStoreSetup.reference - cannot find objectstore instance.");
	    return false;
	}
    
	slptr.useCount++;
    
	return true;
    }

private synchronized void destroyStore (String location, ObjectStoreImple toDelete)
    {
	if (toDelete == null)
	    return;
    
	StoreList slptr = null, slmarker = null;
	boolean found = false;
    
	if (location == null)
	    location = "";

	if (_headOfList == null)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("Attempt to destroy object store instance without call on ObjectStore.create");
	    return;
	}
    
	slptr = _headOfList;
	while ((slptr != null) && (!found))
	{
	    if (slptr.instance == toDelete)
		found = true;
	    else
	    {
		slmarker = slptr;
		slptr = slptr.next;
	    }
	}
    
	if (!found)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("Attempt to destroy non-existant object store instance");
	    toDelete = null;
	    return;
	}
    
	slptr.useCount--;
	
	if (slptr.useCount == 0)
	{
	    if (slmarker == null)
		_headOfList = slptr.next;
	    else
		slmarker.next = slptr.next;
	
	    toDelete = null;
	}
    }

private static boolean _alwaysCreate = false;
private static StoreList _headOfList = null;
    
}
