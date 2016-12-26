/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: GridClient.javatmpl,v 1.1.4.1 1999/02/22 11:09:56 nmcl Exp $
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
import com.arjuna.JavaCosTransactions.OTS_Factory;
import com.arjuna.JavaCosTransactions.OTS;
import org.omg.CosTransactions.*;

import org.omg.CosTransactions.Unavailable;
import org.omg.CORBA  .SystemException;
import org.omg.CORBA  .UserException;
import org.omg.CosNaming  .NamingContextPackage.InvalidName;

public class GridClient
{

public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();
	
	TransactionFactory theOTS = null;
	Control myControl = null;
	grid gridVar = null;
	int h = -1, w = -1, v = -1;
	String gridReference = "/tmp/grid.ref";
	String markerName = "MyGrid";

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: GridClient [-marker <name>] [-reffile <file>] [-help]");
		System.exit(0);
	    }
	    if (args[i].compareTo("-marker") == 0)
	    {

		System.out.println("Sorry, marker name not supported by ORB.");
		System.exit(0);

	    }
	    if (args[i].compareTo("-reffile") == 0)
		gridReference = args[i+1];
	}
		    
	try
	{

	    String[] params = new String[1];

	    params[0] = ORBServices.otsKind;
	    
	    org.omg.CORBA  .Object obj = ORBServices.getService(ORBServices.transactionService,
								  params);

	    params = null;
	    theOTS =  TransactionFactoryHelper .narrow(obj);

	}
	catch (Exception e)
	{
	    System.err.println("Unexpected bind exception: "+e);
	    System.exit(1);
	}

	System.out.println("Creating transaction.");
	
	try
	{
	    myControl = theOTS.create(0);
	}
	catch (Exception e)
	{
	    System.err.println("Create call failed: "+e);
	    System.exit(1);
	}

	try
	{

	    gridVar =  gridHelper .narrow(ORBObject.fileToObject(gridReference));

	}
	catch (Exception e)
	{
	    System.err.println("Grid bind failed: "+e);
	    System.exit(1);
	}

	try
	{
	    h = gridVar.height();
	    w = gridVar.width();
	}
	catch (Exception e)
	{
	    System.err.println("Grid invocation failed: "+e);
	    System.exit(1);
	}

	System.out.println("height is "+h);
	System.out.println("width  is "+w);

	try
	{
	    System.out.println("calling set");
	     
	    gridVar.set(2, 4, 123, myControl);

	    System.out.println("calling get");
	
	    v = gridVar.get(2, 4, myControl);
	}
	catch (Exception sysEx)
	{
	    System.err.println("Grid set/get failed: "+sysEx);
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
		myControl.get_terminator().rollback();
	    }
	    catch (Exception e)
	    {
	    }
	
	    System.exit(1);
	}
	else
	{
	    System.out.println("Committing transaction.");
	    
	    try
	    {
		Terminator handle = myControl.get_terminator();
      
		handle.commit(true);
	    }
	    catch (Exception sysEx)
	    {
		System.err.println("Transaction commit error: "+sysEx);
		System.exit(1);
	    }
	}
	
	/*
	 * OTSArjuna specific call to tell the system
	 * that we are finished with this transaction.
	 */

	try
	{
	    OTS.destroyControl(myControl);
	}
	catch (Exception e)
	{
	    System.out.println("Caught destroy exception: "+e);
	    System.exit(0);
	}

	ORBInterface.shutdownBOA();
	ORBInterface.shutdownORB();

	System.out.println("Test completed successfully.");
    }
 
};

