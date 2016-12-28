/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: RecoverableTest.java,v 1.1 2000/02/25 14:04:44 nmcl Exp $
 */

import ArjunaLiteTests.RecoverableObject;
import com.arjuna.ArjunaLite.Atomic.*;
import com.arjuna.ArjunaCommon.Common.*;

import ArjunaLiteTests.TestException;

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
