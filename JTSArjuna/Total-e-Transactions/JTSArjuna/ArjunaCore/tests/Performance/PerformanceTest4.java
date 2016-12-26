/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: PerformanceTest4.java,v 1.1.4.1 2000/10/20 09:17:46 nmcl Exp $
 */

import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCommon.Common.*;

import java.lang.NumberFormatException;

public class PerformanceTest4
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

	AtomicAction A = new AtomicAction();
	A.begin();
	A.commit();
	A = null;
	
	long t1 = System.currentTimeMillis();
	
	for (int c = 0; c < iters; c++)
	{
	    A = new AtomicAction();
	
	    A.begin();
	    
	    A.commit();

	    A = null;
	}

	long t2 = System.currentTimeMillis();

	System.out.println(t1+" "+t2);
	
	System.out.print("Time taken to create/commit "+iters+" atomic actions: ");

	System.out.println(t2-t1+" milliseconds");

	AtomicAction B = null;
	t1 = System.currentTimeMillis();

	for (int c = 0; c < iters; c++)
	{
	    A = new AtomicAction();
	    
	    A.begin();

	    B = new AtomicAction();

	    B.begin();

	    B.commit();
	    
	    A.commit();

	    B = null;
	    A = null;
	}

	t2 = System.currentTimeMillis();

	System.out.println(t1+" "+t2);
	
	System.out.print("Time taken to create/commit "+iters+" nested atomic actions: ");

	System.out.println(t2-t1+" milliseconds.");
    }
    
};
