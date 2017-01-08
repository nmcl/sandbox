/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CrashRecoveryTest.java,v 1.3 1998/07/06 10:49:55 nmcl Exp $
 */

import com.arjuna.JavaGandiva.Common.*;
import com.arjuna.JavaArjunaLite.Atomic.*;
import com.arjuna.JavaArjunaLite.Interface.ObjectStore;
import JavaCosRecoveryTests.*;
import JavaArjunaLiteTests.*;
import java.io.*;

import com.arjuna.JavaArjunaLite.Common.ObjectStoreException;

public class CrashRecoveryTest
{
    
public static void main (String[] args)
    {
	AtomicAction A = new AtomicAction();
	Uid u = null;
	AtomicObject o = null;

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-uid") == 0)
	    {
		u = new Uid(args[i+1]);
	    }
	}

	if (u == null)
	{
	    o = new AtomicObject();

	    System.out.println("Atomic action uid: "+A.get_uid());
	
	    A.begin();
	
	    A.add(new CrashRecoveryRecord());

	    try
	    {
		o.incr(2);

		A.commit();
	    }
	    catch (TestException e)
	    {
		System.err.println("Atomic object raised exception.");
	    }
	}
	else
	{
	    o = new AtomicObject(u);

	    A.begin();

	    try
	    {
		int v = o.get();
		
		System.out.println("Atomic object value: "+v);

		A.commit();		
	    }
	    catch (TestException e)
	    {
		System.out.println("Cannot determine atomic object value: "+e);

		A.abort();
	    }
	}
    }
    
}
