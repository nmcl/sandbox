/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HashFunctions.java,v 1.2 1998/07/17 09:02:45 nmcl Exp $
 */

import JavaArjuna.Common.Uid;
import arjuna.JavaSim.Distributions.*;
import arjuna.JavaSim.Statistics.*;

import JavaArjuna.Common.UidException;

/*
 * This program creates a hash key of a specified size, populated with
 * random numbers. Then each key is hashed using one of the hash functions,
 * and the corresponding value placed into a histogram. At the end, the
 * histogram contents are output to give an indication of the distribution
 * of hash values.
 */

public class HashFunctions
{
    
public static void main (String[] args)
    {
	boolean done = false;
	
	for (int j = 0; j < args.length; j++)
	{
	    if (args[j].compareTo("-pjw") == 0)
	    {
		done = true;
		HashFunctions.pjw();
	    }
	    if (args[j].compareTo("-random1") == 0)
	    {
		done = true;
		HashFunctions.random1();
	    }
	    if (args[j].compareTo("-random2") == 0)
	    {
		done = true;
		HashFunctions.random2();
	    }
	}

	if (!done)
	    HashFunctions.pjw();
	
	HashFunctions.print();
    }

private static void print ()
    {
	for (i = 0; i < (int) histogram.numberOfBuckets(); i++)
	{
	    try
	    {
		double name = histogram.bucketName(i);
		
		System.out.println(name+" "+histogram.sizeByName(name));
	    }
	    catch (Exception e)
	    {
	    }
	}
    }
    
private static void random1 ()
    {
	for (i = 0; i < HashFunctions.iterations; i++)
	{
	    try
	    {
		Uid u = new Uid();
		UniformStream stream = new UniformStream(0, u.hashCode());

		histogram.setValue(stream.getNumber());
	    }
	    catch (Exception e)
	    {
	    }
	}
    }

private static void random2 ()
    {
	for (i = 0; i < HashFunctions.iterations; i++)
	{
	    try
	    {
		Uid u = new Uid();
		long seed = (long) u.hashCode();

		if (seed%2 == 0)  // make sure seed is odd
		    seed++;

		UniformStream stream = new UniformStream(0, u.hashCode(), 0, seed, 1878892440);

		histogram.setValue(stream.getNumber());
	    }
	    catch (Exception e)
	    {
	    }
	}
    }

private static void pjw ()
    {
	for (i = 0; i < HashFunctions.iterations; i++)
	{
	    try
	    {
		Uid u = new Uid();

		histogram.setValue(u.hashCode());
	    }
	    catch (Exception e)
	    {
	    }
	}
    }

private static PrecisionHistogram histogram = new PrecisionHistogram();
private static int i = 0;
private static final int iterations = 1000;
    
};
