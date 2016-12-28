/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LockManagerAttribute.java,v 1.1 2000/02/25 14:03:50 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

import com.arjuna.ArjunaCommon.Common.ClassName;

public class LockManagerAttribute
{

public LockManagerAttribute ()
    {
	remoteLockStore = false;
	lockStoreType = null;
    }

public boolean remoteLockStore;
public ClassName lockStoreType;
    
};
