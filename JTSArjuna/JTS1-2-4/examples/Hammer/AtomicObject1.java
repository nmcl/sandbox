/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AtomicObject1.java,v 1.1 1998/11/12 13:32:07 nmcl Exp $
 */

import JavaCosTransactionsTests.*;
import TestModule.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.JavaCosTransactions.*;
import java.util.Random;

import JavaCosTransactionsTests.TestException;

public class AtomicObject1
{
    
public static void main (String[] args)
    {
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();
	
	AtomicWorker1.current = OTS.current();

	AtomicWorker1.atomicObject_1 = new AtomicObject();
	AtomicWorker1.atomicObject_2 = new AtomicObject();

	System.out.println(AtomicWorker1.atomicObject_1.get_uid());
	System.out.println(AtomicWorker1.atomicObject_2.get_uid());
	
	if (!AtomicWorker1.atomicObject_1.set(10))
	    System.out.println("m set1 : failed");
	if (!AtomicWorker1.atomicObject_2.set(101))
	    System.out.println("m set2 : failed");

	AtomicWorker1.get12('m', 0);
	AtomicWorker1.get21('m', 0);
	
	for (int i = 0; i < 100; i++)
	    AtomicWorker1.randomOperation('1', 0);

	AtomicWorker1.get12('m', 0);
	AtomicWorker1.get21('m', 0);

	ORBInterface.shutdownBOA();
	ORBInterface.shutdownORB();
    }

};
