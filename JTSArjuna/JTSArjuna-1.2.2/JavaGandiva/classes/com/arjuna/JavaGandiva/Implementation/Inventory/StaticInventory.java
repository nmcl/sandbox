/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StaticInventory.java,v 1.6 1998/07/06 13:30:57 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Implementation.Inventory;

import com.arjuna.JavaGandiva.JavaGandivaNames;
import com.arjuna.JavaGandiva.Common.*;
import com.arjuna.JavaGandiva.Implementation.InventoryImple;
import com.arjuna.JavaGandiva.Implementation.Implementations;
import com.arjuna.JavaGandiva.Base.InventoryElement;
import java.io.PrintStream;

public class StaticInventory extends InventoryImple
{

public StaticInventory ()
    {
    }
    
public synchronized Object createVoid (ClassName typeName)
    {
	InventoryElement creator = find(typeName);

	if (creator != null)
	    return creator.createVoid();
	else
	    return null;
    }

public synchronized Object createClassName (ClassName typeName, ClassName paramClassName)
    {
	InventoryElement creator = find(typeName);

	if (creator != null)
	    return creator.createClassName(paramClassName);
	else
	    return null;
    }
    
public synchronized Object createObjectName (ClassName typeName, ObjectName paramObjectName)
    {
	InventoryElement creator = find(typeName);

	if (creator != null)
	    return creator.createObjectName(paramObjectName);
	else
	    return null;
    }

public synchronized Object createResources (ClassName typeName, Object[] paramResources)
    {
	InventoryElement creator = find(typeName);

	if (creator != null)
	    return creator.createResources(paramResources);
	else
	    return null;
    }

public synchronized Object createClassNameResources (ClassName typeName, ClassName paramClassName,
						     Object[] paramResources)
    {
	InventoryElement creator = find(typeName);

	if (creator != null)
	    return creator.createClassNameResources(paramClassName, paramResources);
	else
	    return null;
    }

public synchronized Object createObjectNameResources (ClassName typeName,
						      ObjectName paramObjectName,
						      Object[] paramResources)
    {
	InventoryElement creator = find(typeName);

	if (creator != null)
	    return creator.createObjectNameResources(paramObjectName, paramResources);
	else
	    return null;
    }

public synchronized void addToList (InventoryElement creator)
    {
	if ((creator != null) && (creator.className() != null))
	{
	    if (headOfList == null)
		headOfList = new InventoryList(creator);
	    else
	    {
		boolean found = false;
		InventoryList marker = headOfList, trail = null;
		
		while ((!found) && (marker != null))
		{
		    if (!creator.className().equals(marker._instance.className()))
		    {
			trail = marker;
			marker = marker._next;
		    }
		    else
			found = true;
		}

		if (!found)
		    trail._next = new InventoryList(creator);
	    }
	}
    }
    
public synchronized void printList (PrintStream toUse)
    {
	InventoryList marker = headOfList;

	toUse.println("StaticInventory contains:");
    
	while (marker != null)
	{
	    toUse.println(marker._instance.className());
	    marker = marker._next;
	}

	toUse.println("End of list.");
    }

public ClassName className ()
    {
	return JavaGandivaNames.Implementation_Inventory_StaticInventory();
    }

public static ClassName type ()
    {
	return JavaGandivaNames.Implementation_Inventory_StaticInventory();
    }
    
    /*
     * No need to synchronize since it can only be called
     * from other synchronized methods.
     */
    
protected InventoryElement find (ClassName className)
    {
	if (headOfList == null)
	    Implementations.initialise();
	    
	if (headOfList != null)
	{    
	    InventoryList marker = headOfList;
    
	    while (marker != null)
	    {
		if ((marker._instance.className() != null) &&
		    (marker._instance.className().equals(className)))
		    return marker._instance;
		else
		    marker = marker._next;
	    }
	}

	System.err.println("WARNING: StaticInventory cannot find "+className+" implementation.");
	
	return null;
    }

private static InventoryList headOfList = null;
    
}
