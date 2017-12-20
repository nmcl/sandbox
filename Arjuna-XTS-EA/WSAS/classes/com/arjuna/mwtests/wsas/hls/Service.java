/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Service.java,v 1.1 2002/11/25 10:51:47 nmcl Exp $
 */

package com.arjuna.mwtests.wsas.hls;

import com.arjuna.mw.wsas.UserActivity;
import com.arjuna.mw.wsas.UserActivityFactory;
import com.arjuna.mw.wsas.ActivityManagerFactory;
import com.arjuna.mw.wsas.ActivityManager;

import com.arjuna.mw.wsas.activity.ActivityHierarchy;

import com.arjuna.mw.wsas.exceptions.NoActivityException;

import com.arjuna.mwtests.wsas.common.DemoHLS;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: Service.java,v 1.1 2002/11/25 10:51:47 nmcl Exp $
 * @since 1.0.
 */

public class Service
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    UserActivity ua = UserActivityFactory.userActivity();
	    ActivityManagerFactory.activityManager().addHLS(new DemoHLS());
	    
	    ua.start();
	    
	    System.out.println("Started: "+ua.activityName());

	    ua.start();
	    
	    System.out.println("Started: "+ua.activityName());

	    ua.end();
	    
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
