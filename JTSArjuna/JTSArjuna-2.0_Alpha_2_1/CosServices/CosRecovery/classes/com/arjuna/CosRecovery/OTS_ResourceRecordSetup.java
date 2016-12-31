/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ResourceRecordSetup.java,v 1.1.2.1 2000/07/27 16:12:09 ndbi Exp $
 */

package com.arjuna.CosRecovery;

import com.arjuna.ArjunaCommon.Common.ClassName;
import com.arjuna.ArjunaCommon.Common.ObjectName;
import com.arjuna.ArjunaCommon.Base.InventoryElement;

import com.arjuna.ArjunaLite.Atomic.*;

import com.arjuna.CosTransactions.CosTransactionsNames;
import com.arjuna.CosTransactions.OTS_ResourceRecord;


public class OTS_ResourceRecordSetup implements InventoryElement
{

public OTS_ResourceRecordSetup ()
    {
    }

public synchronized Object createVoid ()
    {
	return OTS_ResourceRecord.create();
    }
    
public synchronized Object createClassName (ClassName className)
    {
	return null;
    }
    
public synchronized Object createObjectName (ObjectName objectName)
    {
	return null;
    }
    
public synchronized Object createResources (Object[] resources)
    {
	return null;
    }
    
public synchronized Object createClassNameResources (ClassName className, Object[] resources)
    {
	return null;
    }
    
public synchronized Object createObjectNameResources (ObjectName objectName, Object[] resources)
    {
	return null;
    }
    
public ClassName className ()
    {
	return new ClassName("RecordType.USER_DEF_FIRST0");
	// return CosTransactionsNames.AbstractRecord_OTS_ResourceRecord();
    }
    
}
