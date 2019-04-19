package com.hp.mwtests.ts.txoj.performance;

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: PerformanceTest3.java,v 1.3 2003/06/19 11:56:28 nmcl Exp $
 */

import com.hp.mwtests.ts.txoj.*;
import com.arjuna.ats.arjuna.*;
import com.arjuna.ats.txoj.common.*;

import com.hp.mwtests.ts.txoj.common.exceptions.TestException;
import com.hp.mwtests.ts.txoj.common.resources.SimpleObject;

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
