/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ShadowingStoreSetup.java,v 1.6 1998/07/06 13:26:48 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ObjectStore;

import com.arjuna.JavaArjuna.ObjectStore.ObjectStoreImple;
import com.arjuna.JavaArjuna.Common.*;

public class ShadowingStoreSetup
{

public ShadowingStoreSetup ()
    {
    }

public synchronized Object createVoid ()
    {
	return ShadowingStore.create();
    }
    
public synchronized Object createResources (Object[] param)
    {
	return ShadowingStore.create(param);
    }

    /*
     * https://github.com/nmcl/sandbox/issues/66
public synchronized Object createClassName (ClassName className)
    {
	return null;
    }
    
public synchronized Object createObjectName (ObjectName objectName)
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
	return JavaArjunaLiteNames.Implementation_ObjectStore_ShadowingStore();
    }
    */
}
