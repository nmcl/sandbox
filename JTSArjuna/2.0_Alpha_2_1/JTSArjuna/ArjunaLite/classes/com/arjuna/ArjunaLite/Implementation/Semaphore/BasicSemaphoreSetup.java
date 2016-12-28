/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: BasicSemaphoreSetup.java,v 1.1 2000/02/25 14:04:15 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Implementation.Semaphore;

import com.arjuna.ArjunaLite.ArjunaLiteNames;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Base.InventoryElement;

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
	return ArjunaLiteNames.Implementation_Semaphore_BasicSemaphore();
    }
    
}
