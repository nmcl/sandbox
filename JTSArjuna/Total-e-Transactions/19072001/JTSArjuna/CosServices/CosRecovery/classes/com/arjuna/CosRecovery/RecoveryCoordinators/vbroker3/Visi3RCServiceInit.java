/*
 * Copyright (C) 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Visi3RCServiceInit.java,v 1.1.2.2 2001/02/06 16:10:34 pfurniss Exp $
 */

package com.arjuna.CosRecovery.RecoveryCoordinators.vbroker3;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.CosTransactions.OTS_RecoveryCreator;
import com.arjuna.CosRecovery.RecoveryCoordinators.*;

import org.omg.CORBA.*;
import org.omg.CosTransactions.*;


/**
 * Initialises Visibroker3.4 RecoveryCoordinator creation subsystem
 *
 */
public class Visi3RCServiceInit 
{
	
private Visi3RCServiceInit()
{
}

/** this starts the service in the RecoveryManagaer
 */
public static boolean startRCservice ()
{
    String rcServiceName = GenericRecoveryCreator.getRecCoordServiceName();

    if (DebugController.enabled())
    {
	DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
			 FacilityCode.FAC_CRASH_RECOVERY,
			 " Starting activator with service name " + rcServiceName);
    }

    // register in a non-blocking way    
    OAInterface.boa().impl_is_ready(rcServiceName, 
	    new com.arjuna.CosRecovery.RecoveryCoordinators.vbroker3.Visi3RCActivator(),
	    false);

    return true;
    
}


};


