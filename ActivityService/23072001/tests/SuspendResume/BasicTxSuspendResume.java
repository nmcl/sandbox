/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: BasicTxSuspendResume.java,v 1.1 2001/07/23 13:45:03 nmcl Exp $
 */

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.OrbCommon.OAInterface;
import com.arjuna.Activity.*;
import com.arjuna.CosTransactions.*;
import org.omg.CosActivity.*;
import org.omg.CosTransactions.*;
import java.io.*;

public class BasicTxSuspendResume
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

	    txCurrent.begin();

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
	     * Check that we now have no transaction associated with thread.
	     */

	    org.omg.CosTransactions.Status txStatus = txCurrent.get_status();
	    
	    if (txStatus != org.omg.CosTransactions.Status.StatusNoTransaction)
	    {
		System.err.print("Error - transaction still associated with thread. Status is ");
		OTS_Utility.printStatus(txStatus);
		
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

	    /*
	     * Now try to complete out of order (transaction is still running!)
	     */

	    try
	    {
	        current.complete();

		System.err.println("Error - managed to complete activity with active transactions!");

		System.exit(0);
	    }
	    catch (Exception e)
	    {
	    }

	    txCurrent.rollback();

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
