/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JTAHammer.java,v 1.1.2.1.10.1.88.1.8.1.2.1 2002/07/08 11:52:33 nmcl Exp $
 */

import com.arjuna.OrbCommon.*;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.jta.*;
import javax.transaction.*;
import javax.transaction.xa.*;
import java.util.*;

import java.lang.IllegalAccessException;

class Worker extends Thread
{

public Worker (XACreator c, String s, int iters)
    {
	_creator = c;
	_connectionString = s;
	_iters = iters;
    }
    
public void run ()
    {
	for (int i = 0; i < _iters; i++)
	{
	    try
	    {
		XAResource theResource = _creator.create(_connectionString);

		if (theResource == null)
		{
		    System.err.println("Error - creator "+_creator+" returned null resource.");
		    System.exit(0);
		}

		TransactionManager tm = JTA_TransactionManager.transactionManager();

		if (tm != null)
		{
		    tm.begin();
	    
		    Transaction theTransaction = tm.getTransaction();

		    if (theTransaction != null)
		    {
			if (!theTransaction.enlistResource(theResource))
			{
			    System.err.println("Error - could not enlist resource in transaction!");
			    tm.rollback();
			    
			    System.exit(0);
			}

			/*
			 * XA does not support subtransactions.
			 */
		    
			/*
			 * Do some work and decide whether to commit or rollback.
			 * (Assume commit for example.)
			 */

			tm.commit();
		    }
		    else
		    {
			System.err.println("Error - could not get transaction!");
			tm.rollback();
			System.exit(0);
		    }
		}
		else
		    System.err.println("Error - could not get transaction manager!");
	    }
	    catch (Exception e)
	    {
		e.printStackTrace();
	    }
	}

	JTAHammer.doSignal();
    }
    
private XACreator _creator;
private String _connectionString;
private int _iters;

}

public class JTAHammer
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
	
	String xaResource = "DummyCreator";
	String connectionString = null;
	int threads = 10;
	int work = 100;
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-connect") == 0)
		connectionString = args[i+1];
	    if (args[i].compareTo("-creator") == 0)
		xaResource = args[i+1];
	    if (args[i].compareTo("-threads") == 0)
	    {
		try
		{
		    Integer v = new Integer(args[i+1]);
		    
		    threads = v.intValue();
		}
		catch (Exception e)
		{
		    System.err.println(e);
		}
	    }
	    if (args[i].compareTo("-work") == 0)
	    {
		try
		{
		    Integer v = new Integer(args[i+1]);
		    
		    work = v.intValue();
		}
		catch (Exception e)
		{
		    System.err.println(e);
		}
	    }
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: JTAHammer -creator <name> [-connect <string>] [-help] [-threads <number>] [-work <number>]");
		System.exit(0);
	    }
	}

	if (xaResource == null)
	{
	    System.err.println("Error - no resource creator specified.");
	    System.exit(0);
	}

	/*
	 * We should have a reference to a factory object (see JTA
	 * specification). However, for simplicity we will ignore this.
	 */

	XACreator creator = null;

	try
	{
	    creator = (XACreator) Class.forName(xaResource).newInstance();
	}
	catch (Exception e)
	{
	    System.err.println(e);
	    
	    System.exit(0);
	}

	number = threads;

	int numberOfTransactions = threads * work;
	long stime = Calendar.getInstance().getTime().getTime();
	
	for (int i = 0; i < threads; i++)
	{
	    Worker w = new Worker(creator, connectionString, work);
	    
	    w.start();
	}

	JTAHammer.doWait();

	long ftime = Calendar.getInstance().getTime().getTime();
	long timeTaken = ftime - stime;
	
	System.out.println("time for "+numberOfTransactions+" write transactions is "+timeTaken);
	System.out.println("number of transactions: "+numberOfTransactions);
	System.out.println("throughput: "+(float) (numberOfTransactions/(timeTaken / 1000.0)));

	OAInterface.shutdownOA();
	ORBInterface.shutdownORB();
    }

public static void doWait ()
    {
	try
	{
	    synchronized (sync)
		{
		    if (number > 0)
			sync.wait();
		}
	}
	catch (Exception e)
	{
	}
    }

public static void doSignal ()
    {
	synchronized (sync)
	    {
		if (--number == 0)
		    sync.notify();
	    }
    }
	
private static Object sync = new Object();
private static int number = 0;
    
}
