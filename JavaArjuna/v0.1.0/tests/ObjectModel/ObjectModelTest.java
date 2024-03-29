/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjectModelTest.java,v 1.1 1998/11/12 09:39:20 nmcl Exp $
 */

import com.arjuna.JavaArjuna.ClassLib.*;
import com.arjuna.JavaArjuna.Common.*;
import JavaArjunaTests.AtomicObject;

public class ObjectModelTest
{
    
public static void main (String[] args)
    {
	LockManagerAttribute lmAttributes = new LockManagerAttribute();
	boolean write = false;
	String uid = null;
	int objectModel = ObjectModel.SINGLE;
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-single") == 0)
		objectModel = ObjectModel.SINGLE;
	    if (args[i].compareTo("-multiple") == 0)
		objectModel = ObjectModel.MULTIPLE;
	    if (args[i].compareTo("-write") == 0)
		write = true;
	    if (args[i].compareTo("-uid") == 0)
		uid = args[i+1];
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: ObjectModelTest [-uid <uid>] [-single] [-multiple] [-write] [-help]");
		System.exit(0);
	    }
	}

	lmAttributes.objectModel = objectModel;

	try
	{
	    AtomicObject obj = null;

	    if (uid != null)
	    {
		Uid u = new Uid(uid);
	
		obj = new AtomicObject(u, lmAttributes);
	    }
	    else
		obj = new AtomicObject(lmAttributes);

	    AtomicAction A = new AtomicAction();

	    A.begin();
	    
	    if (write)
	    {
		obj.set(1234);
		System.out.println("set state to 1234");
	    }
	    else
		System.out.println("got "+obj.get());

	    System.out.println("Thread sleeping");
	    
	    Thread.sleep(10000);

	    A.commit();
	    
	    obj = null;
	}
	catch (Exception e)
	{
	    System.err.println(e);
	    System.exit(-1);
	}
    }

}
