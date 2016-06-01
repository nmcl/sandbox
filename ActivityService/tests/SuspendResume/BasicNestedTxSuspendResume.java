/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: BasicNestedTxSuspendResume.java,v 1.1 2001/07/23 13:45:03 nmcl Exp $
 */

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.OrbCommon.OAInterface;
import com.arjuna.Activity.*;
import com.arjuna.CosTransactions.*;
import org.omg.CosActivity.*;
import org.omg.CosTransactions.*;
import java.io.*;

public class BasicNestedTxSuspendResume
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
	     * Start a nested activity/transaction and try to suspend it.
	     */

	    current.begin(0);
	    System.out.println("Created activity: "+current.get_activity_name()+"\n");

	    txCurrent.begin();
	    System.out.println("Created transaction: "+txCurrent.get_transaction_name()+"\n");

	    current.begin(0);
	    System.out.println("Created activity: "+current.get_activity_name()+"\n");

	    txCurrent.begin();
	    System.out.println("Created transaction: "+txCurrent.get_transaction_name()+"\n");

	    /*
	     * Since we cannot suspend a part of a transaction hierarchy
	     * in the OTS, trying to call suspend on the current (nested)
	     * activity should fail.
	     */

	    try
	    {
		token = current.suspend();

		System.err.println("Error - managed to suspend a nested transactional activity!");
		
		System.exit(0);
	    }
	    catch (Exception e)
	    {
	    }

	    txCurrent.rollback();

	    current.complete();

	    txCurrent.rollback();

	    /*
	     * Make sure we now have no transaction within this activity.
	     */

	    if (txCurrent.get_status() != org.omg.CosTransactions.Status.StatusNoTransaction)
	    {
		System.err.println("Error - have transaction associated with thread!");

		System.exit(0);
	    }

	    /*
	     * Now start a new nested transactional activity.
	     */

	    current.begin(0);

	    txCurrent.begin();

	    txCurrent.begin();

	    /*
	     * Now suspend the current activity and transactions. This
	     * Should work since the current activity is not nested within
	     * a transaction.
	     */

	    try
	    {
		token = current.suspend();
	    }
	    catch (Exception e)
	    {
		System.err.println("Error - could not suspend nested transactional activity. Got: "+e);

		System.exit(0);
	    }

	    System.out.println("Suspended activity context is:");
	    Utility.printToken(token, strm);
	    strm.flush();
	    System.out.println("\n");

	    /*
	     * Check we still have the top-level activity associated with
	     * this thread.
	     */

	    if (current.get_status() != org.omg.CosActivity.Status.StatusActive)
	    {
		System.err.println("Error - no activity associated with thread.");

		System.exit(0);
	    }
	    else
		System.out.println("Current activity is: "+current.get_activity_name());

	    /*
	     * Make sure no transaction is active!
	     */

	    if (txCurrent.get_status() != org.omg.CosTransactions.Status.StatusNoTransaction)
	    {
		System.err.println("Error - still have transaction associated with thread!");

		System.exit(0);
	    }

	    /*
	     * Now resume.
	     */

	    current.resume(token);

	    txCurrent.rollback();

	    txCurrent.rollback();

	    current.complete();
	    
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
