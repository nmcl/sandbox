/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BasicSemaphoreSetup.java,v 1.5 1998/07/06 13:26:50 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Implementation.Semaphore;

import com.arjuna.JavaArjunaLite.JavaArjunaLiteNames;
import com.arjuna.JavaGandiva.Common.*;
import com.arjuna.JavaGandiva.Base.InventoryElement;

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
	return JavaArjunaLiteNames.Implementation_Semaphore_BasicSemaphore();
    }
    
}
