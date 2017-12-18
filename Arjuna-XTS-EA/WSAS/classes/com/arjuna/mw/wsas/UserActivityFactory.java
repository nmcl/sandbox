/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: UserActivityFactory.java,v 1.4 2003/03/24 10:36:43 nmcl Exp $
 */

package com.arjuna.mw.wsas;

import com.arjuna.mwlabs.wsas.UserActivityImple;

import java.io.FileNotFoundException;

/**
 * Return the UserActivity implementation to use.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: UserActivityFactory.java,v 1.4 2003/03/24 10:36:43 nmcl Exp $
 * @since 1.0.
 */

public class UserActivityFactory
{
    
    /**
     * @return The UserActivity for the application to use.
     */

    public static UserActivity userActivity ()
    {
	return _imple;
    }

    private static UserActivityImple _imple = new UserActivityImple();

    static
    {
	try
	{
	    com.arjuna.mw.wsas.utils.Configuration.initialise("/config/wsas.xml");
	}
	catch (FileNotFoundException ex)
	{
	}
	catch (Exception ex)
	{
	    throw new ExceptionInInitializerError(ex.toString());
	}
    }
    
}

