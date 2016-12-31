/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Hammer.java,v 1.1 2000/02/25 14:04:47 nmcl Exp $
 */

import ArjunaLiteTests.AtomicObject;
import ArjunaLiteTests.HammerThreadedObject;
import com.arjuna.ArjunaLite.Atomic.*;
import com.arjuna.ArjunaCommon.Common.*;
import java.lang.Thread;
import java.util.Random;

import ArjunaLiteTests.TestException;
import com.arjuna.ArjunaCommon.Common.UidException;
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
