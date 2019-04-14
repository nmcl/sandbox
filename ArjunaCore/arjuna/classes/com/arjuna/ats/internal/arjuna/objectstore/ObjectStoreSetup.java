/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectStoreSetup.java,v 1.1 2003/06/19 10:50:33 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.objectstore;

import com.arjuna.ats.arjuna.objectstore.ObjectStoreImple;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.gandiva.*;
import com.arjuna.ats.arjuna.gandiva.inventory.*;
import com.arjuna.ats.arjuna.logging.tsLogger;

/**
 * @message com.arjuna.ats.internal.arjuna.objectstore.ObjectStoreSetup_1 [com.arjuna.ats.internal.arjuna.objectstore.ObjectStoreSetup_1] - ObjectStoreSetup.reference - cannot find objectstore instance.
 * @message com.arjuna.ats.internal.arjuna.objectstore.ObjectStoreSetup_2 [com.arjuna.ats.internal.arjuna.objectstore.ObjectStoreSetup_2] - Attempt to destroy object store instance without call on ObjectStore.create
 * @message com.arjuna.ats.internal.arjuna.objectstore.ObjectStoreSetup_3 [com.arjuna.ats.internal.arjuna.objectstore.ObjectStoreSetup_3] - Attempt to destroy non-existant object store instance

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
	    if (tsLogger.arjLoggerI18N.isWarnEnabled())
		tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.ObjectStoreSetup_1");
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
	    if (tsLogger.arjLoggerI18N.isWarnEnabled())
		tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.ObjectStoreSetup_2");
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
	    if (tsLogger.arjLoggerI18N.isWarnEnabled())
		tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.ObjectStoreSetup_3");
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
