/*
 * Copyright (C) 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Orbix2kRecoveryInit.java,v 1.1.2.1.50.1 2001/08/07 11:11:22 nmcl Exp $
 */

package com.arjuna.CosRecovery.RecoveryCoordinators.orbix2000;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.CosTransactions.OTS_RecoveryCreator;
import com.arjuna.CosRecovery.RecoveryCoordinators.*;

import org.omg.CORBA.*;
import org.omg.PortableServer.*;
import org.omg.CosTransactions.*;

/**
 * Initialises Orbix2000 RecoveryCoordinator IOR creation mechanism
 *
 * An instance of this class is constructed by RecoveryEnablement and 
 * registered as an OAAttribute whose initialise method is called after
 * root POA is set up
 *
 * All orbs are likely to be the same, constructing a GenericRecoveryCreator,
 * but with an orb-specific manager
 */
public class Orbix2kRecoveryInit extends OAAttribute
{
    public Orbix2kRecoveryInit ()
    {
    }

public void initialise(String [] params)
{
    // make a orbix-style manager for the RC IOR creation
    RcvCoManager theManager = new Orbix2kRCManager();
	
    // and register it (which will cause creation of a GenericRecoveryCreator
    // and it's registration with CosTransactions)
    GenericRecoveryCreator.register(theManager);

    if (DebugController.enabled()) 
    {
	DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					     FacilityCode.FAC_CRASH_RECOVERY,
					     "com.arjuna.CosRecovery.RecoveryCoordinators.Orbix2000.Orbix2kRecoveryInit: "
					     + "Orbix2000 RecoveryCoordinator creator setup");
    }
}
    
};


