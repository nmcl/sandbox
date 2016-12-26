/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Implementations.java,v 1.1.28.2.4.1 2000/12/21 11:20:52 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Implementation;

import com.arjuna.ArjunaCommon.Interface.Inventory;
import com.arjuna.ArjunaCommon.Implementation.NameService.JNSSetup;
import com.arjuna.ArjunaCommon.Implementation.NameService.PNSSetup;
import com.arjuna.ArjunaCommon.Base.InventoryElement;

/**
 * This class automatically adds any necessary implementations to
 * the static inventory implementation.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Implementations.java,v 1.1.28.2.4.1 2000/12/21 11:20:52 nmcl Exp $
 * @since JTS 1.0.
 */

public class Implementations
{

    /**
     * Add the implementations to the static inventory.
     */

public static synchronized void initialise ()
    {
	Inventory.inventory().addToList(new JNSSetup());
	Inventory.inventory().addToList(new PNSSetup());

	added = true;
    }

private Implementations ()
    {
    }

private static boolean added = false;
    
}

