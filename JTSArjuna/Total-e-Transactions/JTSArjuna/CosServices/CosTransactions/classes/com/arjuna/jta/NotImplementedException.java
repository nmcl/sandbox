/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: NotImplementedException.java,v 1.1.28.1.4.1 2000/12/21 11:21:24 nmcl Exp $
 */

package com.arjuna.jta;

import java.lang.Exception;

/**
 * Exception may be thrown under certain circumstances.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: NotImplementedException.java,v 1.1.28.1.4.1 2000/12/21 11:21:24 nmcl Exp $
 * @since JTS 1.2.4.
 */

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

