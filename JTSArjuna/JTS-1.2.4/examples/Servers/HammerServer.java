/*
 * Copyright (C) 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HammerServer.javatmpl,v 1.1.4.1 1999/01/28 19:04:25 nmcl Exp $
 */

/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability.h,v 1.4 1998/06/30 16:34:10 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2 1998/11/12 08:52:40 nmcl Exp $
 */













































import JavaCosTransactionsTests.*;
import TestModule.*;
import com.arjuna.OrbCommon.*;

public class HammerServer
{
    
public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();
	
	String refFile1 = "/tmp/hammer1.ref";
	String refFile2 = "/tmp/hammer2.ref";
	String refFile = null;

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-reffile") == 0)
		refFile = args[i+1];
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: HammerServer 1 | 2 [-reffile <file>] [-help]");
		System.exit(0);
	    }
	}
	
	if ((args.length == 1) && (refFile == null))
	{
	    if (args[0].compareTo("1") == 0)
		refFile = refFile1;
	    if (args[0].compareTo("2") == 0)
		refFile = refFile2;
	}

	if (refFile == null)
	{
	    System.out.println("Usage: HammerServer 1 | 2 [-reffile <file>] [-help]");
	    System.exit(0);
	}

	Hammer theObject = new  TestModule._HammerImplBase_tie (new HammerObject());

	ORBInterface.orb().connect( theObject ) ;

	try
	{
	    ORBObject.objectToFile(theObject, refFile);
    
	    System.out.println("**HammerServer started**");
	    System.out.println("\nIOR file: "+refFile);

	    ORBInterface.run();
	}
	catch (Exception e)
	{
	    System.err.println("HammerServer caught exception: "+e);
	    System.exit(1);
	}

	ORBInterface.orb().disconnect( theObject ) ;

	System.out.println("**HammerServer exiting**");
    }

};
