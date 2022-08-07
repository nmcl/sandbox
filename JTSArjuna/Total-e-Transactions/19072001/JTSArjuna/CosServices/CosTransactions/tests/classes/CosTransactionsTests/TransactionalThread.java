/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TransactionalThread.java,v 1.1 2000/02/25 14:08:55 nmcl Exp $
 */

package CosTransactionsTests;

import TestModule.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.CosTransactions.*;
import org.omg.CosTransactions.*;
import java.util.Random;

import java.lang.InterruptedException;

public class TransactionalThread extends Thread
{

public TransactionalThread ()
    {
	done = false;
	control = null;
    }
    
public TransactionalThread (Control currentTran)
    {
	done = false;
	control = currentTran;
    }

public void run ()
    {
	boolean shouldWork = false;
	
	OTS_Current current = OTS.current();

	if (control != null)
	{
	    System.out.println("New thread resuming transaction.");

	    try
	    {
		current.resume(control);
	    }
	    catch (Exception e)
	    {
		System.err.println("Caught unexpected exception: "+e);
		System.exit(1);
	    }
	}
	
	try
	{
	    System.out.print("Non-creating thread trying to commit transaction. ");
	    
	    if (control == null)
		System.out.println("Should fail - no transaction associated with thread!");
	    else
		System.out.println("Should succeed.");
		
	    current.commit(true);

	    System.out.print("Non-creating thread committed transaction. ");

	    if (control == null)
	    {
		System.out.println("Error.");
		System.exit(1);
	    }
	    else
		System.out.println();
	}
	catch (Exception e)
	{
	    System.err.println("Caught unexpected exception: "+e);
	}

	done = true;
    }

public synchronized boolean finished ()
    {
	return done;
    }

private boolean done;
private Control control;
    
};
