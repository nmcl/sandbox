/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Hammer.java,v 1.1.4.1.2.1 2000/11/08 14:32:51 nmcl Exp $
 */

import ArjunaCoreTests.AtomicObject;
import ArjunaCoreTests.HammerThreadedObject;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCommon.Common.*;
import java.lang.Thread;
import java.util.Random;

import ArjunaCoreTests.TestException;
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
