/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: PeriodicRecovery.java,v 1.1.2.1.4.3.4.1.20.1.4.1.2.2 2001/02/15 21:42:11 pfurniss Exp $
 */

package com.arjuna.CosRecovery;

import java.util.Vector;
import java.util.Properties;
import java.util.Enumeration;
import java.util.Date;
import java.text.*;

import com.arjuna.ArjunaCommon.Common.*;

/**
 * Threaded object to perform the periodic recovery. Instantiated in
 * the RecoveryManager. The work is actually completed by the recovery
 * modules. These modules are dynamically loaded. The modules to load
 * are specified by properties beginning with "RecoveryExtension"
 * <P>
 * @author Dave Ingham (dave@arjuna.com)
 * @version $Id: PeriodicRecovery.java,v 1.1.2.1.4.3.4.1.20.1.4.1.2.2 2001/02/15 21:42:11 pfurniss Exp $
 */

public class PeriodicRecovery extends Thread
{
    public PeriodicRecovery()
    {
	// Load the recovery modules that actually do the work.
	loadModules();

	start();
    }

    /**
     * Start the background thread to perform the periodic recovery
     */
    public void run()
    {
	while(true)
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			 FacilityCode.FAC_RECOVERY_NORMAL,
			 "\n  --- Periodic recovery - first pass ----" + _theTimestamper.format(new Date()) + "----");
	    }
	    
	    Enumeration modules = _recoveryModules.elements();
	    while (modules.hasMoreElements())
	    {
		RecoveryModule m = (RecoveryModule) modules.nextElement();
		m.periodicWorkFirstPass();
		if (DebugController.enabled())
		{
		    // bit of space if detailing
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			     FacilityCode.FAC_CRASH_RECOVERY," ");			 
		}
	    }
	    
	    // wait for a bit to avoid catching (too many) transactions etc. that
	    // are really progressing quite happily
	    try
	    {
		Thread.sleep(_backoffPeriod*1000);
	    }
	    catch (java.lang.InterruptedException e1)
	    {
		break;
	    }

	    if (DebugController.enabled()) 
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_RECOVERY_NORMAL,
				     "\n  --- Periodic recovery - second pass ----" + _theTimestamper.format(new Date()) + "----");

	    }
	    modules = _recoveryModules.elements();
	    while (modules.hasMoreElements())
	    {
		RecoveryModule m = (RecoveryModule) modules.nextElement();
		m.periodicWorkSecondPass();
		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			     FacilityCode.FAC_CRASH_RECOVERY," ");			 
		}
	    }	
	    
	    try
	    {
		Thread.sleep(_recoveryPeriod *1000);
	    }
	    catch (java.lang.InterruptedException e1)
	    {
		break;
	    }
	}
    }

    private static void loadModules ()
    {
	// scan the relevant properties so as to get them into sort order
	Properties properties = PropertyManager.getProperties();
	
	if (properties != null)
	{
	    Vector moduleNames = new Vector();
	    Enumeration names = properties.propertyNames();

	    while (names.hasMoreElements())
	    {
		String attrName = (String) names.nextElement();

		if (attrName.startsWith(RecoveryEnvironment.MODULEPROPERTYPREFIX) ) {
		    // this is one of ours - put it in the right place
		    int position = 0;
		    while ( position < moduleNames.size() &&
			attrName.compareTo( moduleNames.elementAt(position)) > 0 ) 
		    {
			position++;
		    }
		    moduleNames.add(position,attrName);
		}
	    }
	    // now go through again and load them
	    names = moduleNames.elements();
	    
	    while (names.hasMoreElements())
	    {
		String attrName = (String) names.nextElement();
		loadModule(properties.getProperty(attrName));
	    }

	}
    }

    private static void loadModule (String className)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "Loading recovery module "+className);
	}

	if (className == null)
	{
	    ErrorStream.warning().println("Attempt to load recovery module with null class name!");
	    return;
	}
	else
	{
	    try
	    {
		Class c = _classLoader.loadClass(className);

		try
		{
		    RecoveryModule m = (RecoveryModule) c.newInstance();
		    _recoveryModules.add( m);
		}
		catch (ClassCastException e)
		{
		    ErrorStream.warning().println("Recovery module "+className
								    + " does not conform to RecoveryModule interface");
		}
		catch (IllegalAccessException e1)
		{
		    ErrorStream.warning().println("Loading recovery module: "+e1);
		}
		catch (InstantiationException e2)
		{
		    ErrorStream.warning().println("Loading recovery module: "+e2);
		}

		c = null;
	    }
	    catch (ClassNotFoundException e)
	    {
		ErrorStream.warning().println("Loading recovery module: could not find class "+className);
	    }
	}
    }

    private static Vector		 _recoveryModules = new Vector();
    private static final RecoveryClassLoader _classLoader = new RecoveryClassLoader();
    private static int		       _defaultRecoveryPeriod = 120;
    private static int		       _recoveryPeriod = 0;
    private static int			 _defaultBackoffPeriod = 10;
    private static int			_backoffPeriod = 0;
    private static SimpleDateFormat	 _theTimestamper = new SimpleDateFormat("EEE, d MMM yyyy HH:mm:ss");

    /*
     * Read the system properties to set the configurable options
     */
    static
    {
	_recoveryPeriod = _defaultRecoveryPeriod;

	String recoveryPeriodString = PropertyManager.getProperty(RecoveryEnvironment.PERIODIC_RECOVERY_PERIOD);

	if (recoveryPeriodString != null)
	{
	    try
	    {
		Integer recoveryPeriodInteger = new Integer(recoveryPeriodString);
		_recoveryPeriod = recoveryPeriodInteger.intValue() ;

		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
							 FacilityCode.FAC_CRASH_RECOVERY,
							 "com.arjuna.CosRecovery.PeriodicRecovery"
							 +": Recovery period set to "+_recoveryPeriod+" seconds");
		}
	    }
	    catch (NumberFormatException e)
	    {
		ErrorStream.warning().println(RecoveryEnvironment.PERIODIC_RECOVERY_PERIOD
								+" has inappropriate value ("
								+recoveryPeriodString+")");
	    }
	}

	_backoffPeriod = _defaultBackoffPeriod;

	String backoffPeriodString=
	    PropertyManager.getProperty(RecoveryEnvironment.RECOVERY_BACKOFF_PERIOD);

	if (backoffPeriodString != null)
	{
	    try
	    {
		Integer backoffPeriodInteger = new Integer(backoffPeriodString);
		_backoffPeriod = backoffPeriodInteger.intValue();
		
		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
							 FacilityCode.FAC_CRASH_RECOVERY,
							 "com.arjuna.CosRecovery.PeriodicRecovery"
							 +": Backoff period set to "+_backoffPeriod+" seconds");
		}
	    }
	    catch (NumberFormatException e)
	    {
		ErrorStream.warning().println(RecoveryEnvironment.RECOVERY_BACKOFF_PERIOD 
								+" has inappropriate value ("
								+backoffPeriodString+")");
	    }
	}
    }


}




