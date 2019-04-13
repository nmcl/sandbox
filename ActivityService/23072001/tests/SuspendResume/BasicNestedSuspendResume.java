/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: BasicNestedSuspendResume.java,v 1.1 2001/07/23 13:45:03 nmcl Exp $
 */

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.OrbCommon.OAInterface;
import com.arjuna.Activity.*;
import org.omg.CosActivity.*;
import java.io.*;

public class BasicNestedSuspendResume
{
    
public static void main (String[] args)
    {
	try
	{
	    ORBInterface.initORB(args, null);
	    OAInterface.initOA();

	    org.omg.CosActivity.Current current = CosActivity.get_current();
	    PrintWriter strm = new PrintWriter(System.out);
	    ActivityToken token1 = null;
	    ActivityToken token2 = null;
	    
	    /*
	     * Start a nested activity and suspend it.
	     */

	    current.begin(0);
	    System.out.println("Created activity: "+current.get_activity_name()+"\n");

	    current.begin(0);
	    System.out.println("Created activity: "+current.get_activity_name()+"\n");

	    token1 = current.get_token();
	    
	    current.begin(0);
	    System.out.println("Created activity: "+current.get_activity_name()+"\n");

	    current.begin(0);
	    
	    System.out.println("Created activity: "+current.get_activity_name()+"\n");
	    
	    token2 = current.suspend();
	    
	    if (token2 == null)
	    {
		System.err.println("Error - did not get token for active context!");
		
		System.exit(0);
	    }
	    else
	    {
		System.out.println("Activity context is:");
		Utility.printToken(token2, strm);
		strm.flush();
		System.out.println("\n");
	    }
	    
	    /*
	     * Check that we still have an activity associated with thread.
	     */

	    org.omg.CosActivity.Status status = current.get_status();
	    
	    if (status == org.omg.CosActivity.Status.StatusNoActivity)
	    {
		System.err.println("Error - no activity associated with thread. Status is "+Utility.getStatus(status));
		
		System.exit(0);
	    }

	    /*
	     * Now try a resume!
	     */
	    
	    current.resume(token2);

	    System.out.println("resumed activity: "+current.get_activity_name());

	    token2 = current.get_token();
		
	    System.out.println("\nActivity context is:");
	    Utility.printToken(token2, strm);
	    strm.flush();
	    System.out.println("\n");

	    System.out.println("\nCompleting: "+current.get_activity_name());
	    
	    current.complete();

	    System.out.println("\nCompleting: "+current.get_activity_name());

	    current.complete();

	    try
	    {
		current.begin(0);

		System.out.println("\nCreated activity: "+current.get_activity_name());
	    
		current.resume(token1); // should fail!

		System.err.println("Error - was able to resume into wrong context!");

		System.exit(0);
	    }
	    catch (Exception e)
	    {
	    }

	    System.out.println("\nCompleting: "+current.get_activity_name());

	    current.complete();

	    System.out.println("\nCompleting: "+current.get_activity_name());
	    
	    current.complete();

	    System.out.println("\nCompleting: "+current.get_activity_name());

	    current.complete();
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	}

	OAInterface.shutdownOA();
	ORBInterface.shutdownORB();

	System.exit(0);
    }

};
