/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: InventoryImple.java,v 1.4 1998/07/06 13:30:55 nmcl Exp $
 */

package com.arjuna.JavaGandiva.Implementation;

import com.arjuna.JavaGandiva.Common.*;
import com.arjuna.JavaGandiva.Base.InventoryElement;
import com.arjuna.JavaGandiva.Implementation.Implementations;

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
