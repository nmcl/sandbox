/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: RecoveryEnablement.javatmpl,v 1.1.2.3.2.3.50.1 2001/08/07 11:11:21 nmcl Exp $
 */

package com.arjuna.CosRecovery;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCommon.Common.FatalError;
import com.arjuna.OrbCommon.*;
import java.net.InetAddress;
import java.net.UnknownHostException;

import com.arjuna.CosRecovery.RecoveryCoordinators.GenericRecoveryCreator;

/**
 * Registers the appropriate classes with the ORB.
 *   An application using the Transaction Service should load an instance of this class
 *   prior to orb-initialisation.
 *   Loading an instance can be achieved by naming the class in an OrbPreInit
 *   property.
 *    <p>Orb-specific details of recovery are handled by this class.
 *  <p>
 *  The class also includes the static startRCservice method (package access),
 *  used by the RecoveryManager, which is orb-specific
 *  
 * @author Peter Furniss (peter.furniss@arjuna.com)
 * @version $Id: RecoveryEnablement.javatmpl,v 1.1.2.3.2.3.50.1 2001/08/07 11:11:21 nmcl Exp $
 * @since JTS 2.1.
 */

public class RecoveryEnablement
{

private static boolean _isNormalProcess = true;
private static String  _RecoveryManagerTag;

    // no accessible variable for this first property name prefix
private static final String postInitPropertyPrefix = "ORBPostInit";
private static final String eventHandlerPropertyPrefix = ORBEnvironment.EVENT_HANDLER;

    /** 
     * Constructor does the work as a result of being registered as an ORBPreInit
     * class
     */
public RecoveryEnablement ()
    {
	String postInitPropertyName = postInitPropertyPrefix + "_Recovery";
	String postInitPropertyValue = null;
	
	// the eventhandler is the same for all orbs (at the moment)
	String eventHandlerPropertyName = eventHandlerPropertyPrefix + "_Recovery";
	String eventHandlerPropertyValue = "com.arjuna.CosRecovery.Contact.RecoveryContactWriter";

	OAAttribute recoveryCoordinatorInitialiser = null;







	recoveryCoordinatorInitialiser = new com.arjuna.CosRecovery.RecoveryCoordinators.orbix2000.Orbix2kRecoveryInit();
	


	/*
	 * Only add if we are not the recovery daemon!
	 * Why create in the first place then?!
	 */


	if (_isNormalProcess)
	    OAInterface.addOAAttribute(recoveryCoordinatorInitialiser);


	if ( _isNormalProcess) 
	{
	    // register the ContactWriter to watch for the first ArjunaFactory construction
	    PropertyManager.setProperty(eventHandlerPropertyName,eventHandlerPropertyValue);
	}
    
	if (DebugController.enabled())
	{

	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				 FacilityCode.FAC_CRASH_RECOVERY,
				 "added ORBAttribute for recoveryCoordinatorInitialiser");

	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				 FacilityCode.FAC_CRASH_RECOVERY,
				 "Set property " + eventHandlerPropertyName + "=" + eventHandlerPropertyValue);
	}
    }

/**
 *  Used by the RecoveryManager to start the recoverycoordinator 
 *  service, using whatever orb-specific techniques are appropriate.
 *  This is placed here because it may need to set post-orbinitialisation 
 *  properties, like the regular enabler
 */
 
static boolean startRCservice(String recoveryManagerTag)
{
    if (_isNormalProcess) {
	ErrorStream.warning().println(" RecoveryCoordinator service can only be provided in RecoveryManager");
	return false;
    }







    return com.arjuna.CosRecovery.RecoveryCoordinators.orbix2000.Orbix2kRCServiceInit.startRCservice();



    
}


/**
 * This static method is used by the RecoveryManager to suppress 
 * aspects of recovery enablement in it's own
 * process, without requiring further property manipulations
 */
static void isNotANormalProcess() 
{
    _isNormalProcess = false;
}

static boolean isNormalProcess ()
{
    return _isNormalProcess;
}

static
    {
	// see if there is a property defining the recoverymanager
	// servicename
		
	_RecoveryManagerTag = PropertyManager.getProperty(RecoveryEnvironment.RECOVERY_MANAGER_TAG);
	
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			     FacilityCode.FAC_CRASH_RECOVERY,
			     "Property " + RecoveryEnvironment.RECOVERY_MANAGER_TAG + " is " + 
			     _RecoveryManagerTag);
	}
	
	if (_RecoveryManagerTag == null)
	{
	    // if no property, use the hostname/ip address

	    InetAddress thisAddress = null;
	    try
	    {
		thisAddress = InetAddress.getLocalHost();
		_RecoveryManagerTag = thisAddress.getHostName();
		if (DebugController.enabled())
		{
		    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_CRASH_RECOVERY,
				     "Setting RecoveryManagerTag from hostname as " +
				     _RecoveryManagerTag);
		}
	    }
	    catch (UnknownHostException uhe)
	    {
		ErrorStream.warning().println("GenericRecoveryCreator: "
								+"Could not get the local IP address");
	    }
	    
	}
	// prune off any spaces
	if (_RecoveryManagerTag != null)
	{
	    _RecoveryManagerTag =_RecoveryManagerTag.trim();
	}
    }

/**
 * Return the RecoveryManager tag. This can be set by a property.
 */
public static String getRecoveryManagerTag()
{
    if (_RecoveryManagerTag != null) {
	return _RecoveryManagerTag;
    } else {
	ErrorStream.warning().println(" RecoveryManagerTag is null");
	return null;
    }
}

/**
 *  Get the name of the Recovery Admin Service, used by the RecoveryManagerPinger
 */
public static String getRecoveryAdminName()
{
    if (_RecoveryManagerTag != null) {
	return RecoveryEnvironment.ADMINSERVICENAME + "-" + _RecoveryManagerTag;
    } else {
	ErrorStream.warning().println(" RecoveryManagerTag is null - admin service name has no tag");
	return RecoveryEnvironment.ADMINSERVICENAME;
    }
}

}
