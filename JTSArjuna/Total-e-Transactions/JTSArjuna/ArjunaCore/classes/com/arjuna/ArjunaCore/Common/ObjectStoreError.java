/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectStoreError.java,v 1.1.2.1.4.1 2000/12/21 11:21:08 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Common;

import java.lang.Error;

/**
 * Error that can be thrown when attempting to access
 * the object store.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ObjectStoreError.java,v 1.1.2.1.4.1 2000/12/21 11:21:08 nmcl Exp $
 * @since JTS 2.1.
 */

public class ObjectStoreError extends Error
{

public ObjectStoreError ()
    {
	super();
    }

public ObjectStoreError (String s)
    {
	super(s);
    }
    
}

