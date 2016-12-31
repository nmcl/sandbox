/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: InventoryElement.java,v 1.1 2000/02/25 14:02:31 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Base;

import com.arjuna.ArjunaCommon.Common.ClassName;
import com.arjuna.ArjunaCommon.Common.ObjectName;

public interface InventoryElement
{
    
public abstract Object createVoid ();
public abstract Object createClassName (ClassName className);
public abstract Object createObjectName (ObjectName objectName);
public abstract Object createResources (Object[] resources);
public abstract Object createClassNameResources (ClassName className, Object[] resources);
public abstract Object createObjectNameResources (ObjectName objectName, Object[] resources);

public abstract ClassName className ();
    
}
