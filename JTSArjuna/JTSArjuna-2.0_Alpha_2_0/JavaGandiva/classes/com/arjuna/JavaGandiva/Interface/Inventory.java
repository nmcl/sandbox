/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Inventory.java,v 1.4 1998/07/06 13:31:00 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Interface;

import com.arjuna.JavaGandiva.Common.*;
import com.arjuna.JavaGandiva.Base.InventoryElement;
import com.arjuna.JavaGandiva.Implementation.InventoryImple;
import com.arjuna.JavaGandiva.Implementation.Inventory.StaticInventory;
import java.io.PrintStream;

public class Inventory
{

public Inventory (InventoryImple imple)
    {
	_imple = imple;
    }
    
public static Inventory inventory ()
    {
	if (Inventory._inventory == null)
	    Inventory._inventory = new Inventory(new StaticInventory());
	
	return Inventory._inventory;
    }
    
public synchronized Object createVoid (ClassName typeName)
    {
	return ((_imple == null) ? null : _imple.createVoid(typeName));
    }

public synchronized Object createClassName (ClassName typeName, ClassName paramClassName)
    {
	return ((_imple == null) ? null : _imple.createClassName(typeName, paramClassName));
    }
    
public synchronized Object createObjectName (ClassName typeName, ObjectName paramObjectName)
    {
	return ((_imple == null) ? null : _imple.createObjectName(typeName, paramObjectName));
    }

public synchronized Object createResources (ClassName typeName, Object[] paramResources)
    {
	return ((_imple == null) ? null : _imple.createResources(typeName, paramResources));
    }

public synchronized Object createClassNameResources (ClassName typeName, ClassName paramClassName,
						     Object[] paramResources)
    {
	return ((_imple == null) ? null : _imple.createClassNameResources(typeName, paramClassName, paramResources));
    }

public synchronized Object createObjectNameResources (ClassName typeName,
						      ObjectName paramObjectName,
						      Object[] paramResources)
    {
	return ((_imple == null) ? null : _imple.createObjectNameResources(typeName, paramObjectName, paramResources));
    }
    
public synchronized void addToList (InventoryElement creator)
    {
	if (_imple == null)
	    System.err.println("Inventory.addToList error - no implementation!");
	else
	    _imple.addToList(creator);
    }
    
public synchronized void printList (PrintStream toUse)
    {
	if (_imple == null)
	    toUse.println("Inventory implementation null.");
	else
	    _imple.printList(toUse);
    }

private InventoryImple _imple;
    
private static Inventory _inventory = null;
    
}
