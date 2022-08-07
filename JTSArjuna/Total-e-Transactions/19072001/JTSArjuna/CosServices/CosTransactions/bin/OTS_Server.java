/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_Server.java,v 1.4.28.1 2000/10/20 16:17:50 nmcl Exp $
 */

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.CosTransactions.*;
import com.arjuna.OrbCommon.*;
import org.omg.CosTransactions.*;

public class OTS_Server
{
    
public static void main (String[] args)
    {
	String refFile = ORBServices.transactionService;
	String objectName = null;
	boolean printReady = false;
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-otsname") == 0)
		objectName = args[i+1];
	    if (args[i].compareTo("-test") == 0)
		printReady = true;
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: OTS_Server [-otsname <name>] [-help] [-version]");
		System.exit(0);
	    }
	    if (args[i].compareTo("-version") == 0)
	    {
		System.out.println("CosServices version "+com.arjuna.CosTransactions.Configuration.version());
		System.exit(0);
	    }
	}

	OTS_Factory theOTS = null;
	int resolver = com.arjuna.OrbCommon.Configuration.bindDefault();
	String resolveService = PropertyManager.getProperty(ORBEnvironment.RESOLVE_SERVICE);

	if (resolveService != null)
	{
	    if (resolveService.compareTo("NAME_SERVICE") == 0)
		resolver = ORBServices.NAME_SERVICE;
	    else
	    {
		if (resolveService.compareTo("BIND_CONNECT") == 0)
		    resolver = ORBServices.BIND_CONNECT;
		else
		{
		    if (resolveService.compareTo("FILE") == 0)
			resolver = ORBServices.FILE;
		    else
		    {
			if (resolveService.compareTo("RESOLVE_INITIAL_REFERENCES") == 0)
			    resolver = ORBServices.RESOLVE_INITIAL_REFERENCES;
		    }
		}
	    }
	}
	
	try
	{
	    ORBInterface.initORB(args, null);
	    OAInterface.initOA();

	    theOTS = new OTS_Factory(objectName);

	    try
	    {
		if (resolver != ORBServices.BIND_CONNECT)
		{
		    String[] params = new String[1];

		    params[0] = ORBServices.otsKind;

		    /*
		     * Register using the default mechanism.
		     */

		    ORBServices.registerService(theOTS.getReference(), ORBServices.transactionService, params, resolver);

		    params = null;
		}

		if (!printReady)
		    System.out.println("Transaction manager registered.");
	    }
	    catch (Exception e1)
	    {
		System.err.println("Failed to bind transaction manager: "+e1);
		System.exit(0);
	    }

	    if (printReady)
		System.out.println("Ready");
	    else
		System.out.println("Arjuna OTS Server startup.");

	    if (resolver == ORBServices.BIND_CONNECT)
		ORBInterface.run(ORBServices.transactionService);
	    else
		ORBInterface.run();
	}
	catch (Exception e2)
	{
	    System.err.println("Caught exception "+e2);
	}
    
	System.out.println("Arjuna OTS Server shutdown");

	theOTS = null;

	OAInterface.shutdownOA();
	ORBInterface.shutdownORB();
    }

};
