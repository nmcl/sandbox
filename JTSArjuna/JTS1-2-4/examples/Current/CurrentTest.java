/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CurrentTest.javatmpl,v 1.1.4.2 1999/02/22 11:09:57 nmcl Exp $
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
import com.arjuna.OrbCommon.*;
import com.arjuna.JavaCosTransactions.*;
import org.omg.CosTransactions.*;

import org.omg.CosTransactions.Unavailable;
import org.omg.CORBA  .SystemException;
import org.omg.CORBA  .UserException;
import org.omg.CosNaming  .NamingContextPackage.InvalidName;

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





public class CurrentTest
{
    
public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();
    
	OTS_Current current = OTS.current();
	Control myControl = null;
	String gridReference = "/tmp/grid.ref";
	grid gridVar = null;                    // pointer the grid object that will be used.
	int h = -1, w = -1, v = -1;

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: CurrentTest [-reffile <file>] [-help]");
		System.exit(0);
	    }
	    if (args[i].compareTo("-reffile") == 0)
		gridReference = args[i+1];
	}

	System.out.println("Beginning transaction.");
	
	try
	{
	    current.begin();
	    
	    myControl = current.get_control();

	    if (myControl == null)
	    {
		System.err.println("Error - control is null!");
		System.exit(-1);
	    }
	}
	catch (Exception sysEx)
	{
	    System.err.println("Unexpected exception in begin: "+sysEx);
	    System.exit(1);
	}
  
	try
	{
	    gridVar =  gridHelper .narrow(ORBObject.fileToObject(gridReference));
	}
	catch (Exception sysEx)
	{
	    System.err.println("failed to bind to grid: "+sysEx);
	    System.exit(1);
	}

	try
	{
	    h = gridVar.height();
	    w = gridVar.width();
	}
	catch (Exception sysEx)
	{
	    System.err.println("grid height/width failed: "+sysEx);
	    System.exit(1);
	}

	System.out.println("height is "+h);
	System.out.println("width  is "+w);

	try
	{
	    gridVar.set(2, 4, 123, myControl);
	    v = gridVar.get(2, 4, myControl);
	}
	catch (Exception sysEx)
	{
	    System.err.println("grid set/get failed: "+sysEx);
	    System.exit(1);
	}

	// no problem setting and getting the elememt:
	System.out.println("grid[2,4] is "+v);

	// sanity check: make sure we got the value 123 back:
	if (v != 123)
	{
	    // oops - we didn't:
	    System.err.println("something went seriously wrong");

	    try
	    {
		current.rollback();
	    }
	    catch (Exception e)
	    {
		System.err.println("rollback error: "+e);
	    }
	
	    System.exit(1);
	}
	else
	{
	    System.out.println("Committing transaction.");
	    
	    try
	    {
		current.commit(true);
	    }
	    catch (Exception e)
	    {
		System.err.println("commit error: "+e);
		System.exit(0);
	    }

	    ORBInterface.shutdownBOA();
	    ORBInterface.shutdownORB();

	    System.out.println("Test completed successfully.");
	}
    }
 
};

