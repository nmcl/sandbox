/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK. 
 *
 * $Id: UidException.java,v 1.1 2000/02/25 14:02:40 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

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
