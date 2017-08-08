/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BasicThreadedObject.java,v 1.2 1998/07/06 13:27:08 nmcl Exp $
 */

package JavaArjunaLiteTests;

import com.arjuna.JavaArjunaLite.Atomic.*;
import com.arjuna.JavaGandiva.Common.*;
import java.lang.Thread;
import java.lang.Math;
import java.util.Random;

import JavaArjunaLiteTests.TestException;
import java.lang.InterruptedException;

public class BasicThreadedObject extends Thread
{
    
public BasicThreadedObject (boolean start)
    {
	startAction = start;
	uid = new Uid();
    }

public void run ()
    {
	if (startAction)
	{
	    BasicThreadedObject.A = new AtomicAction();

	    System.out.println("BasicThreadedObject "+uid+" created action "+BasicThreadedObject.A.get_uid());

	    BasicThreadedObject.A.begin();

	    Thread.yield();	    
	}
	else
	{
	    System.out.println("BasicThreadedObject "+uid+" adding to action "+BasicThreadedObject.A.get_uid());
	    
	    BasicThreadedObject.A.addThread();

 	    Thread.yield();	    
	}

	BasicAction act = BasicAction.Current();

	if (act != null)
	    System.out.println("BasicThreadedObject "+uid+" current action "+act.get_uid());
	else
	    System.out.println("BasicThreadedObject "+uid+" current action null");

	try
	{
	    BasicThreadedObject.O.incr(4);

	    Thread.yield();
	}
	catch (TestException e)
	{
	}
	
	if (startAction)
	{
	    System.out.println("\nBasicThreadedObject "+uid+" committing action "+act.get_uid());
	    BasicThreadedObject.A.commit();
	    System.out.println("BasicThreadedObject "+uid+" action "+act.get_uid()+" committed\n");
	}
	else
	{
	    System.out.println("\nBasicThreadedObject "+uid+" aborting action "+act.get_uid());
	    BasicThreadedObject.A.abort();
	    System.out.println("BasicThreadedObject "+uid+" action "+act.get_uid()+" aborted\n");
	}
    }

private Uid uid;
private boolean startAction;

public static AtomicAction A = null;
public static AtomicObject O = new AtomicObject();
    
};
