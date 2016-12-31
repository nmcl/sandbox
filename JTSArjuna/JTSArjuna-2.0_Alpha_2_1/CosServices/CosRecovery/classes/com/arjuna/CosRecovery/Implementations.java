/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Implementations.java,v 1.1.2.1 2000/07/27 16:12:08 ndbi Exp $
 */

package com.arjuna.CosRecovery;

import com.arjuna.ArjunaCommon.Common.*;

import com.arjuna.ArjunaCommon.Interface.Inventory;

public class Implementations
{

public static synchronized boolean added ()
    {
	return _added;
    }
    
public static synchronized void initialise ()
    {
	if (!_added)
	{
	    if (DebugController.enabled())
		System.out.println("com.arjuna.CosRecovery.Implementations setup");

	    Inventory.inventory().addToList(new OTS_ResourceRecordSetup());

	    _added = true;
	}
    }

private Implementations ()
    {
    }

private static boolean _added = false;

    static
    {
	if (!_added)
	{
	    if (DebugController.enabled())
		System.out.println("com.arjuna.CosRecovery.Implementations setup");
	    
	    Inventory.inventory().addToList(new OTS_ResourceRecordSetup());

	    _added = true;
	}
    }
    
}
