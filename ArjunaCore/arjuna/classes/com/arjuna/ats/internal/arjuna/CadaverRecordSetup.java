/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: CadaverRecordSetup.java,v 1.1 2003/06/19 10:50:28 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna;

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.CadaverRecord;
import com.arjuna.ats.arjuna.gandiva.*;
import com.arjuna.ats.arjuna.gandiva.inventory.*;

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
	return ArjunaNames.Implementation_AbstractRecord_CadaverRecord();
    }
    
}
