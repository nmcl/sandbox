/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_AbstractRecordSetup.java,v 1.1.2.1.2.2 2000/08/02 14:11:35 nmcl Exp $
 */

package com.arjuna.CosTransactions.Implementation.AbstractRecord;

import com.arjuna.CosTransactions.CosTransactionsNames;
import com.arjuna.CosTransactions.OTS_AbstractRecord;
import com.arjuna.ArjunaCommon.Common.ClassName;
import com.arjuna.ArjunaCommon.Common.ObjectName;
import com.arjuna.ArjunaCommon.Base.InventoryElement;

public class OTS_AbstractRecordSetup implements InventoryElement
{

public OTS_AbstractRecordSetup ()
    {
    }

public synchronized Object createVoid ()
    {
	return OTS_AbstractRecord.create();
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
	return CosTransactionsNames.AbstractRecord_OTS_AbstractRecord();
    }
    
}
