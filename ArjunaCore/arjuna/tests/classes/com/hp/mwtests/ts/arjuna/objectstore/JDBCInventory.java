package com.hp.mwtests.ts.arjuna.objectstore;

/*
 * Copyright (C) 2001,
 *
 * Hewlett-Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JDBCInventory.java,v 1.6 2003/06/19 10:51:19 nmcl Exp $
 */

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.common.Environment;
import com.arjuna.ats.arjuna.common.arjPropertyManager;
import com.arjuna.ats.arjuna.objectstore.ObjectStoreImple;
import com.arjuna.ats.arjuna.gandiva.inventory.Inventory;
import com.arjuna.mwlabs.testframework.unittest.Test;
import com.hp.mwtests.ts.arjuna.licence.LicenceTest;

import java.sql.*;

public class JDBCInventory extends Test
{

public void run(String[] args)
    {
	arjPropertyManager.propertyManager.setProperty(Environment.STATIC_INVENTORY_IMPLE+"1", "com.arjuna.ats.internal.arjuna.objectstore.JDBCStoreSetup");
	arjPropertyManager.propertyManager.setProperty(Environment.JDBC_USER_DB_ACCESS, "com.hp.mwtests.ts.arjuna.objectstore.MyAccess");

	ObjectStoreImple os = (ObjectStoreImple) Inventory.inventory().createVoid(ArjunaNames.Implementation_ObjectStore_JDBCStore());

	if (os != null)
        {
	    logInformation("\nPassed.");
            assertSuccess();
        }
	else
	{
	    Inventory.inventory().printList(System.out);

	    logInformation("\nFailed.");
            assertFailure();
	}
    }

public static void main(String[] args)
    {
        JDBCInventory test = new JDBCInventory();
        test.initialise(null, null, args, new com.arjuna.mwlabs.testframework.unittest.LocalHarness());
        test.run(args);
    }
}
