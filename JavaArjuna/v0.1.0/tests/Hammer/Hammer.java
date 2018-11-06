/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Hammer.java,v 1.1 1998/11/12 09:39:17 nmcl Exp $
 */

import JavaArjunaLiteTests.AtomicObject;
import JavaArjunaLiteTests.HammerThreadedObject;
import com.arjuna.JavaArjunaLite.Atomic.*;
import com.arjuna.JavaGandiva.Common.*;
import java.lang.Thread;
import java.util.Random;

import JavaArjunaLiteTests.TestException;
import com.arjuna.JavaGandiva.Common.UidException;
import java.lang.InterruptedException;

    
public class Hammer
{
    
public static void main (String[] args)
    {
	HammerThreadedObject.object = new AtomicObject();
	HammerThreadedObject object1 = new HammerThreadedObject(2);
	HammerThreadedObject object2 = new HammerThreadedObject(-2);
	
	object1.start();
	object2.start();

	try
	{
	    object1.join();
	    object2.join();
	}
	catch (InterruptedException e)
	{
	}
    }


    
};
