/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: BasicSuspendResume.java,v 1.1 2001/07/23 13:45:03 nmcl Exp $
 */

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.OrbCommon.OAInterface;
import com.arjuna.Activity.*;
import org.omg.CosActivity.*;
import java.io.*;

public class BasicSuspendResume
{
    
public static void main (String[] args)
    {
	try
	{
	    ORBInterface.initORB(args, null);
	    OAInterface.initOA();

	    org.omg.CosActivity.Current current = CosActivity.get_current();
	    PrintWriter strm = new PrintWriter(System.out);
	    
	    /*
	     * First a basic suspend without any active context.
	     */

	    ActivityToken token = current.suspend();
	    
	    if (token != null)
	    {
		System.err.println("Error - got token from null context!");
		
		System.exit(0);
	    }
	    
	    /*
	     * Now start a top-level activity and suspend it.
	     */

	    current.begin(0);
	    
	    System.out.println("Created activity: "+current.get_activity_name()+"\n");
	    
	    token = current.suspend();
	    
	    if (token == null)
	    {
		System.err.println("Error - did not get token for active context!");
		
		System.exit(0);
	    }
	    else
	    {
		System.out.println("Activity context is:");
		Utility.printToken(token, strm);
		strm.flush();
		System.out.println("\n");
	    }
	    
	    /*
	     * Check that we now have no activity associated with thread.
	     */

	    org.omg.CosActivity.Status status = current.get_status();
	    
	    if (status != org.omg.CosActivity.Status.StatusNoActivity)
	    {
		System.err.println("Error - activity still associated with thread. Status is "+Utility.getStatus(status));
		
		System.exit(0);
	    }

	    /*
	     * Now try to do a resume_all!
	     */

	    try
	    {
		current.resume_all(token);
		
		/*
		 * If here, then error!
		 */

		System.err.println("Error - managed to resume_all on a suspend token!");
		
		System.exit(0);
	    }
	    catch (InvalidToken e1)
	    {
		// ok!
	    }
	    catch (Exception e2)
	    {
		e2.printStackTrace();
		
		System.exit(0);
	    }
	    
	    /*
	     * Now try a resume!
	     */
	    
	    current.resume(token);

	    System.out.println("resumed activity: "+current.get_activity_name());

	    token = current.get_token();
		
	    System.out.println("\nActivity context is:");
	    Utility.printToken(token, strm);
	    strm.flush();
	    System.out.println("\n");
	    
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
