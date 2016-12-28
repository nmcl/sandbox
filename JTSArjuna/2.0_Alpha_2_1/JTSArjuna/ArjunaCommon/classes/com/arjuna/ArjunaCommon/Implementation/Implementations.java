/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Implementations.java,v 1.1 2000/02/25 14:02:44 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Implementation;

import com.arjuna.ArjunaCommon.Interface.Inventory;
import com.arjuna.ArjunaCommon.Implementation.NameService.JNSSetup;
import com.arjuna.ArjunaCommon.Implementation.NameService.PNSSetup;
import com.arjuna.ArjunaCommon.Base.InventoryElement;

public class Implementations
{

public static synchronized void initialise ()
    {
	Inventory.inventory().addToList(new JNSSetup());
	Inventory.inventory().addToList(new PNSSetup());
    }

private Implementations ()
    {
    }
    
};
