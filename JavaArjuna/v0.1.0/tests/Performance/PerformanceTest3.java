/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PerformanceTest3.java,v 1.1 1998/11/12 09:39:24 nmcl Exp $
 */

import JavaArjunaTests.*;
import com.arjuna.JavaArjuna.ClassLib.*;
import com.arjuna.JavaArjuna.Common.*;

import JavaArjunaTests.TestException;
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
