/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectStoreException.java,v 1.1 2000/02/25 14:04:00 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Common;

import java.lang.Exception;

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

