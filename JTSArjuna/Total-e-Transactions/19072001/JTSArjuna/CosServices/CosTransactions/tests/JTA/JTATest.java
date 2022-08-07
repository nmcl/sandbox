/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JTATest.java,v 1.1.2.1.28.2.4.1 2000/12/14 15:09:49 nmcl Exp $
 */

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.jta.*;
import javax.transaction.*;
import javax.transaction.xa.*;

import java.lang.IllegalAccessException;

public class JTATest
{

public static void main (String[] args)
    {
	try
	{
	    ORBInterface.initORB(args, null);
	    OAInterface.initOA();
	}
	catch (Exception e)
	{
	    ErrorStream.fatal().println("Initialisation failed: "+e);
	}
	
	String xaResource = null;
	String connectionString = null;
	boolean tmCommit = true;
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-connect") == 0)
		connectionString = args[i+1];
	    if (args[i].compareTo("-creator") == 0)
		xaResource = args[i+1];
	    if (args[i].equals("-txcommit"))
		tmCommit = false;
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: JTATest -creator <name> [-connect <string>] [-txcommit] [-help]");
		System.exit(0);
	    }
	}

	if (xaResource == null)
	{
	    System.err.println("Error - no resource creator specified.");
	    System.exit(0);
	}

	/*
	 * We should have a reference to a factory object (see JTA
	 * specification). However, for simplicity we will ignore this.
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
		    
		    if (!theTransaction.enlistResource(theResource))
		    {
			System.err.println("Error - could not enlist resource in transaction!");
			tm.rollback();

			System.exit(0);
		    }
		    else
			System.out.println("\nResource enlisted successfully.");
		    /*
		     * XA does not support subtransactions.
		     * By default we ignore any attempts to create such
		     * transactions. Appropriate settings can be made which
		     * will cause currently running transactions to also
		     * rollback, if required.
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

		    Synchronization s = new Synchronization();

		    tm.getTransaction().registerSynchronization(s);
		    
		    System.out.println("\nCommitting transaction.");

		    System.out.println("using tm: "+tmCommit);
		    
		    if (tmCommit)
			tm.commit();
		    else
			tm.getTransaction().commit();
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

	OAInterface.shutdownOA();
	ORBInterface.shutdownORB();
    }

};
