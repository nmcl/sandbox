/*
 * Copyright (C) 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoveryManager.java,v 1.1.2.1.2.1.6.1.6.2 2001/09/21 15:14:07 nmcl Exp $
 */

package com.arjuna.CosRecovery;

import java.util.Hashtable;
import java.util.Properties;
import java.util.Enumeration;

import org.omg.CosTransactions.*;

import com.arjuna.CosRecovery.RecoveryCoordinators.*;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.CosTransactions.*;
import com.arjuna.OrbCommon.*;

/**
 * The RecoveryManager daemon
 */
public class RecoveryManager
{
    /**
     * Run the RecoveryManager. See Administration manual for details.
     */
    public static void main (String[] args)
    {
	boolean testMode = false;

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: com.arjuna.CosRecovery.RecoveryManager [-help] [-test] [-version]");
		System.exit(0);
	    }
	    if (args[i].compareTo("-version") == 0)
	    {
		System.out.println("CosServices version "+com.arjuna.CosTransactions.Configuration.version());
		System.exit(0);
	    }
	    if (args[i].compareTo("-test") == 0)
	    {
		testMode = true;
	    }
	}

	// tell the recovery enabler we aren't a normal TS-user
	RecoveryEnablement.isNotANormalProcess();

	try
	{
	    ORBInterface.initORB(args, null);
	    OAInterface.initOA();
	}
	catch (Exception e)
	{
	    ErrorStream.fatal().println("ORB/OA initialisation failed: "+e);

	    System.exit(0);
	}

	try
	{
	    RecoveryManagerImple theImple = new RecoveryManagerImple();
	
	    if (testMode)
		System.out.println("Ready");

	    ORBInterface.run();
	}
	catch (Throwable e)
	{
	    e.printStackTrace();
	}

	OAInterface.shutdownOA();
	ORBInterface.shutdownORB();
    }
    
    /** not used */
    private RecoveryManager()
    {
    }
    
}
