/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: PersistenceRecordSetup.java,v 1.1.4.1 2000/10/16 16:00:27 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation.AbstractRecord;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCore.Atomic.PersistenceRecord;
import com.arjuna.ArjunaCommon.Common.ClassName;
import com.arjuna.ArjunaCommon.Common.ObjectName;
import com.arjuna.ArjunaCommon.Base.InventoryElement;

public class PersistenceRecordSetup implements InventoryElement
{

public PersistenceRecordSetup ()
    {
    }

public synchronized Object createVoid ()
    {
	return PersistenceRecord.create();
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
	return ArjunaCoreNames.Implementation_AbstractRecord_PersistenceRecord();
    }
    
}
