/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: CadaverRecordSetup.java,v 1.1.4.1 2000/10/16 16:00:26 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation.AbstractRecord;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCore.Atomic.CadaverRecord;
import com.arjuna.ArjunaCommon.Common.ClassName;
import com.arjuna.ArjunaCommon.Common.ObjectName;
import com.arjuna.ArjunaCommon.Base.InventoryElement;

public class CadaverRecordSetup implements InventoryElement
{

public CadaverRecordSetup ()
    {
    }

public synchronized Object createVoid ()
    {
	return CadaverRecord.create();
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
	return ArjunaCoreNames.Implementation_AbstractRecord_CadaverRecord();
    }
    
}
