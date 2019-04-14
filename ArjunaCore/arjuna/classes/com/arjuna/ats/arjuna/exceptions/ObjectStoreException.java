/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectStoreException.java,v 1.1 2003/06/19 10:50:19 nmcl Exp $
 */

package com.arjuna.ats.arjuna.exceptions;

import java.lang.Exception;

/**
 * Exception that can be thrown when attempting to access
 * the object store.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ObjectStoreException.java,v 1.1 2003/06/19 10:50:19 nmcl Exp $
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

