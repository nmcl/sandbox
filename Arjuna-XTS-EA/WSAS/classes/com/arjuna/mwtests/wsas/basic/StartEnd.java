/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: StartEnd.java,v 1.1 2002/11/25 10:51:47 nmcl Exp $
 */

package com.arjuna.mwtests.wsas.basic;

import com.arjuna.mw.wsas.UserActivity;
import com.arjuna.mw.wsas.UserActivityFactory;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: StartEnd.java,v 1.1 2002/11/25 10:51:47 nmcl Exp $
 * @since 1.0.
 */

public class StartEnd
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    UserActivity ua = UserActivityFactory.userActivity();

	    ua.start();
	
	    ua.end();

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
