/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OutOfOrderActivity.java,v 1.1 2002/04/22 14:04:17 nmcl Exp $
 */

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.OrbCommon.OAInterface;
import com.arjuna.Activity.*;
import org.omg.CosActivity.*;
import java.io.*;

/*
 * Try to terminate activities out of order.
 */

public class OutOfOrderActivity
{
    
public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    ORBInterface.initORB(args, null);
	    OAInterface.initOA();

	    org.omg.CosActivity.Current current = CosActivity.get_current();
	    org.omg.CosActivity.ActivityCoordinator rootCoord = null;
	    org.omg.CosActivity.ActivityCoordinator childCoord = null;
	    
	    current.begin(0);
	    System.out.println("Created activity: "+current.get_activity_name()+"\n");

	    rootCoord = current.get_coordinator();
	    
	    current.begin(0);
	    System.out.println("Created activity: "+current.get_activity_name()+"\n");

	    childCoord = current.get_coordinator();
	    
	    if (!current.get_parent_coordinator().is_same_activity(rootCoord))
	    {
		System.out.println("Parent not the root!");

		System.exit(0);
	    }
	    else
		System.out.println("Parent is the root.");

	    System.out.println("\nTrying to terminate root with running child.");
	    
	    try
	    {
		rootCoord.complete_activity(null, CompletionStatus.CompletionStatusSuccess);
	    }
	    catch (ChildContextPending ex)
	    {
		passed = true;
	    }
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	}

	OAInterface.shutdownOA();
	ORBInterface.shutdownORB();

	if (passed)
	    System.out.println("\nPassed.");
	else
	    System.out.println("\nFailed.");
	
	System.exit(0);
    }

};
