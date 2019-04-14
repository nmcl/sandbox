/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: InventoryImple.java,v 1.1 2003/06/19 10:50:20 nmcl Exp $
 */

package com.arjuna.ats.arjuna.gandiva.inventory;

import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.gandiva.*;
import com.arjuna.ats.internal.arjuna.Implementations;

import java.io.PrintStream;

/**
 * Inventory implementations must inherit from this base class
 * and provide implementations of the abstract methods.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: InventoryImple.java,v 1.1 2003/06/19 10:50:20 nmcl Exp $
 * @since JTS 1.0.
 */

public abstract class InventoryImple
{

    /**
     * Create a new implementation of the specified type using a default
     * constructor.
     */

public abstract Object createVoid (ClassName typeName);

    /**
     * Create a new implementation of the specified type, and pass its
     * constructor the ClassName parameter.
     */

public abstract Object createClassName (ClassName typeName, ClassName paramClassName);
    
    /**
     * Create a new implementation of the specified type, and pass its
     * constructor the ObjectName parameter.
     */

public abstract Object createObjectName (ClassName typeName, ObjectName paramObjectName);

    /**
     * Create a new implementation of the specified type, and pass its
     * constructor the array of Objects as the parameter.
     */

public abstract Object createResources (ClassName typeName, Object[] paramResources);

    /**
     * Create a new implementation of the specified type, and pass its
     * constructor the ClassName and array of Objects as the parameter.
     */

public abstract Object createClassNameResources (ClassName typeName, ClassName paramClassName,
						 Object[] paramResources);

    /**
     * Create a new implementation of the specified type, and pass its
     * constructor the ObjectName and array of Objects as the parameter.
     */

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
