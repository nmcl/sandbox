/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: NullEnd.java,v 1.1 2002/11/25 10:51:46 nmcl Exp $
 */

package com.arjuna.mwtests.wsas.basic;

import com.arjuna.mw.wsas.UserActivity;
import com.arjuna.mw.wsas.UserActivityFactory;

import com.arjuna.mw.wsas.exceptions.NoActivityException;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: NullEnd.java,v 1.1 2002/11/25 10:51:46 nmcl Exp $
 * @since 1.0.
 */

public class NullEnd
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    UserActivity ua = UserActivityFactory.userActivity();
	
	    ua.end();
	}
	catch (NoActivityException ex)
	{
	    passed = true;
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}
	
	if (passed)
	    System.out.println("\nPassed.");
	else
	    System.out.println("\nFailed.");
    }

}
