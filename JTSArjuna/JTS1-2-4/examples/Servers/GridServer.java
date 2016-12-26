/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: GridServer.javatmpl,v 1.1 1998/11/12 13:32:24 nmcl Exp $
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

public class GridServer
{
    
public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();
	
	String gridReference = "/tmp/grid.ref";
	String markerName = "MyGrid";

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: GridServer [-marker <name>] [-reffile <file>] [-help]");
		System.exit(0);
	    }
	    if (args[i].compareTo("-reffile") == 0)
		gridReference = args[i+1];
	    if (args[i].compareTo("-marker") == 0)
	    {

		System.out.println("Sorry, marker name not supported by ORB.");
		System.exit(0);

	    }
	}

	grid_i myGrid = new grid_i(100, 100, markerName);

	ORBInterface.orb().connect( myGrid ) ;
	
	try
	{
	    ORBObject.objectToFile(myGrid, gridReference);

	    System.out.println("**Grid server started**");

	    ORBInterface.run();
	}
	catch (Exception e)
	{
	    System.err.println("GridServer caught exception: "+e);
	    System.exit(1);
	}

	ORBInterface.orb().disconnect( myGrid ) ;
	
	System.out.println("**Grid server exiting**");
    }
 
};
