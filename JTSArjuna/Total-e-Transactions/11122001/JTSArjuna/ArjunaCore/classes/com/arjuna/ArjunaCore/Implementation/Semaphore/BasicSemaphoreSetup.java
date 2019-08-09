/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: BasicSemaphoreSetup.java,v 1.1.4.1 2000/10/16 16:00:33 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation.Semaphore;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
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
	return ArjunaCoreNames.Implementation_Semaphore_BasicSemaphore();
    }
    
}
