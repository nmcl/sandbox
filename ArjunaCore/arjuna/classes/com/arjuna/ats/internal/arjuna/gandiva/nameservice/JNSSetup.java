/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JNSSetup.java,v 1.1 2003/06/19 10:50:30 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.gandiva.nameservice;

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.gandiva.*;
import com.arjuna.ats.arjuna.gandiva.inventory.*;

/**
 * This class is essentially responsible for adding the JNS implementation
 * to the inventory. It is responsible for creating JNS instances when
 * required.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: JNSSetup.java,v 1.1 2003/06/19 10:50:30 nmcl Exp $
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
	return ArjunaNames.Implementation_NameService_JNS();
    }
    
}
