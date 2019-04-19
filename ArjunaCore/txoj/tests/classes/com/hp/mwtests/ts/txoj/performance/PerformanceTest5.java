package com.hp.mwtests.ts.txoj.performance;

/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: PerformanceTest5.java,v 1.3 2003/06/19 11:56:28 nmcl Exp $
 */

import com.hp.mwtests.ts.txoj.*;
import com.arjuna.ats.arjuna.*;
import com.arjuna.ats.txoj.common.*;
import java.util.*;

import com.hp.mwtests.ts.txoj.common.exceptions.TestException;
import java.lang.NumberFormatException;

public class PerformanceTest5
{
    
public static void main (String[] args)
    {
	try
	{
	    double iters = 1000.0;
	    boolean doCommit = true;
	    
	    for (int i = 0; i < args.length; i++)
	    {
		if (args[i].equals("-rollback"))
		    doCommit = false;
		else
		{
		    if (args[i].equals("-help"))
		    {
			System.out.println("Usage: com.hp.mwtests.ts.txoj.performance.PerformanceTest5 [-rollback] [-help]");
			System.exit(0);
		    }
		    else
		    {
			System.err.println("Unknown option: "+args[i]);
			System.exit(0);
		    }
		}
	    }
	    
	    // Run ten interations first.

	    for (int i = 0; i < 10; i++)
	    {
		AtomicAction A = new AtomicAction();
		
		A.begin();

		if (doCommit)
		    A.commit(true);
		else
		    A.abort();
	    }

	    // Record the start time.

	    Date startTime = new Date();

	    // Run 1000 interations.
	    
	    for (int i = 0; i < iters; i++)
	    {
		AtomicAction A = new AtomicAction();
		
		A.begin();

		if (doCommit)
		    A.commit(true);
		else
		    A.abort();
	    }

	    // Record the end time.

	    Date endTime = new Date();
	    double txnTime = (float)((endTime.getTime()-startTime.getTime())/iters);
	    double txnPSec = 1000.0/txnTime;

	    System.out.println("Average time for empty transaction = "+txnTime);
	    System.out.println("Transactions per second = "+txnPSec);
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	}
    }    
    
};
