/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ExpiredEntryMonitor.java,v 1.1.2.3 2001/02/15 21:37:00 pfurniss Exp $
 */

package com.arjuna.CosRecovery;

import java.util.Vector;
import java.util.Properties;
import java.util.Enumeration;
import java.util.Date;
import java.text.*;

import com.arjuna.ArjunaCommon.Common.*;

/**
 * Threaded object to run {@link ExpiryScanner} implementations to scan 
 * the action store to remove items deemed expired by some algorithm.
 * Performs a scan at interval defined by property EXPIRY_SCAN_INTERVAL (hours).
 * ExpiryScanner implementations are registered as properties beginning with
 * "ExpiryScanner".
 * <P>
 * Singleton, instantiated in the RecoveryManager. 
 * <P>
 * @version $Id: ExpiredEntryMonitor.java,v 1.1.2.3 2001/02/15 21:37:00 pfurniss Exp $
 */

public class ExpiredEntryMonitor extends Thread
{

    private static boolean		 _started = false;
    private static Vector		_expiryScanners = new Vector();
    private static int		    _scanIntervalSeconds = 12 * 60 * 60;
    private static SimpleDateFormat	_theTimestamper = new SimpleDateFormat("EEE, d MMM yyyy HH:mm:ss");
    private static boolean		notSkipping = true;
    private static final RecoveryClassLoader _classLoader = new RecoveryClassLoader();


    /**
     *  Start the monitor thread, if the properties make it appropriate
     */ 
     
    static boolean startUp()
    {
	// ensure singleton
	if (_started) {
	    return false;
	}
	
	/*
	 * Read the system properties to set the configurable options
	 */

	String scanIntervalString = PropertyManager.getProperty(RecoveryEnvironment.EXPIRY_SCAN_INTERVAL);

	if (scanIntervalString != null)
	{
	    try
	    {
		Integer scanIntervalInteger = new Integer(scanIntervalString);
		// convert to seconds
		_scanIntervalSeconds = scanIntervalInteger.intValue() * 60 *60;

		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
							 FacilityCode.FAC_CRASH_RECOVERY,
							 "Expiry scan interval set to "+_scanIntervalSeconds+" seconds");
		}
	    }
	    catch (NumberFormatException e)
	    {
		ErrorStream.warning().println(RecoveryEnvironment.EXPIRY_SCAN_INTERVAL
								+" has inappropriate value ("
								+scanIntervalString+")");
	    }
	}
	if (_scanIntervalSeconds == 0) {
	    // no scanning wanted
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
				     FacilityCode.FAC_CRASH_RECOVERY,
				     "Expiry scan zero - not scanning");
	    }
	    return false;
	}

	// is it being used to skip the first time
	if (_scanIntervalSeconds < 0)
	{
	    notSkipping = false;
	    _scanIntervalSeconds = - _scanIntervalSeconds;
	}

	loadScanners();
	
	if ( _expiryScanners.size() == 0) {
	    // nothing to do
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
				     FacilityCode.FAC_CRASH_RECOVERY,
				     "No Expiry scanners loaded - not scanning");
	    }
	    return false;
	}
	
	// create, and thus launch the monitor
	ExpiredEntryMonitor theInstance = new ExpiredEntryMonitor();
	
	return _started;
	    
    }
    
    private ExpiredEntryMonitor()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
				 FacilityCode.FAC_CRASH_RECOVERY,
				 "ExpiredEntryMonitor - constructed");
	}
	_started = true;
	start();
    }

    /**
     * Start the background thread to perform the periodic scans
     */
    public void run()
    {
	
	while(true)
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			 FacilityCode.FAC_RECOVERY_NORMAL,
			 "\n  --- ExpiredEntryMonitor ----" + _theTimestamper.format(new Date()) + "----");
	    }

	    if (notSkipping) {	    
		Enumeration scanners = _expiryScanners.elements();
		while (scanners.hasMoreElements())
		{
		    ExpiryScanner m = (ExpiryScanner) scanners.nextElement();
		    m.scan();
		    if (DebugController.enabled())
		    {
			// bit of space if detailing
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				 FacilityCode.FAC_CRASH_RECOVERY," ");			 
		    }
		}
	    } else {
		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			     FacilityCode.FAC_RECOVERY_NORMAL,
			     "ExpiredEntryMonitor - no scans on first iteration");
		}
		notSkipping = true;
	    }

	    // wait for a bit to avoid catching (too many) transactions etc. that
	    // are really progressing quite happily
	    try
	    {
		Thread.sleep(_scanIntervalSeconds*1000);
	    }
	    catch (java.lang.InterruptedException e1)
	    {
		break;
	    }
	    
	}
    }

    private static void loadScanners ()
    {

	// search our properties
	Properties properties = PropertyManager.getProperties();

	if (properties != null)
	{
	    Enumeration names = properties.propertyNames();
	    
	    while (names.hasMoreElements())
	    {
		String propertyName = (String) names.nextElement();

		if (propertyName.startsWith(RecoveryEnvironment.SCANNERPROPERTYPREFIX))
		    loadScanner(properties.getProperty(propertyName));
	    }
	}
    }

    private static void loadScanner (String className)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_CRASH_RECOVERY,
						 "Loading expiry scanner "+className);
	}

	if (className == null)
	{
	    ErrorStream.warning().println("Attempt to load expiry scanner with null class name!");
	    return;
	}
	else
	{
	    try
	    {
		Class c = _classLoader.loadClass(className);

		try
		{
		    ExpiryScanner m = (ExpiryScanner) c.newInstance();
		    if (m.toBeUsed()) {
			_expiryScanners.add( m);
		    } else {
			if (DebugController.enabled())
			{
			    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
								 FacilityCode.FAC_CRASH_RECOVERY,
								 "Not using expiry scanner "+className);
			}
		    }
		    
		}
		catch (ClassCastException e)
		{
		    ErrorStream.warning().println("Expiry scanner "+className
								    + " does not conform to ExpiryScanner interface");
		}
		catch (IllegalAccessException e1)
		{
		    ErrorStream.warning().println("Loading expiry scanner: "+e1);
		}
		catch (InstantiationException e2)
		{
		    ErrorStream.warning().println("Loading expiry scanner: "+e2);
		}

		c = null;
	    }
	    catch (ClassNotFoundException e)
	    {
		ErrorStream.warning().println("Loading expiry scanner: could not find class "+className);
	    }
	}
    }

    

}




