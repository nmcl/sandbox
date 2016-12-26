/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ImplicitClient.javatmpl,v 1.1 1998/11/12 13:32:15 nmcl Exp $
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
import com.arjuna.JavaCosTransactions.OTS;
import com.arjuna.JavaCosTransactions.OTS_Current;
import org.omg.CosTransactions.*;

import org.omg.CosTransactions.Unavailable;
import org.omg.CORBA  .SystemException;
import org.omg.CORBA  .UserException;
import org.omg.CosNaming  .NamingContextPackage.InvalidName;

public class ImplicitClient
{

public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();
	
	String refFile = "/tmp/trangrid.ref";
	String markerName = null;

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: ImplicitClient [-reffile <file>] [-marker <name>] [-help]");
		System.exit(0);
	    }
	    if (args[i].compareTo("-reffile") == 0)
		refFile = args[i+1];
	    if (args[i].compareTo("-marker") == 0)
	    {

		System.err.println("Error - marker name not supported.");
		System.exit(1);

	    }
	}

	OTS_Current current = OTS.current();
	    
	TranGrid TranGridVar = null;   // pointer the grid object that will be used.
	short h = 0, w = 0, v = 0;

	try
	{
	    current.begin();
	    

	    TranGridVar =  TranGridHelper .narrow(ORBObject.fileToObject(refFile));


	    //	    TRANSACTIONAL_OBJECT_(TranGridVar);

	    try
	    {
		h = TranGridVar.height();
		w = TranGridVar.width();
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
		
		TranGridVar.set((short) 2, (short) 4, (short) 123);
		
		System.out.println("calling get");
		
		v = TranGridVar.get((short) 2, (short) 4);
	    }
	    catch (Exception sysEx)
	    {
		System.err.println("Grid set/get failed: "+sysEx);
		System.exit(1);
	    }

	    // no problem setting and getting the element:
	    
	    System.out.println("trangrid[2,4] is "+v);

	    // sanity check: make sure we got the value 123 back:
	    
	    if (v != 123)
	    {
		// oops - we didn't:
	
		current.rollback();
	
		System.exit(1);
	    }
	    else
	    {
		current.commit(true);
	    }
	}
	catch (Exception e)
	{
	    System.out.println("Caught exception: "+e);
	}
    }
 
};
