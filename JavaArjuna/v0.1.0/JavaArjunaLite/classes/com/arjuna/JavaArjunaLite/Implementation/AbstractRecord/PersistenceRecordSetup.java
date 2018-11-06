/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PersistenceRecordSetup.java,v 1.3 1998/07/06 13:26:40 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Implementation.AbstractRecord;

import com.arjuna.JavaArjunaLite.JavaArjunaLiteNames;
import com.arjuna.JavaArjunaLite.Atomic.PersistenceRecord;
import com.arjuna.JavaGandiva.Common.ClassName;
import com.arjuna.JavaGandiva.Common.ObjectName;
import com.arjuna.JavaGandiva.Base.InventoryElement;

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
	return JavaArjunaLiteNames.Implementation_AbstractRecord_PersistenceRecord();
    }
    
}
