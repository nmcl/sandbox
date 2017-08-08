/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StoreList.java,v 1.3 1998/07/06 13:26:48 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Implementation.ObjectStore;

import com.arjuna.JavaArjunaLite.Implementation.ObjectStoreImple;

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

public long useCount;
public ObjectStoreImple instance;
public StoreList next;
    
}
