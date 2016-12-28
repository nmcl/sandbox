/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: InterposedTransactionRecoveryModule.java,v 1.1.2.1 2000/07/27 16:12:08 ndbi Exp $
 */

package com.arjuna.CosRecovery;

import com.arjuna.ArjunaCommon.Common.*;

/**
 * This class is a plug-in module for the recovery manager.
 * It is responsible for the recovery of interposed transactions
 */

public class InterposedTransactionRecoveryModule implements RecoveryModule
{
    public InterposedTransactionRecoveryModule ()
    {
    }

    public void periodicWork ()
    {
	if (DebugController.enabled()) 
	    System.out.println("InterposedTransactionRecoveryModule.periodicWork()");
    }

    protected void initialise ()
    {
	if (DebugController.enabled()) 
	    System.out.println("InterposedTransactionRecoveryModule.initialise()");
    }
};


