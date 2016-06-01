/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: BasicReaper.java,v 1.1 2001/07/23 13:45:04 nmcl Exp $
 */

import com.arjuna.ArjunaCommon.Common.PropertyManager;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.OrbCommon.OAInterface;
import com.arjuna.Activity.*;
import org.omg.CosActivity.*;
import java.io.*;

import org.omg.CORBA.BAD_OPERATION;

public class BasicReaper
{
    
public static void main (String[] args)
    {
	try
	{
	    ORBInterface.initORB(args, null);
	    OAInterface.initOA();

	    org.omg.CosActivity.Current current = CosActivity.get_current();
	    PrintWriter strm = new PrintWriter(System.out);

	    PropertyManager.setProperty(Environment.ACTIVITY_REAPER_TIMEOUT, "1000000");
	    
	    /*
	     * Start a top-level activity and suspend it.
	     */

	    current.begin(2);
	    
	    System.out.println("Created activity: "+current.get_activity_name()+"\n");
	    
	    /*
	     * Now sleep.
	     */

	    Thread.sleep(5000);
	    
	    /*
	     * Now try to complete the activity. The reaper should have
	     * terminated the activity, so this should fail.
	     */

	    try
	    {
		current.complete();
		
		/*
		 * If here, then error!
		 */

		System.err.println("Error - managed to complete the activity!");
		
		System.exit(0);
	    }
	    catch (NoActivity e1)
	    {
		e1.printStackTrace();
		
		System.exit(0);
	    }
	    catch (BAD_OPERATION e2)
	    {
		// ok!!

		System.out.println("Activity was terminated by reaper.");
	    }
	    catch (Exception e3)
	    {
		e3.printStackTrace();
		
		System.exit(0);
	    }

	    /*
	     * Make sure we have no activity associated with the thread.
	     */

	    org.omg.CosActivity.Status status = current.get_status();
	    
	    if (status != org.omg.CosActivity.Status.StatusNoActivity)
	    {
		System.err.println("Error - activity still associated with thread. Status is "+Utility.getStatus(status));
		
		System.exit(0);
	    }
	    
	    /*
	     * Now try nested activities. Different from nested transactions,
	     * since timeouts are associated with all types of activities,
	     * and not just top-level activities.
	     */

	    current.begin(100);

	    current.begin(2);

	    /*
	     * Now sleep.
	     */

	    Thread.sleep(5000);
	    
	    /*
	     * Now try to complete the activity. The reaper should have
	     * terminated the activity, so this should fail.
	     */

	    try
	    {
		current.complete();
		
		/*
		 * If here, then error!
		 */

		System.err.println("Error - managed to complete the nested activity!");
		
		System.exit(0);
	    }
	    catch (NoActivity e1)
	    {
		e1.printStackTrace();
		
		System.exit(0);
	    }
	    catch (BAD_OPERATION e2)
	    {
		// ok!!

		System.out.println("Nested activity was terminated by reaper.");
	    }
	    catch (Exception e3)
	    {
		e3.printStackTrace();
		
		System.exit(0);
	    }
	    
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
