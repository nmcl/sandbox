/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: InventoryList.java,v 1.1 2000/02/25 14:02:48 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Implementation.Inventory;

import com.arjuna.ArjunaCommon.Base.InventoryElement;

/*
 * Default visibility.
 */

class InventoryList
{
    
public InventoryList (InventoryElement creator)
    {
	_instance = creator;
	_next = null;
    }
    
public InventoryElement _instance;
public InventoryList _next;
    
}
