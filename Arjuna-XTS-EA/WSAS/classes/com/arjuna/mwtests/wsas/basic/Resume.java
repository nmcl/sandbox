/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Resume.java,v 1.1 2002/11/25 10:51:47 nmcl Exp $
 */

package com.arjuna.mwtests.wsas.basic;

import com.arjuna.mw.wsas.UserActivity;
import com.arjuna.mw.wsas.UserActivityFactory;

import com.arjuna.mw.wsas.activity.ActivityHierarchy;

import com.arjuna.mw.wsas.common.GlobalId;

import com.arjuna.mw.wsas.exceptions.NoActivityException;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: Resume.java,v 1.1 2002/11/25 10:51:47 nmcl Exp $
 * @since 1.0.
 */

public class Resume
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    UserActivity ua = UserActivityFactory.userActivity();
	    GlobalId ac1 = null;
	    GlobalId ac2 = null;
	    
	    ua.start();
	    
	    ac1 = ua.activityId();
	    
	    System.out.println("Started: "+ac1);
	    
	    ua.start();
	    
	    ac2 = ua.activityId();

	    System.out.println("\nStarted: "+ac2);
	    
	    ActivityHierarchy ctx = ua.suspend();
	    
	    System.out.println("\nSuspended: "+ctx);
	    
	    if (ua.currentActivity() == null)
	    {
		ua.resume(ctx);
		
		if (ac2.equals(ua.activityId()))
		{
		    ua.end();

		    if (ac1.equals(ua.activityId()))
		    {
			ua.end();
			
			passed = true;
		    }
		    else
			System.out.println("\nCurrent: "+ua.activityId()+" "+ac1);
		}
		else
		    System.out.println("\nCurrent: "+ua.activityId()+" "+ac2);
	    }
	    else
		System.out.println("\nCurrent: "+ua.currentActivity());
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
