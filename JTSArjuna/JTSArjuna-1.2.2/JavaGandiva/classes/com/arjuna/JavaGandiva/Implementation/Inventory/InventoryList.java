/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: InventoryList.java,v 1.3 1998/07/06 13:30:57 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Implementation.Inventory;

import com.arjuna.JavaGandiva.Base.InventoryElement;

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
