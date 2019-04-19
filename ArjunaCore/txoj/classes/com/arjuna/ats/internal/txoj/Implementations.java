/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Implementations.java,v 1.1 2003/06/19 11:56:08 nmcl Exp $
 */

package com.arjuna.ats.internal.txoj;

import com.arjuna.ats.arjuna.gandiva.inventory.Inventory;
import com.arjuna.ats.internal.txoj.lockstore.BasicLockStoreSetup;
import com.arjuna.ats.internal.txoj.lockstore.BasicPersistentLockStoreSetup;
import com.arjuna.ats.internal.txoj.semaphore.BasicSemaphoreSetup;

/**
 * Module specific class that is responsible for adding any implementations
 * to the inventory.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Implementations.java,v 1.1 2003/06/19 11:56:08 nmcl Exp $
 * @since JTS 1.0.
 */

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
	    Inventory.inventory().addToList(new BasicLockStoreSetup());
	    Inventory.inventory().addToList(new BasicPersistentLockStoreSetup());
	    Inventory.inventory().addToList(new BasicSemaphoreSetup());	

	    _added = true;
	}
    }

private Implementations ()
    {
    }

private static boolean _added = false;

    static
    {
	initialise();
    }
    
}
