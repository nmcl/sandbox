/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TransactionTest3.java,v 1.1.28.1.2.1 2000/11/24 14:37:38 nmcl Exp $
 */

import CosTransactionsTests.*;
import TestModule.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.CosTransactions.*;
import org.omg.CosTransactions.*;
import com.arjuna.ArjunaCommon.Common.*;

import org.omg.CosTransactions.Unavailable;
import org.omg.CosTransactions.SubtransactionsUnavailable;
import org.omg.CosTransactions.NotPrepared;
import org.omg.CosTransactions.HeuristicRollback;
import org.omg.CosTransactions.HeuristicCommit;
import org.omg.CosTransactions.HeuristicMixed;
import org.omg.CosTransactions.HeuristicHazard;

public class TransactionTest3
{
    
public static void main (String[] args)
    {
	try
	{
	    ORBInterface.initORB(args, null);
	    OAInterface.initOA();
	}
	catch (Exception e)
	{
	    ErrorStream.fatal().println("Initialisation failed: "+e);
	}

	int count = 0;

	System.out.println("Testing memory allocation.");
	System.out.println("Creating as many transactions as possible.\n");
	    
	try
	{
	    for (;;)
	    {
		OTS.current().begin();
		count++;
	    }
	}
	catch (Exception e)
	{
	    System.err.println("begin caught: "+e);

	    System.gc();
	}
	catch (Error e)
	{
	    System.err.println("begin caught: "+e);
	    e.printStackTrace();

	    System.gc();
	}	

	System.out.println("\nbegan: "+count);

	try
	{
	    int created = count;

	    System.out.println("\nNow rolling back transactions.");
	    
	    for (int i = 0; i < created; i++)
	    {
		try
		{
		    System.out.println(""+count);
		    OTS.current().rollback();
		    count--;
		}
		catch (OutOfMemoryError em)
		{
		    em.printStackTrace();
		    
		    System.gc();
		}
	    }
	}
	catch (Exception e)
	{
	    System.err.println("rollback caught: "+e);

	    System.gc();
	}
	catch (Error e)
	{
	    System.err.println("rollback caught: "+e);
	    e.printStackTrace();

	    System.gc();
	}

	System.out.println("\nStill to rollback: "+count);

	OAInterface.shutdownOA();
	ORBInterface.shutdownORB();
    }
 
}


