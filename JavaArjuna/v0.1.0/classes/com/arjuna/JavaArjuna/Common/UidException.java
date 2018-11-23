/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: UidException.java,v 1.2 1998/07/06 13:30:51 nmcl Exp $
 */

package com.arjuna.JavaArjuna.Common;

import java.lang.Exception;

public class UidException extends Exception
{

public UidException ()
    {
	super();
    }

public UidException (String s)
    {
	super(s);
    }
}
