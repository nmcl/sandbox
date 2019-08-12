/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ThreadActionTest.java,v 1.1.4.1.2.1 2000/11/08 14:32:51 nmcl Exp $
 */

import ArjunaCoreTests.AtomicObject;
import ArjunaCoreTests.BasicThreadedObject;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Atomic.*;
import java.lang.Thread;

import ArjunaCoreTests.TestException;
import java.lang.InterruptedException;
    
public class ThreadActionTest
{
    
public static void main (String[] args)
    {
	BasicThreadedObject object1 = new BasicThreadedObject(true);
	BasicThreadedObject object2 = new BasicThreadedObject(false);

	System.out.println("Main thread has action "+BasicAction.Current());

	object1.start();
	object2.start();

	Thread.yield();
	
	try
	{
	    object1.join();
	    object2.join();
	}
	catch (InterruptedException e)
	{
	}

	System.out.println("Main thread has action "+BasicAction.Current());
    }
    
};
