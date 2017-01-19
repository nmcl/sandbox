/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Implementations.java,v 1.4 1998/07/20 09:08:25 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Implementation;

import com.arjuna.JavaGandiva.Interface.Inventory;
import com.arjuna.JavaGandiva.Implementation.NameService.JNSSetup;
import com.arjuna.JavaGandiva.Implementation.NameService.PNSSetup;
import com.arjuna.JavaGandiva.Base.InventoryElement;

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
