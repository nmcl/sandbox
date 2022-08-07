/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Performance1.java,v 1.1.2.1.4.1.24.1 2000/11/08 14:25:58 nmcl Exp $
 */

import com.arjuna.OrbCommon.*;
import com.arjuna.CosTransactions.OTS;
import com.arjuna.CosTransactions.OTS_Factory;
import com.arjuna.CosTransactions.OTS_Current;
import org.omg.CosTransactions.*;
import java.util.*;

public class Performance1
{
    
public static void main (String[] args)
    {
	try
	{
	    ORBInterface.initORB(args, null);
	    OAInterface.initOA();

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
			System.out.println("Usage: Performance1 [-rollback] [-help]");
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

	    OTS_Current current = OTS.current();

	    for (int i = 0; i < 10; i++)
	    {
		current.begin();

		if (doCommit)
		    current.commit(true);
		else
		    current.rollback();
	    }

	    // Record the start time.

	    Date startTime = new Date();

	    // Run 1000 interations.
	    
	    for (int i = 0; i < iters; i++)
	    {
		current.begin();

		if (doCommit)
		    current.commit(true);
		else
		    current.rollback();
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
	
	OAInterface.shutdownOA();
	ORBInterface.shutdownORB();
    }

};
