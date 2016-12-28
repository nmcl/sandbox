/*
 * Copyright (C) 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Init.java,v 1.1.2.1 2000/07/27 16:12:18 ndbi Exp $
 */

package com.arjuna.CosRecovery.Creators.vbroker;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.CosTransactions.OTS_RecoveryCreator;

public class Init
{

    public Init ()
    {
	Visi3RecoveryCreator visi3RecoveryCreator = new Visi3RecoveryCreator();
	OTS_RecoveryCreator.setCreator(visi3RecoveryCreator);

	if (DebugController.enabled()) 
	    System.out.println("com.arjuna.CosRecovery.Creators.vbroker.Init: "
			       + "Visibroker RecoveryCoordinator creator setup");
    }
    
};


