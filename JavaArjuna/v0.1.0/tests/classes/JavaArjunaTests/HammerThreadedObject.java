/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HammerThreadedObject.java,v 1.2 1998/07/06 13:27:09 nmcl Exp $
 */

package JavaArjunaTests;

import com.arjuna.JavaArjuna.ClassLib.*;
import com.arjuna.JavaArjuna.Common.*;

import java.lang.Thread;
import java.lang.Math;
import java.util.Random;

import JavaArjunaTests.TestException;
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
