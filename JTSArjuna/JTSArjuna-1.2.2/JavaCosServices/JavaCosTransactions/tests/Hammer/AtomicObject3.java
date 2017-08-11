/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AtomicObject3.java,v 1.1 1998/11/12 13:32:09 nmcl Exp $
 */

import JavaCosTransactionsTests.*;
import TestModule.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.JavaCosTransactions.*;
import java.util.Random;

import JavaCosTransactionsTests.TestException;
import java.lang.InterruptedException;

public class AtomicObject3
{

public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();
	
	AtomicWorker3.current = OTS.current();
	
	AtomicWorker3.atomicObject_1 = new AtomicObject();
	AtomicWorker3.atomicObject_2 = new AtomicObject();

	System.out.println(AtomicWorker3.atomicObject_1.get_uid());
	System.out.println(AtomicWorker3.atomicObject_2.get_uid());

	if (!AtomicWorker3.atomicObject_1.set(10))
	    System.out.println("m set1 : failed");
	if (!AtomicWorker3.atomicObject_2.set(101))
	    System.out.println("m set2 : failed");

	Thread thr1 = new ThreadObject3b(1);
	Thread thr2 = new ThreadObject3b(2);

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

	AtomicWorker3.get12(0, 0);
	AtomicWorker3.get21(0, 0);
    }

};
