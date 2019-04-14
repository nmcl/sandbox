/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: DynamicInventory.java,v 1.2 2003/08/11 14:07:41 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.gandiva.inventory;

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.gandiva.*;
import com.arjuna.ats.arjuna.gandiva.inventory.*;
import java.io.PrintStream;

import com.arjuna.ats.arjuna.logging.tsLogger;

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
 * @version $Id: DynamicInventory.java,v 1.2 2003/08/11 14:07:41 nmcl Exp $
 * @since JTS 1.0.
 *
 * @message com.arjuna.ats.internal.arjuna.gandiva.inventory.DynamicInventory_1 [com.arjuna.ats.internal.arjuna.gandiva.inventory.DynamicInventory_1] -  could not load {0}
 * @message com.arjuna.ats.internal.arjuna.gandiva.inventory.DynamicInventory_2 [com.arjuna.ats.internal.arjuna.gandiva.inventory.DynamicInventory_2] -  could not find {0}
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
	return ArjunaNames.Implementation_Inventory_DynamicInventory();
    }

public static ClassName type ()
    {
	return ArjunaNames.Implementation_Inventory_DynamicInventory();
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
		    if (tsLogger.arjLogger.isWarnEnabled())
			tsLogger.arjLogger.warn(e.toString());
		}
		
		ptr = _staticInventory.find(className);
	    }
	    else
	    {
		if (tsLogger.arjLoggerI18N.isWarnEnabled())
		{
		    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.gandiva.inventory.DynamicInventory_1", 
						new Object[]{setupClass});
		}
	    }
	   
	    if (ptr == null)
	    {
		if (tsLogger.arjLoggerI18N.isWarnEnabled())
		{
		    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.gandiva.inventory.DynamicInventory_2", 
						new Object[]{className});
		}
	    }
	}  
  
	return ptr;
    }
	    
    private StaticInventory _staticInventory;
    
}
    
    
    
    
    
