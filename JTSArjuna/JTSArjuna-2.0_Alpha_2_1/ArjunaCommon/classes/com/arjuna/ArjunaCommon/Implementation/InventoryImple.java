/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: InventoryImple.java,v 1.1 2000/02/25 14:02:45 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Implementation;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Base.InventoryElement;
import com.arjuna.ArjunaCommon.Implementation.Implementations;

import java.io.PrintStream;

public abstract class InventoryImple
{

public abstract Object createVoid (ClassName typeName);
public abstract Object createClassName (ClassName typeName, ClassName paramClassName);
public abstract Object createObjectName (ClassName typeName, ObjectName paramObjectName);
public abstract Object createResources (ClassName typeName, Object[] paramResources);
public abstract Object createClassNameResources (ClassName typeName, ClassName paramClassName,
						 Object[] paramResources);
public abstract Object createObjectNameResources (ClassName typeName,
						  ObjectName paramObjectName,
						  Object[] paramResources);
    
public abstract void addToList (InventoryElement creator);
public abstract void printList (PrintStream toUse);

public ClassName className ()
    {
	return InventoryImple._className;
    }

public static ClassName type ()
    {
	return InventoryImple._className;
    }
    
private static final ClassName _className = new ClassName("InventoryImple");
    
}
