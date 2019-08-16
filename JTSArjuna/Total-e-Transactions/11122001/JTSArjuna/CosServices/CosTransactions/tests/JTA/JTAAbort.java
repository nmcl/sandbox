/*
 * Copyright (C) 2002,
 *
 * Hewlett-Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JTAAbort.java,v 1.1.2.1.2.2.16.1 2002/06/19 12:44:09 nmcl Exp $
 */

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.jta.*;
import javax.transaction.*;
import javax.transaction.xa.*;

import java.lang.IllegalAccessException;

public class JTAAbort
{

public static void main (String[] args)
    {
	boolean passed = false;
	
	try
	{
	    ORBInterface.initORB(args, null);
	    OAInterface.initOA();
	}
	catch (Exception e)
	{
	    ErrorStream.fatal().println("Initialisation failed: "+e);
	}

	try
	{
	    TransactionManager tm = JTA_TransactionManager.transactionManager();

	    if (tm != null)
	    {
		System.out.println("Starting top-level transaction.");
		
		tm.begin();
	    
		Transaction theTransaction = tm.getTransaction();

		if (theTransaction != null)
		{
		    System.out.println("\nRolling back transaction.");
		    
		    theTransaction.rollback();
		    
		    System.out.println("\nTransaction now: "+theTransaction);

		    System.out.println("\nThread associated: "+tm.getStatus());
		    
		    theTransaction = tm.suspend();
		    
		    System.out.println("\nSuspended: "+theTransaction);

		    try
		    {
			tm.resume(theTransaction);
		    
			System.out.println("\nResumed: "+tm.getTransaction());
		    }
		    catch (InvalidTransactionException ite)
		    {
			System.out.println("\nCould not resume a dead transaction.");

			passed = true;
		    }
		}
		else
		{
		    System.err.println("Error - could not get transaction!");
		    tm.rollback();
		}
	    }
	    else
		System.err.println("Error - could not get transaction manager!");
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	}

	if (passed)
	    System.out.println("\nTest completed successfully.");
	else
	    System.out.println("\nTest did not complete successfully.");

	OAInterface.shutdownOA();
	ORBInterface.shutdownORB();
    }

}
