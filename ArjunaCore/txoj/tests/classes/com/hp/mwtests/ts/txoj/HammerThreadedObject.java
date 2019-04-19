/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: HammerThreadedObject.java,v 1.3 2003/06/19 11:56:24 nmcl Exp $
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

public class HammerThreadedObject extends Thread
{
    
public HammerThreadedObject (int value)
    {
	_uid = new Uid();
	_value = value;
    }

public void run ()
    {
	for (int i = 0; i < HammerThreadedObject.iter; i++)
	{
	    AtomicAction A = new AtomicAction();
	    float f = HammerThreadedObject.rand.nextFloat();

	    try
	    {
		Thread.yield();
		
		A.begin();

		int v = HammerThreadedObject.object.get();

		if (f > 0.25)
		    System.out.println(_uid+": atomic object value: "+v);
		else
		{
		    int nv = v+_value;
		
		    HammerThreadedObject.object.set(nv);
		    
		    System.out.println(_uid+": atomic object value set to : "+nv);
		}
	    
		A.commit();
		
		try
		{
		    Thread.sleep((int) HammerThreadedObject.rand.nextFloat()*1000);
		}
		catch (InterruptedException e)
		{
		}
	    }
	    catch (TestException e)
	    {
		System.out.println(_uid+": AtomicObject exception raised.");
		A.abort();

		Thread.yield();
	    }
	}
    }

private Uid _uid;
private int _value;

public static AtomicObject object = null;
public static int iter = 100;
public static Random rand = new Random();
    
};
