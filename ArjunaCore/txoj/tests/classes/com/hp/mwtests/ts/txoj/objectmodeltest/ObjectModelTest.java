package com.hp.mwtests.ts.txoj.objectmodeltest;

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ObjectModelTest.java,v 1.3 2003/06/19 11:56:27 nmcl Exp $
 */

import com.hp.mwtests.ts.txoj.*;
import com.arjuna.ats.arjuna.*;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.gandiva.*;
import com.arjuna.ats.txoj.common.*;
import com.arjuna.ats.txoj.*;

import com.hp.mwtests.ts.txoj.common.exceptions.TestException;
import com.hp.mwtests.ts.txoj.common.resources.AtomicObject;

public class ObjectModelTest
{
    
public static void main (String[] args)
    {
	ObjectName objName = null;
	boolean write = false;
	String uid = null;
	long objectModel = ObjectModel.SINGLE;
	
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
		System.out.println("Usage: com.hp.mwtests.ts.txoj.objectmodeltest.ObjectModelTest [-uid <uid>] [-single] [-multiple] [-write] [-help]");
		System.exit(0);
	    }
	}

	try
	{
	    objName = ObjectName.uniqueObjectName("PNS");	    
	    objName.setLongAttribute(ArjunaNames.StateManager_objectModel(), objectModel);

	    System.out.println("ObjectName: "+objName);
	}
	catch (Exception e)
	{
	    System.err.println(e);
	    System.exit(-1);
	}

	try
	{
	    AtomicObject obj = null;

	    if (uid != null)
	    {
		Uid u = new Uid(uid);
	
		obj = new AtomicObject(u, objName);
	    }
	    else
		obj = new AtomicObject(objName);

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
