/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AtomicObject2.java,v 1.1 1998/11/12 13:32:08 nmcl Exp $
 */

import JavaCosTransactionsTests.*;
import TestModule.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.JavaCosTransactions.*;
import java.util.Random;

import JavaCosTransactionsTests.TestException;
import java.lang.InterruptedException;

public class AtomicObject2
{
    
public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();
	
	AtomicWorker2.current = OTS.current();
	
	AtomicWorker2.atomicObject_1 = new AtomicObject();
	AtomicWorker2.atomicObject_2 = new AtomicObject();

	System.out.println(AtomicWorker2.atomicObject_1.get_uid());
	System.out.println(AtomicWorker2.atomicObject_2.get_uid());

	if (!AtomicWorker2.atomicObject_1.set(10))
	    System.out.println("m set1 : failed");
	if (!AtomicWorker2.atomicObject_2.set(101))
	    System.out.println("m set2 : failed");

	ThreadObject2 thr1 = new ThreadObject2('1');
	ThreadObject2 thr2 = new ThreadObject2('2');
	
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

	AtomicWorker2.get12('m', 0);
	AtomicWorker2.get21('m', 0);
    }

};
