/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JNSSetup.java,v 1.1.32.1 2000/12/21 11:20:54 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Implementation.NameService;

import com.arjuna.ArjunaCommon.ArjunaCommonNames;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Base.InventoryElement;

/**
 * This class is essentially responsible for adding the JNS implementation
 * to the inventory. It is responsible for creating JNS instances when
 * required.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: JNSSetup.java,v 1.1.32.1 2000/12/21 11:20:54 nmcl Exp $
 * @since JTS 1.0.
 */

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
	return ArjunaCommonNames.Implementation_NameService_JNS();
    }
    
}
