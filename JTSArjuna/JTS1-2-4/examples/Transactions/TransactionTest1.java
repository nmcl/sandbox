/*
 * Copyright (C) 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TransactionTest1.javatmpl,v 1.1.4.1 1999/02/22 11:10:07 nmcl Exp $
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
import com.arjuna.JavaCosTransactions.OTS_Current;
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





public class TransactionTest1
{
    
public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();
	
	TransactionFactory theFactory = OTS.factory();
	Control topLevelControl = null;
	Control nestedControl = null;
	Terminator terminator = null;

	/*
	 * First with current.
	 */

	try
	{
	    OTS_Current current = OTS.current();

	    current.begin();

	    topLevelControl = current.get_control();

	    current.begin();

	    nestedControl = current.get_control();

	    /*
	     * Try to commit top-level action without terminating nested
	     * action.
	     */
    
	    terminator = topLevelControl.get_terminator();

	    System.out.println("\nAttempting to terminate out of sequence 'current'. Should fail!");

	    try
	    {
		terminator.commit(true);
	    }
	    catch (TRANSACTION_ROLLEDBACK  e)
	    {
		System.err.println("Commit failed! "+e);
	    }

	    System.out.println("\nNow attempting to terminate 'current' hierarchy. Should fail!");

	    /*
	     * This should fail since Arjuna will already have aborted the top-level
	     * action and all of its children.
	     */

	    try
	    {
		current.commit(true);
	    }
	    catch (INVALID_TRANSACTION  e1)
	    {
		System.err.println("Nested commit failed! "+e1);
	    }
	    catch (Exception e2)
	    {
		System.err.println("Nested commit threw unexpected exception: "+e2);
		System.exit(0);
	    }

	    try
	    {
		current.commit(true);
	    }
	    catch (TRANSACTION_ROLLEDBACK  e1)
	    {
		System.err.println("Top-level commit failed! "+e1);
	    }
	    catch (INVALID_TRANSACTION  e2)
	    {
		System.err.println("Top-level commit failed! "+e2);
	    }
	    catch (Exception e3)
	    {
		System.err.println("Top-level commit threw unexpected exception: "+e3);
		System.exit(0);
	    }
	    
	    /*
	     * Now with the factory.
	     */

	    System.out.println("\nNow creating new top-level action.");
    
	    topLevelControl = theFactory.create(0);
	    nestedControl = topLevelControl.get_coordinator().create_subtransaction();

	    terminator = topLevelControl.get_terminator();

	    System.out.println("\nAttempting to terminate out of sequence 'factory' action. Should fail!");
    
	    try
	    {
		terminator.commit(true);
	    }
    	    catch (TRANSACTION_ROLLEDBACK  e1)
	    {
		System.err.println("Commit failed! "+e1);
	    }
    	    catch (INVALID_TRANSACTION  e2)
	    {
		System.err.println("Commit failed! "+e2);
	    }	    

	    terminator = nestedControl.get_terminator();

	    System.out.println("\nNow attempting to terminate 'factory' nested action. Should fail!");

	    try
	    {
		terminator.commit(true);
	    }
    	    catch (TRANSACTION_ROLLEDBACK  e1)
	    {
		System.err.println("Commit failed! "+e1);
	    }
	    catch (INVALID_TRANSACTION  e2)
	    {
		System.err.println("Commit failed! "+e2);
	    }

	    System.out.println("Test completed successfully.");
	}
	catch (Exception e)
	{
	    System.err.println("Test failed - received unexpected exception "+e);
	    System.exit(1);
	}

	ORBInterface.shutdownBOA();
	ORBInterface.shutdownORB();
    }
 
};
