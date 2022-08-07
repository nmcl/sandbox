/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: DestroyTest.java,v 1.1.2.1 2000/11/08 14:31:43 nmcl Exp $
 */

import ArjunaCoreTests.AtomicObject;
import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.ArjunaCore.Atomic.*;
import java.util.Random;
import java.lang.Math;

import ArjunaCoreTests.TestException;

public class DestroyTest
{

public static void main (String[] args)
    {
	AtomicObject atomicObject = new AtomicObject();
	Uid u = atomicObject.get_uid();
	AtomicAction a = new AtomicAction();
	
	a.begin();
	
	try
	{
	    atomicObject.set(10);
	}
	catch (TestException e)
	{
	    System.out.println("set : failed");

	    System.exit(0);
	}
	
	System.out.println("set : ok");

	if (!atomicObject.destroy())
	{
	    System.out.println("destroy : failed");

	    a.abort();
	    
	    System.exit(0);
	}

	a.commit();
	
	atomicObject = new AtomicObject(u);
	
	try
	{
	    int val = atomicObject.get();
	    
	    if (val != -1)
	    {
		System.out.println("got : "+val);

		System.out.println("destroy did not work!");
	    }
	    else
		System.out.println("object destroyed!");
	}
	catch (TestException e)
	{
	    System.out.println("object destroyed!");
	}
    }
    
}
