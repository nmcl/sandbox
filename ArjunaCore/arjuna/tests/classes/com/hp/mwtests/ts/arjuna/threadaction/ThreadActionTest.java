package com.hp.mwtests.ts.arjuna.threadaction;

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ThreadActionTest.java,v 1.3 2003/06/19 10:51:22 nmcl Exp $
 */

import com.hp.mwtests.ts.arjuna.resources.BasicThreadedObject;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.coordinator.*;
import java.lang.Thread;

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
