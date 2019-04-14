package com.hp.mwtests.ts.arjuna.objectstore;

/*
 * Copyright (C) 2001,
 *
 * Hewlett-Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ObjectStoreTest.java,v 1.7 2003/06/19 10:51:19 nmcl Exp $
 */

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.common.Environment;
import com.arjuna.ats.arjuna.objectstore.ObjectStore;
import com.arjuna.ats.arjuna.gandiva.ObjectName;
import com.arjuna.ats.arjuna.gandiva.ClassName;
import com.arjuna.mwlabs.testframework.unittest.Test;

public class ObjectStoreTest extends Test
{

public void run(String[] args)
    {
	ClassName imple = ArjunaNames.Implementation_ObjectStore_defaultStore();
	String localOSRoot = "foo";
	String objectStoreDir = "/bar";
	String shareStatus = "OS_SHARED";

	ObjectName objName = new ObjectName("JNS:myname");

	try
	{
	    objName.setClassNameAttribute(Environment.OBJECTSTORE_TYPE, imple);
	    objName.setStringAttribute(Environment.LOCALOSROOT, localOSRoot);
	    objName.setStringAttribute(Environment.OBJECTSTORE_DIR, objectStoreDir);
	    objName.setStringAttribute(Environment.OBJECTSTORE_SHARE, shareStatus);
	}
	catch (Exception e)
	{
	    e.printStackTrace(System.err);
	    assertFailure();
	}

	ObjectStore objStore = new ObjectStore(objName);

	System.out.println("\nchecking assumed valid store.");

	boolean passed = validate(objStore);

	if (passed)
	{
	    // check with a known invalid implementation

	    objStore = new ObjectStore();

	    System.out.println("\nchecking known invalid store.");

	    if (validate(objStore))
		passed = false;
	}

	if (passed)
	    assertSuccess();
	else
	    assertFailure();
    }

private static final boolean validate (ObjectStore objStore)
    {
	boolean passed = false;

	if (objStore.className().equals(imple))
	{
	    if (objStore.shareState() == ObjectStore.OS_SHARED)
	    {
		if (objStore.storeDir().equals(objectStoreDir))
		{
		    if (objStore.storeRoot().equals(localOSRoot))
			passed = true;
		    else
			System.err.println("ObjectStore root wrong: "+objStore.storeRoot());
		}
		else
		    System.err.println("ObjectStore dir wrong: "+objStore.storeDir());
	    }
	    else
		System.err.println("Share state wrong: "+objStore.shareState());
	}
	else
	    System.err.println("Implementation wrong: "+objStore.className());

	return passed;
    }

public static void main(String[] args)
    {
        ObjectStoreTest test = new ObjectStoreTest();
        test.initialise(null, null, args, new com.arjuna.mwlabs.testframework.unittest.LocalHarness());
        test.run(args);
    }

private static ClassName imple = ArjunaNames.Implementation_ObjectStore_defaultStore();
private static String localOSRoot = "foo";
private static String objectStoreDir = "/bar";
private static String shareStatus = "OS_SHARED";

}
