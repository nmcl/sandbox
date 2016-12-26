/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: PerformanceTest3.java,v 1.1.4.1 2000/10/20 09:17:46 nmcl Exp $
 */

import ArjunaCoreTests.*;

import ArjunaCoreTests.TestException;
import java.lang.NumberFormatException;

public class PerformanceTest3
{
    
public static void main (String[] args)
    {
	long iters = 1000;

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-iter") == 0)
	    {
		try
		{
		    iters = Long.parseLong(args[i+1]);
		}
		catch (NumberFormatException e)
		{
		}
	    }
	}
	
	SimpleObject foo = new SimpleObject();
	long t1 = System.currentTimeMillis();

	for (int c = 0; c < iters; c++)
	{
	    foo.set(c);
	}

	long t2 = System.currentTimeMillis();

	System.out.print("Time taken to perform "+iters+" iterations on a simple object: ");

	System.out.println(t2-t1+" milliseconds.");
    }
    
};
