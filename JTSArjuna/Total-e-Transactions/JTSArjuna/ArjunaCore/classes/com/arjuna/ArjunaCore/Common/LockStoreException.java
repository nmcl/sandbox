/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LockStoreException.java,v 1.1.4.2.6.1 2000/12/21 11:21:07 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Common;

import java.lang.Exception;

/**
 * Exception that can be thrown when attempting to access
 * the lock store.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: LockStoreException.java,v 1.1.4.2.6.1 2000/12/21 11:21:07 nmcl Exp $
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

