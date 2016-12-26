/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: NestedTopLevelAction.java,v 1.1.4.1 2000/10/20 09:17:39 nmcl Exp $
 */

import ArjunaCoreTests.AtomicObject;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCommon.Common.*;

import ArjunaCoreTests.TestException;

public class NestedTopLevelAction
{
    
public static void main (String[] args)
    {
	AtomicAction A = new AtomicAction();
	TopLevelAction B = new TopLevelAction();
	AtomicAction C = new AtomicAction();
	AtomicObject foo1 = new AtomicObject();
	AtomicObject foo2 = new AtomicObject();

	try
	{
	    System.out.println("\nStarting top-level action.\n");
	
	    A.begin();

	    System.out.println(A);

	    foo1.set(5);

	    System.out.println("Current atomic object 1 state: " + foo1.get());

	    System.out.println("\nStarting nested top-level action.");

	    B.begin();

	    System.out.println(B);

	    foo2.set(7);

	    System.out.println("Current atomic object 2 state: " + foo2.get());

	    System.out.println("\nCommitting nested top-level action.");
	
	    B.commit();

	    System.out.println("\nAborting top-level action.");

	    A.abort();

	    C.begin();

	    int val1 = foo1.get();
	    int val2 = foo2.get();
	    
	    System.out.println("\nFinal atomic object 1 state: " + val1);
	    
	    if (val1 == 0)
		System.out.println("This is correct.");
	    else
		System.out.println("This is incorrect.");
	    
	    System.out.println("\nFinal atomic object 2 state: " + val2);

	    if (val2 == 7)
		System.out.println("This is correct.");
	    else
		System.out.println("This is incorrect.");		
	    
	    C.commit();
	}
	catch (TestException e)
	{
	    System.out.println("AtomicObject exception raised.");

	    A.abort();
	    B.abort();
	    C.abort();
	}
    }
    
};
