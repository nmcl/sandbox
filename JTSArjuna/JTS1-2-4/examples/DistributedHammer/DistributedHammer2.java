/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DistributedHammer2.javatmpl,v 1.1.4.1 1999/01/28 19:04:14 nmcl Exp $
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

public class DistributedHammer2
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
		System.out.println("Usage: DistributedHammer2 [-reffile1 <file>] [-reffile2 <file>] [-help]");
		System.exit(0);
	    }
	}
	
	try
	{
	    DistributedHammerWorker2.hammerObject_1 =  HammerHelper .narrow(ORBObject.fileToObject(server1));
	    DistributedHammerWorker2.hammerObject_2 =  HammerHelper .narrow(ORBObject.fileToObject(server2));

	    if (! DistributedHammerWorker2.hammerObject_1.set(10, null))
		System.out.println("m set1 : failed");
	    if (! DistributedHammerWorker2.hammerObject_2.set(101, null))
		System.out.println("m set2 : failed");

	    DistributedHammerWorker2.get12('m', 0);
	    DistributedHammerWorker2.get21('m', 0);
	}
	catch (Exception e)
	{
	    System.err.println("DistributedHammer2: "+e);
	    System.exit(0);
	}

	DHThreadObject2 thr1 = new DHThreadObject2('1');
	DHThreadObject2 thr2 = new DHThreadObject2('2');

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

	DistributedHammerWorker2.get12('m', 0);
	DistributedHammerWorker2.get21('m', 0);

	ORBInterface.shutdownBOA();
	ORBInterface.shutdownORB();

	System.exit(0);  // nneded for ORBacus?!
    }

};
