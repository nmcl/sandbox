/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: AtomicTest.java,v 1.1.4.1 2000/10/20 09:17:41 nmcl Exp $
 */

import ArjunaCoreTests.AtomicObject;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCommon.Common.*;

import ArjunaCoreTests.TestException;

public class AtomicTest
{
    
public static void main (String[] args)
    {
	AtomicObject foo = new AtomicObject();
	Uid u = foo.get_uid();
	int v;
	
	AtomicAction A = new AtomicAction();

	try
	{
	    A.begin();
	    
	    System.out.println("current object value is "+foo.get());
	    
	    foo.set(2);

	    System.out.println("object value is now "+foo.get());

	    A.commit();

	    int finalVal = foo.get();
	    
	    System.out.println("final object value is now "+finalVal);

	    if (finalVal == 2)
		System.out.println("This is correct.");
	    else
	    {
		System.out.println("This is incorrect.");
		System.exit(0);
	    }

	    System.out.println("\nStarting new action.");
	
	    foo = new AtomicObject(u);
	
	    A = new AtomicAction();
	
	    A.begin();

	    System.out.println("current object value is "+foo.get());
	
	    foo.set(4);

	    A.commit();

	    finalVal = foo.get();
	    
	    System.out.println("final object value is "+finalVal);

	    if (finalVal == 4)
		System.out.println("This is correct.");
	    else
		System.err.println("This is incorrect.");
	}
	catch (TestException e)
	{
	    System.out.println("AtomicObject exception raised.");
	    
	    A.abort();
	}
    }
    
};
