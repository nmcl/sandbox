/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectStoreException.java,v 1.1.4.2.6.1 2000/12/21 11:21:08 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Common;

import java.lang.Exception;

/**
 * Exception that can be thrown when attempting to access
 * the object store.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ObjectStoreException.java,v 1.1.4.2.6.1 2000/12/21 11:21:08 nmcl Exp $
 * @since JTS 1.0.
 */

public class ObjectStoreException extends Exception
{

public ObjectStoreException ()
    {
	super();
    }

public ObjectStoreException (String s)
    {
	super(s);
    }
    
}

