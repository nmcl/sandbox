/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: BasicSemaphoreSetup.java,v 1.1 2003/06/19 11:56:09 nmcl Exp $
 */

package com.arjuna.ats.internal.txoj.semaphore;

import com.arjuna.ats.txoj.TxOJNames;
import com.arjuna.ats.txoj.common.*;
import com.arjuna.ats.arjuna.gandiva.ClassName;
import com.arjuna.ats.arjuna.gandiva.ObjectName;
import com.arjuna.ats.arjuna.gandiva.inventory.InventoryElement;

public class BasicSemaphoreSetup implements InventoryElement
{

public BasicSemaphoreSetup ()
    {
    }

public void finalize ()
    {
    }

public synchronized Object createVoid ()
    {
	return null;
    }
    
public synchronized Object createResources (Object[] param)
    {
	return BasicSemaphore.create(param);
    }

public synchronized Object createClassName (ClassName className)
    {
	return null;
    }
    
public synchronized Object createObjectName (ObjectName objectName)
    {
	return null;
    }
    
public Object createClassNameResources (ClassName className, Object[] resources)
    {
	return null;
    }
    
public Object createObjectNameResources (ObjectName objectName, Object[] resources)
    {
	return null;
    }
    
public ClassName className ()
    {
	return TxOJNames.Implementation_Semaphore_BasicSemaphore();
    }
    
}
