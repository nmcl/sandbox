/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: StoreList.java,v 1.1.4.1 2000/10/16 16:00:32 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation.ObjectStore;

import com.arjuna.ArjunaCore.Implementation.ObjectStoreImple;

/*
 * Default visibility.
 */

class StoreList
{

public StoreList ()
    {
	useCount = 0;
	instance = null;
	next = null;
    }

public long             useCount;
public ObjectStoreImple instance;
public StoreList        next;
    
}
