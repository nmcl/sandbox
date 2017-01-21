/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NestedTester.javatmpl,v 1.1.4.1 1999/02/22 11:10:04 nmcl Exp $
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
import org.omg.CORBA  .SystemException;
import org.omg.CORBA  .UserException;

public class NestedTester
{
    
public static void main (String[] args)
    {
	boolean registerSubtran = false;
	boolean doAbort = false;

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: NestedTester [-subtran] [-abort] [-help]");
		System.exit(0);
	    }
	    if (args[i].compareTo("-subtran") == 0)
		registerSubtran = true;
	    if (args[i].compareTo("-abort") == 0)
		doAbort = true;
	}

	DemoResource r = null;
	DemoSubTranResource sr = null;
	
	try
	{
	    ORBInterface.initORB(args, null);
	    ORBInterface.initBOA();

	    OTS_Current current = OTS.current();
	    
	    r = new DemoResource();
	    sr = new DemoSubTranResource();
    
	    current.begin();
	    current.begin();
	    current.begin();

	    sr.registerResource(registerSubtran);
	    r.registerResource();

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

	    System.out.println("Test completed successfully.");
	}
	catch (UserException e)
	{
	    System.err.println("Caught UserException: "+e);
	}
	catch (SystemException e)
	{
	    System.err.println("Caught SystemException: "+e);
	}

	ORBInterface.orb().disconnect( r ) ;
	ORBInterface.orb().disconnect( sr ) ;

	ORBInterface.shutdownBOA();
	ORBInterface.shutdownORB();
    }
    
};
