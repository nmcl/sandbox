/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HeuristicTest.javatmpl,v 1.1.4.1 1999/02/22 11:09:59 nmcl Exp $
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
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.JavaCosTransactions.*;
import org.omg.CosTransactions.*;

import org.omg.CosTransactions.Unavailable;
import org.omg.CORBA  .SystemException;

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





public class HeuristicTest
{

public static void main (String[] args)
    {
	boolean shouldCommit = true;
	boolean heuristicPrepare = false;

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-abort") == 0)
		shouldCommit = false;
	    if (args[i].compareTo("-prepare") == 0)
		heuristicPrepare = true;
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: HeuristicTest [-abort] [-prepare]");
		System.exit(0);
	    }
	}

	Coordinator coord = null;
	
	try
	{
	    ORBInterface.initORB(args, null);
	    ORBInterface.initBOA();

	    OTS_Current current = OTS.current();
	    Resource heuristicObject = new heuristic(heuristicPrepare);
	    Resource atomicObject = new AtomicResource(shouldCommit);

	    System.out.println("beginning top-level transaction.");
	    
	    current.begin();

	    Control myControl = current.get_control();

	    if (myControl == null)
	    {
		System.err.println("Error - myControl is nil");
		System.exit(0);
	    }
	    
	    System.out.println("getting coordinator");
	    
	    coord = myControl.get_coordinator();

	    myControl = null;
	    
	    System.out.println("registering resources.");

	    try
	    {
		coord.register_resource(heuristicObject);
		coord.register_resource(atomicObject);
	    }
	    catch (Exception ex)
	    {
		System.out.println("Failed to register resources: "+ex);
		System.exit(0);
	    }

	    System.out.println("committing top-level transaction.");
	    
	    current.commit(true);
	}
	catch (TRANSACTION_ROLLEDBACK  e1)
	{
	    System.out.println("\nTransaction RolledBack exception");
	}
	catch (HeuristicMixed e2)
	{
	    System.out.println("\nTransaction HeuristicMixed exception");
	}
	catch (HeuristicHazard e3)
	{
	    System.out.println("\nTransaction HeuristicHazard exception");
	}	
	catch (Exception e4)
	{
	    System.out.println("Caught unexpected exception: "+e4);
	}

	System.out.println("Trying to determing final transaction outcome.");
	
	try
	{
	    if (coord != null)
	    {
		System.out.print("\nFinal action status: ");
		OTS.printStatus(System.out, coord.get_status());
		System.out.println();
		
		coord = null;

		System.out.println("Test completed successfully.");
	    }
	    else
		System.out.println("\nCould not determine action status.");
	}
	catch (Exception e5)
	{
	    System.out.println("Caught unexpected exception:" +e5);
	}

	ORBInterface.shutdownBOA();
	ORBInterface.shutdownORB();
    }

};
