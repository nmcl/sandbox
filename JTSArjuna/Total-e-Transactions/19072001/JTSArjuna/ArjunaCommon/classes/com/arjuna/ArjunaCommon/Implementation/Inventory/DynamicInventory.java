/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: DynamicInventory.java,v 1.1.8.2.22.1.4.1 2000/12/21 11:20:53 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Implementation.Inventory;

import com.arjuna.ArjunaCommon.ArjunaCommonNames;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Implementation.InventoryImple;
import com.arjuna.ArjunaCommon.Base.InventoryElement;
import java.io.PrintStream;

import java.lang.SecurityException;

/**
 * Basic dynamic inventory uses X = Class.forName(typeName.toString());
 *
 * X.getInstance() ...
 *
 * Assume a naming scheme of XSetup then we can imply
 * the setup class name given X, and load it rather than X.
 *
 * Another implementation would be based upon the network class
 * loader and RMI.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: DynamicInventory.java,v 1.1.8.2.22.1.4.1 2000/12/21 11:20:53 nmcl Exp $
 * @since JTS 1.0.
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
	return ArjunaCommonNames.Implementation_Inventory_DynamicInventory();
    }

public static ClassName type ()
    {
	return ArjunaCommonNames.Implementation_Inventory_DynamicInventory();
    }
    
    /**
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
	    Class theClass = null;
		
		try
		{
			theClass = Class.forName(setupClass);
		}
		catch (Exception e)
		{
			theClass = null;
		}

	    if (theClass != null)
	    {
		/*
		 * Have setup class, so create instance. This should then add
		 * itself to the inventory, and we can simple call the
		 * find method again.
		 */

			try
			{
				theClass.newInstance();
			}
			catch (Exception e)
			{
				ErrorStream.stream(ErrorStream.WARNING).println(e);
			}

		ptr = _staticInventory.find(className);
	    }
	    else
		ErrorStream.stream(ErrorStream.WARNING).println("DynamicInventory could not load "+setupClass);
	}

	if (ptr == null)
	    ErrorStream.stream(ErrorStream.WARNING).println("DynamicInventory could not find "+className);
	
	return ptr;
    }

private StaticInventory _staticInventory;

};
