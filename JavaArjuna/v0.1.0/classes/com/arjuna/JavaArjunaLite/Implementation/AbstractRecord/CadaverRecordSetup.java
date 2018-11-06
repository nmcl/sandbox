/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CadaverRecordSetup.java,v 1.3 1998/07/06 13:26:39 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Implementation.AbstractRecord;

import com.arjuna.JavaArjunaLite.JavaArjunaLiteNames;
import com.arjuna.JavaArjunaLite.Atomic.CadaverRecord;
import com.arjuna.JavaGandiva.Common.ClassName;
import com.arjuna.JavaGandiva.Common.ObjectName;
import com.arjuna.JavaGandiva.Base.InventoryElement;

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
	return JavaArjunaLiteNames.Implementation_AbstractRecord_CadaverRecord();
    }
    
}
