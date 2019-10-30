/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: InventoryElement.java,v 1.1.34.1 2000/12/21 11:20:47 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Base;

import com.arjuna.ArjunaCommon.Common.ClassName;
import com.arjuna.ArjunaCommon.Common.ObjectName;

/**
 * Implementations which are to be made available to the Inventory can
 * be created using one of the methods provided by implementations of
 * this interface. This enables the Inventory to not have to know about
 * implementation specific.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: InventoryElement.java,v 1.1.34.1 2000/12/21 11:20:47 nmcl Exp $
 * @since JTS 1.0.
 */

public interface InventoryElement
{
    
    /**
     * Create an implementation using a default constructor.
     */

public abstract Object createVoid ();

/**
 * Create an implementation and pass the constructor the ClassName parameter.
 */
 
public abstract Object createClassName (ClassName className);

/**
 * Create an implementation and pass the constructor the ObjectName parameter.
 */
 
public abstract Object createObjectName (ObjectName objectName);

/**
 * Create an implementation and pass the constructor the array of Objects.
 */
 
public abstract Object createResources (Object[] resources);

/**
 * Create an implementation and pass the constructor the ClassName
 * and array of Objects.
 */
 
public abstract Object createClassNameResources (ClassName className, Object[] resources);

/**
 * Create an implementation and pass the constructor the ObjectName and
 * array of Objects.
 */
 
public abstract Object createObjectNameResources (ObjectName objectName, Object[] resources);

/**
 * Return the ClassName of the implementation to be created.
 */
 
public abstract ClassName className ();
    
}
