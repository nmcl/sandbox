/*
 * Copyright (C) 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ArjunaNestingTest.javatmpl,v 1.1.4.1 1999/02/22 11:09:55 nmcl Exp $
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

public class ArjunaNestingTest
{

public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();

	boolean doAbort = false;
	boolean registerSubtran = false;
	OTS_Current current = OTS.current();
	DemoArjunaResource sr = new DemoArjunaResource();
    
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: ArjunaNestingTest [-abort] [-subtran] [-help]");
		System.exit(0);
	    }
	    if (args[i].compareTo("-abort") == 0)
		doAbort = true;
	    if (args[i].compareTo("-subtran") == 0)
		registerSubtran = true;
	}
    
	try
	{
	    current.begin();
	    current.begin();
	    current.begin();
	}
	catch (SystemException sysEx)
	{
	    System.err.println("Unexpected system exception:" +sysEx);
	    System.exit(0);
	}
	catch (UserException se)
	{
	    System.err.println("Unexpected user exception:" +se);
	    System.exit(0);
	}	
  
	try
	{
	    sr.registerResource(registerSubtran);
	}
	catch (SystemException ex1)
	{
	    System.err.println("Unexpected system exception: "+ex1);
	    System.exit(0);
	}
	catch (Exception e)
	{
	    System.err.println("call to registerSubtran failed: "+e);
	    System.exit(0);
	}

	try
	{
	    System.out.println("committing first nested transaction");
	    current.commit(true);

	    System.out.println("committing second nested transaction");
	    current.commit(true);

	    if (!doAbort)
	    {
		System.out.println("committing top-level transaction");
		current.commit(true);
	    }
	    else
	    {
		System.out.println("aborting top-level transaction");
		current.rollback();
	    }
	}
	catch (Exception ex)
	{
	    System.err.println("Caught unexpected exception: "+ex);
	    System.exit(0);
	}

	System.out.println("Test completed successfully.");

	ORBInterface.orb().disconnect( sr ) ;

	ORBInterface.shutdownBOA();
	ORBInterface.shutdownORB();
    }
 
};

