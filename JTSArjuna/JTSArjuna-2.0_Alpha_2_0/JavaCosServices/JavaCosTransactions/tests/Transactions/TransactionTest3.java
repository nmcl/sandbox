/*
 * Copyright (C) 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TransactionTest3.java,v 1.4 1999/05/14 10:18:28 nmcl Exp $
 */

import JavaCosTransactionsTests.*;
import TestModule.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.JavaCosTransactions.*;
import org.omg.CosTransactions.*;

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
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();
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
	}
	catch (Error e)
	{
	    System.err.println("begin caught: "+e);
	    e.printStackTrace();	    
	}	

	System.out.println("\nbegan: "+count);
	
	try
	{
	    int created = count;

	    System.out.println("\nNow rolling back transactions.");
	    
	    for (int i = 0; i < created; i++)
	    {
		System.out.println(""+count);
		OTS.current().rollback();
		count--;
	    }
	}
	catch (Exception e)
	{
	    System.err.println("rollback caught: "+e);
	}
	catch (Error e)
	{
	    System.err.println("rollback caught: "+e);
	    e.printStackTrace();
	}

	System.out.println("\nstill to rollback: "+count);

	ORBInterface.shutdownBOA();
	ORBInterface.shutdownORB();
    }
 
};

