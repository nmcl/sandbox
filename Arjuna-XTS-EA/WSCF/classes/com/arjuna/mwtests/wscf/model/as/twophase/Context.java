/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Context.java,v 1.1 2003/01/07 10:33:58 nmcl Exp $
 */

package com.arjuna.mwtests.wscf.model.as;

import com.arjuna.mw.wscf.UserCoordinator;
import com.arjuna.mw.wscf.UserCoordinatorFactory;

import com.arjuna.mw.wsas.activity.ActivityHierarchy;

import com.arjuna.mw.wsas.context.ContextManager;

import com.arjuna.mw.wsas.exceptions.NoActivityException;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: Context.java,v 1.1 2003/01/07 10:33:58 nmcl Exp $
 * @since 1.0.
 */

public class Context
{

    public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    UserCoordinator ua = UserCoordinatorFactory.userCoordinator();
	
	    ua.start();
	    
	    System.out.println("Started: "+ua.activityName());
	    
	    ua.start();

	    System.out.println("Started: "+ua.activityName());
	    
	    ContextManager manager = new ContextManager();
	    com.arjuna.mw.wsas.context.Context[] contexts = manager.contexts();

	    if (contexts != null)
	    {
		for (int i = 0; i < contexts.length; i++)
		    System.out.println(contexts[i]);
	    }

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
