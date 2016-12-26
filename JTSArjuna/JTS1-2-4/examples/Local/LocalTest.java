/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LocalTest.javatmpl,v 1.1.4.1 1999/02/22 11:10:02 nmcl Exp $
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
import com.arjuna.JavaCosTransactions.OTS;
import com.arjuna.JavaCosTransactions.OTS_Factory;
import com.arjuna.JavaCosTransactions.OTS_Current;
import org.omg.CosTransactions.*;

import org.omg.CosTransactions.Unavailable;
import org.omg.CORBA  .SystemException;
import org.omg.CORBA  .UserException;

/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CosTranExceptions.h,v 1.3 1998/07/06 13:29:23 nmcl Exp $
 */





/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CosTranNames.h,v 1.3 1998/06/29 09:35:35 nmcl Exp $
 */




/*
 * Simple renaming macros for portability.
 *
 * Slightly different from C++ version in that we don't need the
 * typedefs. However, we do need some mapping for OTS exceptions which
 * should now be system exceptions.
 */

/*
 * CosTransaction module exceptions
 */






/*
 * We need to check this. WRONG_TRANSACTION is supposed to
 * be a system exception (in the latest draft of the OTS),
 * but two ORBs have it as a user exception.
 */





/**/





/*
 * An attempt at a portable way of including the OTS exceptions,
 * which are either mapped as system exceptions or user exceptions,
 * depending upon the Orb.
 */


import org.omg.CORBA  .WrongTransaction;
import org.omg.CORBA  .INVALID_TRANSACTION;
import org.omg.CORBA  .TRANSACTION_REQUIRED;
import org.omg.CORBA  .TRANSACTION_ROLLEDBACK;





public class LocalTest
{
    
public static void main (String[] args)
    {
	try
	{
	    ORBInterface.initORB(args, null);
	    ORBInterface.initBOA();

	    TransactionFactory theOTS = new OTS_Factory();
	    Control myControl;
	    grid localGrid = new grid_i(100, 100);
	    int h, w, v;

	    ORBInterface.orb().connect( localGrid ) ;
	    
	    myControl = theOTS.create(0);

	    if (myControl == null)
	    {
		System.err.println("Could not create transaction!");
		System.exit(-1);
	    }

	    h = localGrid.height();
	    w = localGrid.width();

	    localGrid.set( 2, 4, 123, myControl);
	    v = localGrid.get(2, 4, myControl);

	    // no problem setting and getting the elememt:
	    
	    System.out.println("grid[2,4] is "+v);

	    // sanity check: make sure we got the value 123 back:
	    if (v != 123)
	    {
		// oops - we didn't:
		System.err.println("something went seriously wrong");
		
		myControl.get_terminator().rollback();
	
		System.exit(1);
	    }
	    else
	    {
		Terminator handle = myControl.get_terminator();

		try
		{
		    if (handle != null)
		    {
			handle.commit(false);

			System.out.println("Test completed successfully.");
			
		    }
		    else
			System.err.println("Error - no transaction terminator!");
		}
		catch (Exception ex)
		{
		    System.out.println("Test error!");
		}
	    }

	    ORBInterface.orb().disconnect( theOTS ) ;
	    ORBInterface.orb().disconnect( localGrid ) ;

	    OTS.destroyControl(myControl);
	}
	catch (UserException e)
	{
	    System.err.println("Caught UserException: "+e);
	}
	catch (SystemException e)
	{
	    System.err.println("Caught SystemException: "+e);
	}

	System.out.println("\nWill now try different thread terminating transaction.\n");

	try
	{
	    OTS_Current current = OTS.current();
	    
	    System.out.println("Starting new transaction.");
	    
	    current.begin();

	    Control tc = current.get_control();

	    if (tc != null)
	    {
		System.out.println("Creating new thread.");
		
		TransactionalThread tranThread = new TransactionalThread(tc);

		System.out.println("Waiting for thread to terminate transaction.\n");
		
		tranThread.start();

		while (!tranThread.finished())
		    Thread.yield();

		System.out.println("\nCreator will now attempt to rollback transaction. Should fail.");

		try
		{
		    current.rollback();

		    System.out.println("Error - managed to rollback transaction!");
		}
		catch (NoTransaction e1)
		{
		    System.out.println("Correct termination - caught: "+e1);
		}
		catch (INVALID_TRANSACTION  e2)
		{
		    System.out.println("Correct termination - caught: "+e2);
		}
		catch (Exception e3)
		{
		    System.out.println("Wrong termination - caught unexpected exception: "+e3);
		    System.exit(0);
		}

		System.out.println("Test completed successfully.");
	    }
	    else
		System.err.println("Error - null transaction control!");
	}
	catch (Exception e)
	{
	    System.out.println("Caught unexpected exception: "+e);
	}

	ORBInterface.shutdownBOA();
	ORBInterface.shutdownORB();
    }

};
