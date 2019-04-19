/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LockStoreException.java,v 1.1 2003/06/19 11:56:12 nmcl Exp $
 */

package com.arjuna.ats.txoj.exceptions;

import java.lang.Exception;

/**
 * Exception that can be thrown when attempting to access
 * the lock store.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: LockStoreException.java,v 1.1 2003/06/19 11:56:12 nmcl Exp $
 * @since JTS 1.0.
 */

public class LockStoreException extends Exception
{

public LockStoreException ()
    {
	super();
    }

public LockStoreException (String s)
    {
	super(s);
    }
    
}

