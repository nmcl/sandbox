/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: AtomicObject2.java,v 1.1.2.1 2000/05/02 08:41:14 nmcl Exp $
 */

import CosTransactionsTests.*;
import TestModule.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.CosTransactions.*;
import java.util.Random;

import CosTransactionsTests.TestException;
import java.lang.InterruptedException;

public class AtomicObject2
{
    
public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initOA();
	
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

	ORBInterface.shutdownOA();
	ORBInterface.shutdownORB();

	//	ORBInterface.destroyOA();
	ORBInterface.destroyORB();
    }

};
