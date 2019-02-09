/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BasicActionTest.java,v 1.1 1998/11/12 09:39:08 nmcl Exp $
 */

import JavaArjunaTests.AtomicObject;
import com.arjuna.JavaArjuna.ClassLib.*;
import com.arjuna.JavaArjuna.Common.*;

import JavaArjunaTests.TestException;

public class BasicActionTest
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

	AtomicAction A = new AtomicAction();

	try
	{
	    A.begin();
	
	    System.out.println("current object value is "+foo.get());

	    foo.set(foo.get()+2);

	    System.out.println("object value is now "+foo.get());

	    A.commit();

	    System.out.println("final object value is now "+foo.get());
	}
	catch (TestException e)
	{
	    System.out.println("AtomicObject exception raised.");
	    A.abort();
	}
    }
    
};
