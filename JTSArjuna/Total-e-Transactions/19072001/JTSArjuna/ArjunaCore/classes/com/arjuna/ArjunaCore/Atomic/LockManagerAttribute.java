/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LockManagerAttribute.java,v 1.1.4.1.2.1.4.1 2000/12/21 11:21:02 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import com.arjuna.ArjunaCommon.Common.ClassName;

/**
 * Instances of this class may be passed to a LockManager object at
 * construction time and used to configure it.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: LockManagerAttribute.java,v 1.1.4.1.2.1.4.1 2000/12/21 11:21:02 nmcl Exp $
 * @since JTS 1.0.
 */

public class LockManagerAttribute
{

public LockManagerAttribute ()
    {
	remoteLockStore = false;
	lockStoreType = null;
    }

public boolean   remoteLockStore;
public ClassName lockStoreType;
    
};
