/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: PeriodicRecovery.java,v 1.1.2.1 2000/07/27 16:12:09 ndbi Exp $
 */

package com.arjuna.CosRecovery;

import java.util.Hashtable;
import java.util.Properties;
import java.util.Enumeration;

import com.arjuna.ArjunaCommon.Common.*;

/**
 * Threaded object to perform the periodic recovery. Instantiated in
 * the RecoveryManager. The work is actually completed by the recovery
 * modules. These modules are dynamically loaded. The modules to load
 * are specified in the System properties.
 */

public class PeriodicRecovery extends Thread
{
    public PeriodicRecovery() 
    {
	// Get recovery period from system properties or use default.
	setupRecoveryPeriod();

	// Load the recovery modules that actually do the work.
	loadModules();

	start();
    }

    public void run()
    {
	while(true)
	{
	    if (DebugController.enabled())
		System.out.println("com.arjuna.CosRecovery.PeriodicRecovery"
				   +": Doing periodic recovery");

	    Enumeration modules = recoveryModules.keys();
	    while (modules.hasMoreElements())
	    {
		String moduleName = (String) modules.nextElement();

		RecoveryModule m = (RecoveryModule) recoveryModules.get(moduleName);
		m.periodicWork();
	    }
	    try
	    {
		Thread.sleep(recoveryPeriod);
	    }
	    catch (java.lang.InterruptedException e1)
	    {
		break;
	    }
	}
    }

    private static void setupRecoveryPeriod ()
    {
	// Get recovery period from system properties or use default
	recoveryPeriod = defaultRecoveryPeriod;
	Properties properties = System.getProperties();
	String recoveryPeriodString = properties.getProperty(recoveryPeriodPropertyKey);
	if (recoveryPeriodString != null)
	{
	    try
	    {
		Integer recoveryPeriodInteger = new Integer(recoveryPeriodString);
		recoveryPeriod = recoveryPeriodInteger.intValue();
		
		if (DebugController.enabled())
		    System.out.println("com.arjuna.CosRecovery.PeriodicRecovery"
				       +": Recovery period set to "+recoveryPeriod+" ms");
	    }
	    catch (NumberFormatException e)
	    {
		System.err.println(recoveryPeriodPropertyKey+" has inappropriate value ("+recoveryPeriodString+")");
	    }
	}
    }
    
    private static void loadModules ()
    {
	Properties properties = System.getProperties();

	if (properties != null)
	{
	    Enumeration names = properties.propertyNames();
	    
	    while (names.hasMoreElements())
	    {
		String attrName = (String) names.nextElement();

		if (attrName.indexOf(propertyName) != -1)
		    loadModule(properties.getProperty(attrName));
  	    }
	}
    }

    private static void loadModule (String className)
    {
	if (DebugController.enabled())
	    System.out.println("Loading recovery module "+className);
	
	if (className == null)
	{
	    System.err.println("Attempt to load recovery module with null class name!");
	    return;
	}
	else
	{
	    try
	    {
		Class c = classLoader.loadClass(className);

		try
		{
		    RecoveryModule m = (RecoveryModule) c.newInstance();
		    recoveryModules.put(className, m);
		}
		catch (ClassCastException e)
		{
		    System.err.println("Recovery module "+className+ " does not conform to RecoveryModule interface");
		}
		catch (IllegalAccessException e1)
		{
		    System.err.println("Loading recovery module: "+e1);
		}
		catch (InstantiationException e2)
		{
		    System.err.println("Loading recovery module: "+e2);
		}

		c = null;
	    }
	    catch (ClassNotFoundException e)
	    {
		System.err.println("Loading recovery module: warning - could not find class "+className);
	    }
	}
    }

    private static int recoveryPeriod = 0;
    private static String recoveryPeriodPropertyKey = "RecoveryPeriod";
    private static int defaultRecoveryPeriod = 60000;
    private static Hashtable recoveryModules = new Hashtable();
    private static String propertyName = "RecoveryExtension";
    private static final RecoveryClassLoader classLoader = new RecoveryClassLoader();
}




