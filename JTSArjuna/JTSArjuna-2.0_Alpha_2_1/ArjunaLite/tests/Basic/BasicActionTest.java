/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: BasicActionTest.java,v 1.1 2000/02/25 14:04:42 nmcl Exp $
 */

import ArjunaLiteTests.AtomicObject;
import com.arjuna.ArjunaLite.Atomic.*;
import com.arjuna.ArjunaCommon.Common.*;

import ArjunaLiteTests.TestException;

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
