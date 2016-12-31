/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Inventory.java,v 1.2.4.1 2000/04/25 08:35:56 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Interface;

import com.arjuna.ArjunaCommon.ArjunaCommonNames;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Base.InventoryElement;
import com.arjuna.ArjunaCommon.Implementation.InventoryImple;
import com.arjuna.ArjunaCommon.Implementation.Inventory.StaticInventory;
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

public static ClassName name ()
    {
	return ArjunaCommonNames.Interface_Inventory();
    }

public ClassName className ()
    {
	return null;
    }

public ClassName impleClassName ()
    {
	if (_imple != null)
	    return _imple.className();
	else
	    return ClassName.invalid();
    }

public Inventory castup (ClassName theType)
    {
	if (theType.equals(className()))
	    return this;
	else
	    return null;
    }

private InventoryImple _imple;
    
private static Inventory _inventory = null;
    
}
