/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: PerformanceTest2.java,v 1.1 2000/02/25 14:04:52 nmcl Exp $
 */

import ArjunaLiteTests.*;
import com.arjuna.ArjunaLite.Atomic.*;
import com.arjuna.ArjunaCommon.Common.*;

import ArjunaLiteTests.TestException;
import java.lang.NumberFormatException;

public class PerformanceTest2
{
    
public static void main (String[] args)
    {
	boolean persistent = true;
	long iters = 1000;
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-recoverable") == 0)
		persistent = false;

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

	long totalTime = 0;

	AtomicAction A = new AtomicAction();
	A.begin();
	A.commit();
	A = null;

	if (persistent)
	    totalTime = persistentTest(iters);
	else
	    totalTime = recoverableTest(iters);

	System.out.print("Time taken to perform "+iters+" iterations on a ");

	if (persistent)
	    System.out.print("persistent object: ");
	else
	    System.out.print("recoverable object: ");

	System.out.println(totalTime+" milliseconds");
    }

public static long recoverableTest (long iters)
    {
	RecoverableObject foo = new RecoverableObject();
	AtomicAction A = null;
	long t1 = System.currentTimeMillis();

	for (int c = 0; c < iters; c++)
	{
	    A = new AtomicAction();
	    
	    A.begin();
	    
	    foo.set(2);

	    A.commit();

	    A = null;
	}

	foo = null;
	
	return System.currentTimeMillis() - t1;
    }

public static long persistentTest (long iters)
    {
	AtomicObject foo = new AtomicObject();
	AtomicAction A = null;
	long t1 = System.currentTimeMillis();

	try
	{
	    for (int c = 0; c < iters; c++)
	    {
		A = new AtomicAction();
		
		A.begin();
		
		foo.set(2);

		A.commit();

		A = null;
	    }
	}
	catch (TestException e)
	{
	    System.out.println("AtomicObject exception raised.");

	    if (A != null)
		A.abort();
	}

	foo = null;
	
	return System.currentTimeMillis() - t1;
    }    
    
};
