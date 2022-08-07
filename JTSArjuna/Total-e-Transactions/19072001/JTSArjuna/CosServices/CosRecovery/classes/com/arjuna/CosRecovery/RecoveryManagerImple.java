/*
 * Copyright (C) 2001,
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoveryManagerImple.javatmpl,v 1.1.2.2.2.1.2.1.4.1 2001/08/07 11:11:22 nmcl Exp $
 */

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2.4.1.2.3.26.1.2.2.4.2 2001/02/02 11:51:52 nmcl Exp $
 */
































































package com.arjuna.CosRecovery;

import java.util.Hashtable;
import java.util.Properties;
import java.util.Enumeration;

import org.omg.CosTransactions.*;

import com.arjuna.CosRecovery.Admin.*;
import com.arjuna.CosRecovery.RecoveryCoordinators.*;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.CosTransactions.*;
import com.arjuna.OrbCommon.*;

import org.omg.CORBA.ORBPackage.InvalidName;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.SystemException;
import java.io.IOException;

/**
 * The RecoveryManagerImple - does the real work.
 * Currently we can have only one of these per node, so
 * each instance checks it's the only one running. If it isn't
 * it will kill itself before doing any work.
 *
 * @author Dave Ingham, Peter Furniss, Mark Little.
 * @since JTS 2.1.2.
 */

public class RecoveryManagerImple
{

    /**
     * Does the work of setting up crash recovery. The ORB
     * must have been initialised before this object is
     * created.
     */

public RecoveryManagerImple ()
    {

	
	String rmPropertyFile = RecoveryConfiguration.recoveryManagerPropertiesFile() ;
	
	PropertyManager.addPropertiesFile(rmPropertyFile);
	
	try
	{
	    PropertyManager.loadProperties(true);
	}
	catch (java.io.IOException ex)
	{
	    ErrorStream.warning().println(" property io exception " + ex);
	}
	
	// force normal recovery trace on
	DebugController.controller().mergeFacilityCode(FacilityCode.FAC_RECOVERY_NORMAL);

	// this next would force debugging on, but separate recovery mgr file makes this
	// unnecessary
	
	// DebugController.enable(true);
       
	// get the tag to go on the end of our service names
	String ourTag =  RecoveryEnablement.getRecoveryManagerTag();

	if (ourTag == null)
	{
	    ErrorStream.fatal().println("Not able to find RecoveryManagerTag or hostname - stopping");

	    throw new FatalError("Not able to find RecoveryManagerTag or hostname - stopping");
	}

	/*
	 * setup the inventory
	 */

	com.arjuna.CosTransactions.Implementation.Implementations.initialise();

	/*
	 * Check whether there is a recovery daemon running - only allow
	 * one per machine (currently!)
	 */

	if (activeRecoveryManager())
	    throw new FatalError("Recovery manager already active!");
	
	// create and register the admin interface

	// Create the admin object
	recManAdmin = new RecManAdmin();

	// Register it with the ORB
	OAInterface.objectIsReady( recManAdmin ) ;

	recManRef =  com.arjuna.CosRecovery.Admin.RecoveryManagerAdminHelper.narrow(OAInterface.corbaReference( recManAdmin )) ;

	// set the name for it
	String adminServiceName = RecoveryEnablement.getRecoveryAdminName();
	
	// Register it using the orbs default mechanism
	try 
	{
	    ORBServices.registerService(recManRef, adminServiceName,null);
	}
	catch (Exception e)
	{
	    ErrorStream.fatal().println("Recovery Manager caught exception registering RecManAdmin: "+e);
	    e.printStackTrace(ErrorStream.stream());

	    throw new FatalError("Recovery Manager caught exception registering RecManAdmin: "+e);
	}

	RecoveryEnablement.startRCservice(ourTag);
	
	// start the expiry scanners
	
	ExpiredEntryMonitor.startUp();
	
	// start the periodic recovery thread
	//  (don't start this until just about to go on the other stuff)
	PeriodicRecovery periodicRecovery = new PeriodicRecovery();

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_RECOVERY_NORMAL,
						 "RecoveryManagerImple (" + ourTag + ") is ready");
	}
    }

public void finalize ()
    {
	// destroy the implementation

	OAInterface.shutdownObject( recManRef ) ;
	recManAdmin = null;
    }
    
private final boolean activeRecoveryManager ()
    {
	// this must be aligned with what happens in RecoveryManager
	
	String recoveryManagerAdminName = RecoveryEnablement.getRecoveryAdminName();

	try
	{
	    org.omg.CORBA.Object obj = ORBServices.getService(recoveryManagerAdminName,
							      null);
	    
	    RecoveryManagerAdmin recManagerAdmin =  com.arjuna.CosRecovery.Admin.RecoveryManagerAdminHelper .narrow(obj);

	    recManagerAdmin.ping();

	    recManagerAdmin = null;
	    
	    return true;
	}
	catch (BAD_PARAM ex1)
	{
	    // Not a recovery manager IOR! Shouldn't happen!

	    return false;
	}
	catch (SystemException ex2)
	{
	    return false;
	}
	catch (InvalidName ex3)
	{
	    return false;
	}
	catch (IOException ex4)
	{
	    return false;
	}
    }

private RecManAdmin recManAdmin = null;
private com.arjuna.CosRecovery.Admin.RecoveryManagerAdmin recManRef;
    
}


