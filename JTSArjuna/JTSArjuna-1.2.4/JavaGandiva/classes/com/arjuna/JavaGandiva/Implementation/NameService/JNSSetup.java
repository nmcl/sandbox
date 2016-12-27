/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: JNSSetup.java,v 1.4 1998/07/06 13:30:59 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Implementation.NameService;

import com.arjuna.JavaGandiva.JavaGandivaNames;
import com.arjuna.JavaGandiva.Common.*;
import com.arjuna.JavaGandiva.Base.InventoryElement;

public class JNSSetup implements InventoryElement
{

public JNSSetup ()
    {
    }

public synchronized Object createVoid ()
    {
	return new JNS();
    }
    
public synchronized Object createString (String location)
    {
	return null;
    }
    
public synchronized Object createClassName (ClassName className)
    {
	return null;
    }
    
public synchronized Object createObjectName (ObjectName objectName)
    {
	return null;
    }
    
public synchronized Object createResources (Object[] resources)
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
	return JavaGandivaNames.Implementation_NameService_JNS();
    }
    
}
