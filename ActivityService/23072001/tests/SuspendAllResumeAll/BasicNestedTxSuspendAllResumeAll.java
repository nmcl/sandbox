/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: BasicNestedTxSuspendAllResumeAll.java,v 1.1 2001/07/23 13:45:02 nmcl Exp $
 */

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.OrbCommon.OAInterface;
import com.arjuna.Activity.*;
import com.arjuna.CosTransactions.*;
import org.omg.CosActivity.*;
import org.omg.CosTransactions.*;
import java.io.*;

public class BasicNestedTxSuspendAllResumeAll
{
    
public static void main (String[] args)
    {
	try
	{
	    ORBInterface.initORB(args, null);
	    OAInterface.initOA();

	    org.omg.CosActivity.Current current = CosActivity.get_current();
	    org.omg.CosTransactions.Current txCurrent = OTS.get_current();
	    PrintWriter strm = new PrintWriter(System.out);
	    ActivityToken token = null;
	    
	    /*
	     * Start a nested activity and suspend it.
	     */

	    current.begin(0);
	    System.out.println("Created activity: "+current.get_activity_name()+"\n");

	    txCurrent.begin();

	    current.begin(0);
	    System.out.println("Created activity: "+current.get_activity_name()+"\n");

	    txCurrent.begin();

	    /*
	     * We should be able to suspend_all on a nested
	     * activity/transaction context.
	     */

	    try
	    {
		token = current.suspend_all();
	    }
	    catch (Exception e)
	    {
		System.err.println("Error - could not suspend_all on a transaction/activity context: "+e);

		System.exit(0);
	    }
	    
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
		System.err.println("Error - still have an activity associated with thread. Status is "+Utility.getStatus(status));
		
		System.exit(0);
	    }

	    /*
	     * Check that we now have no transaction associated with thread.
	     */

	    if (txCurrent.get_status() != org.omg.CosTransactions.Status.StatusNoTransaction)
	    {
		System.err.println("Error - still have a transaction associated with thread.");
		
		System.exit(0);
	    }

	    /*
	     * Now try a resume_all!
	     */
	    
	    current.resume_all(token);

	    System.out.println("resumed activity: "+current.get_activity_name());

	    System.out.println("\nActivity context is:");
	    Utility.printContext(current.get_context(), strm);
	    strm.flush();
	    System.out.println("\n");

	    /*
	     * Now try to complete out-of-order.
	     */

	    try
	    {
		System.out.println("\nCompleting: "+current.get_activity_name());
	    
		current.complete();

		System.err.println("Error - managed to complete out-of-order activity!");

		System.exit(0);
	    }
	    catch (Exception e)
	    {
	    }
	    
	    txCurrent.rollback();
	    
	    current.complete();

	    txCurrent.rollback();

	    System.out.println("\nCompleting: "+current.get_activity_name());

	    current.complete();

	    /*
	     * Make sure we have no activity or transaction now!
	     */

	    if (txCurrent.get_status() != org.omg.CosTransactions.Status.StatusNoTransaction)
	    {
		System.err.println("Error - still have transaction associated with thread!");

		System.exit(0);
	    }

	    if (current.get_status() != org.omg.CosActivity.Status.StatusNoActivity)
	    {
		System.err.println("Error - still have activity associated with thread!");

		System.exit(0);
	    }
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
