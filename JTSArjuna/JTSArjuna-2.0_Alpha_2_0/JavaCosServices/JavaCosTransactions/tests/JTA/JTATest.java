/*
 * Copyright (C) 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: JTATest.java,v 1.4 1999/05/14 10:18:19 nmcl Exp $
 */

import com.arjuna.OrbCommon.*;
import com.arjuna.jta.*;
import javax.transaction.*;
import javax.transaction.xa.*;

import java.lang.IllegalAccessException;

public class JTATest
{

public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();
	
	String xaResource = null;
	String connectionString = null;
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-connect") == 0)
		connectionString = args[i+1];
	    if (args[i].compareTo("-creator") == 0)
		xaResource = args[i+1];
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: JTATest -creator <name> [-connect <string>] [-help]");
		System.exit(0);
	    }
	}

	if (xaResource == null)
	{
	    System.err.println("Error - no resource creator specified.");
	    System.exit(0);
	}

	/*
	 * We should have a reference to a factory object (see JTA specification).
	 * However, for simplicity we will ignore this.
	 */
	
	try
	{
	    XACreator creator = (XACreator) Class.forName(xaResource).newInstance();
	    XAResource theResource = creator.create(connectionString);

	    if (theResource == null)
	    {
		System.err.println("Error - creator "+xaResource+" returned null resource.");
		System.exit(0);
	    }

	    TransactionManager tm = JTA_TransactionManager.transactionManager();

	    if (tm != null)
	    {
		System.out.println("Starting top-level transaction.");
		
		tm.begin();
	    
		Transaction theTransaction = tm.getTransaction();

		if (theTransaction != null)
		{
		    System.out.println("\nTrying to register resource with transaction.");
		    
		    if (!theTransaction.enlistResource(theResource, null))
		    {
			System.err.println("Error - could not enlist resource in transaction!");
			tm.rollback();

			System.exit(0);
		    }
		    else
			System.out.println("\nResource enlisted successfully.");

		    /*
		     * XA does not support subtransactions.
		     * By default we ignore any attempts to create such transactions.
		     * Appropriate settings can be made which will cause currently
		     * running transactions to also rollback, if required.
		     */
		    
		    System.out.println("\nTrying to start another transaction - should fail!");

		    try
		    {
			tm.begin();

			System.err.println("Error - transaction started!");
			System.exit(0);
		    }
		    catch (Exception e)
		    {
			System.out.println("Transaction did not begin: "+e);
		    }
		    
		    /*
		     * Do some work and decide whether to commit or rollback.
		     * (Assume commit for example.)
		     */

		    System.out.println("\nCommitting transaction.");
		    
		    tm.commit();
		}
		else
		{
		    System.err.println("Error - could not get transaction!");
		    tm.rollback();
		    System.exit(0);
		}

		System.out.println("\nTest completed successfully.");
	    }
	    else
		System.err.println("Error - could not get transaction manager!");
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	}

	ORBInterface.shutdownBOA();
	ORBInterface.shutdownORB();
    }

};
