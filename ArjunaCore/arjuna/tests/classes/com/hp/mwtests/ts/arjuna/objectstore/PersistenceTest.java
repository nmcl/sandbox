/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: PersistenceTest.java,v 1.5 2003/06/26 14:04:44 jcoleman Exp $
 */

package com.hp.mwtests.ts.arjuna.objectstore;

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.objectstore.ObjectStore;
import com.arjuna.ats.arjuna.*;
import com.arjuna.ats.arjuna.state.*;
import com.arjuna.ats.arjuna.common.*;

import java.io.*;

import java.util.*;

import com.arjuna.ats.arjuna.exceptions.ObjectStoreException;

import com.arjuna.mwlabs.testframework.unittest.Test;
import com.arjuna.mwlabs.testframework.unittest.LocalHarness;

public class PersistenceTest extends Test
{
    
    public void run(String[] args)
    {
	boolean passed = false;
	boolean threaded = false;
	long stime = Calendar.getInstance().getTime().getTime();

	for (int j = 0; j < args.length; j++)
	{
	    if (args[j].equals("-cached"))
		threaded = true;
	}
	
	for (int i = 0; i < 1000; i++)
	{
	    try
	    {
		ObjectStore store = null;
		
		if (!threaded)
		    store = new ObjectStore(ArjunaNames.Implementation_ObjectStore_ShadowingStore());
		else
		    store = new ObjectStore(ArjunaNames.Implementation_ObjectStore_CacheStore());
	    
		byte[] data = new byte[10240];
		OutputObjectState state = new OutputObjectState();
		Uid u = new Uid();

		state.packBytes(data);
		
		if (store.write_committed(u, "/StateManager/LockManager/foo", state))
		{
		    passed = true;
		}
		else
		    passed = false;
	    }
	    catch (ObjectStoreException e)
	    {
		System.out.println(e.getMessage());

		passed = false;
	    }
	    catch (IOException ex)
	    {
		ex.printStackTrace();
		
		passed = false;
	    }
	}

	try
	{
	    Thread.currentThread().sleep(1000);
	}
	catch (Exception ex)
	{
	}
	
	long ftime = Calendar.getInstance().getTime().getTime();
	long timeTaken = ftime - stime;

	System.out.println("time for 1000 write transactions is "+timeTaken);
	
	try
	{
	    Thread.currentThread().sleep(1000);
	}
	catch (Exception ex)
	{
	}

	if (passed)
        {
	    System.out.println("Test passed");
	    assertSuccess();
        }
	else
        {
	    System.out.println("Test failed");
	    assertFailure();
        }
    }

    public static void main(String[] args)
    {
        PersistenceTest test = new PersistenceTest();
        test.run(args);
    }
}
