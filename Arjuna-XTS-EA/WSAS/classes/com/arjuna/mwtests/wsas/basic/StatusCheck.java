/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: StatusCheck.java,v 1.1 2002/11/25 10:51:47 nmcl Exp $
 */

package com.arjuna.mwtests.wsas.basic;

import com.arjuna.mw.wsas.UserActivity;
import com.arjuna.mw.wsas.UserActivityFactory;

import com.arjuna.mw.wsas.activity.Outcome;

import com.arjuna.mw.wsas.status.NoActivity;
import com.arjuna.mw.wsas.status.Active;

import com.arjuna.mw.wsas.completionstatus.Failure;

import com.arjuna.mw.wsas.exceptions.NoActivityException;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: StatusCheck.java,v 1.1 2002/11/25 10:51:47 nmcl Exp $
 * @since 1.0.
 */

public class StatusCheck
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    UserActivity ua = UserActivityFactory.userActivity();
	
	    if (ua.status() == NoActivity.instance())
	    {
		ua.start();
		
		if (ua.status() == Active.instance())
		{
		    Outcome res = ua.end();

		    if (res.completedStatus().equals(Failure.instance()))
			passed = true;
		    else
			System.out.println("End status: "+res.completedStatus());
		}
		else
		    System.out.println("Start status: "+ua.status());
	    }
	    else
		System.out.println("Create status: "+ua.status());
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
