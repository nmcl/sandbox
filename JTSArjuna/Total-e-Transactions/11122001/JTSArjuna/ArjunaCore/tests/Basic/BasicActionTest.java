/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: BasicActionTest.java,v 1.1.4.1.2.1.4.1 2001/01/04 12:40:58 nmcl Exp $
 */

import ArjunaCoreTests.AtomicObject;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCommon.Common.*;

import ArjunaCoreTests.TestException;

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
	
	    try
	    {
		System.out.print("current object value is ");
		System.out.println(foo.get());
	    }
	    catch (Exception e)
	    {
	    }

	    try
	    {
		System.out.println("setting value");
		
		foo.set(foo.get()+2);
	    }
	    catch (Exception e)
	    {
	    }

	    try
	    {
		System.out.print("object value is now ");
		System.out.println(foo.get());
	    }
	    catch (Exception e)
	    {
	    }

	    A.commit();

	    try
	    {
		System.out.println("final object value is now "+foo.get());
	    }
	    catch (Exception e)
	    {
	    }
	}
	catch (Exception e)
	{
	    System.out.println("AtomicObject exception raised.");
	    A.abort();
	}

	try
	{
	    AtomicAction B = new AtomicAction();
	    
	    u = new Uid();
	    foo = new AtomicObject(u);
	    
	    B.begin();

	    try
	    {
		System.out.print("attempting to get value from non-existent object: ");
		System.out.println(foo.get());
	    }
	    catch (Exception e)
	    {
	    }
	    
	    System.out.println("\ntrying to set value to 5");

	    try
	    {
		foo.set(5);
	    }
	    catch (Exception e)
	    {
	    }

	    try
	    {
		System.out.print("attempting to get value again: ");
		System.out.println(foo.get());
	    }
	    catch (Exception e)
	    {
	    }

	    B.commit();
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	}
    }
    
}
