/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockStoreException.java,v 1.3 1998/07/06 13:26:34 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Common;

import java.lang.Exception;

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

