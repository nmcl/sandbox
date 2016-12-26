/*
 * Copyright (C) 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TransactionTest2.javatmpl,v 1.1.4.1 1999/02/22 11:10:08 nmcl Exp $
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




import JavaCosTransactionsTests.*;
import TestModule.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.JavaCosTransactions.*;
import org.omg.CosTransactions.*;

import org.omg.CosTransactions.Unavailable;
import org.omg.CosTransactions.SubtransactionsUnavailable;
import org.omg.CosTransactions.NotPrepared;
import org.omg.CosTransactions.HeuristicRollback;
import org.omg.CosTransactions.HeuristicCommit;
import org.omg.CosTransactions.HeuristicMixed;
import org.omg.CosTransactions.HeuristicHazard;

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
 * An attempt at a portable way of including the OTS exceptions,
 * which are either mapped as system exceptions or user exceptions,
 * depending upon the Orb.
 */


import org.omg.CORBA  .WrongTransaction;
import org.omg.CORBA  .INVALID_TRANSACTION;
import org.omg.CORBA  .TRANSACTION_REQUIRED;
import org.omg.CORBA  .TRANSACTION_ROLLEDBACK;





public class TransactionTest2
{
    
public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();

	boolean local = true;

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: TransactionTest2 [-remote] [-help]");
		System.exit(0);
	    }
	    if (args[i].compareTo("-remote") == 0)
		local = false;
	}
	
	TransactionFactory theOTS = null;
	Control theControl = null;
	Control currentControl = null;
	OTS_Current current = OTS.current();

	try
	{
	    current.begin();

	    currentControl = current.get_control();

	    try
	    {
		if (!local)
		{

		    String[] params = new String[1];

		    params[0] = ORBServices.otsKind;
		    
		    org.omg.CORBA  .Object obj = ORBServices.getService(ORBServices.transactionService,
									  params);
		    params = null;
		    theOTS =  TransactionFactoryHelper .narrow(obj);

		}
		else
		    theOTS = OTS.factory();
	    }
	    catch (Exception sysEx)
	    {
		System.err.println("OTS server bind exception: "+sysEx);
		System.exit(1);
	    }

	    try
	    {
		theControl = theOTS.create(0);
	    }
	    catch (Exception sysEx)
	    {
		System.err.println("OTS server create exception: "+sysEx);
		System.exit(1);
	    }

	    Coordinator theCoordinator = currentControl.get_coordinator();
	    String name = theCoordinator.get_transaction_name();
    
	    System.out.println("\nCurrent transaction: "+name);
	    name = null;
	    
	    Coordinator otherCoordinator = theControl.get_coordinator();

	    name = otherCoordinator.get_transaction_name();

	    System.out.println("New transaction from server: "+name);
	    name = null;
	    
	    System.out.print("These transactions are ");

	    if (theCoordinator.is_same_transaction(otherCoordinator))
		System.out.println("the same. Incorrect result!");
	    else
		System.out.println("not the same. Correct result.");

	    System.out.print("These transactions are ");
	    
	    if (theCoordinator.is_ancestor_transaction(otherCoordinator))
		System.out.println("ancestors. Incorrect result!");
	    else
		System.out.println("not ancestors. Correct result.");
	    
	    System.out.print("These transactions are ");

	    if (theCoordinator.is_related_transaction(otherCoordinator))
		System.out.println("related. Incorrect result!");
	    else
		System.out.println("not related. Correct result.");

	    System.out.println("\nCreating nested transaction using current.");
	    
	    current.begin();

	    theControl = current.get_control();
	    otherCoordinator = theControl.get_coordinator();

	    name = theCoordinator.get_transaction_name();
    
	    System.out.println("\nNested's parent transaction: "+name);
	    name = null;
	    
	    name = otherCoordinator.get_transaction_name();

	    System.out.println("Nested transaction is: "+name);
	    name = null;
	    
	    System.out.print("These transactions are ");

	    if (theCoordinator.is_related_transaction(otherCoordinator))
		System.out.println("related. Correct result.");
	    else
		System.out.println("not related. Incorrect result!");

	    org.omg.CosTransactions.Status s = theCoordinator.get_parent_status();

	    System.out.print("\nParent (self) status: ");
	    OTS.printStatus(System.out, s);
	    System.out.println();

	    if (theCoordinator.is_ancestor_transaction(theCoordinator))
		System.out.println("Self is an ancestor. Correct result.");
	    else
	    {
		System.out.println("Error - self is not an ancestor!");
		System.exit(0);
	    }

	    Control child = theCoordinator.create_subtransaction();
	    Coordinator childCoord = child.get_coordinator();

	    if (childCoord.is_ancestor_transaction(theCoordinator))
	    {
		System.out.println("Error - my child is an ancestor!");
		System.exit(0);
	    }
	    else
		System.out.println("My child is not an ancestor! Correct result.");

	    System.out.println("Test completed successfully.");
	}
	catch (Exception e)
	{
	    System.err.println("Caught unexpected exception: "+e);
	    System.exit(1);
	}

	ORBInterface.shutdownBOA();
	ORBInterface.shutdownORB();
    }
 
};

