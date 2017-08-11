/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DynamicInventory.java,v 1.3 1998/07/06 13:30:56 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Implementation.Inventory;

import com.arjuna.JavaGandiva.JavaGandiva;
import com.arjuna.JavaGandiva.Common.*;
import com.arjuna.JavaGandiva.Implementation.InventoryImple;
import com.arjuna.java.io.PrintStream;

import java.lang.SecurityException;

/*
 * Basic dynamic inventory uses X = Class.forName(typeName.toString());
 *
 * X.getInstance() ...
 *
 * Assume a naming scheme of XSetup then we can imply
 * the setup class name given X, and load it rather than X.
 *
 * Another implementation would be based upon the network class
 * loader and RMI.
 */

public class DynamicInventory extends StaticInventory
{

public DynamicInventory ()
    {
	_staticInventory = new StaticInventory();
    }

public synchronized void addToList (InventoryElement creator)
    {
	_staticInventory.addToList(creator);
    }

public ClassName className ()
    {
	return JavaGandiva.Implementation_Inventory_DynamicInventory();
    }

public static ClassName type ()
    {
	return JavaGandiva.Implementation_Inventory_DynamicInventory();
    }
    
    /*
     * No need to synchronize since it can only be called
     * from other synchronized methods.
     */
    
protected InventoryElement find (ClassName className)
    {
	InventoryElement ptr = _staticInventory.find(className);

	if (ptr == null)
	{
	    /*
	     * Try to load setup class.
	     */

	    String setupClass = className.toString()+"Setup";
	    Class theClass = Class.forName(setupClass);

	    if (theClass != null)
	    {
		/*
		 * Have setup class, so create instance. This should then add
		 * itself to the inventory, and we can simple call the
		 * find method again.
		 */

		theClass.getInstance();

		ptr = _staticInventory.find(className);
	    }
	    else
		System.err.println("WARNING: DynamicInventory could not load "+setupClass);
	}

	if (ptr == null)
	    System.err.println("WARNING: DynamicInventory could not find "+className);
	
	return ptr;
    }

private StaticInventory _staticInventory;

};
