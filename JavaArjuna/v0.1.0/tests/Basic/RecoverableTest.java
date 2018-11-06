/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecoverableTest.java,v 1.1 1998/11/12 09:39:10 nmcl Exp $
 */

import JavaArjunaLiteTests.RecoverableObject;
import com.arjuna.JavaArjunaLite.Atomic.*;
import com.arjuna.JavaGandiva.Common.*;

import JavaArjunaLiteTests.TestException;

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
    }
    
};
