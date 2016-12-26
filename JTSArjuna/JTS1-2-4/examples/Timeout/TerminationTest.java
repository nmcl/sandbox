/*
 * Copyright (C) 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TerminationTest.javatmpl,v 1.1.2.1 1999/04/16 13:47:40 nmcl Exp $
 */

/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability.h,v 1.4 1998/06/30 16:34:10 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2 1998/11/12 08:52:40 nmcl Exp $
 */













































import JavaCosTransactionsTests.*;
import TestModule.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.JavaCosTransactions.OTS_Factory;
import com.arjuna.JavaCosTransactions.OTS_Current;
import com.arjuna.JavaCosTransactions.OTS;
import org.omg.CosTransactions.*;

import org.omg.CosTransactions.Unavailable;
import org.omg.CORBA  .SystemException;
import org.omg.CORBA  .UserException;

public class TerminationTest
{
    
public static void main (String[] args)
    {
	boolean commit = true;

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-commit") == 0)
		commit = true;
	    if (args[i].compareTo("-rollback") == 0)
		commit = false;
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: TerminationTest [-commit] [-rollback] [-help]");
		System.exit(0);
	    }
	}

	Control myControl = null;
	
	try
	{
	    ORBInterface.initORB(args, null);
	    ORBInterface.initBOA();

	    TransactionFactory theOTS = new OTS_Factory();

	    System.out.println("Creating transaction with 2 second timeout.");
	    
	    myControl = theOTS.create(2);

	    if (myControl == null)
	    {
		System.err.println("Could not create transaction!");
		System.exit(-1);
	    }

	    Terminator handle = myControl.get_terminator();
	    
	    try
	    {
		System.out.println("Sleeping for 5 seconds.");
		
		Thread.sleep(5000);
	    }
	    catch (Exception e)
	    {
	    }
	    
	    if (handle != null)
	    {
		System.out.print("Attempting to ");

		if (commit)
		    System.out.println("commit transaction. Should fail!");
		else
		    System.out.println("rollback transaction. Should fail!.");

		if (commit)
		    handle.commit(true);
		else
		    handle.rollback();

		if (commit)
		    System.out.println("Test did not completed successfully.");

		System.out.println("\nNow attempting to destroy transaction. Should fail!");
		    
		OTS.destroyControl(myControl);
	    }
	    else
		System.err.println("No transaction terminator!");
	}
	catch (UserException e)
	{
	    System.err.println("Caught UserException: "+e);
	}
	catch (SystemException e)
	{
	    System.err.println("Caught SystemException: "+e);

	    try
	    {
		Coordinator coord = myControl.get_coordinator();
		Status s = coord.get_status();
		
		System.err.print("Transaction status: ");
		OTS.printStatus(System.err, s);

		System.err.println();

		coord = null;
	    }
	    catch (Exception ex)
	    {
	    }
	}

	try
	{
	    OTS_Current current = OTS.current();

	    current.set_timeout(2);
	
	    System.out.println("\nNow creating current transaction with 2 second timeout.");

	    current.begin();

	    myControl = current.get_control();
	    
	    try
	    {
		System.out.println("Sleeping for 5 seconds.");
		
		Thread.sleep(5000);
	    }
	    catch (Exception e)
	    {
	    }
	    
	    System.out.print("Attempting to ");

	    if (commit)
		System.out.println("commit transaction. Should fail!");
	    else
		System.out.println("rollback transaction. Should fail!.");

	    if (commit)
		current.commit(true);
	    else
		current.rollback();

	    if (commit)
		System.out.println("Test did not completed successfully.");
	}
	catch (UserException e)
	{
	    System.err.println("Caught UserException: "+e);
	}
	catch (SystemException e)
	{
	    System.err.println("Caught SystemException: "+e);

	    try
	    {
		Coordinator coord = myControl.get_coordinator();
		Status s = coord.get_status();
		
		System.err.print("Transaction status: ");
		OTS.printStatus(System.err, s);

		System.err.println();

		myControl = null;
		coord = null;
	    }
	    catch (Exception ex)
	    {
	    }
	}

	ORBInterface.shutdownBOA();
	ORBInterface.shutdownORB();
    }

};
