/*
 * Copyright (C) 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Visi3RecoveryInit.java,v 1.1.2.1.2.1 2001/02/06 15:40:25 pfurniss Exp $
 */

package com.arjuna.CosRecovery.RecoveryCoordinators.vbroker3;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.OrbCommon.OAAttribute;
import com.arjuna.CosTransactions.OTS_RecoveryCreator;
import com.arjuna.CosRecovery.RecoveryCoordinators.*;

/**
 * Initialises Visibroker RecoveryCoordinator creation subsystem
 * and provides the Visi-specific implementations of stuff
 *
 * An instance of this class is constructed via an
 * OrbPostInit mechanism
 *
 * All orbs are likely to be the same, constructing a GenericRecoveryCreator,
 * but with an orb-specific manager
 */
public class Visi3RecoveryInit extends OAAttribute
{
    public Visi3RecoveryInit ()
    {
    }
    
    public void initialise(String [] params)
    {
	
	RcvCoManager theManager = new Visi3RCManager();
	
	GenericRecoveryCreator.register(theManager);
    
	if (DebugController.enabled()) 
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				 FacilityCode.FAC_CRASH_RECOVERY,
				 "com.arjuna.CosRecovery.RecoveryCoordinators.vbroker3.Visi3RecoveryInit: "
				 + "Visibroker RecoveryCoordinator creator setup");
	}
    }
};


