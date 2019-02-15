/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadActionTest.java,v 1.1 1998/11/12 09:39:27 nmcl Exp $
 */

import JavaArjunaTests.AtomicObject;
import JavaArjunaTests.BasicThreadedObject;
import com.arjuna.JavaArjuna.Common.*;
import com.arjuna.JavaArjuna.ClassLib.*;
import java.lang.Thread;

import JavaArjunaTests.TestException;
import com.arjuna.JavaArjuna.Common.UidException;
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
