/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ThreadActionTest.java,v 1.1 2000/02/25 14:04:55 nmcl Exp $
 */

import ArjunaLiteTests.AtomicObject;
import ArjunaLiteTests.BasicThreadedObject;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaLite.Atomic.*;
import java.lang.Thread;

import ArjunaLiteTests.TestException;
import com.arjuna.ArjunaCommon.Common.UidException;
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
