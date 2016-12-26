/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: AtomicObject1.java,v 1.1.2.1.28.1 2000/11/08 14:25:56 nmcl Exp $
 */

import CosTransactionsTests.*;
import TestModule.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.OrbCommon.OAInterface;
import com.arjuna.CosTransactions.*;
import java.util.Random;
import com.arjuna.ArjunaCommon.Common.ErrorStream;

import CosTransactionsTests.TestException;

public class AtomicObject1
{
    
public static void main (String[] args)
    {
	try
	{
	    ORBInterface.initORB(args, null);
	    OAInterface.initOA();
	}
	catch (Exception e)
	{
	    ErrorStream.fatal().println("Initialisation failed: "+e);
	}
	
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

	OAInterface.shutdownOA();
	ORBInterface.shutdownORB();

	//	OAInterface.destroyOA();
	ORBInterface.destroyORB();
    }

};
