/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DistributedHammer1.javatmpl,v 1.1.4.2.2.1 1999/04/16 13:47:39 nmcl Exp $
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
import java.util.Random;

import JavaCosTransactionsTests.TestException;

public class DistributedHammer1
{

public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();
	
	String server1 = "/tmp/hammer1.ref";
	String server2 = "/tmp/hammer2.ref";

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-reffile1") == 0)
		server1 = args[i+1];
	    if (args[i].compareTo("-reffile2") == 0)
		server2 = args[i+1];
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: DistributedHammer1 [-reffile1 <file>] [-reffile2 <file>] [-help]");
		System.exit(0);
	    }
	}
	
	try
	{
	    DistributedHammerWorker1.hammerObject_1 =  HammerHelper .narrow(ORBObject.fileToObject(server1));
	    DistributedHammerWorker1.hammerObject_2 =  HammerHelper .narrow(ORBObject.fileToObject(server2));

	    if (! DistributedHammerWorker1.hammerObject_1.set(10, null))
		System.out.println("m set1 : failed");
	    if (! DistributedHammerWorker1.hammerObject_2.set(101, null))
		System.out.println("m set2 : failed");

	    DistributedHammerWorker1.get12('m', 0);
	    DistributedHammerWorker1.get21('m', 0);
	}
	catch (Exception e)
	{
	    System.err.println("DistributedHammer1: "+e);
	    System.exit(0);
	}
    
	for (int i = 0; i < 100; i++)
	    DistributedHammerWorker1.randomOperation('1', 0);
	
	DistributedHammerWorker1.get12('m', 0);
	DistributedHammerWorker1.get21('m', 0);

	ORBInterface.shutdownBOA();
	ORBInterface.shutdownORB();

	System.exit(0);  // needed for ORBacus?!
    }

};
