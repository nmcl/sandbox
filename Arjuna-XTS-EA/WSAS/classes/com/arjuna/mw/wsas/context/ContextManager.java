/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ContextManager.java,v 1.1 2002/11/25 10:51:42 nmcl Exp $
 */

package com.arjuna.mw.wsas.context;

import com.arjuna.mw.wsas.UserActivity;
import com.arjuna.mw.wsas.UserActivityFactory;
import com.arjuna.mw.wsas.ActivityManager;
import com.arjuna.mw.wsas.ActivityManagerFactory;

import com.arjuna.mw.wsas.activity.HLS;
import com.arjuna.mw.wsas.activity.ActivityHierarchy;

/**
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ContextManager.java,v 1.1 2002/11/25 10:51:42 nmcl Exp $
 * @since 1.0.
 */

public class ContextManager
{

    public ContextManager ()
    {
	_manager = ActivityManagerFactory.activityManager();
    }
    
    public final Context[] contexts ()
    {
	Context[] ctxs = null;
	HLS[] services = null;

	try
	{
	    services = _manager.allHighLevelServices();
	}
	catch (Exception ex)
	{
	    System.err.println("ContextManager.contexts: "+ex);
	}
	
	if (services != null)
	{
	    /*
	     * Null entries are allowed and should be ignored.
	     */

	    ctxs = new Context[services.length];

	    try
	    {
		//		ActivityHierarchy ctx = _activity.currentActivity();

		/*
		 * Check for null or leave to hls? (leave to hls at
		 * the moment).
		 */

		for (int i = 0; i < services.length; i++)
		    ctxs[i] = services[i].context();
	    }
	    catch (Exception ex)
	    {
		System.err.println("ContextManager.contexts: "+ex);

		ex.printStackTrace();
		
		ctxs = null;
	    }
	}
	
	return ctxs;
    }

    private ActivityManager _manager;
    
}
