/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ConcurrencyTest.java,v 1.1 1998/11/12 09:39:12 nmcl Exp $
 */

import JavaArjunaTests.AtomicObject;
import com.arjuna.JavaArjuna.ClassLib.*;
import com.arjuna.JavaArjuna.Common.*;

import JavaArjunaTests.TestException;

public class ConcurrencyTest
{
    
public static void main (String[] args)
    {
	AtomicObject foo = null;
	Uid u = null;
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-uid") == 0)
	    {
		u = new Uid(args[i+1]);

		if (!u.valid())
		{
		    System.out.println("Invalid uid.");
		    System.exit(1);
		}
	    }
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: [-uid <uid>] [-help]");
		System.exit(0);
	    }
	}

	if (u == null)
	    foo = new AtomicObject();
	else
	    foo = new AtomicObject(u);

	System.out.println("\nStarting top-level action.\n");

	AtomicAction A = new AtomicAction();

	try
	{
	    A.begin();

	    System.out.println("Current atomic object state: " + foo.get());
	
	    foo.set(7);

	    if (u == null)
	    {
		System.out.println("Now waiting for 20 seconds.");

		try
		{
		    Thread.sleep(20000);
		}
		catch (InterruptedException e)
		{
		}
	    }

	    System.out.println("\nCommitting top-level action.");

	    if (A.commit() != ActionStatus.COMMITTED)
		System.out.println("Error when committing action.");
	    else
		System.out.println("Action committed.");
	}
	catch (TestException e)
	{
	    System.out.println("Could not set state. Aborting action.");
	    A.abort();
	}
    }
    
};
