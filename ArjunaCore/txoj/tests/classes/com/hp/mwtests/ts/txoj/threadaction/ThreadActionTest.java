package com.hp.mwtests.ts.txoj.threadaction;

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ThreadActionTest.java,v 1.4 2003/06/19 11:56:29 nmcl Exp $
 */

import com.hp.mwtests.ts.txoj.*;
import com.arjuna.ats.arjuna.*;
import com.arjuna.ats.arjuna.coordinator.*;
import com.arjuna.ats.txoj.common.*;

import com.hp.mwtests.ts.txoj.common.exceptions.TestException;
import com.hp.mwtests.ts.txoj.common.resources.BasicThreadedObject;

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
