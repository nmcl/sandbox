/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: BasicThreadedObject.java,v 1.3 2003/06/19 11:56:24 nmcl Exp $
 */

package com.hp.mwtests.ts.txoj;

import com.arjuna.ats.arjuna.*;
import com.arjuna.ats.arjuna.coordinator.*;
import com.arjuna.ats.arjuna.state.*;
import com.arjuna.ats.arjuna.gandiva.*;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.txoj.*;
import com.arjuna.ats.txoj.common.*;
import java.lang.Thread;
import java.lang.Math;
import java.util.Random;

import com.hp.mwtests.ts.txoj.TestException;
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
