/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Implementations.java,v 1.1.2.1.26.1 2000/11/08 14:30:28 nmcl Exp $
 */

package com.arjuna.CosTransactions.Implementation;

import com.arjuna.ArjunaCommon.Interface.Inventory;

/*
 * Slightly different for historical reasons.
 */

import com.arjuna.CosTransactions.Implementation.AbstractRecord.OTS_ResourceRecordSetup;
import com.arjuna.CosTransactions.Implementation.AbstractRecord.OTS_AbstractRecordSetup;

public class Implementations
{

public static synchronized boolean added ()
    {
	return _added;
    }
    
public static synchronized void initialise ()
    {
	if (!_added)
	{
	    /*
	     * Now add various abstract records which crash recovery needs.
	     */

	    Inventory.inventory().addToList(new OTS_ResourceRecordSetup());
	    Inventory.inventory().addToList(new OTS_AbstractRecordSetup());

	    _added = true;
	}
    }

private Implementations ()
    {
    }

private static boolean _added = false;

    static
    {
	initialise();
    }
    
}
