/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ExplicitStackServer.javatmpl,v 1.1 1998/11/12 13:32:23 nmcl Exp $
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

public class ExplicitStackServer
{
    
public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();
	
	String refFile = "/tmp/explicitstack.ref";
    
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-reffile") == 0)
		refFile = args[i+1];
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: ExplicitStackServer [-reffile <file>] [-help]");
		System.exit(0);
	    }
	}

	ExplicitStack theObject = new  TestModule._ExplicitStackImplBase_tie (new ExplicitStackImple());
	
	ORBInterface.orb().connect( theObject ) ;

	try
	{
	    ORBObject.objectToFile(theObject, refFile);
    
	    System.out.println("**ExplicitStackServer started**");
    
	    ORBInterface.run();
	}
	catch (Exception e)
	{
	    System.err.println("ExplicitStackServer caught exception: "+e);
	    System.exit(1);
	}

	ORBInterface.orb().disconnect( theObject ) ;
	
	System.out.println("**ExplicitStackServer exiting**");
    }

};
