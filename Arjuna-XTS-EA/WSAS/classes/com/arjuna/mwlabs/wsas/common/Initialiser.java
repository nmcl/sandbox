/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Initialiser.java,v 1.1 2002/11/25 10:51:46 nmcl Exp $
 */

package com.arjuna.mwlabs.wsas.common;

import com.arjuna.mw.wsas.common.Environment;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: Initialiser.java,v 1.1 2002/11/25 10:51:46 nmcl Exp $
 * @since 1.0.
 */

public class Initialiser
{

    public static final synchronized void initialise ()
    {
	if (!_done)
	{
	    try
	    {
		String timeout = System.getProperty(Environment.ACTIVITY_TIMEOUT);
		
		try
		{
		    Integer l = new Integer(timeout);
		    _defaultTimeout = l.intValue();
		}
		catch (NumberFormatException e)
		{
		    System.err.println("Activity timeout format incorrect: "+e);
		}
	    }
	    catch (Exception ex)
	    {
	    }
	    
	    _done = true;
	}
    }

    private static boolean _done = false;
    private static int     _defaultTimeout = 0;
    
}

