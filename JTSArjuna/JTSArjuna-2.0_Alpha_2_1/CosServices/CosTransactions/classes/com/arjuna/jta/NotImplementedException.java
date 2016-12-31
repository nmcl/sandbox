/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: NotImplementedException.java,v 1.1 2000/04/06 11:00:30 nmcl Exp $
 */

package com.arjuna.jta;

import java.lang.Exception;

public class NotImplementedException extends Exception
{

public NotImplementedException ()
    {
	super();
    }

public NotImplementedException (String s)
    {
	super(s);
    }
    
}

