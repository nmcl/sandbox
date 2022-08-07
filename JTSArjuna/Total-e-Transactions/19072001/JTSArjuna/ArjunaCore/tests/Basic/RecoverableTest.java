/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: RecoverableTest.java,v 1.1.4.1.2.1 2000/11/14 12:01:16 nmcl Exp $
 */

import ArjunaCoreTests.RecoverableObject;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCommon.Common.*;

import ArjunaCoreTests.TestException;

public class RecoverableTest
{
    
public static void main (String[] args)
    {
	RecoverableObject foo = new RecoverableObject();

	AtomicAction A = new AtomicAction();

	A.begin();
	
	System.out.println("value is "+foo.get());

	foo.set(2);

	System.out.println("value is "+foo.get());

	A.abort();

	System.out.println("value is now "+foo.get());

	AtomicAction B = new AtomicAction();
	
	B.begin();
	
	System.out.println("value is "+foo.get());

	foo.set(4);

	System.out.println("value is "+foo.get());

	B.commit();

	System.out.println("value is now "+foo.get());
    }
    
}

