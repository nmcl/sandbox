/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: BasicLockStoreSetup.java,v 1.1 2003/06/19 11:56:08 nmcl Exp $
 */

package com.arjuna.ats.internal.txoj.lockstore;

import com.arjuna.ats.txoj.TxOJNames;
import com.arjuna.ats.arjuna.gandiva.*;
import com.arjuna.ats.arjuna.gandiva.inventory.InventoryElement;

public class BasicLockStoreSetup implements InventoryElement
{

public BasicLockStoreSetup ()
    {
    }

public synchronized Object createVoid ()
    {
	return null;
    }
    
public synchronized Object createResources (Object[] param)
    {
	return BasicLockStore.create(param);
    }

public synchronized Object createClassName (ClassName className)
    {
	return null;
    }
    
public synchronized Object createObjectName (ObjectName objectName)
    {
	return null;
    }

public synchronized Object createClassNameResources (ClassName className, Object[] resources)
    {
	return null;
    }
    
public synchronized Object createObjectNameResources (ObjectName objectName, Object[] resources)
    {
	return null;
    }
    
public ClassName className ()
    {
	return TxOJNames.Implementation_LockStore_BasicLockStore();
    }
    
}
