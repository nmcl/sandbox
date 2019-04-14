/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectStoreError.java,v 1.1 2003/06/19 10:50:19 nmcl Exp $
 */

package com.arjuna.ats.arjuna.exceptions;

import java.lang.Error;

/**
 * Error that can be thrown when attempting to access
 * the object store.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ObjectStoreError.java,v 1.1 2003/06/19 10:50:19 nmcl Exp $
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

