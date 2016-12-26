/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SynchTest.javatmpl,v 1.1.4.1 1999/02/22 11:10:06 nmcl Exp $
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





public class SynchTest
{
    
public static void main (String[] args)
    {

	org.omg.CosTransactions.Status status = Status.StatusUnknown;


	tranobject_i localObject = null;
	Synchronization sync = null;
	
	try
	{
	    ORBInterface.initORB(args, null);
	    ORBInterface.initBOA();

	    Control myControl = null;
	    OTS_Current current = OTS.current();
	    Coordinator coord = null;

	    sync = new demosync();
	    localObject = new tranobject_i();
	    
	    current.begin();

	    myControl = current.get_control();

	    coord = myControl.get_coordinator();

	    coord.register_resource(localObject);
	    coord.register_synchronization(sync);

	    try
	    {
		current.commit(true);
	    }
	    catch (TRANSACTION_ROLLEDBACK  e1)
	    {
		System.out.println("Transaction rolledback");
	    }

	    status = coord.get_status();
	}
	catch (UserException e1)
	{
	    System.out.println("Caught UserException: "+e1);
	    System.exit(0);
	}	
	catch (SystemException e2)
	{
	    System.out.println("Caught SystemException:" +e2);
	    System.exit(0);
	}
	    
	System.out.print("Final action status: ");
	OTS.printStatus(System.out, status);
	System.out.println("\nTest completed successfully.");

	ORBInterface.orb().disconnect( sync ) ;
	ORBInterface.orb().disconnect( localObject ) ;

	ORBInterface.shutdownBOA();
	ORBInterface.shutdownORB();
    }

};
