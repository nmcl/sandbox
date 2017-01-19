/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: InventoryElement.java,v 1.3 1998/07/06 13:30:43 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Base;

import com.arjuna.JavaGandiva.Common.ClassName;
import com.arjuna.JavaGandiva.Common.ObjectName;

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
