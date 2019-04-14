/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: InventoryList.java,v 1.1 2003/06/19 10:50:30 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.gandiva.inventory;

import com.arjuna.ats.arjuna.gandiva.inventory.InventoryElement;

/*
 * Default visibility.
 */

public class InventoryList
{
    
public InventoryList (InventoryElement creator)
    {
	_instance = creator;
	_next = null;
    }
    
public InventoryElement _instance;
public InventoryList _next;
    
}
