/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SetGetServer.javatmpl,v 1.1.4.1 1999/02/05 09:57:41 nmcl Exp $
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

public class SetGetServer
{

public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();
    
	String markerName = null;
	String refFile = "/tmp/object.ref";
    
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-marker") == 0)

	    {
		System.err.println("Error - marker name not supported by ORB.");
		System.exit(0);
	    }

	    if (args[i].compareTo("-reffile") == 0)
		refFile = args[i+1];
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: SetGetServer [-marker <name>] [-reffile <file>] [-help]");
		System.exit(0);
	    }
	}

	setget_i impl = new setget_i();

	ORBInterface.orb().connect( impl ) ;
	
	try
	{
	    ORBObject.objectToFile(impl, refFile);
	    
	    System.out.println("**SetGet server started**");

	    ORBInterface.run();
	}
	catch (Exception e)
	{
	    System.err.println("SetGetServer caught exception: "+e);
	    System.exit(1);
	}

	ORBInterface.orb().disconnect( impl ) ;
    
	System.out.println("**Object server exiting**");
    }
    
};
