/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: StaticInventory.java,v 1.1.8.2.22.2.4.1.34.1 2001/05/25 13:27:38 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Implementation.Inventory;

import com.arjuna.ArjunaCommon.ArjunaCommonNames;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Implementation.InventoryImple;
import com.arjuna.ArjunaCommon.Implementation.Implementations;
import com.arjuna.ArjunaCommon.Base.InventoryElement;
import java.io.PrintStream;
import java.util.*;

/**
 * The StaticInventory required each implementation type to be registered
 * with it explicitly. This can happen in an application specific manner,
 * or (more typically) through an "Implementations" class. Each module
 * may specific one or more Implementations classes which are responsible
 * for registering one or more implementations with the inventory. How
 * these Implementations are instantiated and made to do this can either
 * be application specific, or dynamically by specifying each Implementation
 * as a Java property. Any property that starts with STATIC_INVENTORY_IMPLE
 * will be assumed to have a value which is a valid Java class. An instance
 * of this class will be created when the StaticInventory is created and
 * it should then register any implementations with the inventory.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: StaticInventory.java,v 1.1.8.2.22.2.4.1.34.1 2001/05/25 13:27:38 nmcl Exp $
 * @since JTS 1.0.
 */

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
	return ArjunaCommonNames.Implementation_Inventory_StaticInventory();
    }

public static ClassName type ()
    {
	return ArjunaCommonNames.Implementation_Inventory_StaticInventory();
    }
    
    /**
     * No need to synchronize since it can only be called
     * from other synchronized methods.
     */
    
protected InventoryElement find (ClassName className)
    {
	if (!StaticInventory.initialised)
	    initialise();
	    
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

	ErrorStream.stream(ErrorStream.WARNING).println("StaticInventory cannot find "+className+" implementation.");
	
	return null;
    }

    /**
     * Scan the properties for any Implementations that may be
     * added dynamically. If we find them, instantiate them and
     * hope that works!
     */

private synchronized void initialise ()
    {
	Implementations.initialise();

	Enumeration e = PropertyManager.propertyNames();
	
	if (e != null)
	{
	    while (e.hasMoreElements())
	    {
		String name = (String) e.nextElement();
		
		if (name.startsWith(com.arjuna.ArjunaCommon.Common.Environment.STATIC_INVENTORY_IMPLE))
		{
		    String className = PropertyManager.getProperty(name);

		    try
		    {
			Class c = Class.forName(className);
			Object o = c.newInstance();
		    }
		    catch (Exception ex)
		    {
			ex.printStackTrace();
		    }
		}
	    }
	}

	initialised = true;
    }
    
private static InventoryList headOfList = null;
private static boolean       initialised = false;
    
}
