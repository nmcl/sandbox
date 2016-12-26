/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DistributedHammer3.javatmpl,v 1.1.4.1 1999/01/28 19:04:15 nmcl Exp $
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
import java.lang.InterruptedException;

public class DistributedHammer3
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
		System.out.println("Usage: DistributedHammer3 [-reffile1 <file>] [-reffile2 <file>] [-help]");
		System.exit(0);
	    }
	}
	
	try
	{
	    DistributedHammerWorker3.hammerObject_1 =  HammerHelper .narrow(ORBObject.fileToObject(server1));
	    DistributedHammerWorker3.hammerObject_2 =  HammerHelper .narrow(ORBObject.fileToObject(server2));

	    if (! DistributedHammerWorker3.hammerObject_1.set(10, null))
		System.out.println("m set1 : failed");
	    if (! DistributedHammerWorker3.hammerObject_2.set(101, null))
		System.out.println("m set2 : failed");

	    DistributedHammerWorker3.get12(0, 0);
	    DistributedHammerWorker3.get21(0, 0);
	}
	catch (Exception e)
	{
	    System.err.println("DistributedHammer3: "+e);
	    System.exit(0);
	}

	DHThreadObject3b thr1 = new DHThreadObject3b(1);
	DHThreadObject3b thr2 = new DHThreadObject3b(2);

	thr1.start();
	thr2.start();

	try
	{
	    thr1.join();
	    thr2.join();
	}
	catch (InterruptedException e)
	{
	    System.err.println(e);
	}

	DistributedHammerWorker3.get12(0, 0);
	DistributedHammerWorker3.get21(0, 0);

	ORBInterface.shutdownBOA();
	ORBInterface.shutdownORB();

	System.exit(0);  // nneded for ORBacus?!
    }

};
