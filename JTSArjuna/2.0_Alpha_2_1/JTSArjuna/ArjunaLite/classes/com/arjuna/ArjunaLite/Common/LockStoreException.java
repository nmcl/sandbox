/*
 * Copyright (C) 1996, 1997, 1998, 1999, 2000,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockStoreException.java,v 1.1 2000/02/25 14:03:59 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Common;

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

