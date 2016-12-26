/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StackServer.javatmpl,v 1.1 1998/11/12 13:32:28 nmcl Exp $
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

public class StackServer
{
    
public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();
    
	String refFile = "/tmp/stack.ref";
    
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

	stack theStack = new  TestModule._stackImplBase_tie (new StackImple());

	ORBInterface.orb().connect( theStack ) ;

	try
	{
	    ORBObject.objectToFile(theStack, refFile);
    
	    System.out.println("**StackServer started**");
    
	    ORBInterface.run();
	}
	catch (Exception e)
	{
	    System.err.println("StackServer caught exception: "+e);
	    System.exit(1);
	}

	ORBInterface.orb().disconnect( theStack ) ;
	
	System.out.println("**StackServer exiting**");
    }

};
