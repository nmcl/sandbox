/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Performance2.java,v 1.1.2.1.4.1.24.1 2000/11/08 14:25:59 nmcl Exp $
 */

import com.arjuna.OrbCommon.*;
import com.arjuna.CosTransactions.*;
import org.omg.CosTransactions.*;
import java.util.*;

public class Performance2
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
	    
	    OTS_Factory factory = OTS.factory();
	    Control control = null;
	    
	    // Run ten interations first.

	    for (int i = 0; i < 10; i++)
	    {
		control = factory.create(0);

		if (doCommit)
		    control.get_terminator().commit(true);
		else
		    control.get_terminator().rollback();
	    }

	    // Record the start time.

	    Date startTime = new Date();

	    // Run 1000 interations.
	    
	    for (int i = 0; i < iters; i++)
	    {
		control = factory.create(0);

		if (doCommit)
		    control.get_terminator().commit(true);
		else
		    control.get_terminator().rollback();
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
