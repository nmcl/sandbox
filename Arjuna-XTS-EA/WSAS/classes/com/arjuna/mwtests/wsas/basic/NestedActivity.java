/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: NestedActivity.java,v 1.1 2002/11/25 10:51:46 nmcl Exp $
 */

package com.arjuna.mwtests.wsas.basic;

import com.arjuna.mw.wsas.UserActivity;
import com.arjuna.mw.wsas.UserActivityFactory;

import com.arjuna.mw.wsas.exceptions.NoActivityException;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: NestedActivity.java,v 1.1 2002/11/25 10:51:46 nmcl Exp $
 * @since 1.0.
 */

public class NestedActivity
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    UserActivity ua = UserActivityFactory.userActivity();
	
	    ua.start();
	    
	    System.out.println("Started: "+ua.activityName());
	    
	    ua.start();

	    String nested = ua.activityName();
	    
	    System.out.println("Started: "+nested);

	    System.out.println("\nEnding: "+nested);
	    
	    ua.end();

	    String parent = ua.activityName();
	    
	    System.out.println("\nCurrent: "+parent);
	    
	    System.out.println("\nEnding: "+parent);
	    
	    ua.end();

	    System.out.println("\nCurrent: "+ua.activityName());

	    System.out.println("\nEnded.");
	    
	    if (ua.activityName() == null)
		passed = true;
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
